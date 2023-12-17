#ifndef COUNTER_H
#define COUNTER_H

class Counter {
private:
    long long current{};
public:
    Counter() : current(0) {}
    Counter(long long start_point) {
        current = start_point;
    }
    void reset(long long point = 0) {
        current = point;
    }
    void inc() {
        ++current;
    }
    void dec() {
        --current;
    }
    long long cur() {
        return current;
    }
};

#endif
