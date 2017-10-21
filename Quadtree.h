#pragma once

#include <list>

class QuadtreeNode
{
public:
	QuadtreeNode(const AABB& box);
	virtual ~QuadtreeNode();

	bool IsLeaf() const;

	void Insert(GameObject* toInsert);
	void Remove(GameObject* toRemove);
	void CreateChilds();
	//void RedistributeChilds();

public:
	AABB box;
	std::list<GameObject*> objects;
	QuadtreeNode* parent;
	QuadtreeNode* childs[4];

};

class Quadtree
{
public: 
	Quadtree();
	virtual ~Quadtree();
	
	/*void Insert(GameObject* toInsert);
	void Remove(GameObject* toRemove);
	void Clear();
	void Intersect(std::vector<GameObject*>&, Primitive);*/

public:
	QuadtreeNode* root = nullptr;
	std::list<Quadtree*> intersects;
};

