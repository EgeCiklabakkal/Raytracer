#ifndef _KDTREE_H_
#define _KDTREE_H_

#include <algorithm>
#include <vector>

#include "hitpoint.h"

enum class Axis { X, Y, Z };

class KDTreeNode
{
	public:

	HitPoint hitpoint;
	Axis splitAxis;
	KDTreeNode *left;
	KDTreeNode *right;

	KDTreeNode() : hitpoint(HitPoint()), left(nullptr), right(nullptr) {}
	KDTreeNode(const HitPoint& _hitpoint, const Axis& _axis,
			KDTreeNode* _left, KDTreeNode* _right) :
	hitpoint(_hitpoint), splitAxis(_axis), left(_left), right(_right) {}

	bool isLeaf() const { return !(left || right); }	
};

class KDTree
{
	public:

	KDTreeNode *root;

	KDTree() : root(nullptr) {}
	KDTree(std::vector<HitPoint>& hitpoints);
	~KDTree();
	void destructNode(KDTreeNode* node);

	KDTreeNode* buildBranch(std::vector<HitPoint>& hitpoints, int start, int end);
};

#endif
