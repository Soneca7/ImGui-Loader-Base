#pragma once
#include "../includes.hpp"

namespace AppConfig {
    inline const char* WINDOW_TITLE = "ImGui Application";
    inline const char* WINDOW_CLASS_NAME = "ImGuiApplication";
    inline const char* FONT_PATH = "c:\\Windows\\Fonts\\verdana.ttf";
    inline constexpr float FONT_SIZE = 18.0f;

    inline const ImGuiWindowFlags WINDOW_FLAGS =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar;

    inline ImVec2 WINDOW_SIZE{ 400, 300 };
    inline ImVec2 MENU_SIZE{ 450, 450 };
    inline ImVec2 WINDOW_POSITION{ 0, 0 };
    inline ImVec2 SCREEN_RESOLUTION{ 0, 0 };

    inline ImVec4 RGBA(int r, int g, int b, float a = 1.0f) {
        return ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, a);
    }

    inline struct Colors {
        ImVec4 baseColor = RGBA(70, 75, 175, 1.0f);
        ImVec4 baseColorHovered = RGBA(42, 42, 142, 1.0f);
        ImVec4 textColor = RGBA(255, 255, 255, 1.0f);
    } colors;
}

namespace AppState {
    inline bool isLoaderActive = true;
    inline bool isAuthenticated = false;

    inline struct Credentials {
        char username[255] = "";
        char password[255] = "";
    } credentials;

    inline LPDIRECT3DDEVICE9 d3dDevice = nullptr;

    inline struct Fonts {
        ImFont* main = nullptr;
        ImFont* widgets = nullptr;
    } fonts;
}