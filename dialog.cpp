#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(ui->csvButton,&QPushButton::clicked,this,[&](){emit which_to_click(0);});
    connect(ui->excelButton,&QPushButton::clicked,this,[&](){emit which_to_click(1);});
    connect(ui->txtButton,&QPushButton::clicked,this,[&](){emit which_to_click(2);});
    connect(this,SIGNAL(which_to_click(int)),parent,SLOT(exportData(int)));

    connect(ui->csvButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->excelButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->txtButton,SIGNAL(clicked()),this,SLOT(close()));
}

Dialog::~Dialog()
{
    delete ui;
}
