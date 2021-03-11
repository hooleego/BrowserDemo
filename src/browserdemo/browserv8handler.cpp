#include <QtCore/QDebug>
#include <QtWidgets/QMessageBox>
#include "include/cef_parser.h"
#include "browserv8handler.h"

BrowserV8Handler::BrowserV8Handler() : js_callback_id_(0)
{
}

BrowserV8Handler::~BrowserV8Handler()
{
}

bool BrowserV8Handler::Execute(const CefString &name, CefRefPtr<CefV8Value> object, const CefV8ValueList &arguments, CefRefPtr<CefV8Value> &retval, CefString &exception)
{
    qInfo() << QString(QObject::tr("Execute function %1")).arg(name.c_str());

    if (arguments.size() < 2)
    {
        exception = "Invalid arguments: the count of argument less than 2";
        return false;
    }

    CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
    CefRefPtr<CefFrame> frame = context->GetFrame();
    CefRefPtr<CefBrowser> browser = context->GetBrowser();

    int64_t browser_id = browser->GetIdentifier();
    int64_t frame_id = frame->GetIdentifier();

    if (name == "call")
    {
        CefString func = arguments[0]->GetStringValue();

        CefString params = "{}";
        CefRefPtr<CefV8Value> callback;
        if (arguments[0]->IsString() && arguments[1]->IsFunction())
        {
            callback = arguments[1];
        }
        else if (arguments[0]->IsString() && arguments[1]->IsString() && arguments[2]->IsFunction())
        {
            params = arguments[1]->GetStringValue();
            callback = arguments[2];
        }
        else
        {
            exception = "Invalid arguments: arg1 is the function name, arg2 is JSON object or function, arg3 must be a function if exist";
            return false;
        }

        // qInfo() << QString(QObject::tr("Call function %1 with %2 synchronously")).arg(func.c_str()).arg(params.c_str());
        // if (!SyncCall(func, params, retval))
        // {
        //     exception = "Failed to call function.";
        //     return false;
        // }
        qInfo() << QString(QObject::tr("Call function %1 with %2 asynchronously")).arg(func.c_str()).arg(params.c_str());
        if (!AsyncCall(func, params, callback))
        {
            exception = "Failed to call function.";
            return false;
        }

        return true;
    }
    else if (name == "register")
    {
        if (arguments[0]->IsString() && arguments[1]->IsFunction())
        {
            CefString func = arguments[0]->GetStringValue();
            CefRefPtr<CefV8Value> callback = arguments[1];
            if (!RegisterFunc(func, callback))
            {
                exception = "Failed to register function.";
                return false;
            }
            return true;
        }
        else
        {
            exception = "Invalid arguments.";
            return false;
        }
    }

    return false;
}

bool BrowserV8Handler::SyncCall(const CefString &func, const CefString &params, CefRefPtr<CefV8Value> &retval)
{
    CefRefPtr<CefValue> jsonObject = CefParseJSON(params, JSON_PARSER_ALLOW_TRAILING_COMMAS);
    if (jsonObject->IsValid())
    {
        CefRefPtr<CefDictionaryValue> dict = jsonObject->GetDictionary();
        double num1 = dict->GetDouble("num1");
        double num2 = dict->GetDouble("num2");
        qInfo() << QString(QObject::tr("Call function %1 with %2,  the result is %3")).arg(func.c_str()).arg(params.c_str()).arg(num1 + num2);
        retval = CefV8Value::CreateDouble(num1 + num2);
        return true;
    }

    return false;
}

bool BrowserV8Handler::AsyncCall(const CefString &func, const CefString &params, CefRefPtr<CefV8Value> callback)
{
    auto it = render_callback_.find(js_callback_id_);
    if (it == render_callback_.cend())
    {
        CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
        CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("kCallCppFunctionMessage");

        message->GetArgumentList()->SetString(0, func);
        message->GetArgumentList()->SetString(1, params);
        message->GetArgumentList()->SetInt(2, js_callback_id_);

        render_callback_.emplace(js_callback_id_++, std::make_pair(context, callback));

        // 发送消息到Browser进程，触发CefClient::OnProcessMessageReceived调用
        CefRefPtr<CefFrame> frame = context->GetFrame();
        frame->SendProcessMessage(PID_BROWSER, message);

        return true;
    }

    return false;
}

bool BrowserV8Handler::HandleCallback(int id, bool hasError, const CefString &result)
{
    bool handled = false;
    auto it = render_callback_.find(id);
    if (it != render_callback_.cend())
    {
        CefRefPtr<CefV8Context> context = it->second.first;
        CefRefPtr<CefV8Value> callback = it->second.second;

        // Enter the context.
        context->Enter();

        CefV8ValueList arguments;
        arguments.push_back(CefV8Value::CreateString(result));
        CefRefPtr<CefV8Value> retval = callback->ExecuteFunction(nullptr, arguments);
        if (retval.get())
        {
            if (retval->IsBool())
                handled = retval->GetBoolValue();
        }
        // Exit the context.
        context->Exit();
        render_callback_.erase(it);

        // Just for test, remove when release
        CallRegisterFunc("showJsMessage", result, context->GetFrame());
        
    }

    return handled;
}

bool BrowserV8Handler::RegisterFunc(const CefString &func, CefRefPtr<CefV8Value> callback, bool replace /* = false*/)
{
    CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
    CefRefPtr<CefFrame> frame = context->GetFrame();

    if (replace)
    {
        render_registered_function_.emplace(std::make_pair(func, frame->GetIdentifier()), callback);
        return true;
    }
    else
    {
        auto it = render_registered_function_.find(std::make_pair(func, frame->GetIdentifier()));
        if (it == render_registered_function_.cend())
        {
            qInfo() << QString(QObject::tr("Register function %1 identifier=%2")).arg(func.c_str()).arg(frame->GetIdentifier());
            render_registered_function_.emplace(std::make_pair(func, frame->GetIdentifier()), callback);
            return true;
        }

        return false;
    }

    return false;
}

bool BrowserV8Handler::CallRegisterFunc(const CefString &func, const CefString &params, CefRefPtr<CefFrame> frame)
{
    qInfo() << QString(QObject::tr("CallRegisterFunc function %1 identifier=%2")).arg(func.c_str()).arg(frame->GetIdentifier());
    auto it = render_registered_function_.find(std::make_pair(func, frame->GetIdentifier()));
    if (it != render_registered_function_.cend())
    {
        auto context = frame->GetV8Context();
        auto function = it->second;

        if (context.get() && function.get())
        {
            context->Enter();

            CefV8ValueList arguments;

            // 将C++传递过来的JSON转换成Object
            CefV8ValueList json_parse_args;
            json_parse_args.push_back(CefV8Value::CreateString(params));
            CefRefPtr<CefV8Value> json_object = context->GetGlobal()->GetValue("JSON");
            CefRefPtr<CefV8Value> json_parse = json_object->GetValue("parse");
            CefRefPtr<CefV8Value> json_object_args = json_parse->ExecuteFunction(NULL, json_parse_args);
            arguments.push_back(json_object_args);

            // 执行回调函数
            CefRefPtr<CefV8Value> retval = function->ExecuteFunction(NULL, arguments);
            if (retval.get() && retval->IsObject())
            {
                QMessageBox::information(nullptr, "Prompt", QString(QObject::tr("CallRegisterFunc succeed: %1!")).arg(retval->GetValue("message")->GetStringValue().c_str()));
            }

            context->Exit();

            return true;
        }

        return false;
    }

    return false;
}