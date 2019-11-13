#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize( QSize( 750, 800 ));

    menuWidget = new menu(this);
    gameWidget = new game(this);
    chooseWidget = new choosePuzzle(this);

    ui->stackedWidget->addWidget(menuWidget);
    ui->stackedWidget->addWidget(gameWidget);
    ui->stackedWidget->addWidget(chooseWidget);

    slot_toMenuPage();

    //connect
    QObject::connect(menuWidget,SIGNAL(signal_game()),this,SLOT(slot_toGamePage()));
    QObject::connect(menuWidget,SIGNAL(signal_choose()),this,SLOT(slot_tochoosePage()));
    QObject::connect(menuWidget,SIGNAL(signal_quit()),this,SLOT(slot_toQuit()));
    QObject::connect(chooseWidget,SIGNAL(signal_back()),this,SLOT(slot_toMenuPage()));
    QObject::connect(gameWidget,SIGNAL(signal_back()),this,SLOT(slot_toMenuPage()));

    QObject::connect(menuWidget,SIGNAL(signal_game()),gameWidget,SLOT(slot_showing()));
    QObject::connect(menuWidget,SIGNAL(signal_choose()),chooseWidget,SLOT(slot_showing()));

}




MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_toMenuPage(){
    this->ui->stackedWidget->setCurrentWidget(menuWidget);
}

void MainWindow::slot_toGamePage()
{
    this->ui->stackedWidget->setCurrentWidget(gameWidget);
}

void MainWindow::slot_tochoosePage()
{
    this->ui->stackedWidget->setCurrentWidget(chooseWidget);
}

void MainWindow::slot_toQuit()
{
    qDebug() << "close App legally";
    close();
}



