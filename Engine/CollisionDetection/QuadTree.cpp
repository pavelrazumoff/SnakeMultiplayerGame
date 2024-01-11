#include "QuadTree.h"

QuadTree::QuadTree(const AABB& boundary, int _capacity)
	: capacity(_capacity)
{
	root = new QuadTreeNode(boundary);
}

QuadTree::~QuadTree()
{
	delete root;
}

void QuadTree::Insert(ICollider* collider)
{
	Insert(root, collider);
}

void QuadTree::Insert(QuadTreeNode* node, ICollider* collider)
{
	if (!node) return;
	if (!collider ||
		!collider->Intersects(node->boundary)) return;

	if (node->colliders.size() < capacity)
	{
		node->colliders.push_back(collider);
	}
	else
	{
		if (node->children[0] == nullptr)
			Subdivide(node);

		for (int i = 0; i < 4; ++i)
			Insert(node->children[i], collider);
	}
}

void QuadTree::Remove(ICollider* collider)
{
	Remove(root, collider);
}

void QuadTree::Remove(QuadTreeNode* node, ICollider* collider)
{
	if (!node) return;

	for (auto it = node->colliders.begin(); it != node->colliders.end(); ++it)
	{
		if (*it == collider)
		{
			node->colliders.erase(it);
			return;
		}
	}

	for (int i = 0; i < 4; ++i)
		Remove(node->children[i], collider);
}

void QuadTree::QueryRangeFor(const ICollider* colliderCheck, std::vector<ICollider*>& outColliders)
{
	QueryRange(root, colliderCheck, outColliders);
}

void QuadTree::QueryRange(QuadTreeNode* node, const ICollider* colliderCheck, std::vector<ICollider*>& outColliders)
{
	if (!colliderCheck->Intersects(node->boundary)) return;

	for (auto collider : node->colliders)
	{
		if (colliderCheck->CanCollideWith(collider) &&
			collider->Intersects(colliderCheck->GetAABB()))
			outColliders.push_back(collider);
	}

	for (int i = 0; i < 4; ++i)
	{
		if (node->children[i] != nullptr)
			QueryRange(node->children[i], colliderCheck, outColliders);
	}
}

void QuadTree::Subdivide(QuadTreeNode* node)
{
	float minX = node->boundary.minX;
	float minY = node->boundary.minY;
	float maxX = node->boundary.maxX;
	float maxY = node->boundary.maxY;

	float midX = (minX + maxX) / 2.0f;
	float midY = (minY + maxY) / 2.0f;

	node->children[0] = new QuadTreeNode(AABB(minX, minY, midX, midY));
	node->children[1] = new QuadTreeNode(AABB(midX, minY, maxX, midY));
	node->children[2] = new QuadTreeNode(AABB(minX, midY, midX, maxY));
	node->children[3] = new QuadTreeNode(AABB(midX, midY, maxX, maxY));

	for (auto collider : node->colliders)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (collider->Intersects(node->children[i]->boundary))
			{
				node->children[i]->colliders.push_back(collider);
				break;
			}
		}
	}

	node->colliders.clear();
}
