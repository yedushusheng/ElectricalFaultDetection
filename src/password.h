#ifndef PASSWORD_H
#define PASSWORD_H

#include <QDialog>

namespace Ui {
class password;
}

class password : public QDialog
{
    Q_OBJECT

public:
    explicit password(QWidget *parent = 0);
    ~password();

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::password *ui;
};

#endif // PASSWORD_H
