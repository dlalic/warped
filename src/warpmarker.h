//
// Created by Dunja Lalic on 25/08/2020.
//

#pragma once

struct WarpMarker {
    double beat_time, sample_time;

    WarpMarker(double b, double s) {
        beat_time = b;
        sample_time = s;
    }
};
