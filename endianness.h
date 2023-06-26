#ifndef ENDIANNESS_H
#define ENDIANNESS_H

#include <QDialog>

namespace Ui {
class Endianness;
}

class Endianness : public QDialog
{
    Q_OBJECT
signals: void bigOrlittle(bool); // 0 for little, 1 for big
public:
    explicit Endianness(QWidget *parent = nullptr);
    ~Endianness();

private:
    Ui::Endianness *ui;
};

#endif // ENDIANNESS_H
