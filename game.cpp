#include "game.h"
#include "ui_game.h"


#include<QApplication>
#include<QPixmap>
#include<QPainter>
#include <QDebug>

game::game(QWidget *parent) : QWidget(parent), ui(new Ui::game)
{
    ui->setupUi(this);

    this->ui->paint->setFocusPolicy(Qt::StrongFocus);   //设置焦点
    this->ui->paint->installEventFilter(this);//注册监视对象。

}

game::~game()
{
    delete ui;
}

void game::init()
{
    this->game_Createdb = new mysql_conn;     //打开数据库
    this->game_Createdb->linkMySql();   //连接数据库


    QString cmd = "Select * From currentGame;";
    tableData = this->game_Createdb->selectDataFromDb(cmd);   //调用sql类方法
}


void game::do_paint()                     //用数据画图
{
    QPainter painter(this->ui->paint);
    QPen pen(QColor(0,0,0),2);

    painter.setPen(pen);


    for(int j=0;j<701;j+=35){
        painter.drawLine(QPointF(j,0),QPointF(j,700));
        painter.drawLine(QPointF(0,j),QPointF(700,j));
    }

    QListIterator<QStringList> itr(tableData);     //声明迭代器
    while (itr.hasNext()) {   //加载数据
        QStringList row = itr.next();
//        qDebug() << row.at(0) << " " <<row.at(1) << " "<< row.at(2);

        if( row.at(0)=="#"){          //墙壁
            QBrush brush(QColor(0, 255, 0, 125));
            painter.setBrush(brush);

            int x=row.at(1).toInt();
            int y=row.at(2).toInt();
            painter.drawRect(QRect(x*35, y*35, 35,35));
        }
        else if(row.at(0)=="$"){  //箱子
            QBrush brush(QColor(255, 0, 0, 125));
            painter.setBrush(brush);


            int x=row.at(1).toInt();
            int y=row.at(2).toInt();

            painter.drawLine(QPointF(x*35,y*35),QPointF(x*35+35,y*35+35));
            painter.drawLine(QPointF(x*35+35,y*35),QPointF(x*35,y*35+35));
            painter.drawRect(QRect(x*35, y*35, 35,35));
        }

        else if (row.at(0)=="@") {      //人
            QBrush brush(QColor(0, 0, 125, 125));
            painter.setBrush(brush);


            int x=row.at(1).toInt();
            int y=row.at(2).toInt();

            people =people_next= QPointF(x, y);   //给people坐标
            painter.drawEllipse(QPointF(x*35+17.5, y*35+17.5), 15, 15);
        }

        else if (row.at(0)==".") {   //终点
            QBrush brush(QColor(230, 0, 0, 48));
            painter.setBrush(brush);

            int x=row.at(1).toInt();
            int y=row.at(2).toInt();
            painter.drawRect(QRect(x*35, y*35, 35,35));
        }

    }


//    painter.end();
//    return true;
}









void game::slot_showing()    //页面即将显示时 开始初始化
{
    init();
}



bool game::eventFilter(QObject *watched, QEvent *e)       //(在控件上绘图) 事件捕捉器
{
//    if(e->type() == QEvent::MouseButtonPress)  {
//        qDebug() << "mouse here";
//         ui->paint->update();
//         ui->paint->repaint();
//    }

    if (e->type() == QEvent::KeyPress) {     //捕捉键盘事件
        qDebug() << "KeyPress here";
               QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
               if (keyEvent->key() == Qt::Key_Up) {
                   qDebug() << "Key_Up here";
                    on_up_clicked();
               }
               if (keyEvent->key() == Qt::Key_Down) {
                    on_down_clicked();
               }
               if (keyEvent->key() == Qt::Key_Left) {
                    on_left_clicked();
               }
               if (keyEvent->key() == Qt::Key_Right) {
                    on_right_clicked();
               }
           }

    if(watched == ui->paint)
    {
        if(e->type() == QEvent::Paint){
            do_paint();
            return true;
        }
    }
    return QWidget::eventFilter(watched,e);
}

void game::paintEvent(QPaintEvent *)
{
}

void game::on_back_clicked()
{
    this->game_Createdb->closeMysql();    //返回主界面时关闭数据库（否则与其他界面冲突）
    emit signal_back();
}


//上下左右移动

void game::on_up_clicked()
{
    int x = people.toPoint().x();
    int y=people.toPoint().y();

    people_next = QPointF(x, y-1);
    actDire d = up;
    modifyData(d);
}

void game::on_down_clicked()
{
    int x = people.toPoint().x();
    int y=people.toPoint().y();
    people_next = QPointF(x, y+1);
    actDire d = down;
    modifyData(d);
}

void game::on_left_clicked()
{
    int x = people.toPoint().x();
    int y=people.toPoint().y();
    people_next = QPointF(x-1, y);
    actDire d = left;
    modifyData(d);
}

