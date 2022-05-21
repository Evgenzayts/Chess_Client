#include "simulator.h"

Simulator::Simulator()
    : is_first_player(false) {}

void Simulator::set_player_color(Figure_Color _my_color)
{
    my_color = _my_color;
}

QVector<uint8_t> Simulator::possible_move(uint8_t position)
{
    QVector<uint8_t> possible_moves;

    switch (list_of_figures[position]) {
    case PAWN:
        possible_moves = possibilities_of_pawn(position);
        break;

    case ROOK:
        possible_moves = possibilities_of_rook(position);
        break;

    case KNIGHT:
        possible_moves = possibilities_of_knight(position);
        break;

    case BISHOP:
        possible_moves = possibilities_of_bishop(position);
        break;

    case QUEEN:
        possible_moves = possibilities_of_queen(position);
        break;

    case KING:
        possible_moves = possibilities_of_king(position);
        break;

    default:
        break;
    }

    return possible_moves;
}

void Simulator::make_move(uint8_t first_position, uint8_t second_position)
{
//    if (list_of_figures[first_position] == KING)
//        emit win_sig();

    list_of_figures[second_position] = list_of_figures[first_position];
    list_of_figures[first_position] = NONE_FIGURE;

    list_of_squares[second_position] = list_of_squares[first_position];
    list_of_squares[first_position] = NONE_COLOR;

//    if (second_position < 8 && list_of_figures[second_position] == PAWN) {
//        emit change_figure_sig(second_position);
//    }
}

void Simulator::set_my_start_figures()
{
    for (uint8_t i = 0; i < 8; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            list_of_figures[i * 8 + j] = NONE_FIGURE;
            list_of_squares[i * 8 + j] = NONE_COLOR;

            switch (i) {
            case 0:
                if (my_color == WHITE) {
                    list_of_squares[i * 8 + j] = BLACK;
                } else {
                    list_of_squares[i * 8 + j] = WHITE;
                }

                switch (j) {
                case 0:
                    list_of_figures[i * 8 + j] = ROOK;
                    break;

                case 1:
                    list_of_figures[i * 8 + j] = KNIGHT;
                    break;

                case 2:
                    list_of_figures[i * 8 + j] = BISHOP;
                    break;

                case 3:
                    if (my_color == WHITE) {
                        list_of_figures[i * 8 + j] = QUEEN;
                    } else {
                        list_of_figures[i * 8 + j] = KING;
                    }
                    break;

                case 4:
                    if (my_color == WHITE) {
                        list_of_figures[i * 8 + j] = KING;
                    } else {
                        list_of_figures[i * 8 + j] = QUEEN;
                    }
                    break;

                case 5:
                    list_of_figures[i * 8 + j] = BISHOP;
                    break;

                case 6:
                    list_of_figures[i * 8 + j] = KNIGHT;
                    break;

                case 7:
                    list_of_figures[i * 8 + j] = ROOK;
                    break;

                }
                break;

            case 1:
                if (my_color == WHITE) {
                    list_of_squares[i * 8 + j] = BLACK;
                } else {
                    list_of_squares[i * 8 + j] = WHITE;
                }

                list_of_figures[i * 8 + j] = PAWN;
                break;

            case 6:
                if (my_color == WHITE) {
                    list_of_squares[i * 8 + j] = WHITE;
                } else {
                    list_of_squares[i * 8 + j] = BLACK;
                }

                list_of_figures[i * 8 + j] = PAWN;
                break;

            case 7:

                if (my_color == WHITE) {
                    list_of_squares[i * 8 + j] = WHITE;
                } else {
                    list_of_squares[i * 8 + j] = BLACK;
                }

                switch (j) {
                case 0:
                    list_of_figures[i * 8 + j] = ROOK;
                    break;

                case 1:
                    list_of_figures[i * 8 + j] = KNIGHT;
                    break;

                case 2:
                    list_of_figures[i * 8 + j] = BISHOP;
                    break;

                case 3:
                    if (my_color == WHITE) {
                        list_of_figures[i * 8 + j] = QUEEN;
                    } else {
                        list_of_figures[i * 8 + j] = KING;
                    }
                    break;

                case 4:
                    if (my_color == WHITE) {
                        list_of_figures[i * 8 + j] = KING;
                    } else {
                        list_of_figures[i * 8 + j] = QUEEN;
                    }
                    break;

                case 5:
                    list_of_figures[i * 8 + j] = BISHOP;
                    break;

                case 6:
                    list_of_figures[i * 8 + j] = KNIGHT;
                    break;

                case 7:
                    list_of_figures[i * 8 + j] = ROOK;
                    break;

                }
                break;
            }
        }
    }
}

