#include "BTree.h"

BTree::BTree(std::vector<float>& values)
{
	int n = values.size();

	if(!n) { root = nullptr; }
	else if(n == 1) { root = new BTreeNode(values[0], nullptr, nullptr); }

	else
	{
		// Sort the array
		std::sort(values.begin(), values.end());
		int mid = n / 2;

		root = new BTreeNode(values[mid],
					buildBranch(values, 0, mid - 1),
					buildBranch(values, mid + 1, n - 1));
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

BTreeNode* BTree::buildBranch(std::vector<float>& values, int start, int end)
{
	if(end < start) { return nullptr; }

	int mid = (start + end) / 2;

	BTreeNode *node = new BTreeNode(values[mid],
					buildBranch(values, start, mid - 1),
					buildBranch(values, mid + 1, end));
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

bool BTree::query(float key, float& res) const
{
	if(root) { return query(root, key, res); }

	return false;
}

bool BTree::query(BTreeNode* node, float key, float& res) const
{
	if(key > node->value)
	{
		if(node->right) { return query(node->right, key, res); }

		else
		{
			res = node->value;
			return true;
		}
	}

	else
	{
		if(node->left && key > node->left->value)
		{
			res = node->value;
			return true;
		}

		else if(node->left) { return query(node->left, key, res); }

		else
		{
			res = node->value;
			return true;
		}
	}

	res = node->value;
	return true;
}
