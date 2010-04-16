/*
 *    Virtual Printers folder
 *
 *    Copyright 2010 Nikolay Sivov for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include <stdarg.h>

#define COBJMACROS
#define NONAMELESSUNION
#define NONAMELESSSTRUCT

#include "winerror.h"
#include "windef.h"
#include "winbase.h"

#include "shell32_main.h"
#include "wine/debug.h"
#include "debughlp.h"

WINE_DEFAULT_DEBUG_CHANNEL (shell);

typedef struct {
    const IShellFolder2Vtbl   *lpVtbl;
    const IPersistFolder2Vtbl *lpvtblPersistFolder2;
    LONG ref;
} IPrintersFolderImpl;

static inline IPrintersFolderImpl *impl_from_IPersistFolder2(IPersistFolder2 *iface)
{
    return (IPrintersFolderImpl *)((char*)iface - FIELD_OFFSET(IPrintersFolderImpl, lpvtblPersistFolder2));
}

static HRESULT WINAPI IShellFolder_Printers_fnQueryInterface(IShellFolder2 *iface,
               REFIID riid, LPVOID *ppvObj)
{
    IPrintersFolderImpl *This = (IPrintersFolderImpl *)iface;

    TRACE ("(%p)->(%s,%p)\n", This, shdebugstr_guid (riid), ppvObj);

    *ppvObj = NULL;

    if (IsEqualIID (riid, &IID_IUnknown) ||
        IsEqualIID (riid, &IID_IShellFolder) ||
        IsEqualIID (riid, &IID_IShellFolder2))
    {
        *ppvObj = This;
    }
    else if (IsEqualIID (riid, &IID_IPersist) ||
             IsEqualIID (riid, &IID_IPersistFolder) ||
             IsEqualIID (riid, &IID_IPersistFolder2))
    {
        *ppvObj = &This->lpvtblPersistFolder2;
    }

    if (*ppvObj)
    {
        IUnknown_AddRef ((IUnknown *) (*ppvObj));
        TRACE ("-- Interface: (%p)->(%p)\n", ppvObj, *ppvObj);
        return S_OK;
    }

    ERR ("-- interface %s not supported\n", shdebugstr_guid (riid));
    return E_NOINTERFACE;
}

static ULONG WINAPI IShellFolder_Printers_fnAddRef (IShellFolder2 * iface)
{
    IPrintersFolderImpl *This = (IPrintersFolderImpl *)iface;
    ULONG refCount = InterlockedIncrement(&This->ref);
    TRACE ("(%p)->(count=%u)\n", This, refCount - 1);
    return refCount;
}

static ULONG WINAPI IShellFolder_Printers_fnRelease (IShellFolder2 * iface)
{
    IPrintersFolderImpl *This = (IPrintersFolderImpl *)iface;
    ULONG refCount = InterlockedDecrement(&This->ref);

    TRACE ("(%p)->(count=%u)\n", This, refCount + 1);

    if (!refCount)
    {
        TRACE ("-- destroying IShellFolder(%p)\n", This);
        LocalFree (This);
    }
    return refCount;
}

static HRESULT WINAPI IShellFolder_Printers_fnParseDisplayName (IShellFolder2 *iface,
               HWND hwndOwner, LPBC pbc, LPOLESTR lpszDisplayName,
               DWORD * pchEaten, LPITEMIDLIST * ppidl, DWORD * pdwAttributes)
{
    IPrintersFolderImpl *This = (IPrintersFolderImpl *)iface;

    FIXME("(%p)->(HWND=%p,%p,%p=%s,%p,pidl=%p,%p) stub\n", This,
          hwndOwner, pbc, lpszDisplayName, debugstr_w (lpszDisplayName),
          pchEaten, ppidl, pdwAttributes);

    return E_NOTIMPL;
}

static HRESULT WINAPI IShellFolder_Printers_fnEnumObjects (IShellFolder2 *iface,
               HWND hwndOwner, DWORD dwFlags, LPENUMIDLIST *ppEnumIDList)
{
    IPrintersFolderImpl *This = (IPrintersFolderImpl *)iface;
    FIXME("(%p)->(HWND=%p flags=0x%08x pplist=%p) stub\n", This,
          hwndOwner, dwFlags, ppEnumIDList);
    return E_NOTIMPL;
}

static HRESULT WINAPI IShellFolder_Printers_fnBindToObject (IShellFolder2 *iface,
               LPCITEMIDLIST pidl, LPBC pbcReserved, REFIID riid, LPVOID *ppvOut)
{
    IPrintersFolderImpl *This = (IPrintersFolderImpl *)iface;
    FIXME("(%p)->(pidl=%p,%p,%s,%p) stub\n", This,
          pidl, pbcReserved, shdebugstr_guid (riid), ppvOut);
    return E_NOTIMPL;
}

static HRESULT WINAPI IShellFolder_Printers_fnBindToStorage (IShellFolder2 * iface,
               LPCITEMIDLIST pidl, LPBC pbcReserved, REFIID riid, LPVOID *ppvOut)
{
    IPrintersFolderImpl *This = (IPrintersFolderImpl *)iface;

    FIXME("(%p)->(pidl=%p,%p,%s,%p) stub\n", This,
          pidl, pbcReserved, shdebugstr_guid (riid), ppvOut);

    *ppvOut = NULL;
    return E_NOTIMPL;
}

static HRESULT WINAPI IShellFolder_Printers_fnCompareIDs(IShellFolder2 *iface,
               LPARAM lParam, LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
    IPrintersFolderImpl *This = (IPrintersFolderImpl *)iface;
    FIXME("(%p)->(0x%08lx,pidl1=%p,pidl2=%p) stub\n", This, lParam, pidl1, pidl2);
    return E_NOTIMPL;
}

static HRESULT WINAPI IShellFolder_Printers_fnCreateViewObject(IShellFolder2 *iface,
               HWND hwndOwner, REFIID riid, LPVOID *ppvOut)
{
    IPrintersFolderImpl *This = (IPrintersFolderImpl *)iface;
    HRESULT hr = E_NOTIMPL;

    TRACE("(%p)->(hwnd=%p,%s,%p)\n", This,
          hwndOwner, shdebugstr_guid (riid), ppvOut);

    if (!ppvOut)
        return E_INVALIDARG;

    *ppvOut = NULL;

    if (IsEqualIID (riid, &IID_IShellView))
    {
        IShellView *pShellView = IShellView_Constructor ((IShellFolder *) iface);
        if (pShellView)
        {
            hr = IShellView_QueryInterface (pShellView, riid, ppvOut);
            IShellView_Release (pShellView);
        }
    }
    else
        WARN("unsupported interface %s\n", shdebugstr_guid (riid));

    return hr;
}

static HRESULT WINAPI IShellFolder_Printers_fnGetAttributesOf (IShellFolder2 * iface,
                UINT cidl, LPCITEMIDLIST * apidl, DWORD * rgfInOut)
{
    IPrintersFolderImpl *This = (IPrintersFolderImpl *)iface;
    FIXME("(%p)->(cidl=%d apidl=%p mask=%p (0x%08x)) stub\n",
           This, cidl, apidl, rgfInOut, rgfInOut ? *rgfInOut : 0);
    return E_NOTIMPL;
}

static HRESULT WINAPI IShellFolder_Printers_fnGetUIObjectOf (IShellFolder2 * iface,
                HWND hwndOwner, UINT cidl, LPCITEMIDLIST * apidl, REFIID riid,
                UINT * prgfInOut, LPVOID * ppvOut)
{
    IPrintersFolderImpl *This = (IPrintersFolderImpl *)iface;

    FIXME("(%p)->(%p,%u,apidl=%p,%s,%p,%p) stub\n", This,
          hwndOwner, cidl, apidl, shdebugstr_guid (riid), prgfInOut, ppvOut);

    if (!ppvOut)
        return E_INVALIDARG;

    *ppvOut = NULL;

    return E_NOTIMPL;
}

static HRESULT WINAPI IShellFolder_Printers_fnGetDisplayNameOf (IShellFolder2 *iface,
               LPCITEMIDLIST pidl, DWORD dwFlags, LPSTRRET strRet)
{
    IPrintersFolderImpl *This = (IPrintersFolderImpl *)iface;

    FIXME("(%p)->(pidl=%p,0x%08x,%p) stub\n", This, pidl, dwFlags, strRet);

    if (!strRet)
        return E_INVALIDARG;

    return E_NOTIMPL;
}

static HRESULT WINAPI IShellFolder_Printers_fnSetNameOf (
               IShellFolder2 * iface, HWND hwndOwner, LPCITEMIDLIST pidl,
               LPCOLESTR lpName, DWORD dwFlags, LPITEMIDLIST * pPidlOut)
{
    IPrintersFolderImpl *This = (IPrintersFolderImpl *)iface;
    FIXME ("(%p)->(%p,pidl=%p,%s,%u,%p) stub\n", This,
           hwndOwner, pidl, debugstr_w (lpName), dwFlags, pPidlOut);
    return E_FAIL;
}

static HRESULT WINAPI IShellFolder_Printers_fnGetDefaultSearchGUID (
               IShellFolder2 * iface, GUID * pguid)
{
    IPrintersFolderImpl *This = (IPrintersFolderImpl *)iface;
    FIXME ("(%p) stub\n", This);
    return E_NOTIMPL;
}

static HRESULT WINAPI IShellFolder_Printers_fnEnumSearches (
               IShellFolder2 * iface, IEnumExtraSearch ** ppenum)
{
    IPrintersFolderImpl *This = (IPrintersFolderImpl *)iface;
    FIXME ("(%p) stub\n", This);
    return E_NOTIMPL;
}

static HRESULT WINAPI IShellFolder_Printers_fnGetDefaultColumn (
               IShellFolder2 *iface, DWORD dwRes, ULONG *pSort, ULONG *pDisplay)
{
    IPrintersFolderImpl *This = (IPrintersFolderImpl *)iface;
    FIXME("(%p) stub\n", This);
    return E_NOTIMPL;
}

static HRESULT WINAPI IShellFolder_Printers_fnGetDefaultColumnState (
               IShellFolder2 * iface, UINT iColumn, DWORD * pcsFlags)
{
    IPrintersFolderImpl *This = (IPrintersFolderImpl *)iface;
    FIXME("(%p) stub\n", This);
    return E_NOTIMPL;
}

static HRESULT WINAPI IShellFolder_Printers_fnGetDetailsEx (IShellFolder2 * iface,
               LPCITEMIDLIST pidl, const SHCOLUMNID * pscid, VARIANT * pv)
{
    IPrintersFolderImpl *This = (IPrintersFolderImpl *)iface;
    FIXME ("(%p) stub\n", This);
    return E_NOTIMPL;
}

static HRESULT WINAPI IShellFolder_Printers_fnGetDetailsOf (IShellFolder2 * iface,
               LPCITEMIDLIST pidl, UINT iColumn, SHELLDETAILS * psd)
{
    IPrintersFolderImpl *This = (IPrintersFolderImpl *)iface;
    FIXME("(%p)->(%p %i %p) stub\n", This, pidl, iColumn, psd);
    return E_NOTIMPL;
}

static HRESULT WINAPI IShellFolder_Printers_fnMapColumnToSCID (
               IShellFolder2 * iface, UINT column, SHCOLUMNID * pscid)
{
    IPrintersFolderImpl *This = (IPrintersFolderImpl *)iface;
    FIXME ("(%p)->(%u %p) stub\n", This, column, pscid);
    return E_NOTIMPL;
}

static const IShellFolder2Vtbl vtbl_ShellFolder2 =
{
    /* IUnknown */
    IShellFolder_Printers_fnQueryInterface,
    IShellFolder_Printers_fnAddRef,
    IShellFolder_Printers_fnRelease,
    /* IShellFolder */
    IShellFolder_Printers_fnParseDisplayName,
    IShellFolder_Printers_fnEnumObjects,
    IShellFolder_Printers_fnBindToObject,
    IShellFolder_Printers_fnBindToStorage,
    IShellFolder_Printers_fnCompareIDs,
    IShellFolder_Printers_fnCreateViewObject,
    IShellFolder_Printers_fnGetAttributesOf,
    IShellFolder_Printers_fnGetUIObjectOf,
    IShellFolder_Printers_fnGetDisplayNameOf,
    IShellFolder_Printers_fnSetNameOf,
    /* IShellFolder2 */
    IShellFolder_Printers_fnGetDefaultSearchGUID,
    IShellFolder_Printers_fnEnumSearches,
    IShellFolder_Printers_fnGetDefaultColumn,
    IShellFolder_Printers_fnGetDefaultColumnState,
    IShellFolder_Printers_fnGetDetailsEx,
    IShellFolder_Printers_fnGetDetailsOf,
    IShellFolder_Printers_fnMapColumnToSCID
};