QVector<uint8_t> Simulator::possibilities_of_pawn(uint8_t position)
{
    QVector<uint8_t> possible_moves;
    Figure_Color enemy_color = (my_color == WHITE) ? BLACK : WHITE;

    bool is_first_move = false;

    if (position >= 48 && position < 56)
        is_first_move = true;

    if (is_first_move) {
        uint8_t i = 1;
        while (i <= 2 && position - 8 * i >= 0 && list_of_figures[position - 8 * i] == NONE_FIGURE) {
            possible_moves.append(position - 8 * i);
            ++i;
        }

    } else {
        if (position - 8 >= 0 && list_of_figures[position - 8] == NONE_FIGURE) {
            possible_moves.append(position - 8);
        }
    }


    if (position - 7 >= 0 && position % 8 != 7 && list_of_squares[position - 7] == enemy_color) {
        possible_moves.append(position - 7);
    }

    if (position - 9 >= 0 && position % 8 != 0 && list_of_squares[position - 9] == enemy_color) {
        possible_moves.append(position - 9);
    }

    return possible_moves;
}

QVector<uint8_t> Simulator::possibilities_of_rook(uint8_t position)
{
    QVector<uint8_t> possible_moves;
    Figure_Color enemy_color = (my_color == WHITE) ? BLACK : WHITE;

    uint8_t i = 1;

    while (position - i * 8 >= 0 && (list_of_figures[position - i * 8] == NONE_FIGURE ||
                                     list_of_squares[position - i * 8] == enemy_color)) {
        possible_moves.append(position - i * 8);

        if (list_of_squares[position - i * 8] == enemy_color)
            break;

        ++i;
    }

    i = 1;
    while (position + i * 8 <= 63 && (list_of_figures[position + i * 8] == NONE_FIGURE ||
                                      list_of_squares[position + i * 8] == enemy_color)) {
        possible_moves.append(position + i * 8);

        if (list_of_squares[position + i * 8] == enemy_color)
            break;

        ++i;
    }

    i = 1;
    if (position % 8 != 0) {
        while (list_of_figures[position - i] == NONE_FIGURE || list_of_squares[position - i] == enemy_color) {

            possible_moves.append(position - i);

            if ((position - i) % 8 == 0 || list_of_squares[position - i] == enemy_color)
                break;

            ++i;
        }
    }

    i = 1;
    if (position % 8 != 7) {
        while (list_of_figures[position + i] == NONE_FIGURE || list_of_squares[position + i] == enemy_color) {
            possible_moves.append(position + i);

            if ((position + i) % 8 == 7 || list_of_squares[position + i] == enemy_color)
                break;

            ++i;
        }
    }

    return possible_moves;
}

