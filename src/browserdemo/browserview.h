#ifndef BROWSER_VIEW_H
#define BROWSER_VIEW_H

#include <QtWidgets/QWidget>
#include "browserclient.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class Ui_BrowserView;
class BrowserView : public QWidget
{
    Q_OBJECT

public:
    explicit BrowserView(QWidget *parent = 0);
    virtual ~BrowserView();

    void setIdentifier(int browserId);

Q_SIGNALS:

public Q_SLOTS:
    void redirect();

protected:

private Q_SLOTS:

private:
    QScopedPointer<Ui_BrowserView> m_ui;

    int m_browserId;
    CefRefPtr<BrowserClient> m_browserClient;

    Q_DISABLE_COPY(BrowserView)
};

#endif // BROWSER_VIEW_H
