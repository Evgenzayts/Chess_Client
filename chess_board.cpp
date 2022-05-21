#include "chess_board.h"

Chess_board::Chess_board(QWidget *parent)
    :  QWidget(parent)
    , my_simulator(new Simulator)
    , my_color(NONE_COLOR)
    , first_position(0)
    , second_position(0)
    , wrong_highlight_pos(0)
{
    for (uint8_t i = 0; i < BOARD_SIZE; ++i)
        list_of_buttons.append(new SquareButton(this));

    setup();

    //connect(my_simulator, SIGNAL(change_figure_sig(uint8_t)), this, SLOT(change_figure(uint8_t)));
}

void Chess_board::set_player_color(bool is_first_player)
{
    if (is_first_player) {
        my_color = WHITE;
    } else {
        my_color = BLACK;
    }

    my_simulator->set_player_color(my_color);
    my_simulator->set_my_start_figures();
}

void Chess_board::draw_board()
{
    for (uint8_t i = 0; i < BOARD_LINE; ++i) {
        for (uint8_t j = 0; j < BOARD_COLUMN; ++j) {

            if (my_color == WHITE) {
                draw_board_for_white(i, j);
            } else {
                draw_board_for_black(i, j);
            }
        }
    }
}

void Chess_board::ready_for_player_move()
{
    Chess_board::activate_squares();
}

void Chess_board::set_enemy_move(uint32_t motion)
{
    uint32_t new_motion = motion & 0xFFFF;

    uint8_t first_enemy_pos = (new_motion >> 8) & 0xFF;
    uint8_t second_enemy_pos = new_motion & 0xFF;

    reverse_enemy_move(first_enemy_pos, second_enemy_pos);

    draw_move(first_enemy_pos, second_enemy_pos);
    my_simulator->make_move(first_enemy_pos, second_enemy_pos);
}

uint32_t Chess_board::my_move()
{
    uint32_t my_motion = second_position | (((uint16_t) first_position) << 8);

    return my_motion;
}

void Chess_board::setup()
{
    for (uint8_t i = 0; i < 10; ++i) {
        for (uint8_t j = 0; j < 10; ++j) {
            QLabel *label = new QLabel(this);

            label->setStyleSheet("background-color: rgb(50, 10, 0);"
                                 "color: rgb(200, 200, 200)");
            label->setAlignment(Qt::AlignCenter);

            QFont my_font;
            my_font.setPointSize(20);
            label->setFont(my_font);

            if (i == 0 && j == 0) {
                label->setGeometry(0, 0, 60, 60);
                list_of_labels[i * 10 + j] = label;
            }
            if (i == 0 && j == 9) {
                label->setGeometry(0, 60 + 8 * 85, 60, 60);
                list_of_labels[i * 10 + j] = label;
            }
            if (i == 9 && j == 0) {
                label->setGeometry(60 + 8 * 85, 0, 60, 60);
                list_of_labels[i * 10 + j] = label;
            }
            if (i == 9 && j == 9) {
                label->setGeometry(60 + 8 * 85, 60 + 8 * 85, 60, 60);
                list_of_labels[i * 10 + j] = label;
            }

            if (i == 0 && j != 0 && j != 9) {
                label->setGeometry(j * 85 - 25, 0, 85, 60);
                list_of_labels[i * 10 + j] = label;
            }

            if (i != 0 && i != 9 && j == 0) {
                label->setGeometry(0, i * 85  - 25, 60, 85);
                list_of_labels[i * 10 + j] = label;
            }

            if (i != 0 && i != 9 && j == 9) {
                label->setGeometry(8 * 85 + 60, i * 85 - 25, 60, 85);
                list_of_labels[i * 10 + j] = label;
            }

            if (i == 9 && j != 0 && j != 9) {
                label->setGeometry(j * 85 - 25, 8 * 85 + 60, 85, 60);
                list_of_labels[i * 10 + j] = label;
            }
        }
    }

    for (uint8_t i = 0; i < BOARD_LINE; ++i) {
        for (uint8_t j = 0; j < BOARD_COLUMN; ++j) {
            QPushButton *pushbutton = new QPushButton(this);

            pushbutton->setGeometry(j * 85 + 60, i * 85 + 60, 85, 85);
            pushbutton->setIconSize(QSize(pushbutton->width(), pushbutton->height()));
            QPalette my_palette;

            if (i % 2 == j % 2) {
                my_palette.setColor(QPalette::Button, QColor(240, 240, 240));
            } else {
                my_palette.setColor(QPalette::Button, QColor(100, 10, 0));
            }
            pushbutton->setPalette(my_palette);

            list_of_buttons[i * BOARD_COLUMN + j]->setPosition(i * 8 + j);
            list_of_buttons[i * BOARD_COLUMN + j]->setPushbutton(pushbutton);
        }
    }

    connect_buttons_to_slot();
}

