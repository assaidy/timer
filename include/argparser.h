#include <string>
#include <vector>
#include <regex>

#ifndef ARG_PARSER_H
#define ARG_PARSER_H

class Argparser {
private:
    std::string timer_type;
    long long total_seconds{};

    std::vector<std::string> split_str(std::string str) {
        std::vector<std::string> result;
        std::string token;
        for (size_t i = 0; i < str.size(); i++) {
            if (str.at(i) == ' ') {
                result.push_back(token);
                token.clear();
            }
            token += str.at(i);
        }
        result.push_back(token);
        return result;
    }

    bool extract_seconds(std::string token) {
        if (std::regex_match(token, std::regex("\\d+h \\d+m \\d+s"))) {
            auto str_vec{ split_str(token) };
            total_seconds += std::stoi(str_vec.at(0)) * 3600;
            total_seconds += std::stoi(str_vec.at(1)) * 60;
            total_seconds += std::stoi(str_vec.at(2));
        }
        else if (std::regex_match(token, std::regex("\\d+h")))
            total_seconds = std::stoi(token) * 3600;
        else if (std::regex_match(token, std::regex("\\d+m")))
            total_seconds = std::stoi(token) * 60;
        else if (std::regex_match(token, std::regex("\\d+s")))
            total_seconds = std::stoi(token);
        else
            return false;

        return true;
    }

public:
    bool parse_args(int argc, char const* argv[]) {
        if (argc == 2) {
            timer_type = "down";
            return extract_seconds(argv[1]);
        }
        else if (argc == 1) {
            timer_type = "up";
            return true;
        }
        else {
            return false;
        }
    }

    size_t get_toal_seconds() {
        return total_seconds;
    }

    std::string get_timer_type() {
        return timer_type;
    }
};

#endif // ARG_PARSER_H