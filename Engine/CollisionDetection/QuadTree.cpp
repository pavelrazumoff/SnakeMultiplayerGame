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

bool QuadTree::Insert(ICollider* collider)
{
	return Insert(root, collider);
}

bool QuadTree::Insert(QuadTreeNode* node, ICollider* collider)
{
	if (!node) return false;
	if (!collider ||
		!collider->Includes(node->boundary)) return false;

	if (node->colliders.size() < capacity)
	{
		node->colliders.push_back(collider);
	}
	else
	{
		if (node->children[0] == nullptr)
			Subdivide(node);

		for (int i = 0; i < 4; ++i)
		{
			if (Insert(node->children[i], collider))
				return true;
		}

		// If we reach here, the collider didn't fit into any child node
		node->colliders.push_back(collider);
	}

	return true;
}

bool QuadTree::Remove(ICollider* collider)
{
	return Remove(root, collider);
}

bool QuadTree::Remove(QuadTreeNode* node, ICollider* collider)
{
	if (!node) return false;

	for (auto it = node->colliders.begin(); it != node->colliders.end(); ++it)
	{
		if (*it == collider)
		{
			node->colliders.erase(it);
			return true;
		}
	}

	for (int i = 0; i < 4; ++i)
		if (Remove(node->children[i], collider)) return true;

	return false;
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
		if (collider == colliderCheck) continue;

		if (colliderCheck->CanCollideWith(collider) &&
			colliderCheck->Intersects(collider))
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

	float midX = (float)round((minX + maxX) / 2.0f);
	float midY = (float)round((minY + maxY) / 2.0f);

	node->children[0] = new QuadTreeNode(AABB(minX, minY, midX, midY));
	node->children[1] = new QuadTreeNode(AABB(midX, minY, maxX, midY));
	node->children[2] = new QuadTreeNode(AABB(minX, midY, midX, maxY));
	node->children[3] = new QuadTreeNode(AABB(midX, midY, maxX, maxY));

	std::vector<ICollider*> remainingColliders;
	for (auto collider : node->colliders)
	{
		bool bAdded = false;
		for (int i = 0; i < 4; ++i)
		{
			if (collider->Includes(node->children[i]->boundary))
			{
				node->children[i]->colliders.push_back(collider);
				bAdded = true;
				break;
			}
		}

		if (!bAdded) remainingColliders.push_back(collider);
	}

	node->colliders = remainingColliders;
}