/*** IPersistFolder2 ***/
static HRESULT WINAPI IPersistFolder2_Printers_fnQueryInterface(IPersistFolder2 * iface,
               REFIID iid, LPVOID * ppvObj)
{
    IPrintersFolderImpl *This = impl_from_IPersistFolder2(iface);
    return IUnknown_QueryInterface( (IUnknown*) This, iid, ppvObj);
}

static ULONG WINAPI IPersistFolder2_Printers_fnAddRef(IPersistFolder2 *iface)
{
    IPrintersFolderImpl *This = impl_from_IPersistFolder2(iface);
    return IUnknown_AddRef( (IUnknown*) This);
}

static ULONG WINAPI IPersistFolder2_Printers_fnRelease(IPersistFolder2 *iface)
{
    IPrintersFolderImpl *This = impl_from_IPersistFolder2(iface);
    return IUnknown_Release( (IUnknown*) This);
}

static HRESULT WINAPI IPersistFolder2_Printers_fnGetClassID(IPersistFolder2 *iface, CLSID *classId)
{
    IPrintersFolderImpl *This = impl_from_IPersistFolder2(iface);

    FIXME("(%p) stub\n", This);

    if (!classId)
        return E_POINTER;

    return E_NOTIMPL;
}

static HRESULT WINAPI IPersistFolder2_Printers_fnInitialize(IPersistFolder2 *iface, LPCITEMIDLIST pidl)
{
    IPrintersFolderImpl *This = impl_from_IPersistFolder2(iface);
    FIXME("(%p) stub\n", This);
    return E_NOTIMPL;
}

