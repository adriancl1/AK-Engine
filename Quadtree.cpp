#include "Application.h"
#include "Quadtree.h"
#include "GameObject.h"
#include "MathGeo\MathGeoLib.h"
#include "MathGeo\Geometry\AABB.h"

#define MAX_OBJECTS 4


Quadtree::Quadtree()
{

}

Quadtree::~Quadtree()
{

}

QuadtreeNode::QuadtreeNode(const AABB& box): box(box)
{

}

QuadtreeNode::~QuadtreeNode()
{

}

bool QuadtreeNode::IsLeaf() const
{
	if (childs[0] == nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void QuadtreeNode::Insert(GameObject* toInsert)
{
	if (objects.size() < MAX_OBJECTS)
	{
		objects.push_back(toInsert);
	}
	else
	{
		CreateChilds();
	}
}

void QuadtreeNode::Remove(GameObject* toRemove)
{

}

void QuadtreeNode::CreateChilds()
{
	AABB childBox;

	float3 boxMin = this->box.minPoint;
	float3 boxMax = this->box.maxPoint;
	float3 boxSize = this->box.Size();

	//Child 0
	childBox.minPoint = float3(boxMin.x, boxMin.y, boxMin.z);
	childBox.maxPoint = float3(boxMax.x - boxSize.x * 0.5f, boxMax.y, boxMax.z - boxSize.z * 0.5f);
	
	childs[0] = new QuadtreeNode(childBox);

	//Child 1
	childBox.minPoint = float3(boxMin.x + boxSize.x * 0.5f, boxMin.y, boxMin.z);
	childBox.maxPoint = float3(boxMax.x, boxMax.y, boxMax.z - boxSize.z * 0.5f);

	childs[1] = new QuadtreeNode(childBox);

	//Child 2
	childBox.minPoint = float3(boxMin.x, boxMin.y, boxMin.z + boxSize.z * 0.5f);
	childBox.maxPoint = float3(boxMax.x - boxSize.x * 0.5f, boxMax.y, boxMax.z);

	childs[2] = new QuadtreeNode(childBox);

	//Child 3
	childBox.minPoint = float3(boxMax.x - boxSize.x * 0.5f, boxMax.y, boxMax.z - boxSize.z * 0.5f);
	childBox.maxPoint = float3(boxMax.x , boxMax.y, boxMax.z);

	childs[3] = new QuadtreeNode(childBox);

	for (int i = 0; i < 4; i++)
	{
		childs[i]->parent = this;
	}
}