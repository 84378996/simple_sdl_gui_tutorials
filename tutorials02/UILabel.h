#pragma once
#include "UIControl.h"

class UILabel : public UIControl {
public:
    UILabel() = default;
    UILabel(const std::string& text);
    ~UILabel() = default;
    void Render() override;
};