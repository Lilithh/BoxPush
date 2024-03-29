#ifndef CHOOSEPUZZLE_H
#define CHOOSEPUZZLE_H

#include <QWidget>
#include <QTableWidgetItem>
#include "mysql_conn.h"

namespace Ui {
class choosePuzzle;
}

class choosePuzzle : public QWidget
{
    Q_OBJECT

public:
    explicit choosePuzzle(QWidget *parent = nullptr);
    ~choosePuzzle();

    void init_table();
    void appendRow(QStringList rowData);
    void load_data();
    void set_currentTable(QString table);

private slots:
    void on_back_clicked();
    void slot_showing();
    void slot_resetTable();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

signals:
    void signal_back();
    void signal_game();

private:
    Ui::choosePuzzle *ui;

    mysql_conn *choose_Createdb;
    QString CURRTABLE;

    bool isShow;
};

#endif // CHOOSEPUZZLE_H
