#pragma once

#include "Engine/Core/GameMode.h"
#include "ICPlayerController.h"
#include <memory>

namespace InvasionCycle {

class ICGameMode : public InvasionEngine::GameMode {
public:
    ICGameMode();
    virtual ~ICGameMode() = default;

    virtual bool Initialize() override;
    virtual void Update(float deltaTime) override;
    virtual void Shutdown() override;

    void SpawnPlayer();
    void SpawnEnemy();
    void SpawnPickup();

    void SetPlayerController(std::shared_ptr<ICPlayerController> controller);

private:
    std::shared_ptr<ICPlayerController> m_PlayerController;
};

} // namespace InvasionCycle 