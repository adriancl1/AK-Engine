#include "Application.h"
#include "Quadtree.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "MathGeo\MathGeoLib.h"
#include "MathGeo\Geometry\AABB.h"

#define MAX_OBJECTS 4

QuadtreeNode::QuadtreeNode(const AABB& box): box(box)
{

}

QuadtreeNode::~QuadtreeNode()
{
	for (int i = 0; i < 4; i++)
	{
		delete childs[i];
		childs[i] = nullptr;
	}
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
		RedistributeChilds();
	}
}

//TODO: Change function to an iterative one.
void QuadtreeNode::Remove(GameObject* toRemove)
{
	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		if ((*it) == toRemove)
		{
			objects.erase(it);
			return;
		}
	}
	if (IsLeaf() == false)
	{
		for (int i = 0; i < 4; i++)
		{
			childs[i]->Remove(toRemove); 
		}
	}
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

void QuadtreeNode::RedistributeChilds()
{
	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		for (int i = 0; i < 4; i++)
		{
			ComponentMesh* tmp = (ComponentMesh*)(*it)->FindComponent(Component_Mesh);
			if (childs[i]->box.Contains(tmp->enclosingBox))
			{
				childs[i]->Insert((*it));
			}
		}
		this->Remove((*it));
	}
}

template<typename TYPE>
void QuadtreeNode::CollectIntersections(std::vector<GameObject*>& objects, const TYPE & primitive) const
{
	if (primitive.Intersects(box))
	{
		for (std::list<GameObject*>::const_iterator it = this->objects.begin(); it != this->objects.end(); ++it)
		{
			ComponentMesh* tmp = (ComponentMesh*)(*it)->FindComponent(Component_Mesh);
			if (primitive.Intersects(tmp->enclosingBox))
				objects.push_back(*it);
		}
		for (int i = 0; i < 4; ++i)
			if (childs[i] != nullptr) childs[i]->CollectIntersections(objects, primitive);
	}
}

Quadtree::Quadtree(const AABB& box)
{
	root = new QuadtreeNode(box);
}

Quadtree::~Quadtree()
{
	Clear();
}

void Quadtree::Insert(GameObject* toInsert)
{
	ComponentMesh* tmp = (ComponentMesh*)toInsert->FindComponent(Component_Mesh);
	if (root != nullptr && root->box.Contains(tmp->enclosingBox))
	{
		root->Insert(toInsert);
	}
}

void Quadtree::Remove(GameObject* toRemove)
{
	ComponentMesh* tmp = (ComponentMesh*)toRemove->FindComponent(Component_Mesh);
	if (root != nullptr && root->box.Contains(tmp->enclosingBox))
	{
		root->Remove(toRemove);
	}
}

void Quadtree::Clear()
{
	delete root;
	root = nullptr;
}