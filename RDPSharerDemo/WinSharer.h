#pragma once

#include <rdpencomapi.h>
#include <memory>
#include <atlcomcli.h>
#include <stdexcept>
#include <iostream>
#include <Windows.h>
#include "Logger.h"
#include "SessionEvents.h"
#include <fstream>
#include <vector>

class WinSharer
{
public:
    WinSharer();

    void open();

    void close();

    void writeInvitation() const;

private:

    CComPtr<IRDPSRAPISharingSession> m_rdpSession;


    CComPtr<IRDPSRAPIInvitationManager> m_invMgr;
    CComPtr<IRDPSRAPIVirtualChannel> m_channel;

    EventSink m_eventSink;

    mutable bool m_paused = false, m_started = false, m_invited = false;

    void ConnectEvent(IUnknown* rdpSession, const _IRDPSessionEvents& events);
};