//
// Created by Dunja Lalic on 25/08/2020.
//

#ifdef ENABLE_DOCTEST_IN_LIBRARY
#define DOCTEST_CONFIG_IMPLEMENT

#include "doctest.h"

#endif

#include "../src/warpmarker.h"
#include "../src/process.h"

int main() {
    std::vector<WarpMarker> markers;
    double end_tempo = 0.0;
    process(markers, end_tempo);
}
