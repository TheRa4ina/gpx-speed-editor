#pragma once
#include "toolkit.h"
#include <vector>
#include <string>
//Change name to GpxProccessor?
class GpxAdapter
{
public:
	//add stupid open func, rn its more of a toolkit

	std::vector<GpxPoint> readFile(std::string file_name);
	void changeFile(std::vector<GpxPoint> points, std::string file_name);
private:
};

