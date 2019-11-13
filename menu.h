#ifndef MENU_H
#define MENU_H

#include <QWidget>

namespace Ui {
class menu;
}

class menu : public QWidget
{
    Q_OBJECT

public:
    explicit menu(QWidget *parent = nullptr);
    ~menu();

private slots:
    void on_begin_clicked();

    void on_choose_clicked();

    void on_quit_clicked();

signals:
    void signal_game();
    void signal_choose();
    void signal_quit();

private:
    Ui::menu *ui;
};

#endif // MENU_H
