#pragma once
#include <chrono>

struct GpxPoint {
    double lat;
    double lon;
    std::chrono::system_clock::time_point time_point;
    auto operator<=>(const GpxPoint&) const = default;
};

namespace gpxTk {
    double deg2rad(double deg);
    double rad2deg(double rad);

    //distance between two gpxPoints
    // 
    //in meters
    double distance(GpxPoint p1, GpxPoint p2);

    //time difference between two points in seconds
    double timeDelta(GpxPoint p1, GpxPoint p2);



    //speed between two points
    //in m/s
    double speed(GpxPoint p1, GpxPoint p2);

    std::chrono::system_clock::time_point iso8601toTP(const std::string& timeStr);

    std::string TPtoiso8601(std::chrono::system_clock::time_point TimePoint);


}

namespace Tk {
    template<class T> std::vector<T>::iterator closest(std::vector<T>& vec, T value) {
        auto it = std::min_element(vec.begin(), vec.end(), [value](T a, T b) {
            return std::abs(value - a) < std::abs(value - b);
            });

        return it;
    }

    double line_interp(double x1, double x2, double y1, double y2, int x);
}
