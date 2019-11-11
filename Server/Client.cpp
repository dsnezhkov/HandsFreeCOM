#include "Precompiled.h"
#include "Server.h"
#include <wrl.h>
#include "Runtime.h"
#include <chrono>
#include <thread>

using namespace Microsoft::WRL;

INT __stdcall WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
	PSTR /*lpCmdLine*/, INT /*nCmdShow*/)
{


	ComRuntime runtime;

	ComPtr<IDetFactory> armory;

	HR(CoGetClassObject(__uuidof(Det),
		CLSCTX_INPROC_SERVER,
		nullptr,
		__uuidof(armory),
		reinterpret_cast<void **>(armory.GetAddressOf())));

	ComPtr<IDet> det;

	HR(armory->CreateDet(det.GetAddressOf()));
	
	det->Detonate();
	std::this_thread::sleep_for(std::chrono::seconds(30));
	det->EndDetonate();
	det->Release();
}