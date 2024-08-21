#ifndef INTERVAL_H
#define INTERVAL_H

class Interval {
    public:
        float min, max;
        
        Interval(): min(+infinity), max(-infinity) {}
        Interval(float min, float max) : min(min), max(max) {}

        float size() const {
            return max - min;
        }

        bool contains(float x) const {
            return min <= x && x <= max;
        }

        bool surrounds(float x) const {
            return min < x && x < max;
        }

        float clamp(float x) const {
            if (x < min) return min;
            if (x > max) return max;
            return x;
        }

        static const Interval empty, universe;
};

const Interval Interval::empty = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);

#endif