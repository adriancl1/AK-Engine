#pragma once

#include "Globals.h"
#include "Component.h"
#include "MathGeo/Geometry/AABB.h"

class ResourceMesh;

class ComponentMesh : public Component
{
public:
	ComponentMesh();

	~ComponentMesh();

	void Update(float dt);

	//Returns the center point of the mesh's enclosing box.
	float3 GetCenter() const;

	void DrawDebug() const;

	void OnEditor()override;
	bool IntersectsAABB(LineSegment& line) const;
	bool TriIntersection(LineSegment& line, float& distance, float3 &hitPoint);

	void OnSave(Configuration& data) const override;
	void OnLoad(Configuration& data) override;
	void AddResource(int UID) override;


	uint GetIDVertices() const;
	uint GetNumVertices() const;
	const float* GetVertices() const;

	uint GetIDIndices() const;
	uint GetNumIndices() const;
	const uint* GetIndices() const;

	uint GetIDNormals() const;
	const float* GetNormals() const;

	uint GetIDColors() const;
	const float* GetColors() const;

	uint GetIDTextCoords() const;
	const float* GetTexCoords() const;

	AABB GetEnclosingBox() const;

private:
	ResourceMesh* mesh = nullptr;
};