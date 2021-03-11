#include <QtCore/QDebug>
#include "include/wrapper/cef_helpers.h" /* For CEF_REQUIRE_UI_THREAD  */
#include "include/cef_parser.h"
#include "browserapp.h"
#include "browserv8handler.h"

BrowserApp::BrowserApp() : _handler(nullptr)
{
}

CefRefPtr<CefBrowserProcessHandler> BrowserApp::GetBrowserProcessHandler()
{
    return this;
}

CefRefPtr<CefRenderProcessHandler> BrowserApp::GetRenderProcessHandler()
{
    return this;
}

void BrowserApp::OnContextInitialized()
{
    CEF_REQUIRE_UI_THREAD();
}

void BrowserApp::OnWebKitInitialized()
{
    /**
     * JavaScript 扩展代码，这里定义一个 ipcApp 对象提供 call 和 register 方法来让 Web 端触发 CefV8Handler 处理代码
     * param[in] functionName 要调用的 C++ 方法名称
     * param[in] params 调用该方法传递的参数
     * param[in] callback 执行该方法后的回调函数
     * 例如:
     * ipcApp.call('add', { num1: 1, num2: 6 }, (arguments) => {
     *    console.log(arguments)
     * })
     */
    std::string extensionCode = R"(
        var ipcApp = {};
        (() => {
            ipcApp.call = (functionName, arg1, arg2) => {
                if (typeof arg1 === 'function') {
                    native function call(functionName, arg1);
                    return call(functionName, arg1);
                } else {
                    const jsonString = JSON.stringify(arg1);
                    native function call(functionName, jsonString, arg2);
                    return call(functionName, jsonString, arg2);
                }
            };
            ipcApp.register = (functionName, callback) => {
                native function register(functionName, callback);
                return register(functionName, callback);
            };
        })();
    )";

    _handler = new BrowserV8Handler();
    CefRegisterExtension("v8/extern", extensionCode, _handler);
}

bool BrowserApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          CefProcessId source_process,
                                          CefRefPtr<CefProcessMessage> message)
{
    CefString message_name = message->GetName();
    CefRefPtr<CefListValue> args = message->GetArgumentList();
    CefRefPtr<CefValue> response = CefValue::Create();
    response->SetDictionary(args->GetDictionary(2));
    CefString jsonString = CefWriteJSON(response, JSON_WRITER_DEFAULT);

    qInfo() << QString(QObject::tr("OnProcessMessageReceived %1, JS callback id=%2, has error=%3, response=%4"))
                   .arg(message_name.c_str())
                   .arg(args->GetInt(0))
                   .arg(args->GetBool(1))
                   .arg(jsonString.c_str());

    return _handler->HandleCallback(args->GetInt(0), args->GetBool(1), jsonString);
}
