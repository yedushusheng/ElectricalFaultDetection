#include "diagram.h"
#include "ui_diagram.h"
#include <QObject>
#include <QEvent>

diagram::diagram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::diagram)
{
    ui->setupUi(this);
    setWindowTitle(QObject::tr("波形分析结果显示"));
}

diagram::~diagram()
{
    delete ui;
}


