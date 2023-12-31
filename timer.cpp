#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <curses.h>
#include <ncurses.h>
#include <thread>
#include <vector>
#include <regex>
#include <iomanip>

#include "./include/counter.h"
#include "./include/progress.h"
#include "./include/argparser.h"

class Timer {
private:
    Counter our_counter;
    Progress pBar;
    Argparser argp;
    long long total_seonds;
    int argc;
    char const** argv;
    bool is_running{ true };
    bool is_paused{ false };

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

    std::string total_seconds_to_full(long long _total_seconds) {
        long long hours{ _total_seconds / 3600 };
        _total_seconds %= 3600;
        long long minutes{ _total_seconds / 60 };
        _total_seconds %= 60;
        long long seconds{ _total_seconds };

        std::stringstream oss;
        oss << std::setfill('0') << std::setw(2) << hours << ":"
            << std::setfill('0') << std::setw(2) << minutes << ":"
            << std::setfill('0') << std::setw(2) << seconds;

        return oss.str();
    }

    void print_centered(WINDOW* win, int y_displacement, const std::string& text, int char_size = 1, int style = A_NORMAL) {
        int center_col = win->_maxx / 2;
        int half_length = (int)text.size() / char_size / 2;
        int adjust_col = center_col - half_length;
        int center_row = win->_maxy / 2 + y_displacement;

        mvwprintw(win, center_row, adjust_col, "%s", text.c_str());
        wattron(win, style);
        mvwprintw(win, center_row, adjust_col, "%s", text.c_str());
        wattroff(win, style);

        while (is_paused) {
        }
    }

    void toggle_pause() {
        is_paused = !is_paused;
    }

    void quit() {
        endwin();
        exit(0);
    }

    void listen_to_keyboard() {
        cbreak();
        char ch;
        while (true) {
            ch = getch();
            refresh();
            switch (ch) {
            case ' ': toggle_pause(); break;
            case 'q': quit(); break;
            }
        }
    }

public:
    Timer(int argc, char const** argv) : argc(argc), argv(argv) {
        pBar.set_width(50).set_shape_done("█").set_shape_remaining("░");
    }

    void start() {
        if (!argp.parse_args(argc, argv)) {
            printf("[Error] invalid argument.\n\n");
            help_msg();
        }
        else {
            total_seonds = argp.get_toal_seconds();

            std::thread t(&Timer::listen_to_keyboard, this);
            t.detach();

            initscr();
            noecho();
            curs_set(0);
            refresh();

            if (argp.get_timer_type() == "up") {
                while (true) {
                    print_centered(stdscr, 0, total_seconds_to_full(our_counter.cur()));
                    refresh();
                    our_counter.inc();
                    std::this_thread::sleep_for(std::chrono::seconds(1));

                    clear();
                }
            }
            else if (argp.get_timer_type() == "down") {
                our_counter.reset(total_seonds - 1);
                while (our_counter.cur() >= 0) {
                    print_centered(stdscr, -2, total_seconds_to_full(our_counter.cur()), 1, A_BOLD);
                    print_centered(stdscr, 0, pBar.generate_bar(total_seonds, total_seonds - our_counter.cur(), false), pBar.get_shape_size());
                    print_centered(stdscr, 2, "session: " + total_seconds_to_full(argp.get_toal_seconds()), 1, A_ITALIC + A_DIM);
                    refresh();
                    our_counter.dec();
                    std::this_thread::sleep_for(std::chrono::seconds(1));

                    clear();
                }

                play_bell();
            }
        }
        endwin();
    }
};

int main(int argc, char const** argv)
{
    setlocale(LC_CTYPE, "");
    Timer app(argc, argv);
    app.start();

    return 0;
}
