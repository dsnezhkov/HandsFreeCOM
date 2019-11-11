#include "Precompiled.h"
#include "Server.h"
#include <wrl.h>


#include <string>

using std::string;
using namespace Microsoft::WRL;


struct Det : IDet
{
    long m_count;

    Det() : m_count(0) {}

    ULONG __stdcall AddRef() override
    {
        return _InterlockedIncrement(&m_count);
    }

    ULONG __stdcall Release() override
    {
        ULONG result = _InterlockedDecrement(&m_count);

        if (0 == result)
        {
            delete this;
        }

        return result;
    }

    HRESULT __stdcall QueryInterface(IID const & id,
                                     void ** result) override
    {
        ASSERT(result);

        if (id == __uuidof(IDet) ||
            id == __uuidof(IUnknown))
        {
            *result = static_cast<IDet *>(this);
        }
        else
        {
            *result = 0;
            return E_NOINTERFACE;
        }

        static_cast<IUnknown *>(*result)->AddRef();
        return S_OK;
    }

    void __stdcall Detonate() override
    {
		// return; 
		// Open a handle to the file
	   HANDLE hFile = CreateFile(
		  L"Flag.txt",     // Filename
		  GENERIC_WRITE,          // Desired access
		  FILE_SHARE_READ,        // Share mode
		  NULL,                   // Security attributes
		  CREATE_NEW,             // Creates a new file, only if it doesn't already exist
		  FILE_ATTRIBUTE_NORMAL,  // Flags and attributes
		  NULL);                  // Template file handle


	   // Write data to the file
	   std::string strText = "Hello World!"; // For C use LPSTR (char*) or LPWSTR (wchar_t*)
	   DWORD bytesWritten;
	   WriteFile(
		  hFile,            // Handle to the file
		  strText.c_str(),  // Buffer to write
		  strText.size(),   // Buffer size
		  &bytesWritten,    // Bytes written
		  nullptr);         // Overlapped

	   // Close the handle once we don't need it.
	   CloseHandle(hFile);
    }

	void __stdcall EndDetonate() override {
	 
		TRACE("Deleting File.");
		DeleteFile(L"Flag.txt");
	}
};


struct Armory : IDetFactory
{
    ULONG __stdcall AddRef() override
    {
        return 2;
    }

    ULONG __stdcall Release() override
    {
        return 1;
    }

    HRESULT __stdcall QueryInterface(IID const & id,
                                     void ** result) override
    {
        ASSERT(result);

        if (id == __uuidof(IDetFactory) ||
            id == __uuidof(IUnknown))
        {
            *result = static_cast<IDetFactory *>(this);
        }
        else
        {
            *result = 0;
            return E_NOINTERFACE;
        }

        return S_OK;
    }

    HRESULT __stdcall CreateDet(IDet ** result) override
    {
        ASSERT(result);

        *result = new (std::nothrow) Det;

        if (0 == *result)
        {
            return E_OUTOFMEMORY;
        }

        (*result)->AddRef();
        return S_OK;
    }
};

HRESULT __stdcall DllGetClassObject(CLSID const & clsid,
                                    IID const & iid,
                                    void ** result)
{
    ASSERT(result);
    *result = nullptr;

    if (__uuidof(Det) == clsid)
    {
        static Armory armory;

        return armory.QueryInterface(iid, result);
    }

    return CLASS_E_CLASSNOTAVAILABLE;
}


