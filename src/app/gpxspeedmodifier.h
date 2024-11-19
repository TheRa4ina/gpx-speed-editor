#pragma once
#include "toolkit.h"
#include "implot/implot.h"
#include <chrono>
//Changes speeds of points in gpx
//basically resposible by itself to get speeds from points
//and from returning updated points
struct TrackStats {
	double min_velocity;
	double max_velocity;
	double avg_velocity;
	int point_count;

	std::string total_time;
};


class GpxSpeedModifier
{
public:
	void getGui();

	// Load in Gpx points, to show on graph
	void loadPoints(std::vector<GpxPoint> points);
	// Get current Gpx points
	std::vector<GpxPoint>& getPoints();
private:
	std::vector<GpxPoint> m_points;
	std::vector<double> m_velocities_km_per_h;
	std::vector<double> m_time_vec;
	TrackStats m_track_stats;

	// If empty shows graph, otherwise error info
	std::string m_error_info = "gpx file not loaded";

	bool plot_changed = false;
	ImPlotPoint last_mouse_pos{ -1.0, -1.0 };

	// updates plot points
	void updatePlot();

	// calculates time points for gpx points, from velocities values in velocity graph
	void calcTimePoints();
	// calculates velocities, from time points in Gpx points
	void calcPlotPoints();
	// update track stats
	void collectStatistic();

	// checks if ctrl and lmb are down 
	bool isDrawingMode();
	// puts dragPoints on plot, if not drawing mode, else allows to draw own curve
	void handlePlotChange();
	// draws dragPoints, to manipulate graph
	void handlePointManipulation();
	// Updates velocity values based on mouse movement.
	// Ensures velocities stay above a threshold and updates the plot.
	void handleCurveDrawing();

};

