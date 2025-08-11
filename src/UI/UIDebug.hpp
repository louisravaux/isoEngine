#pragma once

#include "UI/UIManager.hpp"

class IsoEngine;

class UIDebug : public UIManager {
private:
    IsoEngine *engine = nullptr;

public:
    UIDebug(IsoEngine *engineRef);
    ~UIDebug();

    void content() override;
};
