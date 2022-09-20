#include "bhop.h"
#include "includes.h"
#include "offsets.h"
#include "main.h"

DWORD WINAPI BunnyHop(LPVOID lp) {
	DWORD gameModule = (DWORD)GetModuleHandle("client.dll");
	//DWORD gameModule = reinterpret_cast<DWORD>(GetModuleHandle("client.dll"));
	DWORD localplayer = *(DWORD*)(gameModule + signatures::dwLocalPlayer);
	while (localplayer == NULL) {
		localplayer = *(DWORD*)(gameModule + signatures::dwLocalPlayer);
	}
	while (true) {
		if (bhop) {
			DWORD flag = *(BYTE*)(localplayer + netvars::m_fFlags);
			
				if (GetAsyncKeyState(VK_RETURN) && flag & (1 << 0)) {
					*(DWORD*)(gameModule + signatures::dwForceJump) = 6;
					
					Sleep(1);
				}
				Sleep(1);
		}
	}
	Sleep(1);
	return 0;
}