QVector<uint8_t> Simulator::possibilities_of_knight(uint8_t position)
{
    QVector<uint8_t> possible_moves;
    Figure_Color enemy_color = (my_color == WHITE) ? BLACK : WHITE;

    if (position % 8 > 1  && position >= 10 && (list_of_figures[position - 10] == NONE_FIGURE ||
                                                list_of_squares[position - 10] == enemy_color)) {
        possible_moves.append(position - 10);
    }

    if (position % 8 != 0  && position >= 17 && (list_of_figures[position - 17] == NONE_FIGURE ||
                                                 list_of_squares[position - 17] == enemy_color)) {
        possible_moves.append(position - 17);
    }

    if (position % 8 != 7  && position >= 15 && (list_of_figures[position - 15] == NONE_FIGURE ||
                                                 list_of_squares[position - 15] == enemy_color)) {
        possible_moves.append(position - 15);
    }

    if (position % 8 < 6  && position >= 6 && (list_of_figures[position - 6] == NONE_FIGURE ||
                                               list_of_squares[position - 6] == enemy_color)) {
        possible_moves.append(position - 6);
    }

    if (position % 8 < 6  && position <= 53 && (list_of_figures[position + 10] == NONE_FIGURE ||
                                                list_of_squares[position + 10] == enemy_color)) {
        possible_moves.append(position + 10);
    }

    if (position % 8 != 7  && position <= 46 && (list_of_figures[position + 17] == NONE_FIGURE ||
                                                 list_of_squares[position + 17] == enemy_color)) {
        possible_moves.append(position + 17);
    }

    if (position % 8 != 0  && position <= 49 && (list_of_figures[position + 15] == NONE_FIGURE ||
                                                 list_of_squares[position + 15] == enemy_color)) {
        possible_moves.append(position + 15);
    }

    if (position % 8 > 1  && position <= 57 && (list_of_figures[position + 6] == NONE_FIGURE ||
                                                list_of_squares[position + 6] == enemy_color)) {
        possible_moves.append(position + 6);
    }

    return possible_moves;
}

QVector<uint8_t> Simulator::possibilities_of_bishop(uint8_t position)
{
    QVector<uint8_t> possible_moves;
    Figure_Color enemy_color = (my_color == WHITE) ? BLACK : WHITE;

    uint8_t i = 1;

    if (position % 8 != 7) {
        while (position - i * 7 >= 0 && (list_of_figures[position - i * 7] == NONE_FIGURE ||
                                         list_of_squares[position - i * 7] == enemy_color)) {
            possible_moves.append(position - i * 7);

            if ((position - i * 7) % 8 == 7 || list_of_squares[position - i * 7] == enemy_color)
                break;

            ++i;
        }
    }

    i = 1;
    if (position % 8 != 0) {
        while (position - i * 9 >= 0 && (list_of_figures[position - i * 9] == NONE_FIGURE ||
                                         list_of_squares[position - i * 9] == enemy_color)) {
            possible_moves.append(position - i * 9);

            if ((position - i * 9) % 8 == 0 || list_of_squares[position - i * 9] == enemy_color)
                break;

            ++i;
        }
    }

    i = 1;
    if (position % 8 != 0) {
        while (position + i * 7 <= 63 && (list_of_figures[position + i * 7] == NONE_FIGURE ||
                                          list_of_squares[position + i * 7] == enemy_color)) {
            possible_moves.append(position + i * 7);

            if ((position + i * 7) % 8 == 0 || list_of_squares[position + i * 7] == enemy_color)
                break;

            ++i;
        }
    }

    i = 1;
    if (position % 8 != 7) {
        while (position + i * 9 <= 63 && (list_of_figures[position + i * 9] == NONE_FIGURE ||
                                          list_of_squares[position + i * 9] == enemy_color)) {
            possible_moves.append(position + i * 9);

            if ((position + i * 9) % 8 == 7 || list_of_squares[position + i * 9] == enemy_color)
                break;

            ++i;
        }
    }

    return possible_moves;
}

