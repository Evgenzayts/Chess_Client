#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , my_client(new Client_network)
{
    ui->setupUi(this);

    ui->pushButton_send->setVisible(false);

    connect(ui->pushButton_connect, SIGNAL(clicked()), this, SLOT(pushbutton_connect_clicked()));
    connect(ui->pushButton_send, SIGNAL(clicked()), this, SLOT(pushbutton_send_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pushbutton_connect_clicked()
{
    ui->pushButton_connect->setVisible(false);
    ui->pushButton_send->setVisible(true);

    QTimer::singleShot(0, this, SLOT(connection()));
}

void MainWindow::pushbutton_send_clicked()
{
    QTimer::singleShot(0, this, SLOT(send()));
}

void MainWindow::connection()
{
    if (my_client->first_connection()) {
        QTimer::singleShot(0, this, SLOT(recieve()));

        QTimer::singleShot(0, this, SLOT(recieve()));
    } else {
        QTimer::singleShot(0, this, SLOT(connection()));
    }
}

void MainWindow::recieve()
{
    static bool is_connected = false;

    static bool is_ready_for_game = false;

    if (!is_connected) {
        ui->label->setText("Wait for connection...");
    }

    if (my_client->recieve_message()) {
        if (!is_connected) {
            if (my_client->get_recv_msg() == 1) {
                ui->widget_chessboard->set_player_color(true);
                ui->label->setText("Wait for second player...");
            } else {
                ui->label->setText("First player writing...");
                ui->widget_chessboard->set_player_color(false);

                is_ready_for_game = true;
            }

            ui->widget_chessboard->draw_board();

            is_connected = true;
        } else {
            if (is_ready_for_game) {
                uint32_t enemy_move = my_client->get_recv_msg();

                ui->widget_chessboard->set_enemy_move(enemy_move);
            } else {
                is_ready_for_game = true;
            }

            ui->widget_chessboard->ready_for_player_move();
        }

        uint16_t recieved_1_msg = my_client->get_recv_msg() & 0xF0;
        uint16_t recieved_2_msg = my_client->get_recv_msg() & 0x0F;

        ui->label->setText(QString("Recieved: %1 - %2").arg(63 - recieved_1_msg).arg(63 - recieved_2_msg));
    } else {
        QTimer::singleShot(0, this, SLOT(recieve()));
    }
}

void MainWindow::send()
{
    uint32_t my_move = ui->widget_chessboard->my_move();

    my_client->set_new_send_msg(my_move);

    if (my_client->send_message()) {
        // ui->pushButton_send->setVisible(false);

        QTimer::singleShot(0, this, SLOT(recieve()));
    } else {
        QTimer::singleShot(0, this, SLOT(send()));
    }
}
