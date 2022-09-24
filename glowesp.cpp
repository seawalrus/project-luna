#include "main.h"
#include "glowesp.h"
#include "includes.h"
DWORD WINAPI GlowEsp(LPVOID lp) {
	constexpr auto dwLocalPlayer = 0xDC14CC;
	constexpr auto dwGlowObjectManager = 0x53265D0;
	constexpr auto m_fFlags = 0x104;
	constexpr auto dwEntityList = 0x4DDD91C;
	constexpr auto m_iTeamNum = 0xF4;
	constexpr auto m_iGlowIndex = 0x10488;
	uintptr_t player;
	DWORD gameModule = (DWORD)GetModuleHandleA("client.dll");
	DWORD localplayer = *(DWORD*)(gameModule + dwLocalPlayer);
	while (localplayer == NULL) {
		localplayer = *(DWORD*)(gameModule + dwLocalPlayer);
	}
	while (true) {
		if (glowesp) {
			const auto entity = (uintptr_t)(gameModule + dwLocalPlayer);
			const auto glowobjectmanager = (uintptr_t)(gameModule + dwGlowObjectManager);
			int flags = (int)(localplayer + m_fFlags);
			for (auto i = 0; i < 195; ++i) {
				const auto entity = (uintptr_t)(gameModule + dwEntityList + i * 0x10);
				if (uintptr_t(entity + m_iTeamNum) == (uintptr_t)(dwLocalPlayer + m_iTeamNum))
					continue;
				player = (uintptr_t)(gameModule + dwLocalPlayer);
				int entityteamid = (int)(entity + m_iTeamNum);

				if (entityteamid == 2) {
					const auto glowindex = (int)(entity + m_iGlowIndex);
					(DWORD)(glowobjectmanager + (glowindex * 0x38) + 0x4, 1.f); //red
					(DWORD)(glowobjectmanager + (glowindex * 0x38) + 0xB, 0.f); //green
					(DWORD)(glowobjectmanager + (glowindex * 0x38) + 0xC, 1.f); //blue
					(DWORD)(glowobjectmanager + (glowindex * 0x38) + 0x10, 1.f); //alpha 
					(DWORD)(glowobjectmanager + (glowindex * 0x38) + 0x24, 1.f); //bool
					(int)(glowobjectmanager + (glowindex * 0x38) + 0x24 , 1); //bool
				}
				/*
				else if(entityteamid == 3) {
					const auto glowindex = (int32_t)(entity + m_iGlowIndex);
					(DWORD)(glowobjectmanager + (glowindex * 0x38) + 0x8, 1.f); //red
					(DWORD)(glowobjectmanager + (glowindex * 0x38) + 0xc, 0.f); //green
					(DWORD)(glowobjectmanager + (glowindex * 0x38) + 0x10, 1.f); //blue
					(DWORD)(glowobjectmanager + (glowindex * 0x38) + 0x14, 1.f); //alpha 
					(bool)(glowobjectmanager + (glowindex * 0x38) + 0x27, false); //bool
					(bool)(glowobjectmanager + (glowindex * 0x38) + 0x28, true); //bool
				}*/
			}
		}
		Sleep(10);
	}
}