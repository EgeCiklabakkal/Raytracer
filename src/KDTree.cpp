#include "KDTree.h"

KDTree::KDTree(std::vector<HitPoint>& hitpoints)
{
	unsigned int n = hitpoints.size();

	if(!n) { root = nullptr; }
	else if(n == 1) { root = new KDTreeNode(hitpoints[0], Axis::X, nullptr, nullptr); }

	else
	{
		// Choose splitting axis by variance
		Axis split_plane(Axis::X);
		float mean_x = 0; float mean_y = 0; float mean_z = 0;
		float var_x  = 0; float var_y  = 0; float var_z  = 0;

		for(const HitPoint& hitpoint : hitpoints)
		{
			mean_x += hitpoint.p.x();
			mean_y += hitpoint.p.y();
			mean_z += hitpoint.p.z();
		}
		mean_x /= float(n);
		mean_y /= float(n);
		mean_z /= float(n);

		for(const HitPoint& hitpoint : hitpoints)
		{
			var_x += (hitpoint.p.x() - mean_x) * (hitpoint.p.x() - mean_x);
			var_y += (hitpoint.p.y() - mean_y) * (hitpoint.p.y() - mean_y);
			var_z += (hitpoint.p.z() - mean_z) * (hitpoint.p.z() - mean_z);
		}

		if(var_x >= var_y && var_x >= var_z)
		{
			std::sort(hitpoints.begin(), hitpoints.end(), []
							(HitPoint hp1, HitPoint hp2)
							{ return hp1.p.x() < hp2.p.x(); });
			split_plane = Axis::X;
		}

		else if(var_y >= var_z && var_y >= var_z)
		{
			std::sort(hitpoints.begin(), hitpoints.end(), []
							(HitPoint hp1, HitPoint hp2)
							{ return hp1.p.y() < hp2.p.y(); });
			split_plane = Axis::Y;
		}

		else
		{
			std::sort(hitpoints.begin(), hitpoints.end(), []
							(HitPoint hp1, HitPoint hp2)
							{ return hp1.p.z() < hp2.p.z(); });
			split_plane = Axis::Z;

		}

		int mid = n / 2;

		root = new KDTreeNode(hitpoints[mid], split_plane,
					buildBranch(hitpoints, 0, mid),
					buildBranch(hitpoints, mid + 1, n));
	}
}

KDTreeNode* KDTree::buildBranch(std::vector<HitPoint>& hitpoints, int start, int end)
{
	if(end <= start) { return nullptr; }

	int n = end - start;
	if(n == 1) { return new KDTreeNode(hitpoints[start], Axis::X, nullptr, nullptr); }

	// Choose splitting axis by variance
	Axis split_plane(Axis::X);
	float mean_x = 0; float mean_y = 0; float mean_z = 0;
	float var_x  = 0; float var_y  = 0; float var_z  = 0;

	std::vector<HitPoint>::iterator it;
	for(it = hitpoints.begin() + start; it != hitpoints.begin() + end; it++)
	{
		mean_x += it->p.x();
		mean_y += it->p.y();
		mean_z += it->p.z();
	}
	mean_x /= float(n);
	mean_y /= float(n);
	mean_z /= float(n);

	for(it = hitpoints.begin() + start; it != hitpoints.begin() + end; it++)
	{
		var_x += (it->p.x() - mean_x) * (it->p.x() - mean_x);
		var_y += (it->p.y() - mean_y) * (it->p.y() - mean_y);
		var_z += (it->p.z() - mean_z) * (it->p.z() - mean_z);
	}

	if(var_x >= var_y && var_x >= var_z)
	{
		std::sort(hitpoints.begin() + start, hitpoints.begin() + end, []
						(HitPoint hp1, HitPoint hp2)
						{ return hp1.p.x() < hp2.p.x(); });
		split_plane = Axis::X;
	}

	else if(var_y >= var_z && var_y >= var_z)
	{
		std::sort(hitpoints.begin() + start, hitpoints.begin() + end, []
						(HitPoint hp1, HitPoint hp2)
						{ return hp1.p.y() < hp2.p.y(); });
		split_plane = Axis::Y;
	}

	else
	{
		std::sort(hitpoints.begin() + start, hitpoints.begin() + end, []
						(HitPoint hp1, HitPoint hp2)
						{ return hp1.p.z() < hp2.p.z(); });
		split_plane = Axis::Z;

	}

	int mid = n / 2;

	KDTreeNode *node = new KDTreeNode(hitpoints[start + mid], split_plane,
				buildBranch(hitpoints, start, start + mid),
				buildBranch(hitpoints, start + mid + 1, end));

	return node;
}

KDTree::~KDTree()
{
	destructNode(root);
}

void KDTree::destructNode(KDTreeNode* node)
{
	if(node)
	{
		destructNode(node->left);
		destructNode(node->right);
		delete node;
	}
}