static HRESULT WINAPI IPersistFolder2_Printers_fnGetCurFolder(IPersistFolder2 *iface, LPITEMIDLIST *pidl)
{
    IPrintersFolderImpl *This = impl_from_IPersistFolder2(iface);
    FIXME("(%p) stub\n", This);
    return E_NOTIMPL;
}

static const IPersistFolder2Vtbl vtbl_PersistFolder2 =
{
    IPersistFolder2_Printers_fnQueryInterface,
    IPersistFolder2_Printers_fnAddRef,
    IPersistFolder2_Printers_fnRelease,
    IPersistFolder2_Printers_fnGetClassID,
    IPersistFolder2_Printers_fnInitialize,
    IPersistFolder2_Printers_fnGetCurFolder
};

HRESULT WINAPI Printers_Constructor(IUnknown * pUnkOuter, REFIID riid, LPVOID * ppv)
{
    IPrintersFolderImpl *sf;

    TRACE("unkOut=%p %s\n", pUnkOuter, shdebugstr_guid (riid));

    if (!ppv)
        return E_POINTER;
    if (pUnkOuter)
        return CLASS_E_NOAGGREGATION;

    sf = LocalAlloc(LMEM_FIXED, sizeof(IPrintersFolderImpl));
    if (!sf)
        return E_OUTOFMEMORY;

    sf->ref = 0;
    sf->lpVtbl = &vtbl_ShellFolder2;
    sf->lpvtblPersistFolder2 = &vtbl_PersistFolder2;

    if (IUnknown_QueryInterface((IShellFolder2 *)sf, riid, ppv) != S_OK)
    {
        IUnknown_Release((IShellFolder2 *) sf);
        return E_NOINTERFACE;
    }

    TRACE("--(%p)\n", sf);
    return S_OK;
}