void Chess_board::draw_move(uint8_t _first_position, uint8_t _second_position)
{
    QIcon first_icon;

    for (uint8_t i = 0; i < BOARD_SIZE; ++i) {
        if (list_of_buttons[i]->getPosition() == _first_position)
            first_icon = list_of_buttons[i]->getPushbutton()->icon();
    }

    for (uint8_t i = 0; i < BOARD_SIZE; ++i) {
        if (list_of_buttons[i]->getPosition() == _first_position) {
            list_of_buttons[i]->getPushbutton()->setIcon(QIcon());
        }

        if (list_of_buttons[i]->getPosition() == _second_position)
            list_of_buttons[i]->getPushbutton()->setIcon(first_icon);
    }

}

void Chess_board::reverse_enemy_move(uint8_t &first_pos, uint8_t &second_pos)
{
    first_pos = BOARD_SIZE - 1 - first_pos;
    second_pos = BOARD_SIZE - 1 - second_pos;
}

void Chess_board::set_wrong_highlight(uint8_t position) {
    QPalette my_palette;

    my_palette.setColor(QPalette::Button, QColor(200, 0, 0));

    list_of_buttons[position]->getPushbutton()->setPalette(my_palette);

    wrong_highlight_pos = position;

    QTimer::singleShot(200, this, SLOT(clear_wrong_highlight()));
}

void Chess_board::set_all_highlights(uint8_t position) {
    QPalette my_palette;

    for (uint8_t j = 0; j < BOARD_SIZE; ++j) {
        if (list_of_buttons[j]->getPosition() == position) {
            my_palette.setColor(QPalette::Button, QColor(0, 200, 0));
            list_of_buttons[j]->getPushbutton()->setPalette(my_palette);

            highlighted_positions.append(list_of_buttons[j]->getPosition());
        }

        for (qsizetype i = 0; i < possible_moves.length(); ++i) {
            if (list_of_buttons[j]->getPosition() == possible_moves[i]) {
                my_palette.setColor(QPalette::Button, QColor(0, 0, 200));
                list_of_buttons[j]->getPushbutton()->setPalette(my_palette);

                highlighted_positions.append(list_of_buttons[j]->getPosition());
            }
        }
    }
}

void Chess_board::clear_highlight()
{
    QPalette my_palette;

    for (qsizetype i = 0; i < highlighted_positions.length(); ++i) {
        for (uint8_t j = 0; j < BOARD_LINE; ++j) {
            for (uint8_t k = 0; k < BOARD_COLUMN; ++k) {
                if (list_of_buttons[j * BOARD_COLUMN + k]->getPosition() == highlighted_positions.value(i)) {
                    if (j % 2 == k % 2) {
                        my_palette.setColor(QPalette::Button, QColor(240, 240, 240));
                    } else {
                        my_palette.setColor(QPalette::Button, QColor(110, 10, 0));
                    }

                    list_of_buttons[j * BOARD_COLUMN + k]->getPushbutton()->setPalette(my_palette);
                }
            }
        }
    }

    highlighted_positions.clear();
}

