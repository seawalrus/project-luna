#include <Windows.h>
#include <cstdint>
#include <thread>
#include "bhop.h"
namespace bhop_offsets
{
	// client
	constexpr std::ptrdiff_t dwLocalPlayer = 0xDBF4BC;
	constexpr std::ptrdiff_t dwForceJump = 0x52858DC;

	// player
	constexpr std::ptrdiff_t m_iHealth = 0x100;
	constexpr std::ptrdiff_t m_fFlags = 0x104;
}

void BunnyHop(const HMODULE instance) noexcept
{
	const auto client = reinterpret_cast<std::uintptr_t>(GetModuleHandle("client.dll"));

	// hack loop
	while (!GetAsyncKeyState(VK_F1))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		// make sure space is pressed
		if (!GetAsyncKeyState(VK_RETURN))
			continue;

		// get local player
		const auto localPlayer = *reinterpret_cast<std::uintptr_t*>(client + bhop_offsets::dwLocalPlayer);

		// is valid
		const auto health = *reinterpret_cast<std::int32_t*>(localPlayer + bhop_offsets::m_iHealth);

		// is alive
		if (!health || !localPlayer)
			continue;

		const auto flags = *reinterpret_cast<std::int32_t*>(localPlayer + bhop_offsets::m_fFlags);

		// on ground
		(flags & (1 << 0)) ?
			*reinterpret_cast<std::uintptr_t*>(client + bhop_offsets::dwForceJump) = 6 : // force jump
			*reinterpret_cast<std::uintptr_t*>(client + bhop_offsets::dwForceJump) = 4; // reset
	}

	// uninject
	FreeLibraryAndExitThread(instance, 0);
}