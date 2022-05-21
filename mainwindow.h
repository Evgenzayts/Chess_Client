#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Class_client.h"
#include "chess_board.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void pushbutton_connect_clicked();

    void pushbutton_send_clicked();

    void connection();

    void recieve();

    void send();

private:
    Ui::MainWindow *ui;

    Client_network *my_client;
};
#endif // MAINWINDOW_H
