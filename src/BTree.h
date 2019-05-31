#ifndef _BTREE_H_
#define _BTREE_H_

#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>       // std::vector

class BTreeNode
{
	public:

	float value;
	BTreeNode *left;
	BTreeNode *right;

	BTreeNode() : value(0), left(nullptr), right(nullptr) {}
	BTreeNode(float _value, BTreeNode* _left, BTreeNode* _right) :
	value(_value), left(_left), right(_right) {}

	bool isLeaf() const { return !(left || right); }
};

class BTree
{
	public:

	BTreeNode *root;

	BTree(std::vector<float>& values);
	~BTree();
	void destructNode(BTreeNode* node);

	BTreeNode* buildBranch(std::vector<float>& values, int start, int end);
	void print_inorder() const;
	void print_inorder(BTreeNode* node) const;
	bool query(float key, float& res) const;
	bool query(BTreeNode* node, float key, float& res) const;
};

#endif
