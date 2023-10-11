#include "pch.h"
#include "classfactory.h"
#include <cstdio>

#define EXPORT

EXPORT BOOL STDMETHODCALLTYPE DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    return TRUE;
}

extern "C" EXPORT HRESULT STDMETHODCALLTYPE DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID * ppv)
{
    OutputDebugString(L"Profiler.dll!DllGetClassObject\n");
    fflush(stdout);

    if (ppv == nullptr)
    {
        return E_FAIL;
    }

    auto factory = new ClassFactory(rclsid);
    if (factory == nullptr)
    {
        return E_FAIL;
    }

    return factory->QueryInterface(riid, ppv);
}

extern "C" EXPORT HRESULT STDMETHODCALLTYPE DllCanUnloadNow()
{
    return S_OK;
}