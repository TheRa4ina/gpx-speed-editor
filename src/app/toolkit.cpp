#include <cmath>
#include "toolkit.h"

constexpr double PI = 3.141592653589793238462643383279502884;
constexpr double R = 6371.137; // mean Earth radius
constexpr double P = PI / 180;
constexpr double MIN_DISTANCE = 0.0001; // in meters
namespace gpxTk
{
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
        double a = 0.5 - cos((p2.lat - p1.lat) * P) / 2 + cos(p1.lat * P) * cos(p2.lat * P) * (1 - cos((p2.lon - p1.lon) * P)) / 2;
        double result = 2 * R * asin(sqrt(a)) * 1000;
        return std::max(result, MIN_DISTANCE);
    }
    double timeDelta(GpxPoint p1, GpxPoint p2)
    {
        return std::chrono::duration<double>(p2.time_point - p1.time_point).count();
    }
    double speed(GpxPoint p1, GpxPoint p2)
    {
        auto S = distance(p1, p2);  // in meters
        auto t = timeDelta(p1, p2); // in s

        return (S / t);
    }
    std::chrono::system_clock::time_point iso8601toTP(const std::string &timeStr)
#ifdef _WIN32
    {
        std::istringstream in{timeStr};
        std::chrono::system_clock::time_point tp;
        in >> std::chrono::parse("%FT%TZ", tp);
        return tp;
    }
#else
    { // gcc-13 doesnt support std::chrono::parse, so yeah
        std::tm tm = {};
        std::chrono::system_clock::time_point tp;
        std::istringstream in{timeStr};

        in >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");

        // Check if milliseconds part exists
        size_t pos = timeStr.find('.');
        if (pos != std::string::npos)
        {
            std::string milliseconds_str = timeStr.substr(pos + 1, 4); // Get 4 digits for milliseconds
            int milliseconds = std::stoi(milliseconds_str);

            tp = std::chrono::system_clock::from_time_t(std::mktime(&tm)) + std::chrono::milliseconds(milliseconds);
        }
        else
        {
            // No milliseconds part, just use the regular time
            time_t t = std::mktime(&tm);
            tp = std::chrono::system_clock::from_time_t(t);
        }

        return tp;
    }
#endif
    std::string TPtoiso8601(std::chrono::system_clock::time_point tp)
#ifdef _WIN32
    {
        return std::format("{:%FT%T0Z}", std::chrono::floor<std::chrono::milliseconds>(tp));
    }
#else
    {
        std::time_t time = std::chrono::system_clock::to_time_t(tp);
        std::tm tm = *std::gmtime(&time);

        auto duration = tp.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;

        // Create string stream to format the date and time
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S")                    // Standard ISO format without milliseconds
            << '.' << std::setw(4) << std::setfill('0') << millis.count() // Add milliseconds
            << 'Z';                                                       // Append 'Z' for UTC

        return oss.str();
    }
#endif
}

double Tk::line_interp(double x1, double x2, double y1, double y2, int x)
{
    if (x2 == x1)
    {
        return ((y2 + y1) / 2);
    }
    double k = (y2 - y1) / (x2 - x1);
    double ans = y1 + k * (x - x1);
    return y1 + k * (x - x1);
}
