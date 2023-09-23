#include "password.h"
#include "ui_password.h"
#include "connection.h"

password::password(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::password)
{
    ui->setupUi(this);
    setWindowTitle(QObject::tr("修改登录密码"));
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
        QMessageBox::warning(this, QObject::tr("请输入密码"), QObject::tr("请输入旧密码和新密码"), QMessageBox::Ok);
    else if(ui->oldPasswordLineEdit->text() != query.value(0).toString())
    {
        QMessageBox::warning(this, QObject::tr("密码错误"), QObject::tr("输入的旧密码错误"), QMessageBox::Ok);
        ui->oldPasswordLineEdit->setFocus();
    }
    else
    {
        if(ui->newPasswordLineEdit->text() == ui->surePasswordlineEdit->text())
        {
            QString newPassword = ui->newPasswordLineEdit->text();
            query.exec(QString("update password set pwd=%1").arg(newPassword));//更新密码
            QMessageBox::information(this, QObject::tr("修改密码"), QObject::tr("修改密码成功！"), QMessageBox::Ok);
        }
        else
            QMessageBox::warning(this, QObject::tr("修改密码失败"), QObject::tr("新密码两次输入不一致！"), QMessageBox::Ok);
    }
}

void password::on_cancelButton_clicked()
{
    ui->oldPasswordLineEdit->clear();
    ui->oldPasswordLineEdit->setFocus();//把输入焦点放在旧密码处，方便重新输入
    ui->newPasswordLineEdit->clear();
    ui->surePasswordlineEdit->clear();
}
