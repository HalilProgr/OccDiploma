#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "view.h"

#include <AIS_InteractiveContext.hxx>

#include <AIS_Shape.hxx>

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QDebug>
#include <QToolBar>
#include <QMessageBox>


class View;

//! Qt main window which include OpenCASCADE for its central widget.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void createActions();
    void createMenus();
    void createToolBars();
    void createMainWidget();

private slots:
    void about();

private:
    Ui::occQtClass ui;

    View* myView;
};
#endif // MAINWINDOW_H
