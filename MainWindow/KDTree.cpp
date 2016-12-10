#include "KDTree.h"

bool myComparefunctionX(HE_face* a, HE_face* b)
{
	if (a->center().x() < b->center().x())
		return 1;
	else
		return 0;
}

bool myComparefunctionY(HE_face* a, HE_face* b)
{
	if (a->center().y() < b->center().y())
		return 1;
	else
		return 0;
}

bool myComparefunctionZ(HE_face* a, HE_face* b)
{
	if (a->center().z() < b->center().z())
		return 1;
	else
		return 0;
}

void KDNode::CreatKDTree(int depth)
{
	depth_ = depth;

	// Check
	if (tris_.size() == 0)
	{
		return;
	};
	if (tris_.size() == 1)
	{
		leftNode_ = new KDNode();
		rightNode_ = new KDNode();
		return;
	};

	for (size_t i = 0; i < tris_.size(); i++)
	{
		tris_[i]->center();
	}
	// Sort
	switch (depth % 3)
	{
	case 0:
		sort(tris_.begin(), tris_.end(), myComparefunctionX);
		break;
	case 1:
		sort(tris_.begin(), tris_.end(), myComparefunctionY);
		break;
	case 2:
		sort(tris_.begin(), tris_.end(), myComparefunctionZ);
		break;
	default:
		break;
	}

	// Build Child Node
	leftNode_ = new KDNode();
	copy(tris_.begin(),
		tris_.begin() + tris_.size() / 2,
		back_inserter(leftNode_->tris_));

	rightNode_ = new KDNode();
	copy(tris_.begin() + tris_.size() / 2,
		tris_.end(),
		back_inserter(rightNode_->tris_));

	// Recursion KD
	leftNode_->CreatKDTree(depth + 1);
	rightNode_->CreatKDTree(depth + 1);

	// Build Bounding Box
	BuildBoundingBox(tris_);
};