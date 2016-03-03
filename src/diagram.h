#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <QDialog>
#include <QEvent>
#include <QColor>
#include <QPoint>
#include <QtXml/QDomDocument>
#include <QStandardItemModel>

namespace Ui {
class diagram;
}

class diagram : public QDialog
{
    Q_OBJECT
    
public:
    explicit diagram(QWidget *parent = 0);
    ~diagram();

    enum DataTimeType{Time, Date, DateTime};//定义时间的枚举类型
    QString getDataTime(DataTimeType type);
    
private:
    Ui::diagram *ui;

    QDomDocument doc;
    QStandardItemModel *chartModel;
    bool docRead();
    bool docWrite();
    void writeXml();
    void createNodes(QDomElement &data);
    void showDailyList();

    void createChartModelView();
    void showChart();

protected:

private slots:
//    void on_sellTypeComboBox_currentIndexChanged(const QString &arg1);
//    void on_sellBrandComboBox_currentIndexChanged(const QString &arg1);
//    void on_sellNumSpinBox_valueChanged(int arg1);
//    void on_sellCancelButton_clicked();
//    void on_sellOkButton_clicked();
//    void on_typeComboBox_currentIndexChanged(const QString &arg1);
//    void on_updateButton_clicked();
//    void on_manageButton_clicked();
//    void on_chartButton_clicked();
//    void on_passwordButton_clicked();
//    void on_okButton_clicked();
//    void on_cancelButton_clicked();
};

#endif // DIAGRAM_H
