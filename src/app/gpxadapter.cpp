#include "gpxadapter.h"
#include "pugixml.hpp"
#include <sstream>
#include <iostream>

std::vector<GpxPoint> GpxAdapter::readFile(std::string file_name)
{
	std::vector<GpxPoint> points;
	pugi::xml_document gpx;
	pugi::xml_parse_result result = gpx.load_file(file_name.c_str());
	if (!result) {
		std::cout << "Failed to load file: " << result.description() << std::endl;
		throw "Couldn't read file";
	}
	for (pugi::xml_node trkpt : gpx.child("gpx").child("trk").child("trkseg").children("trkpt")) {
		GpxPoint point;
		point.lat = trkpt.attribute("lat").as_double();
		point.lon = trkpt.attribute("lon").as_double();
		point.time_point = gpxTk::iso8601toTP(trkpt.child("time").child_value());

		points.push_back(point);
	}
	return points;
}

void GpxAdapter::changeFile(std::vector<GpxPoint> points, std::string file_name)
{
	pugi::xml_document gpx;
	pugi::xml_parse_result result = gpx.load_file(file_name.c_str(), pugi::parse_declaration);
	std::string new_time;
	unsigned int count = 0;
	if (!result) {
		std::cout << "Failed to load file: " << result.description() << std::endl;
	}

	for (pugi::xml_node trkpt : gpx.child("gpx").child("trk").child("trkseg").children("trkpt")) {
		new_time = gpxTk::TPtoiso8601(points[count].time_point);
		trkpt.child("time").text().set(new_time.c_str());
		count++;
	}
	gpx.save_file(file_name.c_str());
}

