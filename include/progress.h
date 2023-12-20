#include <cmath>
#include <sstream>
#include <string>

#ifndef PROGRESS_H
#define PROGRESS_H

class Progress {
private:
    std::string shape_done{ "=" };
    std::string shape_remaining{ "-" };
    int width{ 100 };

    double calculate_progress(int total_tasks, int completed_tasks) {
        return static_cast<double>(completed_tasks) * width / total_tasks;
    }

public:
    // Note to avoid issues: shape_doen.size() should be equal to shape_remaining.size()
    Progress& set_shape_done(const std::string& shape) {
        shape_done = shape;
        return *this;
    }

    // Note to avoid issues: shape_doen.size() should be equal to shape_remaining.size()
    Progress& set_shape_remaining(const std::string& shape) {
        shape_remaining = shape;
        return *this;
    }

    int get_shape_size() {
        return shape_done.size();
    }

    Progress& set_width(size_t new_width) {
        width = new_width;
        return *this;
    }

    double calculate_percentage(int total_tasks, int completed_tasks) {
        return (static_cast<double>(completed_tasks) / total_tasks) * 100.0;
    }

    // NOTE: if using edges with unicode chars of size > 1, there might be issues centerng the bar
    std::string generate_bar(int total_tasks, int completed_tasks, bool with_edges = true) {
        std::stringstream oss;
        if (with_edges)
            oss << "[";
        int progress = calculate_progress(total_tasks, completed_tasks);
        for (int i = 0; i < progress; ++i) {
            oss << shape_done;
        }
        for (int i = progress; i < width; ++i) {
            oss << shape_remaining;
        }
        if (with_edges)
            oss << "]";
        return oss.str();
    }
};

#endif // PROGRESS_H