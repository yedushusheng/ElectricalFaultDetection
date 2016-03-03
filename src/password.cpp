#include "password.h"
#include "ui_password.h"
#include "connection.h"

password::password(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::password)
{
    ui->setupUi(this);
    setWindowTitle(QObject::tr("ÐÞ¸ÄµÇÂ¼ÃÜÂë"));
}

password::~password()
{
    delete ui;
}

void password::on_okButton_clicked()
{
    QSqlQuery query;
    query.exec("select pwd from password");
    query.next();
    if(ui->oldPasswordLineEdit->text().isEmpty())
        QMessageBox::warning(this, QObject::tr("ÇëÊäÈëÃÜÂë"), QObject::tr("ÇëÊäÈë¾ÉÃÜÂëºÍÐÂÃÜÂë"), QMessageBox::Ok);
    else if(ui->oldPasswordLineEdit->text() != query.value(0).toString())
    {
        QMessageBox::warning(this, QObject::tr("ÃÜÂë´íÎó"), QObject::tr("ÊäÈëµÄ¾ÉÃÜÂë´íÎó"), QMessageBox::Ok);
        ui->oldPasswordLineEdit->setFocus();
    }
    else
    {
        if(ui->newPasswordLineEdit->text() == ui->surePasswordlineEdit->text())
        {
            QString newPassword = ui->newPasswordLineEdit->text();
            query.exec(QString("update password set pwd=%1").arg(newPassword));//¸üÐÂÃÜÂë
            QMessageBox::information(this, QObject::tr("ÐÞ¸ÄÃÜÂë"), QObject::tr("ÐÞ¸ÄÃÜÂë³É¹¦£¡"), QMessageBox::Ok);
        }
        else
            QMessageBox::warning(this, QObject::tr("ÐÞ¸ÄÃÜÂëÊ§°Ü"), QObject::tr("ÐÂÃÜÂëÁ½´ÎÊäÈë²»Ò»ÖÂ£¡"), QMessageBox::Ok);
    }
}

void password::on_cancelButton_clicked()
{
    ui->oldPasswordLineEdit->clear();
        ui->oldPasswordLineEdit->setFocus();//°ÑÊäÈë½¹µã·ÅÔÚ¾ÉÃÜÂë´¦£¬·½±ãÖØÐÂÊäÈë
        ui->newPasswordLineEdit->clear();
        ui->surePasswordlineEdit->clear();
}
