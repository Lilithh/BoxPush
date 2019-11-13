#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "menu.h"
#include "game.h"
#include "choosepuzzle.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slot_toMenuPage();
    void slot_toGamePage();
    void slot_tochoosePage();
    void slot_toQuit();

private:
    Ui::MainWindow *ui;

    menu *menuWidget;
    game *gameWidget;
    choosePuzzle *chooseWidget;
};

#endif // MAINWINDOW_H
