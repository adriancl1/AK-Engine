
#include "Globals.h"
#include "Glew\include\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Primitive.h"
#include "glut/glut.h"


#pragma comment (lib, "glut/glut32.lib")

// ------------------------------------------------------------
Primitive::Primitive() : transform(IdentityMatrix), color(White), wire(false), axis(false), type(PrimitiveTypes::Primitive_Point)
{}

// ------------------------------------------------------------
PrimitiveTypes Primitive::GetType() const
{
	return type;
}

// ------------------------------------------------------------
void Primitive::Render() const
{
	glPushMatrix();
	glMultMatrixf(transform.M);

	if(axis == true)
	{
		// Draw Axis Grid
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
		glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
		glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
		glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

		glEnd();

		glLineWidth(1.0f);
	}

	glColor3f(color.r, color.g, color.b);

	if(wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	InnerRender();

	glPopMatrix();
}

// ------------------------------------------------------------
void Primitive::InnerRender() const
{
	glPointSize(5.0f);

	glBegin(GL_POINTS);

	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();

	glPointSize(1.0f);
}

// ------------------------------------------------------------
void Primitive::SetPos(float x, float y, float z)
{
	transform.translate(x, y, z);
}

// ------------------------------------------------------------
void Primitive::SetRotation(float angle, const vec3 &u)
{
	transform.rotate(angle, u);
}

// ------------------------------------------------------------
void Primitive::Scale(float x, float y, float z)
{
	transform.scale(x, y, z);
}

// CUBE ============================================
Cube::Cube() : Primitive(), size(1.0f, 1.0f, 1.0f)
{
	type = PrimitiveTypes::Primitive_Cube;
}

Cube::Cube(float sizeX, float sizeY, float sizeZ) : Primitive(), size(sizeX, sizeY, sizeZ)
{
	type = PrimitiveTypes::Primitive_Cube;
}

void Cube::InnerRender() const
{	
	float sx = size.x * 0.5f;
	float sy = size.y * 0.5f;
	float sz = size.z * 0.5f;

	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-sx, -sy, sz);
	glVertex3f( sx, -sy, sz);
	glVertex3f( sx,  sy, sz);
	glVertex3f(-sx,  sy, sz);

	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f( sx, -sy, -sz);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f(-sx,  sy, -sz);
	glVertex3f( sx,  sy, -sz);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(sx, -sy,  sz);
	glVertex3f(sx, -sy, -sz);
	glVertex3f(sx,  sy, -sz);
	glVertex3f(sx,  sy,  sz);

	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f(-sx, -sy,  sz);
	glVertex3f(-sx,  sy,  sz);
	glVertex3f(-sx,  sy, -sz);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-sx, sy,  sz);
	glVertex3f( sx, sy,  sz);
	glVertex3f( sx, sy, -sz);
	glVertex3f(-sx, sy, -sz);

	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f( sx, -sy, -sz);
	glVertex3f( sx, -sy,  sz);
	glVertex3f(-sx, -sy,  sz);

	glEnd();
}

// CUBE1 - DRAWN WITH VERTEX ARRAYS ============================================
Cube1::Cube1(int ID) : Primitive(), size(1.0f, 1.0f, 1.0f)
{
	type = PrimitiveTypes::Primitive_Cube;
	myID = ID;
}

Cube1::Cube1(float sizeX, float sizeY, float sizeZ) : Primitive(), size(sizeX, sizeY, sizeZ)
{
	type = PrimitiveTypes::Primitive_Cube;

	float offsetX = sizeX*0.5f;
	float offsetY = sizeY*0.5f;
	float offsetZ = sizeZ*0.5f;

	float vertexPoints[108] = { 
		-offsetX, -offsetY, sizeZ - offsetZ,
		-offsetX, sizeY - offsetY, -offsetZ,
		-offsetX, -offsetY, -offsetZ,
		-offsetX, -offsetY, sizeZ - offsetZ,
		-offsetX, sizeY - offsetY, sizeZ - offsetZ,
		-offsetX, sizeY - offsetY, -offsetZ,
		sizeX - offsetX, sizeY - offsetY, -offsetZ,
		sizeX - offsetX, -offsetY, -offsetZ,
		-offsetX, -offsetY, -offsetZ,
		-offsetX, -offsetY, -offsetZ,
		-offsetX, sizeY - offsetY, -offsetZ,
		sizeX - offsetX, sizeY - offsetY, -offsetZ,
		sizeX - offsetX, -offsetY, sizeZ - offsetZ,
		sizeX - offsetX, -offsetY, -offsetZ,
		sizeX - offsetX, sizeY - offsetY, -offsetZ,
		sizeX - offsetX, sizeY - offsetY, -offsetZ,
		sizeX - offsetX, sizeY - offsetY, sizeZ - offsetZ,
		sizeX - offsetX, -offsetY, sizeZ - offsetZ,
		-offsetX, sizeY - offsetY, sizeZ - offsetZ,
		-offsetX, -offsetY, sizeZ - offsetZ,
		sizeX - offsetX, -offsetY, sizeZ - offsetZ,
		sizeX - offsetX, -offsetY, sizeZ - offsetZ,
		sizeX - offsetX, sizeY - offsetY, sizeZ - offsetZ,
		-offsetX, sizeY - offsetY, sizeZ - offsetZ,
		sizeX - offsetX, sizeY - offsetY, sizeZ - offsetZ,
		sizeX - offsetX, sizeY - offsetY, -offsetZ,
		-offsetX, sizeY - offsetY, -offsetZ,
		-offsetX, sizeY - offsetY, -offsetZ,
		-offsetX, sizeY - offsetY, sizeZ - offsetZ,
		sizeX - offsetX, sizeY - offsetY, sizeZ - offsetZ,
		sizeX - offsetX, -offsetY, sizeZ - offsetZ,
		-offsetX, -offsetY, -offsetY - offsetZ,
		sizeX - offsetX, -offsetY, -offsetZ,
		-offsetX, -offsetY, -offsetZ,
		sizeX - offsetX, -offsetY, sizeZ - offsetZ,
		-offsetX, -offsetY, sizeZ - offsetZ };

	glGenBuffers(1, (GLuint*) &(myID));
	glBindBuffer(GL_ARRAY_BUFFER, myID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108, &vertexPoints, GL_STATIC_DRAW);
}

