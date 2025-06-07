#include "CameraControllerComponent.h"
#include "Entity.h"
#include "InputManager.h"
#include "Math/Vector.h"
#include "Math/Quaternion.h"

namespace InvasionEngine {

CameraControllerComponent::CameraControllerComponent() : m_MoveSpeed(5.0f), m_RotateSpeed(2.0f) {}

void CameraControllerComponent::Update(float deltaTime) {
    Entity* entity = GetEntity();
    if (!entity) return;

    InputManager& input = InputManager::GetInstance();

    // Handle movement
    Vector3 moveDir(0.0f);
    if (input.IsKeyPressed(Key::W)) moveDir.z += 1.0f;
    if (input.IsKeyPressed(Key::S)) moveDir.z -= 1.0f;
    if (input.IsKeyPressed(Key::A)) moveDir.x -= 1.0f;
    if (input.IsKeyPressed(Key::D)) moveDir.x += 1.0f;

    if (moveDir.LengthSquared() > 0.0f) {
        moveDir.Normalize();
        Vector3 position = entity->GetPosition();
        position += moveDir * m_MoveSpeed * deltaTime;
        entity->SetPosition(position);
    }

    // Handle rotation
    if (input.IsMouseButtonPressed(MouseButton::Right)) {
        float mouseX = input.GetMouseX();
        float mouseY = input.GetMouseY();
        float deltaX = mouseX - m_LastMouseX;
        float deltaY = mouseY - m_LastMouseY;

        Quaternion rotation = entity->GetRotation();
        rotation *= Quaternion::FromEulerAngles(0.0f, deltaX * m_RotateSpeed * deltaTime, 0.0f);
        rotation *= Quaternion::FromEulerAngles(deltaY * m_RotateSpeed * deltaTime, 0.0f, 0.0f);
        entity->SetRotation(rotation);
    }

    m_LastMouseX = input.GetMouseX();
    m_LastMouseY = input.GetMouseY();
}

} // namespace InvasionEngine 