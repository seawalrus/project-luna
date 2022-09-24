#include "includes.h"
#include "main.h"
#include "noflash.h"
DWORD WINAPI NoFlash(LPVOID lp) {
	constexpr auto dwLocalPlayer = 0xDC14CC;
	constexpr auto m_flFlashDuration = 0x10470;
	DWORD gameModule = (DWORD)GetModuleHandleA("client.dll");
	DWORD localplayer = *(DWORD*)(gameModule + dwLocalPlayer);
	while (localplayer == NULL) {
		localplayer = *(DWORD*)(gameModule + dwLocalPlayer);
	}
	while (true) {
		if (noflash) {
			*(float*)(localplayer + m_flFlashDuration) = 0.0f;
			Sleep(1);
		}
		Sleep(1);
	}
	Sleep(10);
}