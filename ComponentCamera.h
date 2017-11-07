#pragma once

#include "Component.h"
#include "MathGeo\Geometry\Frustum.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera(float3 pos = { 0, 0, 0 }, float3 front = { 0, 0, 1 }, float3 up = { 0, 0.5f, 0 }, float nearPlaneDistance = 0.5f, float farPlaneDistance = 1000.0f, float verticalFov = 15, float aspectRatio = (float)16 / 9, FrustumType type = PerspectiveFrustum);

	~ComponentCamera();

	void Update();

	void SetPos(float3 newPos);
	void OnEditor() override;
	void SetFOV();
	void SetVerticalFOV(float value);
	void UpdateCamera(float3 pos, float3 front, float3 up);

	void DrawDebug() const;

	bool Contains(const AABB &aabb) const;
	bool GetFrustumCulling() const;
	Frustum GetFrustum()const;

	const float* GetViewMatrix() const;

	void OnSave(Configuration& data) const override;
	void OnLoad(Configuration& data) override;

private:
	float FOV;
	float aspectRatio;
	Frustum frustum;
	bool frustumCulling;
};