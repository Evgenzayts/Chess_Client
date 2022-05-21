#ifndef SQUAREBUTTON_H
#define SQUAREBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QObject>

class SquareButton : public QWidget
{
    Q_OBJECT
public:
    SquareButton(QWidget *parent = nullptr);

    SquareButton(const SquareButton &squarebutton);

    uint8_t getPosition() const;
    void setPosition(const uint8_t &value);

    QPushButton *getPushbutton() const;
    void setPushbutton(QPushButton *value);

    void activate_square();

    void disactivate_square();

    ~SquareButton() {
        delete pushbutton;
    }

public slots:
    void pushbutton_clicked();

signals:
    void clicked(SquareButton*);

private:
    uint8_t position;

    QPushButton *pushbutton;
};

#endif // SQUAREBUTTON_H
