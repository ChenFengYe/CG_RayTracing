#include "Vec3d.h"
#include "MeshGroup.h"

class PointLight
{
public:
	PointLight()
	{
		m_center = Vec3d(0.0, 10.0, -8.0);
		m_color = Vec3d(1.0, 1.0, 1.0);
		m_lightStrength = 10;
	}
	~PointLight(){}

	Vec3d GetPointColor(Vec3d p, Vec3d color, Vec3d normal, Vec3d EyeDire)
	{
		//-------------------------------------------
		// Phony Lighting Model
		//-------------------------------------------
		double ambient = 0.3;
		double shininess = 70.0f;

		// Light Direction
		Vec3d light_direction = m_center - p; // ?? p - center
		light_direction.normalize();
		
		// Normal
		normal.normalize();

		// Half vector
		Vec3d half_vector = light_direction + EyeDire;
		half_vector.normalize();

		// diffuse
		double diffuse = 0.6*max(0.0, normal.dot(light_direction));
		// specular
		double specular = 0.4*pow(max(0.0, normal.dot(half_vector)), shininess);

		//color = diffuse * color + specular * color;
		color = (ambient + diffuse)  * color + m_color * specular;

		//--------------------------------------------
		// Clamp
		//--------------------------------------------

		return Vec3d(color.x(), color.y(), color.z());
	}

	Vec3d	m_center;
	Vec3d	m_color;
	double	m_lightStrength;

private:
};