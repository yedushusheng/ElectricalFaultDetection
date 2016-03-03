#ifndef CONNECTION_H
#define CONNECTION_H
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QtXml/QDomDocument>

static bool createConnection()
{
    //SqlDatabase为实现数据库连接的类
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","connection1");//创建一个数据库，添加数据库驱动
    db.setDatabaseName("analyse.db");//设置数据库的名字
    if(! db.open())
    {
        QMessageBox::critical(0,QObject::tr("无法加载数据库"),
             QObject::tr("无法建立有效的数据库连接"),QMessageBox::Cancel);
        return false;
    }
    QSqlQuery query(db);//新建一个查询库
    query.exec("create table result (id varchar primary key,percentage varchar(10),analyse varchar(20))");
    query.exec("insert into result values('0','10%','No damage')");
    query.exec("insert into result values('1','20%','Top damage')");
    query.exec("insert into result values('2','15%','Bottom damage')");
    return true;
}

bool static createXml()
{
    QFile file("data.xml");//创建一个xml文件
    if(file.exists())
        return true;
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))//以只写方式打开，且清零以前的信息
        return false;
    QDomDocument doc;//新建一个QDomDocument类对象，它代表一个xml文件
    QDomProcessingInstruction instruction;//添加处理指令
    instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);//xml文件版本等信息
    QDomElement root = doc.createElement(QString("判定结果："));
    doc.appendChild(root);//增加根目录
    QTextStream out(&file);//指定文本流
    doc.save(out, 4);//将xml文档保存到文件data.xml文件中，4表示子元素缩进字符数
    file.close();

    return true;
}

#endif // CONNECTION_H
