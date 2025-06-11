#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Core/ICGameMode.h"

class ICTacticalHUD {
public:
    ICTacticalHUD();
    ~ICTacticalHUD();

    // UI State
    bool bIsCommandMenuOpen;
    bool bIsActionMenuOpen;
    bool bIsTargetingActive;
    bool bIsMovementPreviewActive;
    bool bIsAbilityPreviewActive;

    // UI Elements
    void ShowCommandMenu();
    void HideCommandMenu();
    void ShowActionMenu();
    void HideActionMenu();

    // UI Resources
    std::string CommandMenuTexture;
    std::string ActionMenuTexture;
    std::string TargetingReticleTexture;
    std::string MovementPreviewTexture;
    std::string AbilityPreviewTexture;
    std::string CommandButtonTexture;
    std::string ActionButtonTexture;
    std::string TargetButtonTexture;
    std::string MovementButtonTexture;
    std::string AbilityButtonTexture;
    std::string CommandIconTexture;
    std::string ActionIconTexture;
    std::string TargetIconTexture;
    std::string MovementIconTexture;
    std::string AbilityIconTexture;

private:
    void InitializeUI();
    void UpdateUI();
    void RenderUI();
}; 