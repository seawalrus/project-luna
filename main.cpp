
#include <iostream>
#include <thread>
#include "Windows.h"
#include "offsets.h"
#include "memory.h"

int main() {
	auto mem = Memory{ "csgo.exe" };
	const auto client = mem.GetModuleAddress("client.dll");
	if (!client) {
		std::cout << "could not obtain processID for csgo.. is the game running?";
		return 1;
	}
	std::cout << "Entering glow hackloop! \n";
	std::cout << "noFlash is enabled! \n";
	mem.Read<float>(client + offsets::dwLocalPlayer);
	while (true) {
		const auto LocalPlayer = mem.Read<std::uintptr_t>(client + offsets::dwLocalPlayer);
		const auto GlowObjectManager = mem.Read<std::uintptr_t>(client + offsets::dwGlowObjectManager);
		int flags = mem.Read<int>(LocalPlayer + netvars::m_fFlags);

		for (auto i = 0; i <= 195; ++i) {
			const auto entity = mem.Read<std::uintptr_t>(client + offsets::dwEntityList + i + 0x10);
			if (mem.Read<std::uintptr_t>(entity + netvars::m_iTeamNum) == mem.Read<std::uintptr_t>(offsets::dwLocalPlayer + netvars::m_iTeamNum))
				continue;
			custom::player = mem.Read<uintptr_t>(client + offsets::dwLocalPlayer);
			custom::flash_value = custom::player + netvars::m_flFlashMaxAlpha;
			mem.Write(custom::flash_value, custom::noflash); //write memory to remove flashbang alpha
			//Bhop shite
			if (flags & 1) {
				custom::buffer = 5;
			}
			else {
				custom::buffer = 4;
			}

			if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
				;
				mem.Write<uintptr_t>(client + offsets::dwForceJump, custom::buffer);
			}

			//Bhop Shite
			int entityTeamId = mem.Read<int>(entity + netvars::m_iTeamNum);
			if (entityTeamId == 2) {
				const auto GlowIndex = mem.Read<std::int32_t>(entity + netvars::m_iGlowIndex);
				mem.Write<float>(GlowObjectManager + (GlowIndex * 0x38) + 0x8, 1.f); //red
				mem.Write<float>(GlowObjectManager + (GlowIndex * 0x38) + 0xC, 0.f); //green
				mem.Write<float>(GlowObjectManager + (GlowIndex * 0x38) + 0x10, 1.f); //blue
				mem.Write<float>(GlowObjectManager + (GlowIndex * 0x38) + 0x14, 0.4f); //alpha
				mem.Write<bool>(GlowObjectManager + (GlowIndex * 0x38) + 0x27, false); //dont draw glow of enemies outside of fov (saves cpu usage)
				mem.Write<bool>(GlowObjectManager + (GlowIndex * 0x38) + 0x28, true); //alpha
			}
			else if (entityTeamId == 3) {
				const auto GlowIndex = mem.Read<std::int32_t>(entity + netvars::m_iGlowIndex);
				mem.Write<float>(GlowObjectManager + (GlowIndex * 0x38) + 0x8, 1.f); //red
				mem.Write<float>(GlowObjectManager + (GlowIndex * 0x38) + 0xC, 0.5f); //green
				mem.Write<float>(GlowObjectManager + (GlowIndex * 0x38) + 0x10, 0.f); //blue
				mem.Write<float>(GlowObjectManager + (GlowIndex * 0x38) + 0x14, 0.5f); //alpha
				mem.Write<bool>(GlowObjectManager + (GlowIndex * 0x38) + 0x27, false); //dont draw glow of enemies outside of fov (saves cpu usage)
				mem.Write<bool>(GlowObjectManager + (GlowIndex * 0x38) + 0x28, true); //alpha
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

	}
	return 0;
}
