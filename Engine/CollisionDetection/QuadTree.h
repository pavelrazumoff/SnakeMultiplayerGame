#pragma once

#include "CollisionTypes.h"

#include "Collider.h"
#include <vector>

struct QuadTreeNode
{
public:
	QuadTreeNode(const AABB& _boundary)
		: boundary(_boundary)
	{
		for (int i = 0; i < 4; ++i)
			children[i] = nullptr;
	}

	~QuadTreeNode()
	{
		for (int i = 0; i < 4; ++i)
			delete children[i];
	}

public:
	AABB boundary;
	std::vector<ICollider*> colliders; // TODO: Check if pointers are valid.
	QuadTreeNode* children[4];
};

class QuadTree
{
public:
	QuadTree(const AABB& boundary, int _capacity);
	~QuadTree();

	bool Insert(ICollider* collider);
	bool Insert(QuadTreeNode* node, ICollider* collider);

	void Remove(ICollider* collider);
	void Remove(QuadTreeNode* node, ICollider* collider);

	void QueryRangeFor(const ICollider* colliderCheck, std::vector<ICollider*>& outColliders);
	void QueryRange(QuadTreeNode* node, const ICollider* colliderCheck, std::vector<ICollider*>& outColliders);

private:
	void Subdivide(QuadTreeNode* node);

private:
	QuadTreeNode* root = nullptr;
	int capacity = 0;
};
