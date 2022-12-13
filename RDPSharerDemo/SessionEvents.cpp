#include "SessionEvents.h"
#include "Logger.h"
#include "WinSharer.h"

namespace
{
    void OnAttendeeConnected(IDispatch* pAttendee)
    {
        CComPtr<IRDPSRAPIAttendee> pRDPAtendee;
        CHECK(pAttendee->QueryInterface(__uuidof(IRDPSRAPIAttendee), (void**)&pRDPAtendee));
        CHECK(pRDPAtendee->put_ControlLevel(CTRL_LEVEL::CTRL_LEVEL_VIEW));
        WriteToLog(std::wstring(L"An attendee connected, put CTRL_LEVEL_VIEW"));
    }

    void OnAttendeeDisconnected(IDispatch* pAttendee)
    {
        CComPtr<IRDPSRAPIAttendeeDisconnectInfo> info;
        ATTENDEE_DISCONNECT_REASON reason;
        CHECK(pAttendee->QueryInterface(__uuidof(IRDPSRAPIAttendeeDisconnectInfo), (void**)&info));
        std::wstring textReason;
        if (SUCCEEDED(info->get_Reason(&reason))) {
            switch (reason) {
            case ATTENDEE_DISCONNECT_REASON_APP:
                textReason = L"Viewer terminated session!";
                break;
            case ATTENDEE_DISCONNECT_REASON_ERR:
                textReason = L"Internal Error!";
                break;
            case ATTENDEE_DISCONNECT_REASON_CLI:
                textReason = L"Attendee requested termination!";
                break;
            default:
                break;
                textReason = L"Unknown reason!";
            }
        }
        WriteToLog(L"Attendee disconnected, reason: " + textReason);
    }

    void OnControlLevelChangeRequest(IDispatch* pAttendee, CTRL_LEVEL RequestedLevel)
    {
        CComPtr<IRDPSRAPIAttendee> pRDPAtendee;
        CHECK(pAttendee->QueryInterface(__uuidof(IRDPSRAPIAttendee), (void**)&pRDPAtendee));
        CHECK(pRDPAtendee->put_ControlLevel(RequestedLevel));
        CComBSTR att_name;
        CHECK(pRDPAtendee->get_RemoteName(&att_name));

        WriteToLog(*att_name + std::wstring(L"`s Control level changed, the new one is ") + (RequestedLevel == CTRL_LEVEL::CTRL_LEVEL_INTERACTIVE ? L"CTRL_LEVEL_INTERACTIVE" : L"CTRL_LEVEL_VIEW"));
    }

    void OnVirtualChannelDataReceive(BSTR data)
    {
        if (static_cast<std::wstring>(data) == L"EnableTrackball") {
            keybd_event(VK_MENU, NULL, NULL, NULL);
            keybd_event(VK_F5, NULL, NULL, NULL);

            keybd_event(VK_MENU, NULL, KEYEVENTF_KEYUP, NULL);
            keybd_event(VK_F5, NULL, KEYEVENTF_KEYUP, NULL);
            WriteToLog(L"Trackball message acquired");
        }
    }
}

HRESULT STDMETHODCALLTYPE EventSink::QueryInterface(
    REFIID iid, void** ppvObject) {
    *ppvObject = 0;
    if (iid == IID_IUnknown || iid == IID_IDispatch || iid == __uuidof(_IRDPSessionEvents))
        *ppvObject = this;
    if (*ppvObject)
    {
        ((IUnknown*)(*ppvObject))->AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE  EventSink::AddRef() {
    InterlockedIncrement(&m_cRef);
    return m_cRef;
}

ULONG STDMETHODCALLTYPE  EventSink::Release() {
    ULONG ulRefCount = InterlockedDecrement(&m_cRef);
    if (0 == m_cRef)
    {
        delete this;
    }
    return ulRefCount;
}


HRESULT STDMETHODCALLTYPE  EventSink::GetTypeInfoCount(
    __RPC__out UINT* pctinfo) {
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE  EventSink::GetTypeInfo(
    UINT iTInfo,
    LCID lcid,
    __RPC__deref_out_opt ITypeInfo** ppTInfo) {
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE  EventSink::GetIDsOfNames(
    __RPC__in REFIID riid,
    __RPC__in_ecount_full(cNames) LPOLESTR* rgszNames,
    UINT cNames,
    LCID lcid,
    __RPC__out_ecount_full(cNames) DISPID* rgDispId) {
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE EventSink::Invoke(
    DISPID dispIdMember,
    REFIID riid,
    LCID lcid,
    WORD wFlags,
    DISPPARAMS FAR* pDispParams,
    VARIANT FAR* pVarResult,
    EXCEPINFO FAR* pExcepInfo,
    unsigned int FAR* puArgErr) {
    switch (dispIdMember) {
    case DISPID_RDPSRAPI_EVENT_ON_ATTENDEE_CONNECTED:
        OnAttendeeConnected(pDispParams->rgvarg[0].pdispVal);
        break;
    case DISPID_RDPSRAPI_EVENT_ON_ATTENDEE_DISCONNECTED:
        OnAttendeeDisconnected(pDispParams->rgvarg[0].pdispVal);
        break;
    case DISPID_RDPSRAPI_EVENT_ON_CTRLLEVEL_CHANGE_REQUEST:
        OnControlLevelChangeRequest(pDispParams->rgvarg[1].pdispVal, (CTRL_LEVEL)pDispParams->rgvarg[0].intVal);
        break;
    case DISPID_RDPSRAPI_EVENT_ON_VIRTUAL_CHANNEL_DATARECEIVED:
        OnVirtualChannelDataReceive(pDispParams->rgvarg->bstrVal);
        break;
    case DISPID_RDPSRAPI_EVENT_ON_GRAPHICS_STREAM_PAUSED:
        WriteToLog(L"Root wnd session was paused");
        break;
    case DISPID_RDPSRAPI_EVENT_ON_GRAPHICS_STREAM_RESUMED:
        WriteToLog(L"Root wnd session was resumed");
        break;
    default:
        WriteToLog(std::wstring(L"Invoked ") + std::to_wstring(static_cast<int>(dispIdMember)));
        break;
    }
    return S_OK;
}

