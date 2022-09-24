#include "includes.h"
#include "main.h"
#include "triggerbot.h"

namespace offsets {
	constexpr auto dwLocalPlayer = 0xDC14CC;
	constexpr auto dwEntityList = 0x4DDD91C;
	constexpr auto dwForceAttack = 0x320DE3C;
	constexpr auto m_iHealth = 0x100;
	constexpr auto m_iTeamNum = 0xF4;
	constexpr auto m_iCrosshairId = 0x11838;
}

DWORD WINAPI Triggerbot(LPVOID lp) {
	DWORD gameModule = (DWORD)GetModuleHandleA("client.dll");
	while (true) {
		if (triggerbot && GetAsyncKeyState(0x04)) { //middle mouse button 
			const auto localPlayer = *(DWORD*)(gameModule + offsets::dwLocalPlayer);
			const auto localHealth = *(DWORD*)(localPlayer + offsets::m_iHealth);
			const auto CrosshairId = *(DWORD*)(localPlayer + offsets::m_iCrosshairId);
			const auto player = *(DWORD*)(gameModule + offsets::dwEntityList + (CrosshairId - 1) * 0x10);
			const auto enemyTeam = *(DWORD*)(player + offsets::m_iTeamNum);
			const auto localTeam = *(DWORD*)(localPlayer + offsets::m_iTeamNum);
			if (!CrosshairId || CrosshairId > 64 || localTeam == enemyTeam || !localHealth)
				continue;
			*(DWORD*)(gameModule + offsets::dwForceAttack) = 6;
			Sleep(20);
			*(DWORD*)(gameModule + offsets::dwForceAttack) = 4;
		}
		
		Sleep(1);
	}

	Sleep(100);
}