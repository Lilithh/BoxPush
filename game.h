#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include "mysql_conn.h"

namespace Ui {
class game;
}

class game : public QWidget
{
    Q_OBJECT

    enum actDire   //标记移动方向
    {
        up ,
        down,
        left,
        right
    };


public:
    explicit game(QWidget *parent = nullptr);
    ~game();

    void init();
    void modifyData(actDire direction);

protected:
    void paintEvent(QPaintEvent *);
    bool eventFilter(QObject *watched, QEvent *e);
    void do_paint();


private slots:
    void on_back_clicked();
    void slot_showing();

    void on_up_clicked();

    void on_down_clicked();

    void on_left_clicked();

    void on_right_clicked();

signals:
    void signal_back();

private:
    Ui::game *ui;
    QList<QStringList> tableData;
    mysql_conn *game_Createdb;
    QPointF people,people_next;
};

#endif // GAME_H
