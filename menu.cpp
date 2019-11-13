#include "menu.h"
#include "ui_menu.h"

menu::menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::menu)
{
    ui->setupUi(this);
}

menu::~menu()
{
    delete ui;
}

void menu::on_begin_clicked()
{
    emit signal_game();
}

void menu::on_choose_clicked()
{
    emit signal_choose();
}

void menu::on_quit_clicked()
{
    emit signal_quit();
}
