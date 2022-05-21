#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QMap>
#include <QTimer>
#include <QLabel>
#include <unistd.h>

#include "simulator.h"
#include "squarebutton.h"

#define BOARD_SIZE 64
#define BOARD_LINE 8
#define BOARD_COLUMN 8


class Chess_board : public QWidget
{
    Q_OBJECT
public:
    Chess_board(QWidget *parent = nullptr);

    void set_player_color(bool is_first_player);

    void draw_board();

    void ready_for_player_move();

    void set_enemy_move(uint32_t motion);

    uint32_t my_move();

    ~Chess_board() {
        delete my_simulator;
    }

public slots:
    void pushbutton_clicked(SquareButton*);

private slots:
    void clear_wrong_highlight();

private:
    void clear_highlight();

    void set_wrong_highlight(uint8_t position);

    void set_all_highlights(uint8_t position);

    void draw_move(uint8_t _first_position, uint8_t _second_position);

    void reverse_enemy_move(uint8_t &first_pos, uint8_t &second_pos);

    void setup();

    void draw_board_for_white(uint8_t i, uint8_t j);

    void draw_board_for_black(uint8_t i, uint8_t j);

    void text_labels_for_white();

    void text_labels_for_black();

    void activate_squares();

    void disactivate_squares();

    void connect_buttons_to_slot();

private:
    Simulator *my_simulator;

    Figure_Color my_color;

    QVector<SquareButton*> list_of_buttons;

    QMap<uint8_t, QLabel*> list_of_labels;

    QVector<uint8_t> possible_moves;

    QVector<uint8_t> highlighted_positions;

    uint8_t first_position, second_position;

    uint8_t wrong_highlight_pos;
};

#endif // CHESS_BOARD_H
