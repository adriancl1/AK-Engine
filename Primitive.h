#pragma once

#include "glmath.h"
#include "Color.h"
#include <vector>

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder,
	Primitive_PlaneNoGrid,
	Primitive_Capsule
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const vec3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	mat4x4 transform;
	bool axis,wire;

protected:
	PrimitiveTypes type;
};

// ============================================
class Cube : public Primitive
{
public :
	Cube();
	Cube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	vec3 size;
};

// ============================================
class Cube1 : public Primitive
{
public:
	Cube1(int ID);
	Cube1(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	vec3 size;
private:
	int myID;
};

// ============================================
class Cube2 : public Primitive
{
public:
	Cube2(int ID);
	Cube2(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	vec3 size;
private:
	uint myID;
	uint myVertices;
};

// ============================================
class Sphere : public Primitive
{
public:
	Sphere();
	Sphere(float radius, unsigned int rings, unsigned int sectors);
	void InnerRender() const;
public:
	float radius;
protected:
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texcoords;
	std::vector<ushort> indices;
};

// ============================================
class Cylinder : public Primitive
{
public:
	Cylinder();
	Cylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class Line : public Primitive
{
public:
	Line();
	Line(float x, float y, float z);
	Line(float oX, float oY, float oZ, float dX, float dY, float dZ);
	void InnerRender() const;
public:
	vec3 origin;
	vec3 destination;
};

// ============================================
class Plane : public Primitive
{
public:
	Plane();
	Plane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	vec3 normal;
	float constant;
};

class PlaneNoGrid : public Primitive
{
public:
	PlaneNoGrid();
	PlaneNoGrid(float x, float y, float z, float d);
	void InnerRender() const;
public:
	float constant;
public:
	vec3 size;
private:
	uint myID;
	uint myVertices;
};

// ============================================
class Capsule : public Primitive
{
public:
	Capsule();
	Capsule(float radius, float height, unsigned int rings, unsigned int sectors);
	void InnerRender() const;
public:
	float radius;
	float height;
protected:
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texcoords;
	std::vector<ushort> indices;
};