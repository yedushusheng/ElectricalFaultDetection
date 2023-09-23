#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QDesktopServices>//用于访问桌面服务的类
#include "about.h"
#include <QCloseEvent>//当程序需要关闭时所处理的内容，所有的事件都是受保护的成员函数，即protected

#include "qcustomplot.h"
#include <QInputDialog>

#include <QVector>
#include <QWidget>
#include <QMap>

//#include "fft.h"
#include "diagram.h"
#include "connection.h"

#include "login.h"
#include "password.h"

#include <QGLWidget>

#define SINT short int
#define SumPot 16384

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool readFile(QString fileName);    //读取文件内容
    int QByteArrayToInt(QByteArray bytes);  //QByteArray转int
    const char* getString(const QByteArray ba);

protected:
    void closeEvent(QCloseEvent *);
    void paintEvent(QPaintEvent *event);

private slots:
    //第一个菜单栏槽函数定义
    void on_action_O_triggered();
    void on_action_A_triggered();
    void on_action_P_triggered();
    void on_action_Q_triggered();

    //第二个菜单栏槽函数定义
    void on_action_wave_triggered();
    void on_action_spec_triggered();
    void on_action_2D_triggered();
    void on_action_3D_triggered();

    //第三个菜单栏槽函数定义
    void on_action_zoom_in_triggered();
    void on_action_zoom_out_triggered();
    void on_action_rotate_triggered();
    void on_action_clear_triggered();

    //第四个菜单栏槽函数定义
    void aboutSoftwareSlot();//弹出“关于软件”的子对话框
    void aboutWebsiteSlot();//打开一个网址，使用的是默认的浏览器

    //第三方库的相关定义
    void titleDoubleClick(QMouseEvent *event, QCPPlotTitle *title);
    void axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part);
    void legendDoubleClick(QCPLegend* legend, QCPAbstractLegendItem* item);
    void selectionChanged();
    void mousePress();
    void mouseWheel();
    void addWaveGraph();
    void addSpecGraph();
    void removeSelectedGraph();
    void removeAllGraphs();
    void contextMenuRequest(QPoint pos);
    void moveLegend();
    void graphClicked(QCPAbstractPlottable *plottable);

    int on_action_result_triggered();
    void on_action_login_triggered();
    void on_action_password_triggered();

private:
    Ui::MainWindow *ui;

    struct wav_struct
    {
        unsigned long file_size;        //文件大小
        unsigned short channel;            //通道数
        unsigned long frequency;        //采样频率
        unsigned long Bps;                //Byte率
        unsigned short sample_num_bit;    //一个样本的位数
        int data_size;        //数据大小
        unsigned char *data;            //音频数据 ,这里要定义什么就看样本位数了，我这里只是单纯的复制数据
    };

    QVector<QPointF>  curveMap;//存储曲线图上的定点
};

#endif // MAINWINDOW_H
