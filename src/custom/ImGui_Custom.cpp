#include "ImGui_Custom.h"

// Fade Animations

bool Custom::Fade(std::function<void()> components, float duration)
{
    static float fadeAlpha;
    fadeAlpha = ImLerp(fadeAlpha, 1.0f, ImGui::GetIO().DeltaTime * duration);

    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, fadeAlpha);
    {
        ImGui::BeginGroup();
        components();
        ImGui::EndGroup();
    }
    ImGui::PopStyleVar();

    return std::abs(fadeAlpha - 1.f) < 0.1f;
}

bool Custom::FadeInOut(std::function<void()> components, bool isChecked, float duration)
{
    static float fadeAlpha;
    fadeAlpha = ImLerp(fadeAlpha, isChecked ? 1.0f : 0.f, ImGui::GetIO().DeltaTime * duration);

    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, fadeAlpha);
    {
        ImGui::BeginGroup();
        components();
        ImGui::EndGroup();
    }
    ImGui::PopStyleVar();

    return std::abs(fadeAlpha - 1.f) < 0.1f;
}

bool Custom::FadeDown(std::function<void()> components, float endY, float duration)
{
    static float fade;
    fade = ImLerp(fade, endY, ImGui::GetIO().DeltaTime * duration);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + fade);
    ImGui::BeginGroup();
    components();
    ImGui::EndGroup();

    return std::abs(fade - endY) < 1.0f;
}

bool Custom::FadeUp(std::function<void()> components, float endY, float duration)
{
    static float fade;
    fade = ImLerp(fade, endY, ImGui::GetIO().DeltaTime * duration);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - fade);
    ImGui::BeginGroup();
    components();
    ImGui::EndGroup();

    return std::abs(fade - endY) < 1.0f;
}

bool Custom::FadeLeft(std::function<void()> components, float endX, float duration)
{
    static float fade;
    fade = ImLerp(fade, endX, ImGui::GetIO().DeltaTime * duration);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() - fade);
    ImGui::BeginGroup();
    components();
    ImGui::EndGroup();

    return std::abs(fade - endX) < 1.0f;
}

bool Custom::FadeRight(std::function<void()> components, float endX, float duration)
{
    static float fade;
    fade = ImLerp(fade, endX, ImGui::GetIO().DeltaTime * duration);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + fade);
    ImGui::BeginGroup();
    components();
    ImGui::EndGroup();

    return std::abs(fade - endX) < 1.0f;
}

// Centering Functions

bool Custom::CenterHorizontal(std::function<void()> widget, float width)
{
    ImVec2 windowSize = ImGui::GetWindowSize();
    float centerX = (windowSize.x - width) * 0.5f;

    ImVec2 cursorPos = ImGui::GetCursorPos();
    ImGui::SetCursorPosX(centerX);

    widget();

    return std::abs(cursorPos.x - centerX) < 1.0f;
}

bool Custom::CenterVertical(std::function<void()> widget, float height)
{
    ImVec2 windowSize = ImGui::GetWindowSize();
    float centerY = (windowSize.y - height) * 0.5f;

    ImVec2 cursorPos = ImGui::GetCursorPos();
    ImGui::SetCursorPosY(centerY);

    widget();

    return std::abs(cursorPos.y - centerY) < 1.0f;
}

bool Custom::CenterBoth(std::function<void()> widget, ImVec2 size)
{
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 center = {
        (windowSize.x - size.x) * 0.5f,
        (windowSize.y - size.y) * 0.5f
    };

    ImVec2 cursorPos = ImGui::GetCursorPos();
    ImGui::SetCursorPos(center);

    widget();

    return std::abs(cursorPos.x - center.x) < 1.0f && std::abs(cursorPos.y - center.y) < 1.0f;
}
