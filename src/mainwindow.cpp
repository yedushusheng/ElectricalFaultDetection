#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include "fft.h"
#include "login.h"
#include "password.h"

#include <QtCore>
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QTextCodec>
#include <QPainter>
#include <QPaintDevice>
#include <QMouseEvent>
#include <QProcess>
#include <QProgressDialog>
#include <QMap>
#include <QTextStream>
#include <QDataStream>
#include <qmath.h>

//数据库相关
#include <QSqlDatabase>
#include <QDebug>
#include <QStringList>
#include <QVariant>

/*-----全局变量的定义区-----*/
double global_x[SumPot],global_y[SumPot];//实际传递给绘图函数的变量（波形图绘制）
double global_x1[SumPot],global_y1[SumPot];//频谱曲线绘制数据暂存区
//有关FFT的变量声明
double show1[SumPot]={0};   //实部数组
double show2[SumPot]={0};   //虚部数组
double show3[SumPot]={0};   //用于最后显示使用的数组

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    srand(QDateTime::currentDateTime().toTime_t());
    ui->setupUi(this);

    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    setWindowTitle(QObject::tr("金属瓷柱探伤仪分析软件"));
    resize(1000,500);
    //这是我们认为调整大小，要想让绘图区和主窗体同时方法缩小，只需要将二者选中设为水平或垂直布局即可

    //信号-槽的声明
    QObject::connect(ui->action_Qt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    QObject::connect(ui->action_Website,SIGNAL(triggered()),this,SLOT(aboutWebsiteSlot()));
    QObject::connect(ui->action_about,SIGNAL(triggered()),this,SLOT(aboutSoftwareSlot()));

    ui->customPlot->clearGraphs();
    //使用第三方库进行绘图
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);

//    ui->customPlot->xAxis->setRange(0, SumPot);
//    ui->customPlot->yAxis->setRange(-1, 1);//x、y显示区域范围
//    ui->customPlot->axisRect()->setupFullAxesBox();

//    ui->customPlot->plotLayout()->insertRow(0);
//    ui->customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->customPlot, QObject::tr("频谱曲线显示")));

