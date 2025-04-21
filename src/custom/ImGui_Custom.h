#pragma once
#ifndef _IMGUICUSTOM_H_
#define _IMGUICUSTOM_H_

#include "../includes.hpp"

namespace Custom
{
    // Fade Animations
    bool Fade(std::function<void()> components, float duration = 4.0f);
    bool FadeInOut(std::function<void()> components, bool isChecked, float duration = 4.0f);
    bool FadeDown(std::function<void()> components, float endY, float duration = 4.0f);
    bool FadeUp(std::function<void()> components, float endY, float duration = 4.0f);
    bool FadeLeft(std::function<void()> components, float endX, float duration = 4.0f);
    bool FadeRight(std::function<void()> components, float endX, float duration = 4.0f);

    // Centering Functions
    bool CenterHorizontal(std::function<void()> widget, float width);
    bool CenterVertical(std::function<void()> widget, float height);
    bool CenterBoth(std::function<void()> widget, ImVec2 size);
}

#endif // _IMGUICUSTOM_H_
