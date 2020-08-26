//
// Created by Dunja Lalic on 25/08/2020.
//

#include "interpolate.h"

double interpolate(double x, double x0, double x1, double y0, double y1) {
    return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}

#ifdef ENABLE_DOCTEST_IN_LIBRARY

#include "doctest.h"

TEST_CASE ("interpolate") {
    double result = interpolate(0.5, 0.0, 1.0, 0.0, 5.0);
    double expected = 2.5;
            CHECK(expected == doctest::Approx(result));
}

#endif