void Chess_board::clear_wrong_highlight()
{
    QPalette my_palette;

    for (uint8_t i = 0; i < BOARD_LINE; ++i) {
        for (uint8_t j = 0; j < BOARD_COLUMN; ++j) {
            if (list_of_buttons[i * BOARD_COLUMN + j]->getPosition() == wrong_highlight_pos) {
                if (i % 2 == j % 2) {
                    my_palette.setColor(QPalette::Button, QColor(240, 240, 240));
                } else {
                    my_palette.setColor(QPalette::Button, QColor(110, 10, 0));
                }

                list_of_buttons[i * BOARD_COLUMN + j]->getPushbutton()->setPalette(my_palette);
            }
        }
    }
}

void Chess_board::pushbutton_clicked(SquareButton *squarebutton)
{
    uint8_t choosen_position = squarebutton->getPosition();

    static SquareButton *current_button = nullptr;

    if (current_button) {
        if (current_button == squarebutton) {
            current_button = nullptr;

            clear_highlight();
        } else {
            bool is_click_right = false;

            for (qsizetype i = 0; i < possible_moves.length(); ++i) {
                if (choosen_position == possible_moves[i])
                    is_click_right = true;
            }

            if (is_click_right) {
                second_position = choosen_position;

                my_simulator->make_move(first_position, second_position);
                draw_move(first_position, second_position);

                clear_highlight();
                possible_moves.clear();
                current_button = nullptr;

                disactivate_squares();
            } else {
                set_wrong_highlight(choosen_position);
            }
        }
    } else {
        QMap<uint8_t, Figure_Color> list_of_colors = my_simulator->get_list_of_squares();

        if (list_of_colors[choosen_position] == my_color) {
            current_button = squarebutton;

            first_position = choosen_position;

            possible_moves = my_simulator->possible_move(choosen_position);

            set_all_highlights(choosen_position);
        } else {
            set_wrong_highlight(choosen_position);
        }
    }
}

void Chess_board::draw_board_for_white(uint8_t i, uint8_t j)
{
    text_labels_for_white();

    QPushButton *pushbutton = list_of_buttons[i * BOARD_COLUMN + j]->getPushbutton();

    switch (i) {
    case 0:
        switch (j) {
        case 0:
            pushbutton->setIcon(QIcon("ч_ладья.png"));
            break;

        case 1:
            pushbutton->setIcon(QIcon("ч_конь.png"));
            break;

        case 2:
            pushbutton->setIcon(QIcon("ч_слон.png"));
            break;

        case 3:
            pushbutton->setIcon(QIcon("ч_король.png"));
            break;

        case 4:
            pushbutton->setIcon(QIcon("ч_ферзь.png"));
            break;

        case 5:
            pushbutton->setIcon(QIcon("ч_слон.png"));
            break;

        case 6:
            pushbutton->setIcon(QIcon("ч_конь.png"));
            break;

        case 7:
            pushbutton->setIcon(QIcon("ч_ладья.png"));
            break;

        }

        break;

    case 1:
        pushbutton->setIcon(QIcon("ч_пешка.png"));
        break;

    case 6:
        pushbutton->setIcon(QIcon("б_пешка.png"));
        break;

    case 7:
        switch (j) {
        case 0:
            pushbutton->setIcon(QIcon("б_ладья.png"));
            break;

        case 1:
            pushbutton->setIcon(QIcon("б_конь.png"));
            break;

        case 2:
            pushbutton->setIcon(QIcon("б_слон.png"));
            break;

        case 3:
            pushbutton->setIcon(QIcon("б_король.png"));
            break;

        case 4:
            pushbutton->setIcon(QIcon("б_ферзь.png"));
            break;

        case 5:
            pushbutton->setIcon(QIcon("б_слон.png"));
            break;

        case 6:
            pushbutton->setIcon(QIcon("б_конь.png"));
            break;

        case 7:
            pushbutton->setIcon(QIcon("б_ладья.png"));
            break;

        }
        break;
    }
}

