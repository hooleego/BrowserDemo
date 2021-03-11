#ifndef BROWSER_V8_HANDLER_H
#define BROWSER_V8_HANDLER_H

#include <map>
#include <string>
#include "include/cef_v8.h"

class BrowserV8Handler : public CefV8Handler
{
public:
    BrowserV8Handler();
    ~BrowserV8Handler();

    /**
     * @brief CefV8Handler Methods, Which will be called when the V8 extension 
	 *  is called in the Javascript environment
     * 
     * @param name JavaScript调用的C++方法名字
     * @param object JavaScript调用者对象
     * @param arguments JavaScript传递的参数
     * @param retval 需要返回给JavaScript的值设置给这个对象
     * @param exception 通知异常信息给JavaScript
     * @return true 
     * @return false 
     */
    virtual bool Execute(const CefString &name, CefRefPtr<CefV8Value> object, const CefV8ValueList &arguments, CefRefPtr<CefV8Value> &retval, CefString &exception);

    bool SyncCall(const CefString &function_name, const CefString &params, CefRefPtr<CefV8Value> &retval);
    bool AsyncCall(const CefString &function_name, const CefString &params, CefRefPtr<CefV8Value> callback);
    bool HandleCallback(int id, bool hasError, const CefString &result);

    bool RegisterFunc(const CefString &function_name, CefRefPtr<CefV8Value> callback, bool replace = false);
    bool CallRegisterFunc(const CefString& function_name, const CefString& params, CefRefPtr<CefFrame> frame);
    
private:
    int js_callback_id_;
    // Map of message callbacks.
    typedef std::map<int, std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value>>> CallbackMap;
    CallbackMap render_callback_;

    typedef std::map<std::pair<CefString, int64_t>, CefRefPtr<CefV8Value>> RegisteredMap;
    RegisteredMap render_registered_function_;

    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(BrowserV8Handler);
};

#endif // BROWSER_V8_HANDLER_H
