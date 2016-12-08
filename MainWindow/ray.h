#ifndef RAY_H
#define RAY_H

#include "Vec3d.h"
#include "MeshGroup.h"
#include "ray.h"

class Ray
{

public:
	Ray(){}
	~Ray(){}
	
	Ray(Vec3d P1,Vec3d P2)
	{
		if (P1 == P2)		return;

		m_origin = P1;
		m_dire = P2 - P1;
		m_dire.normalize();
	}

public:
	Vec3d					m_origin;
	Vec3d					m_dire;

};

#endif