#pragma once

// Windows Libraries
#include <Windows.h>
#include <iostream>
#include <dwmapi.h>
#include <TlHelp32.h>
#include <unordered_map>
#include <functional>
#include <thread>

// ImGui
#include "Dependecies/ImGui/imgui.h"
#include "Dependecies/ImGui/imgui_internal.h"
#include "Dependecies/ImGui/imgui_impl_dx9.h"
#include "Dependecies/ImGui/imgui_impl_win32.h"

// ImGui Custom
#include "custom/ImGui_Custom.h"

// DirectX Include
#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")