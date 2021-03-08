#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtCore/QScopedPointer>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class Ui_MainWindow;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    static int const exitCodeReboot;

    int Initialize();
    int Uninitialize();

Q_SIGNALS:

public Q_SLOTS:

protected:

private Q_SLOTS:

private:
private:
    QScopedPointer<Ui_MainWindow> m_ui;

    Q_DISABLE_COPY(MainWindow)
};

#endif // MAINWINDOW_H
