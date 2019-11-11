#pragma once

#include <unknwn.h>

struct __declspec(uuid("5d8a7d33-059f-418a-8d77-5f3944d63b6d")) Det;

struct __declspec(uuid("5a196c0f-e296-4b35-9249-f3d7ad5999fd")) IDet : IUnknown
{
    virtual void __stdcall Detonate() = 0;
	virtual void __stdcall EndDetonate() = 0;
};


   

struct __declspec(uuid("9362f817-85b6-4a80-81de-772c792922ff")) IDetFactory : IUnknown
{
    virtual HRESULT __stdcall CreateDet(IDet ** hen) = 0;
};
