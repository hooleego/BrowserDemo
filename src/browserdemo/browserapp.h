#ifndef BROWSER_APP_H
#define BROWSER_APP_H

#include "include/cef_app.h"

class BrowserApp : public CefApp, public CefBrowserProcessHandler
{
public:
  BrowserApp();

  // CefApp methods:
  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE;

  // CefBrowserProcessHandler methods:
  virtual void OnContextInitialized() OVERRIDE;

private:

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(BrowserApp);
};

#endif // BROWSER_APP_H
