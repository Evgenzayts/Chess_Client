#include "squarebutton.h"

SquareButton::SquareButton(QWidget *parent)
    : QWidget(parent)
    , pushbutton(new QPushButton(this)) {}

uint8_t SquareButton::getPosition() const
{
    return position;
}

void SquareButton::setPosition(const uint8_t &value)
{
    position = value;
}

QPushButton *SquareButton::getPushbutton() const
{
    return pushbutton;
}

void SquareButton::setPushbutton(QPushButton *value)
{
    pushbutton = value;
}

void SquareButton::activate_square()
{
    connect(pushbutton, SIGNAL(clicked()), this, SLOT(pushbutton_clicked()));
}

void SquareButton::disactivate_square()
{
    disconnect(pushbutton, SIGNAL(clicked()), this, SLOT(pushbutton_clicked()));
}

void SquareButton::pushbutton_clicked()
{
    emit clicked(this);
}
