#include "diagram.h"
#include "ui_diagram.h"
#include <QObject>
#include <QEvent>

diagram::diagram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::diagram)
{
    ui->setupUi(this);
    setWindowTitle(QObject::tr("˛¨ĐÎˇÖÎö˝ášűĎÔĘž"));//增加中文字符支持
}

diagram::~diagram()
{
    delete ui;
}


