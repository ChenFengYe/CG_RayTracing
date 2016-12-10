#include "camera.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Camera::Camera()
	: m_center(0.0, 0.0, -2.0)
	, m_lookAt(0.0, 0.0, 8.0)
	, m_up(0.0, 1.0, 0.0)
	, m_L(1.0)
	, m_imageSize(800, 600)
	, m_sampleNum(5)
	, m_theta(PI/4.0)
{
	m_rows = m_imageSize.y();
	m_cols = m_imageSize.x();
	m_lookAt.normalize();
}

Camera::~Camera()
{

}

void Camera::CreateCameraParam()
{
	m_u = m_lookAt.cross(m_up);		m_u.normalize();
	m_v = m_lookAt.cross(m_u);		m_v.normalize();
	m_pixelSize = m_L*tan(m_theta) / (m_cols / 2.0);
	m_v *= m_pixelSize;
	m_u *= m_pixelSize;
	m_origin = m_center + m_L*m_lookAt + m_cols / 2.0*(-m_u) + m_rows / 2.0*(-m_v);
}

QImage* Camera::CreateImage(vector<FatherMesh*> meshs, vector<Light*> lights)
{
	CreateCameraParam();
	m_picture = new QImage(m_cols, m_rows, QImage::Format_RGB32);
	for (int row = 0; row < m_rows; row++)
	{
		for (int col = 0; col < m_cols; col++)
		{
			// Get Rays
			vector<Ray> rays;
			CreateRayParam(rays, row, col, m_sampleNum);
			
			// Get Pixel
			Vec3d pixelValue = Vec3d(0.0, 0.0 ,0.0);
			for (int i_ray = 0; i_ray < rays.size(); i_ray++)
			{
				// Get Intersection
				Vec4d Insec = Vec4d(0.0, 0.0, 0.0, DBL_MAX);					// Vec4d(R, G, B, Distance)
				int Index_mesh = -1;
				for (int i = 0; i < meshs.size(); i++)
				{
					Vec4d insec_temp = meshs[i]->GetIntersection(&rays[i_ray]);
					if (insec_temp[3] > 0 && insec_temp[3] < Insec[3])
					{
						Insec = insec_temp;
						Index_mesh = i;
					}
				}

				// Get Point Value
				if (Index_mesh >= 0)
				{
					pixelValue += meshs[Index_mesh]->
						GetPointValue(&(rays[i_ray]), lights, meshs, Insec, -m_lookAt);
				}
			}

			// Clamp
			pixelValue /= rays.size();
			double k = 0.5*500;
			pixelValue *= k;
			if (pixelValue.x() > 255) pixelValue.x() = 255;
			if (pixelValue.y() > 255) pixelValue.y() = 255;
			if (pixelValue.z() > 255) pixelValue.z() = 255;
			m_picture->setPixel(col, row, qRgb(pixelValue.x(), pixelValue.y(), pixelValue.z()));
			//m_picture->setPixel(col, row, qRgb(pixelValue.x(), pixelValue.y(), pixelValue.z()));
		}
	}
	return m_picture;
}
void Camera::CreateRayParam(vector<Ray>& rays, int r, int c, int s)
{
	if ( s == 1 )
	{
		rays.push_back(Ray(m_center, m_origin + ((double)r + 0.5)*m_v + ((double)c + 0.5)*m_u));
		//cout << m_origin + ((double)r + 0.5)*m_v + ((double)c + 0.5)*m_u << endl;
	}
	else if (s > 1)
	{
		//srand(time(NULL));
		for (int i = 0; i < s; i++)
		{
			for (int j = 0; j < s; j++)
			{
				//double detaX = rand() % s*s;
				//double detaY = rand() % s*s;
				double detaX = (double)i / (double)s;
				double detaY = (double)j / (double)s;
				rays.push_back(Ray(m_center, m_origin + ((double)r + detaX)*m_v + ((double)c + detaY)*m_u));
			}
		}
	}
}
