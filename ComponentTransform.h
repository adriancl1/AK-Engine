#pragma once

#include "Component.h"
#include "MathGeo/Math/float3.h"
#include "MathGeo/Math/Quat.h"
#include "MathGeo/Math/float4x4.h"

class ComponentCamera;

class ComponentTransform : public Component
{
public:
	ComponentTransform(float3 pos = { 0,0,0 }, float3 scale = { 1,1,1 }, Quat rot = { 0, 0, 0, 1 }, ComponentType type = Component_Transform);
	~ComponentTransform();

	void Update();
	void UpdateTrans();

	float4x4 GetGlobalTransform() const;
	float4x4 GetLocalTransform() const;
	float3 GetPosition() const;
	float3 GetGlobalPosition()const;

	void SetPosition(float3 pos);
	void SetRotation(Quat rot);
	void SetScale(float3 scale);

	void OnEditor() override;
	void OnSave(Configuration& data) const override;
	void OnLoad(Configuration& data) override;

	void ShowGizmo(ComponentCamera& camera);

	float4x4 GetTransMatrix() const;

	bool needToUpdate;

private:
	float3 position;
	float3 newPosition;
	float3 scale;
	float3 rotationEuler;
	Quat rotation;

	float4x4 globalTransformMatrix;
	float4x4 localTransformMatrix;
};
