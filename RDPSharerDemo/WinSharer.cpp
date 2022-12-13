#include "WinSharer.h"

WinSharer::WinSharer() {
    const GUID CLSID_RDPSession = { 0x9B78F0E6, 0x3E05, 0x4A5B, 0xB2, 0xE8, 0xE7, 0x43, 0xA8, 0x95, 0x6B, 0x65 };
    const GUID IID_IRDPSRAPISharingSession = { 0xeeb20886, 0xe470, 0x4cf6, 0x84, 0x2b, 0x27, 0x39, 0xc0, 0xec, 0x5c, 0xfb };
    IRDPSRAPISharingSession* rdpSession;
    IUnknown* iUnknow;

    CHECK(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED));
    //CHECK(m_rdpSession.CoCreateInstance(__uuidof(RDPSession), nullptr, CLSCTX_INPROC));
    CHECK(CoCreateInstance(CLSID_RDPSession, NULL, CLSCTX_ALL, IID_IUnknown, (void**)&iUnknow));
    CHECK(iUnknow->QueryInterface(IID_IRDPSRAPISharingSession, (void**)&rdpSession));


    CComPtr<IRDPSRAPISessionProperties> props;
    CHECK(rdpSession->get_Properties(&props));
    const CComBSTR propName = "FrameCaptureIntervalInMs";
    const CComVariant propValue{ 1000, VT_I4 };
    CHECK(props->put_Property(propName, propValue));

    m_rdpSession.Attach(rdpSession);

    WriteToLog(L"Successfully created RDPSession instance");

    CComPtr<IRDPSRAPIVirtualChannelManager> vcm;
    CHECK(m_rdpSession->get_VirtualChannelManager(&vcm));

    CHECK(vcm->CreateVirtualChannel(CComBSTR("mes"), CHANNEL_PRIORITY_HI, 0, &m_channel));

    CHECK(m_rdpSession->SetDesktopSharedRect(0, 0, 1000, 1000));

    ConnectEvent((IUnknown*)m_rdpSession, m_eventSink);
}

void WinSharer::open()
{
    if (!m_started)
    {
        CHECK(m_rdpSession->Open());

        CHECK(m_rdpSession->get_Invitations(&m_invMgr));
        m_started = true;
    }
    else
    {
        CHECK(m_rdpSession->Resume());
        m_paused = false;
    }
    WriteToLog(L"Sharing session successfully started");

    CComPtr<IRDPSRAPISessionProperties> props;
    CHECK(m_rdpSession->get_Properties(&props));
    const CComBSTR propName = L"FrameCaptureIntervalInMs";
    CComVariant propValue;
    CHECK(props->get_Property(propName, &propValue));
    const int frameIntervalMs = propValue.intVal;
    WriteToLog(L"FrameCaptureIntervalInMs: " + std::to_wstring(frameIntervalMs));
}

void WinSharer::close()
{
    CHECK(m_rdpSession->Pause());

    WriteToLog(L"Sharing session successfully stopped");
}

void WinSharer::writeInvitation() const
{
    const wchar_t* path = L"inv.xml";
    if (!m_invited) {
        CComPtr<IRDPSRAPIInvitation> inv;
        CComBSTR con_str, auth(L"WinPresenter"), group(L"PresentationGroup"), password(L"");

        CHECK(m_invMgr->CreateInvitation(auth, group, password, 5, &inv));
        CHECK(inv->get_ConnectionString(&con_str));

        std::wofstream inv_stream(path);
        inv_stream << std::wstring(con_str);
        m_invited = true;
    }
    WriteToLog(std::wstring(L"Invitations were written to ") + path);
}

void WinSharer::ConnectEvent(IUnknown* rdpSession, const _IRDPSessionEvents& events)
{
    CComPtr<IConnectionPointContainer> picpc;
    CComPtr<IConnectionPoint> picp;
    DWORD tid;

    CHECK(rdpSession->QueryInterface(IID_IConnectionPointContainer, (void**)&picpc));
    if (picpc) {
        CHECK(picpc->FindConnectionPoint(__uuidof(_IRDPSessionEvents), &picp));
        if (picp)
            CHECK(picp->Advise((IUnknown*)&events, &tid));
    }
}
