#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <ncurses.h>
#include <thread>
#include <vector>
#include <regex>

#include "./include/Counter-API/counter.h"
#include "./include/progress-bar/progress.h"
#include "./include/argparser/argparser.h"

// TODO: convert total_seconds to "h:m:s"

class Timer {
private:
    Counter our_counter;
    Progress pBar;
    Argparser argp;
    long long total_seonds;
    int argc;
    char const** argv;

    void play_bell() {
        sf::Music music;
        std::string home_dir = getenv("HOME");
        std::string file_path = home_dir + "/.local/share/timer/bell.wav";
        if (!music.openFromFile(file_path))
            throw std::invalid_argument("Cant't open file"); // error
        music.play();
        sf::sleep(sf::seconds(music.getDuration().asSeconds()));
    }

    void help_msg() {
        printf("$ timer\n");
        printf("$ timer [DURATION]\n");
        printf("\tDURATION: '$h $m $s'\n");
        printf("\tDURATION: $h\n");
        printf("\tDURATION: $m\n");
        printf("\tDURATION: $s\n");
    }

public:
    Timer(int argc, char const** argv) : argc(argc), argv(argv) {
        // pBar.set_width(50).set_shape_done("█").set_shape_remaining("-");
        pBar.set_width(50).set_shape_done("█").set_shape_remaining(" ");
    }

    void start() {
        if (!argp.parse_args(argc, argv)) {
            help_msg();
        }
        else {
            total_seonds = argp.get_toal_seconds();

            setlocale(LC_ALL, "");
            initscr();
            noecho();
            curs_set(0);
            refresh();

            if (argp.get_timer_type() == "up") {
                while (true) {
                    // printf("%lld\n", our_counter.cur());
                    printw("%lld", our_counter.cur());
                    refresh();
                    our_counter.inc();
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    clear();
                }
            }
            else if (argp.get_timer_type() == "down") {
                our_counter.reset(total_seonds - 1);
                while (our_counter.cur() >= 0) {
                    // printf("%lld\n", our_counter.cur());
                    printw("%lld\n", our_counter.cur());
                    printw("%s %.2f%%",
                        pBar.generate_bar(total_seonds, total_seonds - our_counter.cur()).c_str(),
                        pBar.calculate_percentage(total_seonds, total_seonds - our_counter.cur())
                    );
                    refresh();
                    our_counter.dec();
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    clear();
                }
                play_bell();
            }
            endwin();
        }
    }
};

int main(int argc, char const* argv[])
{
    Timer app(argc, argv);
    app.start();

    return 0;
}
