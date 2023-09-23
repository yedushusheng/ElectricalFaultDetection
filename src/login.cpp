#include "login.h"
#include "ui_login.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QObject>

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    setWindowTitle(QObject::tr("用户登陆"));
    ui->passwordEdit->setFocus();
    ui->loginButton->setDefault(true);
}

login::~login()
{
    delete ui;
}

void login::on_loginButton_clicked()
{
    if(ui->passwordEdit->text().isEmpty())
    {
        QMessageBox::information(this, QObject::tr("请输入密码"),QObject::tr("请输入密码再登录"), QMessageBox::Ok);
        ui->passwordEdit->setFocus();
    }
    else
    {
        QSqlQuery query;
        query.exec("select pwd from password");
        query.next();
        if(query.value(0).toString() == ui->passwordEdit->text())
        {
            QDialog::accept();//进入程序界面
        }
        else
        {
            QMessageBox::warning(this, tr("密码错误"), tr("请输入正确密码后再登录"),QMessageBox::Ok);
            ui->passwordEdit->clear();
            ui->passwordEdit->setFocus();//给一个输入焦点
        }
    }
}

void login::on_quitButton_clicked()
{
    QDialog::reject();//隐藏对话框，发送拒绝信号
}
