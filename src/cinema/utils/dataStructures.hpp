#pragma once

#include <string>
#include <vector>

struct Structure
{
	std::string type; // halo or points

	size_t numPoints;

	std::vector<float> pos;
	std::vector<float> vel;
	
	float center[3];
	float radius;

	float centerColor[3];


	Structure(size_t _numPoints, std::vector<float> _pos, std::vector<float> _vel, float _center[3], float _radius, float _centerColor[3])
	{
		type = "halo";
		pos = _pos;
		vel = _vel;

		radius = _radius;

		for (int i=0; i<3; i++)
		{
			center[i] = _center[i];
			centerColor[i] = _centerColor[i];
		}
	}


	Structure(size_t _numPoints,  std::vector<float> _posX, std::vector<float> _posY, std::vector<float> _posZ, 
								  std::vector<float> _velX, std::vector<float> _velY, std::vector<float> _velZ, 
								  float centerX, float centerY, float centerZ, float _radius, float r, float g, float b)
	{
		type = "halo";

		numPoints = _numPoints;

		for (int i=0; i<_numPoints; i++)
		{
			pos.push_back(_posX[i]);
			pos.push_back(_posY[i]);
			pos.push_back(_posZ[i]);

			vel.push_back(_velX[i]);
			vel.push_back(_velY[i]);
			vel.push_back(_velZ[i]);
		}

		radius = _radius;

		center[0] = centerX;
		center[1] = centerY;
		center[2] = centerZ;

		centerColor[0] = r;
		centerColor[1] = g;
		centerColor[2] = b;
	}



	Structure(size_t _numPoints, std::vector<float> _pos)
	{
		type = "points";

		numPoints = _numPoints;
		pos = _pos;
	}

	Structure(size_t _numPoints, std::vector<float> _posX, std::vector<float> _posY, std::vector<float> _posZ)
	{
		type = "points";

		numPoints = _numPoints;

		for (int i=0; i<_numPoints; i++)
		{
			pos[i*3+0] = _posX[i];
			pos[i*3+1] = _posY[i];
			pos[i*3+2] = _posZ[i];
		}
	}
};