#include <QtCore/QDebug>
#include <QtWidgets/QDesktopWidget>
#include "browserview.h"
#include "ui_browserview.h"

BrowserView::BrowserView(QWidget *parent) : QWidget(parent), m_ui(new Ui_BrowserView), m_browserId(-1)
{
    m_ui->setupUi(this);
    // m_ui->lineEdit->setText(QLatin1String("http://10.60.0.7:8989/#/login")); // Default web

    m_ui->lineEdit->setText(QLatin1String("file:///D:/Work/demo_project/BrowserDemo/build/src/browserdemo/Release/test.html")); // Default web

    // m_ui->lineEdit->setText(QLatin1String("http://html5test.com/")); // Default web

    connect(m_ui->pushButton, &QPushButton::clicked, this, &BrowserView::redirect);


    QString strUrl = m_ui->lineEdit->text();

    HWND wnd = (HWND)m_ui->widget->winId();

    CefWindowInfo cefWndInfo;
    RECT winRect;

    QDesktopWidget *pDeskTop = QApplication::desktop();
    QRect qtRect = pDeskTop->screenGeometry();
    winRect.left = qtRect.left();
    winRect.top = qtRect.top();
    winRect.right = qtRect.right();
    winRect.bottom = qtRect.bottom();

    cefWndInfo.SetAsChild(wnd, winRect); // Important: set CEF browser as child

    CefBrowserSettings cefBrowSetting;
    m_browserClient = CefRefPtr<BrowserClient>(new BrowserClient(true, this));
    bool browser = CefBrowserHost::CreateBrowser(cefWndInfo, m_browserClient, strUrl.toStdString(), cefBrowSetting, NULL, NULL);

    showMaximized();
}

BrowserView::~BrowserView()
{

}

void BrowserView::setIdentifier(int browserId)
{
    m_browserId = browserId;
}

void BrowserView::redirect()
{
    CefRefPtr<CefBrowser> pBrower = m_browserClient->GetBrowserByID(m_browserId);
    if (pBrower)
    {
        pBrower->GetMainFrame()->LoadURL(m_ui->lineEdit->text().toStdString());
    }
}