//    ui->customPlot->xAxis->setLabel(QObject::tr("x 轴"));
//    ui->customPlot->yAxis->setLabel(QObject::tr("y 轴"));
//    ui->customPlot->legend->setVisible(true);

    QFont legendFont = font();
    legendFont.setPointSize(10);//右上角标识曲线文字的大小
    ui->customPlot->legend->setFont(legendFont);
    ui->customPlot->legend->setSelectedFont(legendFont);
    ui->customPlot->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items

    ui->customPlot->plotLayout()->insertRow(0);
    ui->customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->customPlot, QObject::tr("频谱曲线显示")));
    //addRandomGraph();
    //addRandomGraph();
    //addRandomGraph();
    //addRandomGraph();
    //根据我们实际需要确定显示几条曲线

    // connect slot that ties some axis selections together (especially opposite axes):
    connect(ui->customPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
    // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
    connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

    // make bottom and left axes transfer their ranges to top and right axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // connect some interaction slots:
    connect(ui->customPlot, SIGNAL(titleDoubleClick(QMouseEvent*,QCPPlotTitle*)), this, SLOT(titleDoubleClick(QMouseEvent*,QCPPlotTitle*)));
    connect(ui->customPlot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
    connect(ui->customPlot, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));

    // connect slot that shows a message in the status bar when a graph is clicked:
    connect(ui->customPlot, SIGNAL(plottableClick(QCPAbstractPlottable*,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*)));

    // setup policy and connect slot for context menu popup:
    ui->customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->customPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

    int plot_width = ui->customPlot->width();
    int plot_heigh = ui->customPlot->height();//定义绘图区域的宽度和高度
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_O_triggered()
{
    //文件选择对话框
    QString fileName = QFileDialog::getOpenFileName(this,QObject::tr("文件选择对话框"),
                                                          "F:",QObject::tr("波形文件(*.wav)"));
    //判断文件是否选择，并给出提示信息对话框
    if(fileName.length()==0)//或者使用fileName.isEmpty()
    {
        QMessageBox::information(NULL,QObject::tr("文件选择"),
                                 QObject::tr("请选择波形文件！"));
    }
    else
    {
        QMessageBox::information(NULL,QObject::tr("文件选择"),
                                 QObject::tr("你已选择波形文件：")+fileName);
    }
    readFile(fileName);
}

bool MainWindow::readFile(QString fileName)
{
    //获取读取的文件
    QFile file(fileName);
    wav_struct WAV;
    if(!(file.open(QIODevice::ReadOnly)))
    {
        //QMessageBox::information(this,QObject::tr("打开文件失败！"),file.errorString());
        return false;
    }

    QByteArray content = file.readAll();
    WAV.data_size = content.size() - 44;
    WAV.data = new unsigned char[WAV.data_size];
    //qDebug()<<"\n WAV.data:"<<WAV.data;

    file.seek(0x2c);
    file.read((char *)WAV.data, sizeof(char)*WAV.data_size);
    //qDebug()<<"WAV.data_size:"<<WAV.data_size;

    for(int i=0;i<WAV.data_size;i+=2)
    //for(int i=341;i<3414;i++)
    {
        //右边为大端
        unsigned long data_low = WAV.data[i];
        //qDebug()<<"\n data_low:"<<data_low;
        unsigned long data_high = WAV.data[i + 1];
        double data_true = data_high * 256 + data_low;
        //qDebug()<<"\n data_true:"<<data_true;

        long data_complement = 0;
        //取大端的最高位（符号位）
        int my_sign = (int)(data_high / 128);
        //printf("%d ", my_sign);
        if(my_sign == 1)
        {
            data_complement = data_true - 65536;
            //十进制65536转换为十六进制数值为10000，这里的作用就是去掉符号位
        }
        else
        {
            data_complement = data_true;
        }
        //printf("%d ", data_complement);
        //setprecision(4);
        double float_data = (double)(data_complement/(double)32768);
        //这里是16bit表示一个样本，因此分母为2^16=32768
        //qDebug()<<"float_data"<<float_data*10;
        global_y[i] = float_data;
        show1[i] = float_data;

        //bool equal = global_y[i]==show1[i];
        //qDebug()<<equal;
        //qDebug()<<"\n global_y:"<<global_y*10;
        //addRandomGraph();
    }
    //qDebug()<<"\n global_y:"<<global_y*10;
    if(file.isOpen())
        file.close();

    file.close();
    addWaveGraph();
}

int MainWindow::QByteArrayToInt(QByteArray bytes) {
    int addr = bytes[0] & 0x000000FF;
    addr |= ((bytes[1] << 8) & 0x0000FF00);
    addr |= ((bytes[2] << 16) & 0x00FF0000);
    addr |= ((bytes[3] << 24) & 0xFF000000);
    return addr;
}

void MainWindow::on_action_A_triggered()
{

}

void MainWindow::on_action_P_triggered()
{

}

void MainWindow::on_action_Q_triggered()
{
//    qDebug()<<"quit";
}

void MainWindow::closeEvent(QCloseEvent *event)
{
//    QMessageBox msgBox;
//    msgBox.question(this,QObject::tr("提示"),QObject::tr("确定退出程序吗?"),QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
//    //添加判断处理程序
}

//第二个菜单栏槽函数
void MainWindow::on_action_zoom_in_triggered()
{

}

void MainWindow::on_action_zoom_out_triggered()
{

}

void MainWindow::on_action_rotate_triggered()
{

}

void MainWindow::on_action_clear_triggered()
{

}

//第三个菜单栏槽函数
void MainWindow::on_action_2D_triggered()
{

}

void MainWindow::on_action_3D_triggered()
{

}

void MainWindow::on_action_spec_triggered()
{
    ui->customPlot->clearGraphs();

    ui->customPlot->xAxis->setRange(1, 10);
    ui->customPlot->yAxis->setRange(0, 1);//x、y显示区域范围
    ui->customPlot->axisRect()->setupFullAxesBox();

    ui->customPlot->xAxis->setLabel(QObject::tr("频率"));
    ui->customPlot->yAxis->setLabel(QObject::tr("归一化能量谱"));
    ui->customPlot->legend->setVisible(true);

    double show2[SumPot]={0};//虚部数组
    QVector<double> x(SumPot), y(SumPot);

    //FFT(show1,show2,n,sign);
    FFT(show1,show2,SumPot,1);

    //-----计算功率能量谱-----//
    double m=0,n=0;
    double max=1;

    for(int i=341;i<3414;i++)
    //只计算1K到10K频谱;16384*(1/48~10/48)
    {
        m=(show1[i]/16384)*(show1[i]/16384);
        n=(show2[i]/16384)*(show2[i]/16384);

        //qDebug()<<"show2:"<<show2[i];

        show3[i-341]=(m+n)/16384;
        //sqrt(m+n)/;

        //测试接收到的数据（这里是最终频谱图获取的数据）
        qDebug()<<"show[3]:"<<show3[i-341];
        global_x1[i] = i;
        global_y1[i] = show3[i-341];
        /*-----测试结果：显示的仍未原波形，未经FFT变换-----*/

        //x[i] = global_x1[i];
        //y[i] = global_y1[i];

        if(show3[i-341]>max)
        {
            max=show3[i-341];
        }
        //得到最大值
    }
    addSpecGraph();

}

void MainWindow::on_action_wave_triggered()
{

}

void MainWindow::paintEvent(QPaintEvent *event)
{

}

//第四个菜单栏槽函数
void MainWindow::aboutSoftwareSlot()
{
    about *dialog = new about;
    dialog->show();//unmodal dialog,即非模态对话框
    //dialog->exec();//modal dialog,模态对话框
}

void MainWindow::aboutWebsiteSlot()
{
    QDesktopServices::openUrl(QUrl("http://www.qt.io/"));
}

//第三方库绘图函数
void MainWindow::titleDoubleClick(QMouseEvent* event, QCPPlotTitle* title)
{
    Q_UNUSED(event)
    // Set the plot title by double clicking on it
    bool ok;
    QString newTitle = QInputDialog::getText(this, QObject::tr("更改标题"), QObject::tr("新标题："), QLineEdit::Normal, title->text(), &ok);
    if (ok)
    {
      title->setText(newTitle);
      ui->customPlot->replot();
  }
}

void MainWindow::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Set an axis label by double clicking on it
  if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
  {
    bool ok;
    QString newLabel = QInputDialog::getText(this, QObject::tr("修改坐标轴"), QObject::tr("新坐标名称:"), QLineEdit::Normal, axis->label(), &ok);
    if (ok)
    {
      axis->setLabel(newLabel);
      ui->customPlot->replot();
    }
  }
}

