#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QMap>
#include <QList>

enum _Figure_Color {
    NONE_COLOR,
    WHITE,
    BLACK
};

typedef enum _Figure_Color Figure_Color;

enum _Figure_Type {
    NONE_FIGURE,

    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING
};

typedef enum _Figure_Type Figure_Type;

class Simulator
{
public:
    Simulator();

    void set_player_color(Figure_Color _my_color);

    QVector<uint8_t> possible_move(uint8_t position);

    void make_move(uint8_t first_position, uint8_t second_position);

    QMap<uint8_t, Figure_Color> get_list_of_squares() const;

    void set_my_start_figures();

private:
    QVector<uint8_t> possibilities_of_pawn(uint8_t position);

    QVector<uint8_t> possibilities_of_rook(uint8_t position);

    QVector<uint8_t> possibilities_of_knight(uint8_t position);

    QVector<uint8_t> possibilities_of_bishop(uint8_t position);

    QVector<uint8_t> possibilities_of_queen(uint8_t position);

    QVector<uint8_t> possibilities_of_king(uint8_t position);

signals:
    void change_figure_sig(uint8_t);

    void win_sig();

private:
    bool is_first_player;

    Figure_Color my_color;

    QMap<uint8_t, Figure_Type> list_of_figures;

    QMap<uint8_t, Figure_Color> list_of_squares;

};

#endif // SIMULATOR_H
