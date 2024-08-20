#ifndef INTERVAL_H
#define INTERVAL_H

class Interval {
    public:
        float min, max;
        
        Interval(): min(+infinity), max(-infinity) {}
        Interval(double min, double max) : min(min), max(max) {}

        float size() const {
            return max - min;
        }

        bool contains(float x) const {
            return min <= x && x <= max;
        }

        bool surrounds(float x) const {
            return min < x && x < max;
        }

        static const Interval empty, universe;
};

const Interval Interval::empty = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);

#endif