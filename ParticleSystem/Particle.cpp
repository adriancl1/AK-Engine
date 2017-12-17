#include "Particle.h"
#include "ParticleSystem.h"
#include "..\MathGeo\Algorithm\Random\LCG.h"


Particle::Particle(ParticleSystem * parent, const SystemState & initial, const SystemState & final, float3 direction, float MaxLifeTime)
{
	SetState(Initial, initial);
	SetState(Final, final);

	data.maxLifeTime = MaxLifeTime;
	data.direction = direction;

	pSystem = parent;
	data.position = parent->transformation->position;
}


Particle::~Particle()
{

}

bool Particle::PreUpdate(float dt)
{
	//billboard
	
	return true;
}

bool Particle::Update(float dt) 
{
	float3 Orientation = pSystem->cameraPos - data.position;
	Orientation.y = data.position.y;
	data.rotation = Quat::LookAt(float3(0.0f, 0.0f, 1.0f), Orientation, float3(0.0f, 1.0f, 0.0f), float3(0.0f, 1.0f, 0.0f));
	DrawParticle();

	data.lifeTime += dt;
	CalcInterpolation();	

	if (data.lifeTime >= data.maxLifeTime & killThis==false)
	{
		killThis = true;
	}
	return true;
}

bool Particle::PostUpdate(float dt) 
{
	

	return true;
}


void Particle::SetState(State & myState, const SystemState & sState)
{
	LCG randG;

	//Gravity
	myState.gravity = sState.gravity+ randG.Float(-sState.gravityVariation, sState.gravityVariation);
	
	//Color
	myState.color.x = randG.Float(sState.color.x, sState.color2.x);
	myState.color.y = randG.Float(sState.color.y, sState.color2.y);
	myState.color.z = randG.Float(sState.color.z, sState.color2.z);
	myState.color.w = randG.Float(sState.color.w, sState.color2.w);

	//Size
	myState.size = randG.Float(sState.size1, sState.size2);

	//Speed
	myState.speed = sState.speed;

}

void Particle::CalcInterpolation()
{
	//Position
	data.direction.y += data.gravity*data.lifeTime;
	data.position +=data.direction*data.lifeTime;
	data.speed= Initial.speed + data.lifeTime*(Final.speed - Initial.speed);
	data.direction *= data.speed;
	//Size;
	data.size = Initial.size + data.lifeTime*(Final.size - Initial.size);

	//Color
	data.color.x = Initial.color.x + data.lifeTime*(Final.color.x - Initial.color.x);
	data.color.y = Initial.color.y + data.lifeTime*(Final.color.y - Initial.color.y);
	data.color.z = Initial.color.z + data.lifeTime*(Final.color.z - Initial.color.z);
	data.color.w = Initial.color.w + data.lifeTime*(Final.color.w - Initial.color.w);

	//Gravity

	data.gravity = Initial.gravity + data.lifeTime*(Final.gravity - Initial.gravity);
}

void Particle::DrawParticle()
{
	ParticleMesh* mesh = pSystem->GetMesh();

	float3 newScale = data.scale * data.size*0.1;
	//glDisable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor4f(data.color.x,data.color.y,data.color.z,data.color.z);
	if (mesh->idColors != 0)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, pSystem->tData.textureID);
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->idVertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (mesh->normals != nullptr)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->idNormals);
		glNormalPointer(GL_FLOAT, 0, NULL);
	}
	if (mesh->colors != nullptr)
	{
		
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->idColors);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	}

	glPushMatrix();
	float4x4 ParticleMatrix = float4x4::FromTRS(data.position, data.rotation, newScale).Transposed();
	glMultMatrixf(ParticleMatrix.ptr());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->idIndices);
	glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, NULL);

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	 glEnd();
}

bool Particle::isAlive()
{
	return !killThis;
}

bool Particle::KillParticle()
{
	return killThis;
}



void ParticleMesh::SetMesh(ParticleMesh & newMesh)
{
	idVertices = newMesh.idVertices;
	numVertices = newMesh.numVertices;
	vertices = newMesh.vertices;
	idIndices = newMesh.idIndices;
	numIndices = newMesh.numIndices;
	indices = newMesh.indices;
	idNormals = newMesh.idNormals;
	normals = newMesh.normals;
	idColors = newMesh.idColors;
	colors = newMesh.colors;
	idTexCoords = newMesh.idTexCoords;
	texCoords = newMesh.texCoords;
	enclosingBox = newMesh.enclosingBox;
}