void game::on_right_clicked()
{
    int x = people.toPoint().x();
    int y=people.toPoint().y();
    people_next = QPointF(x+1, y);
    actDire d = right;
    modifyData(d);
}

void game::modifyData(actDire direction)    // 根据移动方向，操作数据，修改数据库，重新加载图像
{
    int x0 = people.toPoint().x();
    int y0 = people.toPoint().y();

    //查数据库判断人移动后的位置是什么物体
    int x = people_next.toPoint().x();
    int y = people_next.toPoint().y();

    QString item = "";
    QList<QStringList> data;
    QString cmd = QString("Select type From currentGame where x=%1 and y=%2;").arg(x).arg(y);
    data = this->game_Createdb->selectDataFromDb(cmd);
//    qDebug() << "type:" << data;
    QListIterator<QStringList> itr(data);     //迭代器
    while (itr.hasNext()) {   //加载数据
        QStringList row = itr.next();
        item = row.at(0);
    }

//根据移动后碰到不同物体，做反应

   if(item=="#"){    //障碍
        people_next=people;
        qDebug() << "碰到障碍物";
   }
   else if (item=="$") {    //箱子
        int xx = x;
        int yy = y;
        if(direction==up) yy--;
        else if (direction==down) yy++;
        else if (direction==left) xx--;
        else if (direction==right) xx++;
//        QPointF box_next = QPointF(xx,yy);

        QString itemm = "";
        QList<QStringList> dataa;
        QString cmdd = QString("Select type From currentGame where x=%1 and y=%2;").arg(xx).arg(yy);
        dataa = this->game_Createdb->selectDataFromDb(cmdd);
    //    qDebug() << "type:" << data;
        QListIterator<QStringList> itrr(dataa);     //迭代器

        while (itrr.hasNext()) {   //加载数据
            QStringList roww = itrr.next();
            itemm = roww.at(0);
        }
        if(itemm=="#" || itemm=="$"){    //障碍
             people_next=people;
             qDebug() << "箱子前有障碍物";
        }
        else if (itemm==".") {    //目标区域
            QString cmd1=QString("delete from currentGame where x=%1 and y=%2;").arg(xx).arg(yy);
            QString cmd2=QString("UPDATE currentGame set x=%1,y=%2 where x=%3 and y=%4;").arg(xx).arg(yy).arg(x).arg(y);
            QString cmd3=QString("UPDATE currentGame set x=%1,y=%2 where x=%3 and y=%4;").arg(x).arg(y).arg(x0).arg(y0);
            if(this->game_Createdb->execCommand(cmd1)) qDebug() << "目标区域删除";
            if(this->game_Createdb->execCommand(cmd2)) qDebug() << "箱子移动";
            if(this->game_Createdb->execCommand(cmd3)) qDebug() << "人移动";
            people=people_next;
        }
        else {     //空地
            QString cmd2=QString("UPDATE currentGame set x=%1,y=%2 where x=%3 and y=%4;").arg(xx).arg(yy).arg(x).arg(y);
            QString cmd3=QString("UPDATE currentGame set x=%1,y=%2 where x=%3 and y=%4;").arg(x).arg(y).arg(x0).arg(y0);
            if(this->game_Createdb->execCommand(cmd2)) qDebug() << "箱子移动";
            if(this->game_Createdb->execCommand(cmd3)) qDebug() << "人移动";
            people=people_next;
        }
   }
   else if (item==".") {    //目标区域
       QString cmd1=QString("delete from currentGame where x=%1 and y=%2;").arg(x).arg(y);
       QString cmd3=QString("UPDATE currentGame set x=%1,y=%2 where x=%3 and y=%4;").arg(x).arg(y).arg(x0).arg(y0);
       if(this->game_Createdb->execCommand(cmd1)) qDebug() << "目标区域删除";
       if(this->game_Createdb->execCommand(cmd3)) qDebug() << "人移动";
       people=people_next;
   }
   else {     //空地
       QString cmd3=QString("UPDATE currentGame set x=%1,y=%2 where x=%3 and y=%4;").arg(x).arg(y).arg(x0).arg(y0);
       if(this->game_Createdb->execCommand(cmd3)) qDebug() << "人移动";
       people=people_next;
   }

   QString cmmd = "Select * From currentGame;";
   tableData = this->game_Createdb->selectDataFromDb(cmmd);   //更新database变量数据
   update(); //更新图像



}



void game::on_restart_clicked()
{
    emit signal_resetTable();

    this->game_Createdb = new mysql_conn;     //数据库重新连接
    this->game_Createdb->linkMySql();
    QString cmmd = "Select * From currentGame;";
    tableData = this->game_Createdb->selectDataFromDb(cmmd);   //更新数据
    update(); //更新图像
}