void MainWindow::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
  // Rename a graph by double clicking on its legend item
  Q_UNUSED(legend)
  if (item) // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
  {
    QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
    bool ok;
    QString newName = QInputDialog::getText(this, QObject::tr("修改曲线名称"), QObject::tr("新曲线名称:"), QLineEdit::Normal, plItem->plottable()->name(), &ok);
    if (ok)
    {
      plItem->plottable()->setName(newName);
      ui->customPlot->replot();
    }
  }
}

void MainWindow::selectionChanged()
{
  /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.

   The selection state of the left and right axes shall be synchronized as well as the state of the
   bottom and top axes.

   Further, we want to synchronize the selection of the graphs with the selection state of the respective
   legend item belonging to that graph. So the user can select a graph by either clicking on the graph itself
   or on its legend item.
  */

  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->customPlot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->customPlot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->customPlot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->customPlot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }

  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<ui->customPlot->graphCount(); ++i)
  {
    QCPGraph *graph = ui->customPlot->graph(i);
    QCPPlottableLegendItem *item = ui->customPlot->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelected(true);
    }
  }
}

void MainWindow::mousePress()
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged

  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->xAxis->orientation());
  else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->yAxis->orientation());
  else
    ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::mouseWheel()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed

  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->xAxis->orientation());
  else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->yAxis->orientation());
  else
    ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::addWaveGraph()
{
    ui->customPlot->clearGraphs();
//    if(ui->customPlot->hasItem(0))
//    {
//        ui->customPlot->removeItem(0);
//    }

    ui->customPlot->xAxis->setRange(0, SumPot);
    ui->customPlot->yAxis->setRange(-1, 1);//x、y显示区域范围
    ui->customPlot->axisRect()->setupFullAxesBox();

//    ui->customPlot->plotLayout()->insertRow(0);
//    ui->customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->customPlot, QObject::tr("频谱曲线显示")));

    ui->customPlot->xAxis->setLabel(QObject::tr("x 轴"));
    ui->customPlot->yAxis->setLabel(QObject::tr("y 轴"));
    ui->customPlot->legend->setVisible(true);

    int n = SumPot;// number of points in graph
    QVector<double> x(n), y(n);
    for (int i=0; i<n; i++)
    //for(int i=341;i<3414;i++)
    {
        x[i] = i;
        //qDebug()<<"\n x[i]:"<<x[i];
        y[i] = global_y[i];
        //qDebug()<<"\n y[i]:"<<y[i];
    }

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName(QString(QObject::tr("曲线 %1").arg(ui->customPlot->graphCount()-1)));//曲线名称
    ui->customPlot->graph()->setData(x, y);//设置坐标值

    QPen graphPen;
    graphPen.setColor(QColor(255,0,0));
    graphPen.setWidthF(0.8);
    ui->customPlot->graph()->setPen(graphPen);
    ui->customPlot->replot();
}

