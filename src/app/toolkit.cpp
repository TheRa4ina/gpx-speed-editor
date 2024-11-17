#include <numbers>
#include <cmath>
#include "toolkit.h"

constexpr double R = 6371.137; // mean Earth radius
constexpr double P = std::numbers::pi / 180;
const double MIN_DISTANCE = 0.0001; // in meters
namespace gpxTk {
    double deg2rad(double deg)
    {
        return deg * P;
    }
    double rad2deg(double rad)
    {
        return rad * P;
    }
    double distance(GpxPoint p1, GpxPoint p2)
    {
        double a = 0.5 - cos((p2.lat - p1.lat) * P) / 2
            + cos(p1.lat * P) * cos(p2.lat * P)
            * (1 - cos((p2.lon - p1.lon) * P)) / 2;
        double result = 2 * R * asin(sqrt(a)) * 1000;
        return std::max(result,MIN_DISTANCE);

    }
    double timeDelta(GpxPoint p1, GpxPoint p2)
    {
        return std::chrono::duration<double>(p2.time_point - p1.time_point).count();
    }
    double speed(GpxPoint p1, GpxPoint p2)
    {
        auto S = distance(p1, p2);// in meters
        auto t = timeDelta(p1, p2);// in s

        return (S / t);
    }
    std::chrono::system_clock::time_point iso8601toTP(const std::string& timeStr)
    {
        std::istringstream in{ timeStr };
        std::chrono::system_clock::time_point tp;
        in >> std::chrono::parse("%FT%TZ", tp);
        return tp;
    }

    std::string TPtoiso8601(std::chrono::system_clock::time_point tp)
    {
        return std::format("{:%FT%T0Z}", std::chrono::floor<std::chrono::milliseconds>(tp));
    }
}

double Tk::line_interp(double x1, double x2, double y1, double y2, int x)
{
    if (x2 == x1) {
        return ((y2 + y1) / 2);
    }
    double k = (y2 - y1) / (x2 - x1);
    double ans = y1 + k * (x - x1);
    return y1 + k * (x - x1);
}
