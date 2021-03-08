#include "browserclient.h"
#include "browserview.h"

#include <sstream>
#include <string>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/cef_parser.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

namespace
{

  // Returns a data: URI with the specified contents.
  std::string GetDataURI(const std::string &data, const std::string &mime_type)
  {
    return "data:" + mime_type + ";base64," +
           CefURIEncode(CefBase64Encode(data.data(), data.size()), false)
               .ToString();
  }

} // namespace

BrowserClient::BrowserClient(bool use_views, BrowserView *view) : use_views_(use_views), is_closing_(false), m_view(view)
{
  
}

BrowserClient::~BrowserClient()
{
}

CefRefPtr<CefDisplayHandler> BrowserClient::GetDisplayHandler()
{
    return this;
}
CefRefPtr<CefLifeSpanHandler> BrowserClient::GetLifeSpanHandler()
{
    return this;
}
CefRefPtr<CefLoadHandler> BrowserClient::GetLoadHandler()
{
    return this;
}

void BrowserClient::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString &title)
{
  CEF_REQUIRE_UI_THREAD();

  if (use_views_)
  {
    // Set the title of the window using the Views framework.
    CefRefPtr<CefBrowserView> browser_view = CefBrowserView::GetForBrowser(browser);
    if (browser_view)
    {
      CefRefPtr<CefWindow> window = browser_view->GetWindow();
      if (window)
        window->SetTitle(title);
    }
  }
  else
  {
    // Set the title of the window using platform APIs.
    PlatformTitleChange(browser, title);
  }
}

void BrowserClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
  CEF_REQUIRE_UI_THREAD();

  // Add to the list of existing browsers.
  m_browserList.push_back(browser);

  if (m_view)
  {
      m_view->setIdentifier(browser->GetIdentifier());
  }
}

bool BrowserClient::DoClose(CefRefPtr<CefBrowser> browser)
{
  CEF_REQUIRE_UI_THREAD();

  // Closing the main window requires special handling. See the DoClose()
  // documentation in the CEF header for a detailed description of this
  // process.
  if (m_browserList.size() == 1)
  {
    // Set a flag to indicate that the window close should be allowed.
    is_closing_ = true;
  }

  // Allow the close. For windowed browsers this will result in the OS close
  // event being sent.
  return false;
}

void BrowserClient::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
  CEF_REQUIRE_UI_THREAD();

  // Remove from the list of existing browsers.
  QList<CefRefPtr<CefBrowser>>::iterator bit = m_browserList.begin();
  for (; bit != m_browserList.end(); ++bit)
  {
    if ((*bit)->IsSame(browser))
    {
        m_browserList.erase(bit);
      break;
    }
  }

  if (m_browserList.empty())
  {
    // All browser windows have closed. Quit the application message loop.
    CefQuitMessageLoop();
  }
}

void BrowserClient::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString &errorText, const CefString &failedUrl)
{
  CEF_REQUIRE_UI_THREAD();

  // Don't display an error for downloaded files.
  if (errorCode == ERR_ABORTED)
    return;

  // Display a load error message using a data: URI.
  std::stringstream ss;
  ss << "<html><body bgcolor=\"white\">"
        "<h2>Failed to load URL "
     << std::string(failedUrl) << " with error " << std::string(errorText)
     << " (" << errorCode << ").</h2></body></html>";

  frame->LoadURL(GetDataURI(ss.str(), "text/html"));
}

CefRefPtr<CefBrowser> BrowserClient::GetBrowserByID(int browserId)
{
    for (auto it = m_browserList.begin(); it != m_browserList.end(); it++)
    {

        if (browserId == it->get()->GetIdentifier())
        {
            return it->get();
        }
    }

    return nullptr;
}

void BrowserClient::CloseAllBrowsers(bool force_close)
{
  if (!CefCurrentlyOn(TID_UI))
  {
    // Execute on the UI thread.
    CefPostTask(TID_UI, base::Bind(&BrowserClient::CloseAllBrowsers, this, force_close));
    return;
  }

  if (m_browserList.empty())
    return;

  QList<CefRefPtr<CefBrowser>>::const_iterator it = m_browserList.begin();
  for (; it != m_browserList.end(); ++it)
    (*it)->GetHost()->CloseBrowser(force_close);
}

bool BrowserClient::IsClosing() const
{
    return is_closing_;
}