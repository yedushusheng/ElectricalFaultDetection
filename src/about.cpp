#include "about.h"
#include "ui_about.h"
#include <QDebug>
#include <QObject>

about::about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);
    setWindowTitle(QObject::tr("¹ØÓÚÈí¼ş"));
}

about::~about()
{
    delete ui;
}