void Chess_board::draw_board_for_black(uint8_t i, uint8_t j)
{
    text_labels_for_black();

    QPushButton *pushbutton = list_of_buttons[i * BOARD_COLUMN + j]->getPushbutton();

    switch (i) {
    case 0:
        switch (j) {
        case 0:
            pushbutton->setIcon(QIcon("б_ладья.png"));
            break;

        case 1:
            pushbutton->setIcon(QIcon("б_конь.png"));
            break;

        case 2:
            pushbutton->setIcon(QIcon("б_слон.png"));
            break;

        case 3:
            pushbutton->setIcon(QIcon("б_ферзь.png"));
            break;

        case 4:
            pushbutton->setIcon(QIcon("б_король.png"));
            break;

        case 5:
            pushbutton->setIcon(QIcon("б_слон.png"));
            break;

        case 6:
            pushbutton->setIcon(QIcon("б_конь.png"));
            break;

        case 7:
            pushbutton->setIcon(QIcon("б_ладья.png"));
            break;

        }

        break;

    case 1:
        pushbutton->setIcon(QIcon("б_пешка.png"));
        break;

    case 6:
        pushbutton->setIcon(QIcon("ч_пешка.png"));
        break;

    case 7:
        switch (j) {
        case 0:
            pushbutton->setIcon(QIcon("ч_ладья.png"));
            break;

        case 1:
            pushbutton->setIcon(QIcon("ч_конь.png"));
            break;

        case 2:
            pushbutton->setIcon(QIcon("ч_слон.png"));
            break;

        case 3:
            pushbutton->setIcon(QIcon("ч_ферзь.png"));
            break;

        case 4:
            pushbutton->setIcon(QIcon("ч_король.png"));
            break;

        case 5:
            pushbutton->setIcon(QIcon("ч_слон.png"));
            break;

        case 6:
            pushbutton->setIcon(QIcon("ч_конь.png"));
            break;

        case 7:
            pushbutton->setIcon(QIcon("ч_ладья.png"));
            break;

        }
        break;
    }
}

void Chess_board::text_labels_for_white()
{
    for (uint8_t i = 0; i < 10; ++i) {
        for (uint8_t j = 0; j < 10; ++j) {
        switch(i) {
        case 0:
            switch(j) {
            case 1:
                list_of_labels[i * 10 + j]->setText("A");
                break;

            case 2:
                list_of_labels[i * 10 + j]->setText("B");
                break;

            case 3:
                list_of_labels[i * 10 + j]->setText("C");
                break;

            case 4:
                list_of_labels[i * 10 + j]->setText("D");
                break;

            case 5:
                list_of_labels[i * 10 + j]->setText("E");
                break;

            case 6:
                list_of_labels[i * 10 + j]->setText("F");
                break;

            case 7:
                list_of_labels[i * 10 + j]->setText("G");
                break;

            case 8:
                list_of_labels[i * 10 + j]->setText("H");
                break;

            default:
                break;
            }

            break;

        case 1:
            if (j == 0 || j == 9) {
                list_of_labels[i * 10 + j]->setText("8");
            }
            break;

        case 2:
            if (j == 0 || j == 9) {
                list_of_labels[i * 10 + j]->setText("7");
            }
            break;

        case 3:
            if (j == 0 || j == 9) {
                list_of_labels[i * 10 + j]->setText("6");
            }
            break;

        case 4:
            if (j == 0 || j == 9) {
                list_of_labels[i * 10 + j]->setText("5");
            }
            break;

        case 5:
            if (j == 0 || j == 9) {
                list_of_labels[i * 10 + j]->setText("4");
            }
            break;

        case 6:
            if (j == 0 || j == 9) {
                list_of_labels[i * 10 + j]->setText("3");
            }
            break;

        case 7:
            if (j == 0 || j == 9) {
                list_of_labels[i * 10 + j]->setText("2");
            }
            break;

        case 8:
            if (j == 0 || j == 9) {
                list_of_labels[i * 10 + j]->setText("1");
            }
            break;

        case 9:
            switch(j) {
            case 1:
                list_of_labels[i * 10 + j]->setText("A");
                break;

            case 2:
                list_of_labels[i * 10 + j]->setText("B");
                break;

            case 3:
                list_of_labels[i * 10 + j]->setText("C");
                break;

            case 4:
                list_of_labels[i * 10 + j]->setText("D");
                break;

            case 5:
                list_of_labels[i * 10 + j]->setText("E");
                break;

            case 6:
                list_of_labels[i * 10 + j]->setText("F");
                break;

            case 7:
                list_of_labels[i * 10 + j]->setText("G");
                break;

            case 8:
                list_of_labels[i * 10 + j]->setText("H");
                break;

            default:
                break;
            }

            break;
        }
        }
    }
}

