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
	void RedistributeChilds();
	template<typename TYPE>
	void CollectIntersections(std::vector<GameObject*>& objects, const TYPE & primitive) const;

public:
	AABB box;
	std::list<GameObject*> objects;
	QuadtreeNode* parent;
	QuadtreeNode* childs[4];
};

class Quadtree
{
public: 
	Quadtree(const AABB& box);
	virtual ~Quadtree();
	
	void Insert(GameObject* toInsert);
	void Remove(GameObject* toRemove);
	void Clear();

public:
	QuadtreeNode* root = nullptr;
};

