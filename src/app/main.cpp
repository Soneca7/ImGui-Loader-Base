#pragma once
#include "../includes.hpp"
#include "variable.h"

class D3DApplication;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class D3DApplication {
private:
    HWND hwnd;
    LPDIRECT3D9 d3d;
    D3DPRESENT_PARAMETERS d3dpp;

public:
    D3DApplication() :
        hwnd(nullptr),
        d3d(nullptr) {
        AppState::d3dDevice = nullptr;
    }

    bool Initialize(HINSTANCE hInstance) {
        if (!CreateMainWindow(hInstance) || !InitializeDirectX() || !InitializeImGui()) {
            Cleanup();
            return false;
        }
        return true;
    }

    void Run() {
        MSG msg;
        ZeroMemory(&msg, sizeof(msg));

        while (msg.message != WM_QUIT && AppState::isLoaderActive) {
            if (ProcessMessages(msg)) continue;
            RenderFrame();
        }
    }

    void Cleanup() {
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        if (AppState::d3dDevice) {
            AppState::d3dDevice->Release();
            AppState::d3dDevice = nullptr;
        }
        if (d3d) {
            d3d->Release();
            d3d = nullptr;
        }

        DestroyWindow(hwnd);
        UnregisterClass(AppConfig::WINDOW_CLASS_NAME, GetModuleHandle(NULL));
    }

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;

        switch (msg) {
        case WM_SIZE:
            if (AppState::d3dDevice != nullptr && wParam != SIZE_MINIMIZED) {
                D3DApplication::Get().HandleResize(LOWORD(lParam), HIWORD(lParam));
            }
            return 0;
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) return 0;
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    static D3DApplication& Get() {
        static D3DApplication instance;
        return instance;
    }

private:
    bool CreateMainWindow(HINSTANCE hInstance) {
        WNDCLASSEX wc = {
            sizeof(WNDCLASSEX),
            CS_CLASSDC,
            WndProc,
            0,
            0,
            hInstance,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            AppConfig::WINDOW_CLASS_NAME,
            nullptr
        };

        if (!RegisterClassEx(&wc)) return false;

        hwnd = CreateWindow(
            wc.lpszClassName,
            "X",
            WS_POPUP,
            0, 0, 5, 5,
            nullptr,
            nullptr,
            wc.hInstance,
            nullptr
        );

        if (!hwnd) return false;

        ShowWindow(hwnd, SW_HIDE);
        UpdateWindow(hwnd);
        return true;
    }

    bool InitializeDirectX() {
        d3d = Direct3DCreate9(D3D_SDK_VERSION);
        if (!d3d) return false;

        ZeroMemory(&d3dpp, sizeof(d3dpp));
        d3dpp.Windowed = TRUE;
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
        d3dpp.EnableAutoDepthStencil = TRUE;
        d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

        if (FAILED(d3d->CreateDevice(
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            hwnd,
            D3DCREATE_HARDWARE_VERTEXPROCESSING,
            &d3dpp,
            &AppState::d3dDevice))) {
            return false;
        }

        return true;
    }

    bool InitializeImGui() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = nullptr;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        AppState::fonts.main = io.Fonts->AddFontFromFileTTF(
            AppConfig::FONT_PATH,
            AppConfig::FONT_SIZE,
            nullptr,
            io.Fonts->GetGlyphRangesJapanese()
        );

        AppState::fonts.widgets = io.Fonts->AddFontFromFileTTF(
            AppConfig::FONT_PATH,
            AppConfig::FONT_SIZE,
            nullptr,
            io.Fonts->GetGlyphRangesJapanese()
        );

        if (!AppState::fonts.main || !AppState::fonts.widgets) return false;

        SetupImGuiStyle();

        if (!ImGui_ImplWin32_Init(hwnd)) return false;
        if (!ImGui_ImplDX9_Init(AppState::d3dDevice)) return false;

        RECT screenRect;
        GetWindowRect(GetDesktopWindow(), &screenRect);
        AppConfig::SCREEN_RESOLUTION = ImVec2(float(screenRect.right), float(screenRect.bottom));
        AppConfig::WINDOW_POSITION = (AppConfig::SCREEN_RESOLUTION - AppConfig::WINDOW_SIZE) * 0.5f;

        return true;
    }

    void SetupImGuiStyle() {
        ImGui::PurpleComfy();
        ImGuiStyle& style = ImGui::GetStyle();
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
    }

    bool ProcessMessages(MSG& msg) {
        if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            return true;
        }
        return false;
    }

    void RenderFrame() {
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        RenderUI();

        ImGui::EndFrame();

        AppState::d3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

        if (AppState::d3dDevice->BeginScene() >= 0) {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            AppState::d3dDevice->EndScene();
        }

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        HRESULT result = AppState::d3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
        if (result == D3DERR_DEVICELOST && AppState::d3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
            ResetDevice();
        }
    }

    void RenderUI() {
        ImGui::SetNextWindowPos(AppConfig::WINDOW_POSITION, ImGuiCond_Once);
        ImGui::SetNextWindowSize(AppConfig::WINDOW_SIZE);
        ImGui::SetNextWindowBgAlpha(1.0f);

        if (!AppState::isAuthenticated) {
            ImGui::Begin(AppConfig::WINDOW_TITLE, &AppState::isLoaderActive, AppConfig::WINDOW_FLAGS);
            Custom::Fade([]() {
                ImGui::InputTextEx("##username", "Username", AppState::credentials.username, sizeof(AppState::credentials.username), ImVec2(ImGui::GetWindowWidth() - 20, 25), ImGuiInputTextFlags_None);
                ImGui::InputTextEx("##Password", "Password", AppState::credentials.password, sizeof(AppState::credentials.password), ImVec2(ImGui::GetWindowWidth() - 20, 25), ImGuiInputTextFlags_Password);

                if (ImGui::Button("Login", ImVec2(ImGui::GetWindowWidth() - 20, 25))) {
                    AppState::isAuthenticated = true;
                }
                });
            ImGui::End();
        }
        else {
            ImGui::SetNextWindowSize(AppConfig::WINDOW_SIZE);
            ImGui::Begin(AppConfig::WINDOW_TITLE, &AppState::isLoaderActive, AppConfig::WINDOW_FLAGS);
            Custom::Fade([]() {
                ImGui::BeginChild("Aimbot"), ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()); {

                }
                ImGui::EndChild();
                });
            ImGui::End();
        }
    }

    void HandleResize(WORD width, WORD height) {
        d3dpp.BackBufferWidth = width;
        d3dpp.BackBufferHeight = height;
        ResetDevice();
    }

    void ResetDevice() {
        ImGui_ImplDX9_InvalidateDeviceObjects();
        HRESULT hr = AppState::d3dDevice->Reset(&d3dpp);
        if (hr == D3DERR_INVALIDCALL) {
            IM_ASSERT(0);
        }
        ImGui_ImplDX9_CreateDeviceObjects();
    }
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    D3DApplication& app = D3DApplication::Get();

    if (!app.Initialize(hInstance)) {
        return 1;
    }

    app.Run();
    app.Cleanup();

    return 0;
}