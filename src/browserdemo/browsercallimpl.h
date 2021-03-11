#ifndef BROWSER_CALL_IMPL_H
#define BROWSER_CALL_IMPL_H

#include <map>
#include <string>
#include <functional>
#include "include/cef_process_message.h"
#include "include/cef_browser.h"
#include "include/cef_frame.h"

class BrowserCallImpl
{
public:
    BrowserCallImpl();
    ~BrowserCallImpl();

    bool OnImplemention(const CefString& function_name, const CefString& params, int js_callback_id, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame);

    bool Add(int callbackId, const CefString &params, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame);
    
private:

};

#endif // BROWSER_CALL_IMPL_H
