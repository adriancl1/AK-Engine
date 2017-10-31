#pragma once

#include "Component.h"
#include "MathGeo\Math\float3.h"
#include "MathGeo\Math\Quat.h"
#include "MathGeo\Math\float4x4.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(float3 pos, float3 scale, Quat rot, ComponentType type = Component_Transform);
	~ComponentTransform();

	void Update();
	void UpdateTrans();

	void UpdateTransFromParent(GameObject* parent);
	void SetLocalTrans(GameObject* parent);

	void OnEditor() override;
	void OnSave(Configuration& data) const override;

	float4x4 GetTransMatrix() const;

private:
	float3 position;
	float3 newPosition;
	float3 scale;
	float3 rotationEuler;
	Quat rotation;

	float4x4 globalTransformMatrix;
	float4x4 localTransformMatrix;

	bool needToUpdate;
};
