#pragma once
#include <rdpencomapi.h>
#include <string>

class EventSink : public _IRDPSessionEvents {
public:
    HRESULT STDMETHODCALLTYPE  QueryInterface(
        REFIID iid, void** ppvObject) override;

    ULONG STDMETHODCALLTYPE  AddRef() override;

    ULONG STDMETHODCALLTYPE  Release() override;

    HRESULT STDMETHODCALLTYPE  GetTypeInfoCount(
        __RPC__out UINT* pctinfo) override;

    HRESULT STDMETHODCALLTYPE  GetTypeInfo(
        UINT iTInfo,
        LCID lcid,
        __RPC__deref_out_opt ITypeInfo** ppTInfo) override;

    HRESULT STDMETHODCALLTYPE  GetIDsOfNames(
        __RPC__in REFIID riid,
        __RPC__in_ecount_full(cNames) LPOLESTR* rgszNames,
        UINT cNames,
        LCID lcid,
        __RPC__out_ecount_full(cNames) DISPID* rgDispId) override;

    HRESULT STDMETHODCALLTYPE Invoke(
        DISPID dispIdMember,
        REFIID riid,
        LCID lcid,
        WORD wFlags,
        DISPPARAMS FAR* pDispParams,
        VARIANT FAR* pVarResult,
        EXCEPINFO FAR* pExcepInfo,
        unsigned int FAR* puArgErr) override;
private:
    unsigned long m_cRef = 0;
};
