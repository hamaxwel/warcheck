#pragma once

#include <memory>
#include "Scene.h"
#include "../Math/Vector.h"
#include "../Math/Quaternion.h"
#include "../Math/Matrix.h"

namespace InvasionEngine {

class Entity;
class Mesh;
class Material;

class Component {
public:
    Component();
    virtual ~Component();

    // Component lifecycle
    virtual void OnCreate() {}
    virtual void OnDestroy() {}
    virtual void OnEnable() {}
    virtual void OnDisable() {}
    virtual void Update(float deltaTime) {}
    virtual void FixedUpdate(float fixedDeltaTime) {}
    virtual void LateUpdate(float deltaTime) {}

    // Entity access
    Entity* GetOwner() const { return m_Owner; }
    void SetOwner(Entity* owner) { m_Owner = owner; }

    // Component state
    bool IsEnabled() const { return m_Enabled; }
    void SetEnabled(bool enabled) { m_Enabled = enabled; }

    const Vector3& GetPosition() const;
    void SetPosition(const Vector3& position);

    const Quaternion& GetRotation() const;
    void SetRotation(const Quaternion& rotation);

    const std::string& GetName() const;
    void SetName(const std::string& name);

    Scene* GetScene() const;

protected:
    Entity* m_Owner;
    bool m_Enabled = true;
};

// Common component types
class TransformComponent : public Component {
public:
    TransformComponent();
    virtual ~TransformComponent();

    void SetPosition(const Vector3& position) { m_Position = position; }
    void SetRotation(const Quaternion& rotation) { m_Rotation = rotation; }
    void SetScale(const Vector3& scale) { m_Scale = scale; }

    const Vector3& GetPosition() const { return m_Position; }
    const Quaternion& GetRotation() const { return m_Rotation; }
    const Vector3& GetScale() const { return m_Scale; }

private:
    Vector3 m_Position;
    Quaternion m_Rotation;
    Vector3 m_Scale = Vector3(1.0f, 1.0f, 1.0f);
};

class MeshComponent : public Component {
public:
    MeshComponent();
    virtual ~MeshComponent();

    void SetMesh(std::shared_ptr<Mesh> mesh) { m_Mesh = mesh; }
    void SetMaterial(std::shared_ptr<Material> material) { m_Material = material; }

    std::shared_ptr<Mesh> GetMesh() const { return m_Mesh; }
    std::shared_ptr<Material> GetMaterial() const { return m_Material; }

private:
    std::shared_ptr<Mesh> m_Mesh;
    std::shared_ptr<Material> m_Material;
};

class LightComponent : public Component {
public:
    LightComponent();
    virtual ~LightComponent();

    enum class Type {
        Directional,
        Point,
        Spot
    };

    void SetType(Type type) { m_Type = type; }
    void SetColor(const Vector3& color) { m_Color = color; }
    void SetIntensity(float intensity) { m_Intensity = intensity; }
    void SetRange(float range) { m_Range = range; }
    void SetSpotAngle(float angle) { m_SpotAngle = angle; }

    Type GetType() const { return m_Type; }
    const Vector3& GetColor() const { return m_Color; }
    float GetIntensity() const { return m_Intensity; }
    float GetRange() const { return m_Range; }
    float GetSpotAngle() const { return m_SpotAngle; }

private:
    Type m_Type = Type::Point;
    Vector3 m_Color = Vector3(1.0f, 1.0f, 1.0f);
    float m_Intensity = 1.0f;
    float m_Range = 10.0f;
    float m_SpotAngle = 45.0f;
};

class RigidBodyComponent : public Component {
public:
    RigidBodyComponent();
    virtual ~RigidBodyComponent();

    void SetMass(float mass) { m_Mass = mass; }
    void SetVelocity(const Vector3& velocity) { m_Velocity = velocity; }
    void SetAngularVelocity(const Vector3& angularVelocity) { m_AngularVelocity = angularVelocity; }
    void SetUseGravity(bool useGravity) { m_UseGravity = useGravity; }
    void SetIsKinematic(bool isKinematic) { m_IsKinematic = isKinematic; }

    float GetMass() const { return m_Mass; }
    const Vector3& GetVelocity() const { return m_Velocity; }
    const Vector3& GetAngularVelocity() const { return m_AngularVelocity; }
    bool GetUseGravity() const { return m_UseGravity; }
    bool GetIsKinematic() const { return m_IsKinematic; }

    void AddForce(const Vector3& force);
    void AddTorque(const Vector3& torque);
    void AddForceAtPosition(const Vector3& force, const Vector3& position);

private:
    float m_Mass = 1.0f;
    Vector3 m_Velocity;
    Vector3 m_AngularVelocity;
    bool m_UseGravity = true;
    bool m_IsKinematic = false;
};

class ColliderComponent : public Component {
public:
    ColliderComponent();
    virtual ~ColliderComponent();

    enum class Type {
        Box,
        Sphere,
        Capsule,
        Mesh
    };

    void SetType(Type type) { m_Type = type; }
    void SetSize(const Vector3& size) { m_Size = size; }
    void SetRadius(float radius) { m_Radius = radius; }
    void SetHeight(float height) { m_Height = height; }
    void SetIsTrigger(bool isTrigger) { m_IsTrigger = isTrigger; }

    Type GetType() const { return m_Type; }
    const Vector3& GetSize() const { return m_Size; }
    float GetRadius() const { return m_Radius; }
    float GetHeight() const { return m_Height; }
    bool GetIsTrigger() const { return m_IsTrigger; }

private:
    Type m_Type = Type::Box;
    Vector3 m_Size = Vector3(1.0f, 1.0f, 1.0f);
    float m_Radius = 0.5f;
    float m_Height = 1.0f;
    bool m_IsTrigger = false;
};

} // namespace InvasionEngine 