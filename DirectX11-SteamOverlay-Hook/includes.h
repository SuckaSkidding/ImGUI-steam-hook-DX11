#pragma once
#include <Windows.h>
#include <vector>
#include <d3d11.h>
#include "SteamOverlay.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
typedef HRESULT(*Present)(IDXGISwapChain*, UINT, UINT);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);