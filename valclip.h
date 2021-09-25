#ifndef VALCLIP_H
#define VALCLIP_H

#pragma once

#include <cmath>

template <typename T>
class valclip {
    T _value, _min, _max, _step;
    int _precision;

    valclip();

public:
    inline T   min      (void) const { return _min; }
    inline T   max      (void) const { return _max; }
    inline T   step     (void) const { return _step; }
    inline int precision(void) const { return _precision; }

    inline T get(void) const { return _value; }

    T set(const T & value) {
        if (value > _max) {
            _value = _max;
        } else if (value < _min) {
            _value = _min;
        } else {
            _value = value;
        }

        return _value;
    }

    valclip(const T & tdefault, const T & tmin, const T & tmax, int precision = 0) {
        /* TODO:
        static_assert(tmax > tmin, "Tmin >= Tmax");
        static_assert(tdefault >= tmin, "Tdefault < Tmin");
        static_assert(tdefault <= tmax, "Tdefault > Tmax");
        */

        _min = tmin;
        _max = tmax;

        _precision = precision;
        if (_precision < 0) { _precision = 0; }

        _step = pow(10, -_precision);

        set(tdefault);
    }

    inline operator T() const { return get(); }
    inline T operator =(const T & value) { return set(value); }
};

#endif
