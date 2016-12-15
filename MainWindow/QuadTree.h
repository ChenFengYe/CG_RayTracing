#include "image.h"
#include <vector>

using namespace lw;
using namespace std;

class QuadTree
{
public:
	QuadTree()
	{
	};
	~QuadTree()
	{

	};

	struct					QuadNode
	{
		Vec2i				originPixel;
		Vec2i				endPixel;
		Vec3d				rgbSum;
		bool				childEmpty = true;

		QuadNode*			c0 = NULL;
		QuadNode*			c1 = NULL;
		QuadNode*			c2 = NULL;
		QuadNode*			c3 = NULL;
	};
	void					BuildQuadTree(QuadNode* n, int depth, double threshold)
	{
		if (SumVec3d(n->rgbSum) > threshold)
		{
			BuildChild(n);
			n->childEmpty = false;
			
			BuildQuadTree(n->c0, depth + 1, threshold);
			BuildQuadTree(n->c1, depth + 1, threshold);
			BuildQuadTree(n->c2, depth + 1, threshold);
			BuildQuadTree(n->c3, depth + 1, threshold);
		}
		else
			return;
	};
	void					BuildSumImage(Image* hdr, double shortScale)
	{
		// hang zhu xu (行主序 先循环 x)
		width = hdr->width();
		height = hdr->height();
		hdrShortScale = shortScale;
		// Get more space

		SumImage.reserve(width * height * 2);
		SumImage.resize(height * width);

		// Init (0, 0)
		SumImage[0] = InitQuadNode(hdr, 0, 0);
		
		// Init Sum Image first row
		for (int i = 1; i < width; i++)
		{
			SumImage[i].originPixel = Vec2i(0, 0);
			SumImage[i].endPixel = Vec2i(i, 0);
			SumImage[i].rgbSum = 
				SumImage[i - 1].rgbSum + 
				Vec3dFromColor(hdr->pixel(i, 0));
		}

		// Init Sum Image first col
		for (int j = 1; j < height; j++)
		{
			SumImage[j * width].originPixel = Vec2i(0, 0);
			SumImage[j * width].endPixel = Vec2i(0, j);
			SumImage[j * width].rgbSum = 
				SumImage[(j - 1) * width].rgbSum 
				+ Vec3dFromColor(hdr->pixel(0, j));
		}

		// Build Sum Image
		for (int y = 1; y < height; y++)
		{
			for (int x = 1; x < width; x++)
			{
				SumImage[y*width + x] = InitQuadNode(
					SumImage[(y - 1)*width + x - 1],
					SumImage[(y - 1)*width + x],
					SumImage[y*width + x - 1], 
					hdr->pixel(x, y));
			}
		}
		rootNode_ = &SumImage[width*height - 1];
	};
	inline QuadNode			InitQuadNode(Image *hdr, int x, int y)
	{
		QuadNode a;
		a.originPixel = Vec2i(0, 0);
		a.endPixel = Vec2i(x, y);
		a.rgbSum = Vec3dFromColor(hdr->pixel(x, y));
		return a;
	};
	inline QuadNode			InitQuadNode(QuadNode s1, QuadNode s2, QuadNode s3, Color c)
	{
		QuadNode s4;

		if (s1.originPixel != s2.originPixel 
			|| s2.originPixel != s3.originPixel
			|| s3.originPixel != s1.originPixel)
		{
			cout << "error: can't compute! return empty result" << endl;
			return s4;
		}

		s4.originPixel = s1.originPixel;
		if (s1.originPixel!=Vec2i(0, 0))
		{
			cout << "error: can't compute! return empty result" << endl;
			return s4;
		}
		s4.endPixel = s1.endPixel + Vec2i(1, 1);
		s4.rgbSum = s2.rgbSum + s3.rgbSum - s1.rgbSum + Vec3dFromColor(c);
		return s4;
	}
	inline bool				BuildQuadNode(QuadNode *rslt,
										QuadNode s1, QuadNode s2, QuadNode s3, QuadNode s4)
	{
		if (s1.originPixel != s2.originPixel
			|| s2.originPixel != s3.originPixel
			|| s3.originPixel != s1.originPixel)
		{
			cout << "error: can't compute! return empty result" << endl;
			return false;
		}
		rslt->originPixel = s1.endPixel;
		rslt->endPixel = s4.endPixel;
		rslt->rgbSum = s4.rgbSum - (s3.rgbSum + s2.rgbSum - s1.rgbSum);
		return true;
	}
	inline Vec3d			Vec3dFromColor(Color c)
	{
		return Vec3d(min(c.red(), hdrShortScale),
			min(c.green(), hdrShortScale),
			min(c.blue(),hdrShortScale));
	}
	inline double			SumVec3d(Vec3d a)
	{
		return a[0] + a[1] + a[2];
	}
	bool					BuildChild(QuadNode *n)
	{
		bool errorChecker = true;
		n->c0 = new QuadNode;
		int x1 = n->originPixel.x();
		int y1 = n->originPixel.y();
		int x2 = (n->originPixel.x() + n->endPixel.x()) / 2;
		int y2 = (n->originPixel.y() + n->endPixel.y()) / 2;
		errorChecker = BuildQuadNode(n->c0, SumImage[x1 + y1*width], SumImage[x2 + y1*width], SumImage[x1 + y2*width], SumImage[x2 + y2*width]);
		if (!errorChecker)	return false;

		n->c1 = new QuadNode;
		int x3 = x2 + 1;
		int y3 = y1;
		int x4 = n->endPixel.x();
		int y4 = y2;
		errorChecker = BuildQuadNode(n->c1, SumImage[x3 + y3*width], SumImage[x4 + y3*width], SumImage[x3 + y4*width], SumImage[x4 + y4*width]);
		if (!errorChecker)	return false;

		n->c2 = new QuadNode;
		int x5 = x1;
		int y5 = y2 + 1;
		int x6 = x2;
		int y6 = n->endPixel.y();
		errorChecker = BuildQuadNode(n->c2, SumImage[x5 + y5*width], SumImage[x6 + y5*width], SumImage[x5 + y6*width], SumImage[x6 + y6*width]);
		if (!errorChecker)	return false;

		n->c3 = new QuadNode;
		int x7 = x3;
		int y7 = y5;
		int x8 = x4;
		int y8 = y6;
		errorChecker = BuildQuadNode(n->c3, SumImage[x7 + y7*width], SumImage[x8 + y7*width], SumImage[x7 + y8*width], SumImage[x8 + y8*width]);
		if (!errorChecker)	return false;
		return true;
	}

	vector<QuadNode>		SumImage;
	int						width;
	int						height;
	QuadNode				*rootNode_;
	double					hdrShortScale;
};