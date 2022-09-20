#include "includes.h"
#include "bhop.h"
#include "noflash.h"
#ifdef _WIN64
#define GWL_WNDPROC GWLP_WNDPROC
#endif

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

EndScene oEndScene = NULL;
WNDPROC oWndProc;
static HWND window = NULL;

void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(pDevice);
}
bool init = false;
bool bhop = false;
bool noflash = false;
//custom
bool showWindow = true;
int screenx = GetSystemMetrics(SM_CXSCREEN);
int screeny = GetSystemMetrics(SM_CYSCREEN);


long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if (!init)
	{
		InitImGui(pDevice);
		init = true;
	}

	if (GetAsyncKeyState(VK_F1) & 1) {
		showWindow = !showWindow;
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	if (showWindow) {
		ImGui::Begin("Luna Project..");
		ImGui::Checkbox("Bhop", &bhop);
		ImGui::Checkbox("NoFlash", &noflash);
		ImGui::End();
		
	}
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return oEndScene(pDevice);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	DWORD wndProcId;
	GetWindowThreadProcessId(handle, &wndProcId);

	if (GetCurrentProcessId() != wndProcId)
		return TRUE; // skip to next window

	window = handle;
	return FALSE; // window found abort search
}

HWND GetProcessWindow()
{
	window = NULL;
	EnumWindows(EnumWindowsCallback, NULL);
	return window;
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool attached = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success)
		{
			kiero::bind(42, (void**)& oEndScene, hkEndScene);
			do
				window = GetProcessWindow();
			while (window == NULL);
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc);
			attached = true;
		}
	} while (!attached);
	return TRUE;
}
/* THIS FILE NOW LIVES IN bhop.cpp & bhop.h
DWORD WINAPI BunnyHop(LPVOID lp) {
	DWORD gameModule = (DWORD)GetModuleHandle("client.dll");
	DWORD localplayer = *(DWORD*)(gameModule + signatures::dwLocalPlayer);
	while (localplayer == NULL) {
		localplayer = *(DWORD*)(gameModule + signatures::dwLocalPlayer);
	}
	while (true) {
		if (bhop) {
			DWORD flag = *(BYTE*)(localplayer + netvars::m_fFlags);
			if (GetAsyncKeyState(VK_RETURN)) {
				if (GetAsyncKeyState(VK_RETURN) && flag & (1 << 0)) {
					*(DWORD*)(gameModule + signatures::dwForceJump) = 6;
				}
			}
		}
	}
	Sleep(1);
	return 0;
}
*/
BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		CreateThread(nullptr, 0, BunnyHop, hMod, 0, nullptr);
		CreateThread(nullptr, 0, NoFlash, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}
