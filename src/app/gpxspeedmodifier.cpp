#include "gpxspeedmodifier.h"
#include "imgui/imgui.h"
#include <numeric>
#include <format>
#include <algorithm>

void GpxSpeedModifier::loadPoints(std::vector<GpxPoint> points)
{
    m_points = points;
    if (points.size() < 3) {
        m_error_info = "Not enough points";
        return;
    }
    m_error_info.clear();
    calcPlotPoints();
    collectStatistic();
    ImPlot::SetNextAxesToFit();
}

void GpxSpeedModifier::collectStatistic()
{
    if (!m_error_info.empty()) {
        return;
    }

    double sum = 0.0;
    m_track_stats.max_velocity = std::numeric_limits<double>::lowest();
    m_track_stats.min_velocity = std::numeric_limits<double>::max();
    m_track_stats.point_count = m_time_vec.size();

    for (const auto& velocity : m_velocities_km_per_h) {
        m_track_stats.max_velocity = std::max(m_track_stats.max_velocity, velocity);
        m_track_stats.min_velocity = std::min(m_track_stats.min_velocity, velocity);
        sum += velocity;
    }
    m_track_stats.avg_velocity = sum / m_velocities_km_per_h.size();

    uint64_t seconds_took = m_time_vec.back() - m_time_vec.front();
    std::chrono::hh_mm_ss seconds{ std::chrono::seconds(seconds_took) };
    m_track_stats.total_time = std::format("{:%T}", seconds);
}

void GpxSpeedModifier::updatePlot()
{
    calcTimePoints();
    calcPlotPoints();
    collectStatistic();
}


void GpxSpeedModifier::calcPlotPoints()
{
    m_velocities_km_per_h.clear();
    m_time_vec.clear();
    for (size_t i = 0; i < m_points.size() - 1; i++) {
        const GpxPoint& p1 = m_points[i];
        const GpxPoint& p2 = m_points[i + 1];
        m_velocities_km_per_h.push_back(gpxTk::speed(p1, p2) * 3.6);// convert m/s to km/h

        auto time_spent = gpxTk::timeDelta(p1, p2);
        if (time_spent < 0) {
            m_error_info = "earlier time point happens after future time point?\n"
                + gpxTk::TPtoiso8601(p1.time_point) + ">" + gpxTk::TPtoiso8601(p2.time_point);
            return;
        }

        double seconds_since_epoch = std::chrono::duration<double>(p1.time_point.time_since_epoch()).count();
        m_time_vec.push_back(seconds_since_epoch);
    }
}

void GpxSpeedModifier::calcTimePoints()
{
    for (size_t i = 0; i < m_points.size() - 1; i++) {
        GpxPoint& p1 = m_points[i];
        GpxPoint& p2 = m_points[i + 1];

        double meters = (gpxTk::distance(p1, p2));// in meters
        double m_per_s = m_velocities_km_per_h[i] / 3.6;//from km/h to m/s

        double new_seconds_delta = (meters / m_per_s);
        uint64_t new_milliseconds_delta = static_cast<int64_t>(new_seconds_delta * 1000);
        p2.time_point = p1.time_point + std::chrono::milliseconds(new_milliseconds_delta);
    }
}

void GpxSpeedModifier::getGui()
{
    if (!m_error_info.empty()) {
        ImGui::TextUnformatted(m_error_info.c_str());
        return;
    }
    ImGui::BulletText("Draw with ctrl+rmb");
    ImGui::Text("Max speed: %f", m_track_stats.max_velocity);
    ImGui::SameLine(); ImGui::Text("Time spent: %s", m_track_stats.total_time.c_str());
    ImGui::Text("Avg speed: %f", m_track_stats.avg_velocity);
    ImGui::Text("Min speed: %f", m_track_stats.min_velocity);
    ImGui::Text("Point Count: %d", m_track_stats.point_count);
    ImPlot::GetStyle().Use24HourClock = true;
    if (ImPlot::BeginPlot("##Speed")) {
        ImPlot::SetupAxes("time", "speed");
        ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Time);
        ImPlot::SetupAxisLimits(ImAxis_Y1,0, m_track_stats.max_velocity + m_track_stats.avg_velocity / 3);
        ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, m_time_vec.front(), m_time_vec.back()+100);
        handlePlotChange();
        ImPlot::PlotLine("track", m_time_vec.data(), m_velocities_km_per_h.data(), m_velocities_km_per_h.size());
        ImPlot::EndPlot();
        collectStatistic();
    }
}

void GpxSpeedModifier::handlePlotChange()
{

    if (isDrawingMode()) {// if lmb and ctrl down
        handleCurveDrawing();
    }
    else
    {
        handlePointManipulation();
    }
}

bool GpxSpeedModifier::isDrawingMode()
{
    ImGuiIO& io = ImGui::GetIO();
    return io.MouseDown[0] and io.KeyCtrl;
}

void GpxSpeedModifier::handlePointManipulation()
{
    ImGuiIO& io = ImGui::GetIO();
    for (auto i = 0; i < m_velocities_km_per_h.size(); i++) {
        if (ImPlot::DragPoint(i, &m_time_vec[i], &m_velocities_km_per_h[i], ImVec4(0, 1, 0.96f, 1), 1.5f)) {
            m_velocities_km_per_h[i] = std::max(0.5, m_velocities_km_per_h[i]);
        }
        if (was_lmb_down and !io.MouseDown[0]) {
            was_lmb_down = false;
            updatePlot();
        }
        if (io.MouseDown[0]) {
            was_lmb_down = true;
        }
    }
}

void GpxSpeedModifier::handleCurveDrawing()
{
    const double MINIMUM_VELOCITY = 0.5;

    ImGuiIO& io = ImGui::GetIO();
    auto cur_pos = ImPlot::GetPlotMousePos();
    if (io.MouseDownDuration[0] == 0.0f) {
        last_mouse_pos.x = cur_pos.x;
        last_mouse_pos.y = cur_pos.y;
    }
    cur_pos.y = std::max(cur_pos.y, MINIMUM_VELOCITY);
    last_mouse_pos.y = std::max(last_mouse_pos.y, MINIMUM_VELOCITY);

    auto start = std::clamp(Tk::closest(m_time_vec, last_mouse_pos.x), m_time_vec.begin(), m_time_vec.end());
    auto end = std::clamp(Tk::closest(m_time_vec, cur_pos.x), m_time_vec.begin(), m_time_vec.end());;

    int start_idx = start - m_time_vec.begin();
    int end_idx = end - m_time_vec.begin();

    double& x1 = m_time_vec[start_idx];
    double& x2 = m_time_vec[end_idx];
    double& y1 = last_mouse_pos.y;
    double& y2 = cur_pos.y;
    int direction = (start_idx < end_idx) ? 1 : -1;
    for (int i = start_idx; i != end_idx + direction; i += direction) {
        auto y = Tk::line_interp(x1, x2, y1, y2, m_time_vec[i]);
        m_velocities_km_per_h[i] = y;
    }

    updatePlot();
    ImPlotPoint new_pos{ m_time_vec[end_idx],m_velocities_km_per_h[end_idx] };
    last_mouse_pos = new_pos;
}


std::vector<GpxPoint>& GpxSpeedModifier::getPoints()
{
    return m_points;
}