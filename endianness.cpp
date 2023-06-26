#include "endianness.h"
#include "ui_endianness.h"

Endianness::Endianness(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Endianness)
{
    ui->setupUi(this);
    connect(ui->bigButton,&QPushButton::clicked, this,[&](){emit bigOrlittle(1);});
    connect(ui->littleButton,&QPushButton::clicked,this,[&](){emit bigOrlittle(0);});

    connect(this,SIGNAL(bigOrlittle(bool)),parent,SLOT(setEndianness(bool)));

    connect(ui->bigButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->littleButton,SIGNAL(clicked()),this,SLOT(close()));
}

Endianness::~Endianness()
{
    delete ui;
}
