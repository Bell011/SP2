#pragma once
#include <MyMath.h>
#include <iostream>
#include <fstream>
#include<Vector3.h>
class corners {
private:
	Vector3 min;
	Vector3 max;
public:
	void setMin(Vector3 m) {
		min = m;
	}
	void setMax(Vector3 m) {
		max = m;
	}
	Vector3 getMin() {
		return min;
	}
	Vector3 getMax() {
		return max;
	}
	void getCoords(const char* file_path, corners& coord) {
		std::ifstream fileStream(file_path, std::ios::binary);
		if (!fileStream.is_open())
		{
			std::cout << "Unable to open " << file_path << ". Are you in the right directory ?\n";
			return;
		}
		while (!fileStream.eof())
		{
			char buf[256];
			fileStream.getline(buf, 256);
			if (strncmp("v ", buf, 2) == 0)
			{
				Vector3 vertex;
				sscanf_s((buf + 2), "%f%f%f", &vertex.x, &vertex.y, &vertex.z);
				coord.setMin(Vector3(Math::Min(vertex.x, coord.getMin().x), Math::Min(vertex.y, coord.getMin().y), Math::Min(vertex.z, coord.getMin().z)));
				coord.setMax(Vector3(Math::Max(vertex.x, coord.getMax().x), Math::Max(vertex.y, coord.getMax().y), Math::Max(vertex.z, coord.getMax().z)));
			}
		}
	}
};