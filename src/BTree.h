#ifndef _BTREE_H_
#define _BTREE_H_

#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>       // std::vector

#include "shape.h"
#include "mesh.h"

class BTreeNode
{
	public:

	float value; 	// Area of the triangle
	MeshTriangle *triangle_ptr;
	BTreeNode *left;
	BTreeNode *right;

	BTreeNode() : value(0), left(nullptr), right(nullptr) {}
	BTreeNode(float _value, MeshTriangle* _triangle_ptr, BTreeNode* _left, BTreeNode* _right) :
	value(_value), triangle_ptr(_triangle_ptr), left(_left), right(_right) {}

	bool isLeaf() const { return !(left || right); }
};

class BTree
{
	public:

	BTreeNode *root;
	float area;	// Sum of the areas of triangles

	BTree(const std::vector<Shape*>& meshTriangles, const glm::mat4& trans=glm::mat4(1.0f));
	~BTree();
	void destructNode(BTreeNode* node);

	BTreeNode* buildBranch(const std::vector<float>& values,
				const std::vector<MeshTriangle*>& items,
				int start, int end);
	void print_inorder() const;
	void print_inorder(BTreeNode* node) const;
	MeshTriangle* query(float key) const;
	MeshTriangle* query(BTreeNode* node, float key) const;
};

#endif