void Chess_board::text_labels_for_black()
{
    for (uint8_t i = 0; i < 10; ++i) {
        for (uint8_t j = 0; j < 10; ++j) {
        switch(i) {
        case 0:
            switch(j) {
            case 1:
                list_of_labels[i * 10 + j]->setText("H");
                break;

            case 2:
                list_of_labels[i * 10 + j]->setText("G");
                break;

            case 3:
                list_of_labels[i * 10 + j]->setText("F");
                break;

            case 4:
                list_of_labels[i * 10 + j]->setText("E");
                break;

            case 5:
                list_of_labels[i * 10 + j]->setText("D");
                break;

            case 6:
                list_of_labels[i * 10 + j]->setText("C");
                break;

            case 7:
                list_of_labels[i * 10 + j]->setText("B");
                break;

            case 8:
                list_of_labels[i * 10 + j]->setText("A");
                break;

            default:
                break;
            }

            break;

        case 1:
            if (j == 0 || j == 9) {
                list_of_labels[i * 10 + j]->setText("1");
            }
            break;

        case 2:
            if (j == 0 || j == 9) {
                list_of_labels[i * 10 + j]->setText("2");
            }
            break;

        case 3:
            if (j == 0 || j == 9) {
                list_of_labels[i * 10 + j]->setText("3");
            }
            break;

        case 4:
            if (j == 0 || j == 9) {
                list_of_labels[i * 10 + j]->setText("4");
            }
            break;

        case 5:
            if (j == 0 || j == 9) {
                list_of_labels[i * 10 + j]->setText("5");
            }
            break;

        case 6:
            if (j == 0 || j == 9) {
                list_of_labels[i * 10 + j]->setText("6");
            }
            break;

        case 7:
            if (j == 0 || j == 9) {
                list_of_labels[i * 10 + j]->setText("7");
            }
            break;

        case 8:
            if (j == 0 || j == 9) {
                list_of_labels[i * 10 + j]->setText("8");
            }
            break;

        case 9:
            switch(j) {
            case 1:
                list_of_labels[i * 10 + j]->setText("H");
                break;

            case 2:
                list_of_labels[i * 10 + j]->setText("G");
                break;

            case 3:
                list_of_labels[i * 10 + j]->setText("F");
                break;

            case 4:
                list_of_labels[i * 10 + j]->setText("E");
                break;

            case 5:
                list_of_labels[i * 10 + j]->setText("D");
                break;

            case 6:
                list_of_labels[i * 10 + j]->setText("C");
                break;

            case 7:
                list_of_labels[i * 10 + j]->setText("B");
                break;

            case 8:
                list_of_labels[i * 10 + j]->setText("A");
                break;

            default:
                break;
            }

            break;
        }
        }
    }
}

void Chess_board::activate_squares()
{
    for (uint8_t i = 0; i < BOARD_SIZE; ++i) {
        list_of_buttons[i]->activate_square();
    }
}

void Chess_board::disactivate_squares()
{
    for (uint8_t i = 0; i < BOARD_SIZE; ++i) {
        list_of_buttons[i]->disactivate_square();
    }
}

void Chess_board::connect_buttons_to_slot()
{
    for (uint8_t i = 0; i < BOARD_SIZE; ++i) {
        QObject::connect(list_of_buttons[i],
                         SIGNAL(clicked(SquareButton*)),
                         this,
                         SLOT(pushbutton_clicked(SquareButton*)));
    }
}
