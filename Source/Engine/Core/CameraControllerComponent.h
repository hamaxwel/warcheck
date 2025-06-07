#pragma once

#include "Component.h"
#include "Math/Vector.h"
#include "Math/Quaternion.h"

namespace InvasionEngine {

class CameraControllerComponent : public Component {
public:
    CameraControllerComponent();
    virtual ~CameraControllerComponent();

    virtual void Update(float deltaTime) override;

    void SetMoveSpeed(float speed) { m_MoveSpeed = speed; }
    void SetLookSpeed(float speed) { m_LookSpeed = speed; }
    float GetMoveSpeed() const { return m_MoveSpeed; }
    float GetLookSpeed() const { return m_LookSpeed; }

private:
    float m_MoveSpeed = 5.0f;
    float m_LookSpeed = 0.1f;
    float m_Pitch = 0.0f;
    float m_Yaw = 0.0f;
};

} // namespace InvasionEngine 