void MainWindow::addSpecGraph()
{
    ui->customPlot->clearGraphs();

    ui->customPlot->xAxis->setRange(341, 3414);
    ui->customPlot->yAxis->setRange(0, 1);//x、y显示区域范围
    ui->customPlot->axisRect()->setupFullAxesBox();

//    ui->customPlot->plotLayout()->insertRow(0);
//    ui->customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->customPlot, QObject::tr("频谱曲线显示")));

    ui->customPlot->xAxis->setLabel(QObject::tr("频率"));
    ui->customPlot->yAxis->setLabel(QObject::tr("归一化功率能量谱"));
    ui->customPlot->legend->setVisible(true);

    int n = SumPot;// number of points in graph
    QVector<double> x(n), y(n);
    for(int i=341;i<3414;i++)
    {
        x[i] = i;
        //qDebug()<<"\n x[i]:"<<x[i];
        //y[i] = global_y1[i];
        y[i] = global_y1[i]*10000000000;//幅值太小显示不出来，需要适当调整
        //qDebug()<<"\n y[i]:"<<y[i];
    }

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName(QString(QObject::tr("曲线 %1").arg(ui->customPlot->graphCount()-1)));//曲线名称
    ui->customPlot->graph()->setData(x, y);//设置坐标值

    QPen graphPen;
    graphPen.setColor(QColor(255,0,0));
    graphPen.setWidthF(0.8);
    ui->customPlot->graph()->setPen(graphPen);
    ui->customPlot->replot();
}

void MainWindow::removeSelectedGraph()
{
  if (ui->customPlot->selectedGraphs().size() > 0)
  {
    ui->customPlot->removeGraph(ui->customPlot->selectedGraphs().first());
    //ui->customPlot->replot();
  }
}

void MainWindow::removeAllGraphs()
{
  ui->customPlot->clearGraphs();
  ui->customPlot->replot();
}

void MainWindow::contextMenuRequest(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);

  if (ui->customPlot->legend->selectTest(pos, false) >= 0) // context menu on legend requested
  {
    menu->addAction(QObject::tr("左上显示"), this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->addAction(QObject::tr("居中显示"), this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    menu->addAction(QObject::tr("右上显示"), this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    menu->addAction(QObject::tr("右下显示"), this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    menu->addAction(QObject::tr("左下显示"), this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
  } else  // general context menu on graphs requested
  {
    //menu->addAction("Add random graph", this, SLOT(addRandomGraph()));
    if (ui->customPlot->selectedGraphs().size() > 0)
      menu->addAction(QObject::tr("删除所选曲线"), this, SLOT(removeSelectedGraph()));
    if (ui->customPlot->graphCount() > 0)
      menu->addAction(QObject::tr("删除所有曲线"), this, SLOT(removeAllGraphs()));
  }

  menu->popup(ui->customPlot->mapToGlobal(pos));
}

void MainWindow::moveLegend()
{
  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      ui->customPlot->replot();
    }
  }
}

void MainWindow::graphClicked(QCPAbstractPlottable *plottable)
{
  ui->statusBar->showMessage(QString(QObject::tr("已选择曲线 '%1'.").arg(plottable->name())), 1000);
}

int MainWindow::on_action_result_triggered()
{
    diagram *analyse_diagram = new diagram;
    analyse_diagram->show();
    // 创建数据库连接
    if (!createConnection())
        return 1;

    //使用QSqlQuery查询连接1的整张表，先要使用连接名获取该连接
    QSqlDatabase db = QSqlDatabase::database("connection1");
    // 使用QSqlQuery查询整张表
    QSqlQuery query(db);
    query.exec("select * from result");
    while(query.next())
    {
        qDebug() << query.value(0).toInt() << query.value(1).toString();
    }
}

//在添加该类之前需要在mainwindow.h中引入信号槽映射关系，这里引入login.h即可
//否则不会识别出该类，出现未定义标识符的错误
void MainWindow::on_action_login_triggered()
{
    login *customer = new login;
    customer->show();
}

void MainWindow::on_action_password_triggered()
{
    password *changepass = new password;
    changepass->show();
}

