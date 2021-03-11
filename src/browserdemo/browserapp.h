#ifndef BROWSER_APP_H
#define BROWSER_APP_H

#include "include/cef_app.h"
#include "browserv8handler.h"

class BrowserApp : public CefApp, public CefBrowserProcessHandler, public CefRenderProcessHandler
{
public:
  BrowserApp();

  // CefApp methods:
  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE;
  virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE;

  // CefBrowserProcessHandler methods:
  virtual void OnContextInitialized() OVERRIDE;

  // CefRenderProcessHandler methods:
  virtual void OnWebKitInitialized() OVERRIDE;
  virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        CefProcessId source_process,
                                        CefRefPtr<CefProcessMessage> message) OVERRIDE;

private:
  CefRefPtr<BrowserV8Handler> _handler;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(BrowserApp);
};

#endif // BROWSER_APP_H