void Cube1::InnerRender() const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, myID);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, 36 * 3);
	glDisableClientState(GL_VERTEX_ARRAY);
}

// CUBE1 - DRAWN WITH VERTEX ARRAYS ============================================
Cube2::Cube2(int ID) : Primitive(), size(1.0f, 1.0f, 1.0f)
{
	type = PrimitiveTypes::Primitive_Cube;
	myID = ID;
}

Cube2::Cube2(float sizeX, float sizeY, float sizeZ) : Primitive(), size(sizeX, sizeY, sizeZ)
{
	type = PrimitiveTypes::Primitive_Cube;
	 
	float offsetX = sizeX * 0.5;
	float offsetY = sizeY * 0.5;
	float offsetZ = sizeZ * 0.5;

	float vertices[24] = {
		-offsetX, -offsetY, sizeZ - offsetZ, // 0
		sizeX - offsetX, -offsetY, sizeZ - offsetZ, // 1
		sizeX - offsetX, sizeY - offsetY, sizeZ - offsetZ, // 2
		-offsetX, sizeY - offsetY, sizeZ - offsetZ,  // 3
		-offsetX, -offsetY, -offsetZ,  // 4
		sizeX - offsetX, -offsetY, -offsetZ, // 5
		sizeX - offsetX, sizeY - offsetY, -offsetZ, // 6
		-offsetX, sizeY - offsetY, -offsetZ // 7
	}; 

	glGenBuffers(1, &myVertices);
	glBindBuffer(GL_ARRAY_BUFFER, myVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	uint index[36] =
	{
		0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1, 7, 6, 5, 5, 4, 7, 4, 0, 3, 3, 7, 4, 4, 5, 1, 1, 0, 4, 3, 2, 6, 6, 7, 3 
	};



	glGenBuffers(1, &myID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
}

void Cube2::InnerRender() const
{
	/*glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, myVertices);
	glVertexPointer(3, GL_FLOAT, 0, &myVertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myID);
	glDrawElements(GL_TRIANGLES, myID, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);*/
	

	/*glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	// draw a cube
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, index);

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);*/

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_ELEMENT_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, myVertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myID);
	glDrawElements(GL_TRIANGLES, sizeof(GLuint) * 36, GL_UNSIGNED_SHORT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_ELEMENT_ARRAY_BUFFER);
	glDisableClientState(GL_VERTEX_ARRAY);

}

// SPHERE ============================================
Sphere::Sphere() : Primitive(), radius(1.0f)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

Sphere::Sphere(float radius) : Primitive(), radius(radius)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

void Sphere::InnerRender() const
{
	glutSolidSphere(radius, 25, 25);
}


// CYLINDER ============================================
Cylinder::Cylinder() : Primitive(), radius(1.0f), height(1.0f)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

Cylinder::Cylinder(float radius, float height) : Primitive(), radius(radius), height(height)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

void Cylinder::InnerRender() const
{
	int n = 30;

	// Cylinder Bottom
	glBegin(GL_POLYGON);
	
	for(int i = 360; i >= 0; i -= (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder Top
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	for(int i = 0; i <= 360; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder "Cover"
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i < 480; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians

		glVertex3f(height*0.5f,  radius * cos(a), radius * sin(a) );
		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a) );
	}
	glEnd();
}

// LINE ==================================================
Line::Line() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
{
	type = PrimitiveTypes::Primitive_Line;
}

Line::Line(float x, float y, float z) : Primitive(), origin(0, 0, 0), destination(x, y, z)
{
	type = PrimitiveTypes::Primitive_Line;
}

void Line::InnerRender() const
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(destination.x, destination.y, destination.z);

	glEnd();

	glLineWidth(1.0f);
}

// PLANE ==================================================
Plane::Plane() : Primitive(), normal(0, 1, 0), constant(1)
{
	type = PrimitiveTypes::Primitive_Plane;
}

Plane::Plane(float x, float y, float z, float d) : Primitive(), normal(x, y, z), constant(d)
{
	type = PrimitiveTypes::Primitive_Plane;
}

void Plane::InnerRender() const
{
	glLineWidth(1.0f);

	glBegin(GL_LINES);

	float d = 200.0f;

	for(float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();
}