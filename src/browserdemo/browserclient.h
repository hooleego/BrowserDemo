#ifndef BROWSER_CLIENT_H
#define BROWSER_CLIENT_H

#include <QtCore/QList>
#include "include/cef_client.h"

class BrowserView;
class BrowserCallImpl;
class BrowserClient : public CefClient, public CefDisplayHandler, public CefLifeSpanHandler, public CefLoadHandler
{
public:
    explicit BrowserClient(bool use_views, BrowserView *view);
    ~BrowserClient();

    // CefClient methods:
    virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE;
    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE;
    virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE;

    // CefDisplayHandler methods:
    virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString &title) OVERRIDE;

    // CefLifeSpanHandler methods:
    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
    virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

    // CefLoadHandler methods:
    virtual void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString &errorText, const CefString &failedUrl) OVERRIDE;

    CefRefPtr<CefBrowser> GetBrowserByID(int browserId);
    void CloseAllBrowsers(bool force_close);
    bool IsClosing() const;

    virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          CefProcessId source_process,
                                          CefRefPtr<CefProcessMessage> message) OVERRIDE;

private:
    // Platform-specific implementation.
    void PlatformTitleChange(CefRefPtr<CefBrowser> browser, const CefString &title);

    // True if the application is using the Views framework.
    const bool use_views_;

    // List of existing browser windows. Only accessed on the CEF UI thread.
    QList<CefRefPtr<CefBrowser>> m_browserList;

    bool is_closing_;

    BrowserView *m_view;
    BrowserCallImpl *m_callImpl;

    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(BrowserClient);
};

#endif // BROWSER_CLIENT_H
