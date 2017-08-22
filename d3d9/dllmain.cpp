/**
* Copyright (C) 2017 Elisha Riedlinger
*
* This software is  provided 'as-is', without any express  or implied  warranty. In no event will the
* authors be held liable for any damages arising from the use of this software.
* Permission  is granted  to anyone  to use  this software  for  any  purpose,  including  commercial
* applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*   1. The origin of this software must not be misrepresented; you must not claim that you  wrote the
*      original  software. If you use this  software  in a product, an  acknowledgment in the product
*      documentation would be appreciated but is not required.
*   2. Altered source versions must  be plainly  marked as such, and  must not be  misrepresented  as
*      being the original software.
*   3. This notice may not be removed or altered from any source distribution.
*
* Code taken from code found here: https://gist.github.com/shaunlebron/3854bf4eec5bec297907
*/

#include "d3d9.h"

std::ofstream LOG;

D3DC9 orig_Direct3DCreate9;

bool WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	static HMODULE d3d9dll = nullptr;

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		LOG.open("d3d9.log", std::ios::trunc);
		LOG << "Loading d3d9.dll\n";
		char path[MAX_PATH];
		GetSystemDirectoryA(path, MAX_PATH);
		strcat_s(path, "\\d3d9.dll");
		d3d9dll = LoadLibraryA(path);
		orig_Direct3DCreate9 = (D3DC9)GetProcAddress(d3d9dll, "Direct3DCreate9");
		break;

	case DLL_PROCESS_DETACH:
		FreeLibrary(d3d9dll);
		LOG.flush();
		break;
	}

	return true;
}

void logf(char * fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	static constexpr DWORD BuffSize = 250;
	static char buffer[BuffSize];
	sprintf_s(buffer, fmt, ap);
	LOG << buffer;
}

IDirect3D9 *WINAPI myDirect3DCreate9(UINT SDKVersion)
{
	return new myIDirect3D9(orig_Direct3DCreate9(SDKVersion));
}