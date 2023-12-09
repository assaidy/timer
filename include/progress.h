#include <cmath>
#include <sstream>
#include <string>

#ifndef PROGRESS_H
#define PROGRESS_H

class Progress {
private:
    std::string shape_done {"="};
    std::string shape_remaining {"-"};
    int width {100};

    double calculate_progress(int total_tasks, int completed_tasks) {
        return static_cast<double>(completed_tasks) * width / total_tasks;
    }

public:
    Progress &set_shape_done(const std::string &shape) {
        shape_done = shape;
        return *this;
    }

    Progress &set_shape_remaining(const std::string &shape) {
        shape_remaining = shape;
        return *this;
    }

    Progress &set_width(size_t new_width) {
        width = new_width;
        return *this;
    }

    double calculate_percentage(int total_tasks, int completed_tasks) {
        return (static_cast<double>(completed_tasks) / total_tasks) * 100.0;
    }

    std::string generate_bar(int total_tasks, int completed_tasks) {
        std::stringstream oss;
        oss << "[";
        int progress = calculate_progress(total_tasks, completed_tasks);
        for (int i = 0; i < progress; ++i) {
            oss << shape_done;
        }
        for (int i = progress; i < width; ++i) {
            oss << shape_remaining;
        }
        oss << "]";
        return oss.str();
    }
};

#endif // PROGRESS_H