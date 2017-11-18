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
	void CollectIntersections(std::vector<GameObject*>& objects, const Frustum & primitive) const;
	void CollectIntersections(std::vector<GameObject*>& objects, const LineSegment & primitive) const;
	void DrawDebug(Color color) const;

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
	Quadtree(const AABB& box);
	virtual ~Quadtree();
	
	void Insert(GameObject* toInsert);
	void Remove(GameObject* toRemove);
	void Clear();
	void CollectIntersections(std::vector<GameObject*>& objects, const Frustum & frustum) const;
	void CollectIntersections(std::vector<GameObject*>& objects, const LineSegment & line) const;

	void DrawDebug(Color color) const;

public:
	QuadtreeNode* root = nullptr;
};

