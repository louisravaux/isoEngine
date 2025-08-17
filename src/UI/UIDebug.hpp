#pragma once

#include "ui/UIManager.hpp"

// Forward declaration
class IsoEngine;

class UIDebug : public UIManager {
private:
    IsoEngine* engine;
    
    // Window visibility states
    bool showPerformanceWindow;
    bool showTileInspector;
    bool showLevelManager;
    bool showTilePalette;
    bool showCameraControls;
    bool showSystemInfo;
    
    // Individual window drawing functions
    void drawMainMenuBar();
    void drawPerformanceWindow();
    void drawTileInspectorWindow();
    void drawLevelManagerWindow();
    void drawTilePaletteWindow();
    void drawCameraControlsWindow();
    void drawSystemInfoWindow();
    
    // Window management utilities
    void hideAllWindows();
    void showAllWindows();

public:
    UIDebug(IsoEngine* engineRef);
    ~UIDebug();

    void content() override;
};