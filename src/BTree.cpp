#include "BTree.h"

BTree::BTree(const std::vector<Shape*>& meshTriangles, const glm::mat4& trans)
{
	unsigned int n = meshTriangles.size();
	float areaAcc = 0.0f;
	std::vector<float> area_data(n);
	std::vector<MeshTriangle*> ptr_data(n);

	for(unsigned int i = 0; i < n; i++)
	{
		MeshTriangle *triangle_ptr = dynamic_cast<MeshTriangle*>(meshTriangles[i]);
		if(!triangle_ptr)
		{
			throw std::runtime_error("Unable to cast downcast Shape* to MeshTriangle*");
		}

		areaAcc += triangle_ptr->area(trans);
		area_data[i] = areaAcc;
		ptr_data[i]  = triangle_ptr;
	}

	// Divide by whole area, obtain cdf
	this->area = areaAcc;
	for(unsigned int i = 0; i < n; i++) { area_data[i] /= areaAcc; }

	if(!n) { root = nullptr; }
	else if(n == 1) { root = new BTreeNode(area_data[0], ptr_data[0], nullptr, nullptr); }

	else
	{
		int mid = n / 2;

		root = new BTreeNode(area_data[mid], ptr_data[mid],
					buildBranch(area_data, ptr_data, 0, mid - 1),
					buildBranch(area_data, ptr_data, mid + 1, n - 1));
	}
}

BTree::~BTree()
{
	destructNode(root);
}

void BTree::destructNode(BTreeNode* node)
{
	if(node)
	{
		destructNode(node->left);
		destructNode(node->right);
		delete node;
	}
}

BTreeNode* BTree::buildBranch(const std::vector<float>& values,
				const std::vector<MeshTriangle*>& items,
				int start, int end)
{
	if(end < start) { return nullptr; }

	int mid = (start + end) / 2;

	BTreeNode *node = new BTreeNode(values[mid], items[mid],
					buildBranch(values, items, start, mid - 1),
					buildBranch(values, items, mid + 1, end));
	return node;
}

void BTree::print_inorder() const
{
	print_inorder(root);
	std::cout << std::endl;
}

void BTree::print_inorder(BTreeNode* node) const
{
	if(node)
	{
		print_inorder(node->left);
		std::cout << node->value << " ";
		print_inorder(node->right);
	}
}

MeshTriangle* BTree::query(float key) const
{
	if(root) { return query(root, key); }

	throw std::runtime_error("Unable to query BTree with nullptr root");
}

MeshTriangle* BTree::query(BTreeNode* node, float key) const
{
	if(key > node->value)
	{
		if(node->right) { return query(node->right, key); }
		else 		{ return node->triangle_ptr; }
	}

	else
	{
		if(node->left && key > node->left->value)
		{
			return node->triangle_ptr;
		}

		else if(node->left) { return query(node->left, key); }

		else { return node->triangle_ptr; }
	}

	return node->triangle_ptr;
}
