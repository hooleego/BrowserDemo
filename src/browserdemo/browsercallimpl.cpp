#include <QtCore/QDebug>
#include "include/cef_parser.h"
#include "browsercallimpl.h"

BrowserCallImpl::BrowserCallImpl()
{

}

BrowserCallImpl::~BrowserCallImpl()
{
}

bool BrowserCallImpl::OnImplemention(const CefString& function_name, const CefString& params, int js_callback_id, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
{
    if (function_name == "add")
    {
        Add(js_callback_id, params, browser, frame);
    }
    else
    {
        // 
    }
    return true;
}

bool BrowserCallImpl::Add(int callbackId, const CefString &params, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
{
    CefRefPtr<CefValue> jsonObject = CefParseJSON(params, JSON_PARSER_ALLOW_TRAILING_COMMAS);
    if (jsonObject->IsValid())
    {
        CefRefPtr<CefDictionaryValue> dict = jsonObject->GetDictionary();
        double num1 = dict->GetDouble("num1");
        double num2 = dict->GetDouble("num2");
        qInfo() << QString(QObject::tr("Call add with %2,  the result is %3")).arg(params.c_str()).arg(num1 + num2);

        CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("kExecuteJsCallbackMessage");
        CefRefPtr<CefListValue> args = message->GetArgumentList();
        args->SetInt(0, callbackId);
        args->SetBool(1, true);

        CefRefPtr<CefDictionaryValue> result = CefDictionaryValue::Create();
        result->SetDouble("result", num1 + num2);
        CefRefPtr<CefDictionaryValue> response = CefDictionaryValue::Create();
        response->SetInt("status", 0);
        response->SetDictionary("data", result);

        args->SetDictionary(2, response);

        // 发送消息到Render进程，触发CefRenderProcessHandler::OnProcessMessageReceived调用
        frame->SendProcessMessage(PID_RENDERER, message);
        return true;
    }
    return false;
}