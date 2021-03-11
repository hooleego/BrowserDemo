#include <QtWidgets/QApplication>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include "mainwindow.h"
#include "config/appconfig.h"
#include <windows.h>
#include "include/cef_command_line.h"
#include "browserapp.h"

void QCefInitSettings(CefSettings & settings)
{
    settings.multi_threaded_message_loop = true;
    // settings.log_severity = LOGSEVERITY_DISABLE;

    settings.no_sandbox = true;
}

int QCefInit(int& argc, char** argv)
{
    HINSTANCE hInstance = static_cast<HINSTANCE>(GetModuleHandle(nullptr));

    CefMainArgs mainArgs(hInstance);
    CefRefPtr<BrowserApp> app(new BrowserApp);

    int exit_code = CefExecuteProcess(mainArgs, app.get(), nullptr);
    if (exit_code >= 0) {
        return exit_code;
    }

    CefSettings settings;
    QCefInitSettings(settings);

    CefInitialize(mainArgs, settings, app, nullptr);

    return -1;
}

void CefQuit()
{
    CefShutdown();
}

void LoggerFunction(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QFile logFile(QLatin1String("logger.log"));

    if (logFile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text))
    {
        QTextStream streamer(&logFile);

        switch (type) {
        case QtDebugMsg:
            break;
        case QtInfoMsg:
            streamer << QDateTime::currentDateTime().toString() << " Info: " << msg << "\n";
            break;
        case QtWarningMsg:
            streamer << QDateTime::currentDateTime().toString() << " Warning: " << msg << "\n";
            break;
        case QtCriticalMsg:
            streamer << QDateTime::currentDateTime().toString() << " Critical: " << msg << "\n";
            break;
        case QtFatalMsg:
            streamer << QDateTime::currentDateTime().toString() << " Fatal: " << msg << "\n";
            break;
        }
        logFile.close();
    }
}

int main(int argc, char *argv[])
{
    int currentExitCode = 0;

    qInstallMessageHandler(LoggerFunction);

    // make an application restartable: https://wiki.qt.io/How_to_make_an_Application_restartable
    do
    {
        QApplication a(argc, argv);

        int result = QCefInit(argc, argv);
        if (result >= 0) {
            return result;
        }

        QIcon icon;
        icon.addFile(QLatin1String(":/images/application_16.png"), QSize(16, 16));
        icon.addFile(QLatin1String(":/images/application_32.png"), QSize(32, 32));
        icon.addFile(QLatin1String(":/images/application_48.png"), QSize(48, 48));
        icon.addFile(QLatin1String(":/images/application_64.png"), QSize(64, 64));
        icon.addFile(QLatin1String(":/images/application_128.png"), QSize(128, 128));
        a.setWindowIcon(icon);

        AppConfig::instance()->read("BrowserDemo.json");

        MainWindow mainwindow;
		if (mainwindow.Initialize() != 0)
		{
			return -1;
		}
        mainwindow.show();

        currentExitCode = a.exec();

        CefQuit();

    } while (currentExitCode == MainWindow::exitCodeReboot);

    return currentExitCode;
}