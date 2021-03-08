#include <QtCore/QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config/appconfig.h"

int const MainWindow::exitCodeReboot = -123456789;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_ui(new Ui_MainWindow)
{
    m_ui->setupUi(this);
}

MainWindow::~MainWindow()
{
}

int MainWindow::Initialize()
{
    return 0;
}

int MainWindow::Uninitialize()
{
    return 0;
}
