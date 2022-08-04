#include <Windows.h>
#include <cstdint>
#include <thread>
#include "noFlash.h"
namespace noFlash_offsets
{
	// client
	DWORD dwLocalPlayer = 0xDBF4BC;
	DWORD m_flFlashMaxAlpha = 0x1046C;

	// player
	DWORD disableDlash = 0.0f;
	DWORD m_fFlags = 0x104;
}

void noFlash(const HMODULE instance) noexcept {
	const auto client = reinterpret_cast<uintptr_t>(GetModuleHandle("client.dll"));
	while (!GetAsyncKeyState(VK_F1)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		const auto localPlayer = *reinterpret_cast<uintptr_t*>(client + noFlash_offsets::dwLocalPlayer);
		const auto flashValue = *reinterpret_cast<uintptr_t*>(client + noFlash_offsets::m_flFlashMaxAlpha);
		*reinterpret_cast<std::float_t*>(client + flashValue) = 0.0f;
	}
	FreeLibraryAndExitThread(instance, 0);
}