#ifndef MYSQL_CONN_H
#define MYSQL_CONN_H

#include <QList>
#include <QStringList>
#include <QSqlDatabase>


class mysql_conn
{
public:
    mysql_conn();

    void linkMySql();  //链接数据库函数
    QList<QStringList> selectDataFromDb(QString command);   //获取数据库数据函数
    void closeMysql();   //关闭数据库函数

    bool execCommand(QString command);    //执行mysql命令函数

private:
    QSqlDatabase db;
};

#endif // MYSQL_CONN_H
