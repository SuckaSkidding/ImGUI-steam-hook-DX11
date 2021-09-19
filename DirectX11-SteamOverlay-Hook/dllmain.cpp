#include "includes.h"


HWND hWnd = NULL;
Present oPresent = NULL;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* pRenderTargetView;
WNDPROC oWndProc;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool ImGuiInit = false;
bool menu = false;
HRESULT hkPresent(IDXGISwapChain* pSwapchain, UINT SyncInterval, UINT Flags) {
	if (!ImGuiInit) {
		if (SUCCEEDED(pSwapchain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice))) {
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapchain->GetDesc(&sd);
			hWnd = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
			ImGui_ImplWin32_Init(hWnd);
			ImGui_ImplDX11_Init(pDevice, pContext);
			ImGuiInit = true;
		}
		else return oPresent(pSwapchain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		menu = !menu;
		

    }
	if (menu)
	{

		ImGui::GetMouseCursor();
		ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
		ImGui::GetIO().WantCaptureMouse = menu;
		ImGui::GetIO().MouseDrawCursor = menu;

		ImGuiStyle& style = ImGui::GetStyle();

		
		style.Alpha = 0.95f;
		style.ChildBorderSize = 1.0f;
		style.WindowPadding = ImVec2(8, 8);
		style.WindowMinSize = ImVec2(32, 32);
		style.WindowRounding = 6.f;
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style.WindowBorderSize = 0.f;
		style.FrameRounding = 4.0f;
		style.ItemSpacing = ImVec2(4, 9);
		style.ItemInnerSpacing = ImVec2(8, 8);
		style.TouchExtraPadding = ImVec2(0, 0);
		style.IndentSpacing = 21.0f;
		style.ColumnsMinSpacing = 3.0f;
		style.ScrollbarSize = 14.0f;
		style.ScrollbarRounding = 0.0f;
		style.GrabMinSize = 5.0f;
		style.GrabRounding = 0.0f;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.DisplayWindowPadding = ImVec2(22, 22);
		style.DisplaySafeAreaPadding = ImVec2(4, 4);
		style.AntiAliasedLines = true;
		stylze

		style.CurveTessellationTol = 1.25f;
		style.Colors[ImGuiCol_TitleBg] = ImColor(25,25,25, 185);
		style.Colors[ImGuiCol_TitleBgActive] = ImColor(36, 38, 52, 255);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(36, 38, 52, 255);
		style.Colors[ImGuiCol_FrameBg] = ImColor(0, 143,248 , 200);
		style.Colors[ImGuiCol_FrameBgActive] = ImColor(36, 38, 52, 255);
		style.Colors[ImGuiCol_FrameBgHovered] = ImColor(36, 38, 52, 255);
		style.Colors[ImGuiCol_Button] = ImColor(78, 79, 85, 255);
		style.Colors[ImGuiCol_ButtonActive] = ImColor(78, 79, 85, 200);
		style.Colors[ImGuiCol_ButtonHovered] = ImColor(78, 79, 85, 255);
		style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
		style.Colors[ImGuiCol_WindowBg] = ImColor(29, 29, 29, 255);
		style.Colors[ImGuiCol_TabActive] = ImColor(28, 28, 39, 255);
		style.Colors[ImGuiCol_TabHovered] = ImColor(28, 28, 39, 255);
		style.Colors[ImGuiCol_TabUnfocused] = ImColor(28, 28, 39, 255);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImColor(28, 28, 39, 255);
		style.Colors[ImGuiCol_Tab] = ImColor(28, 28, 39, 255);
		style.Colors[ImGuiCol_MenuBarBg] = ImColor(17, 40, 55, 255);

		ImGui::Begin("", &menu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoScrollbar);
		ImGui::SetWindowSize(ImVec2(500, 450), ImGuiCond_Always);

		ImGui::End();

	}
	ImGui::Render();
	pContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapchain, SyncInterval, Flags);
}

void STDMETHODCALLTYPE MainThread() 
{
	uintptr_t pPresentAddr = SteamOverlay::FindPattern(SteamOverlay::GetSteamModule(), "48 89 6C 24 18 48 89 74 24 20 41 56 48 83 EC 20 41 8B E8");
	SteamOverlay::CreateHook(pPresentAddr, (uintptr_t)hkPresent, (long long*)&oPresent);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) 
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		MainThread();
	}
	return TRUE;
}