QVector<uint8_t> Simulator::possibilities_of_queen(uint8_t position)
{
    QVector<uint8_t> possible_moves;
    Figure_Color enemy_color = (my_color == WHITE) ? BLACK : WHITE;

    uint8_t i = 1;

    while (position - i * 8 >= 0 && (list_of_figures[position - i * 8] == NONE_FIGURE ||
                                     list_of_squares[position - i * 8] == enemy_color)) {
        possible_moves.append(position - i * 8);

        if (list_of_squares[position - i * 8] == enemy_color)
            break;

        ++i;
    }

    i = 1;
    while (position + i * 8 <= 63 && (list_of_figures[position + i * 8] == NONE_FIGURE ||
                                      list_of_squares[position + i * 8] == enemy_color)) {
        possible_moves.append(position + i * 8);

        if (list_of_squares[position + i * 8] == enemy_color)
            break;

        ++i;
    }

    i = 1;
    if (position % 8 != 0) {
        while (list_of_figures[position - i] == NONE_FIGURE || list_of_squares[position - i] == enemy_color) {

            possible_moves.append(position - i);

            if ((position - i) % 8 == 0 || list_of_squares[position - i] == enemy_color)
                break;

            ++i;
        }
    }

    i = 1;
    if (position % 8 != 7) {
        while (list_of_figures[position + i] == NONE_FIGURE || list_of_squares[position + i] == enemy_color) {
            possible_moves.append(position + i);

            if ((position + i) % 8 == 7 || list_of_squares[position + i] == enemy_color)
                break;

            ++i;
        }
    }

    i = 1;
    if (position % 8 != 7) {
        while (position - i * 7 >= 0 && (list_of_figures[position - i * 7] == NONE_FIGURE ||
                                         list_of_squares[position - i * 7] == enemy_color)) {
            possible_moves.append(position - i * 7);

            if ((position - i * 7) % 8 == 7 || list_of_squares[position - i * 7] == enemy_color)
                break;

            ++i;
        }
    }

    i = 1;
    if (position % 8 != 0) {
        while (position - i * 9 >= 0 && (list_of_figures[position - i * 9] == NONE_FIGURE ||
                                         list_of_squares[position - i * 9] == enemy_color)) {
            possible_moves.append(position - i * 9);

            if ((position - i * 9) % 8 == 0 || list_of_squares[position - i * 9] == enemy_color)
                break;

            ++i;
        }
    }

    i = 1;
    if (position % 8 != 0) {
        while (position + i * 7 <= 63 && (list_of_figures[position + i * 7] == NONE_FIGURE ||
                                          list_of_squares[position + i * 7] == enemy_color)) {
            possible_moves.append(position + i * 7);

            if ((position + i * 7) % 8 == 0 || list_of_squares[position + i * 7] == enemy_color)
                break;

            ++i;
        }
    }

    i = 1;
    if (position % 8 != 7) {
        while (position + i * 9 <= 63 && (list_of_figures[position + i * 9] == NONE_FIGURE ||
                                          list_of_squares[position + i * 9] == enemy_color)) {
            possible_moves.append(position + i * 9);

            if ((position + i * 9) % 8 == 7 || list_of_squares[position + i * 9] == enemy_color)
                break;

            ++i;
        }
    }

    return possible_moves;
}

QVector<uint8_t> Simulator::possibilities_of_king(uint8_t position)
{
    QVector<uint8_t> possible_moves;
    Figure_Color enemy_color = (my_color == WHITE) ? BLACK : WHITE;

    if (position - 8 >= 0 && (list_of_figures[position - 8] == NONE_FIGURE ||
                              list_of_squares[position - 8] == enemy_color)) {
        possible_moves.append(position - 8);
    }

    if (position + 8 <= 63 && (list_of_figures[position + 8] == NONE_FIGURE ||
                               list_of_squares[position + 8] == enemy_color)) {
        possible_moves.append(position + 8);
    }

    if (position % 8 != 0 && position - 1 >= 0 && (list_of_figures[position - 1] == NONE_FIGURE ||
                                                   list_of_squares[position - 1] == enemy_color)) {
        possible_moves.append(position - 1);
    }

    if (position % 8 != 7 && position + 1 <= 63 && (list_of_figures[position + 1] == NONE_FIGURE ||
                                                    list_of_squares[position + 1] == enemy_color)) {
        possible_moves.append(position + 1);
    }

    return possible_moves;
}

QMap<uint8_t, Figure_Color> Simulator::get_list_of_squares() const
{
    return list_of_squares;
}
