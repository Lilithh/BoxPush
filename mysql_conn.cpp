#include "mysql_conn.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>

mysql_conn::mysql_conn()
{
    db = QSqlDatabase::addDatabase("QMYSQL", "NameOfConnection");    //实现
}


void mysql_conn::linkMySql()
{
    this->db.setDatabaseName("AI_BoxPush");        //数据库名
    this->db.setHostName("127.0.0.1");     //服务器地址（本机）
    this->db.setUserName("root");
    this->db.setPassword("qwert12345");
    if (!this->db.open()) {
        qDebug() << "Connect to MySql error: " << db.lastError().text();
    }
}

QList<QStringList> mysql_conn::selectDataFromDb(QString command)
{
    QSqlQuery query(this->db);
    query.exec(command);

    QList<QStringList> dbInfo;
    int col;     //获取字段数目/列数

    while (query.next()) {
        col = query.record().count();

        QStringList singleRowInfo;

        for(int i=0; i<col; i++){
            singleRowInfo <<query.value(i).toString();     //逐个字段加入一个列表
        }

        dbInfo.append(singleRowInfo);  //将一行加入List
    }
    return dbInfo;
}


void mysql_conn::closeMysql()
{
    this->db.close();
    qDebug()<<"close data_Db legally";
}

bool mysql_conn::execCommand(QString command)
{
    QSqlQuery que(this->db);
//    qDebug()<<"last error  "<<que.lastError().text();  //返回最后一条错误消息
    que.prepare(command);
    return que.exec(command);   //执行函数本身是bool型
}
