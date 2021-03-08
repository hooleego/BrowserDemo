#include "include/wrapper/cef_helpers.h" /* For CEF_REQUIRE_UI_THREAD  */
#include "browserapp.h"

BrowserApp::BrowserApp()
{

}

CefRefPtr<CefBrowserProcessHandler> BrowserApp::GetBrowserProcessHandler()
{
    return this;
}

void BrowserApp::OnContextInitialized()
{
  CEF_REQUIRE_UI_THREAD();

}
