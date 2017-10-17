#pragma once

#include "Component.h"
#include "MathGeo\Geometry\Frustum.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera(float3 pos = { 0, 0, 0 }, float3 front = { 0, 0, 1 }, float3 up = { 0, 1, 0 }, float nearPlaneDistance = 20, float farPlaneDistance = 100, float verticalFov = 100, float aspectRatio = 16 / 9, FrustumType type = PerspectiveFrustum);

	~ComponentCamera();

	void Update();

	void SetPos(float3 newPos);
	void SetVerticalFOV(float value);

	void DrawDebug();

	bool Contains(const AABB &aabb) const;

private:
	float aspectRatio;
	Frustum frustum;
};