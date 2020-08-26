//
// Created by Dunja Lalic on 25/08/2020.
//

#pragma once

#include "warpmarker.h"

#include <optional>
#include <string>
#include <vector>

void process(std::vector<WarpMarker> &markers, double end_tempo);

std::optional<double> process_line(const std::string &line, std::vector<WarpMarker> &markers, double end_tempo);
