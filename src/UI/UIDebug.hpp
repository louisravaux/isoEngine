#pragma once

#include "ui/UIManager.hpp"

class UIDebug : public UIManager {
public:
    UIDebug();
    ~UIDebug();

    void content() override;
};
