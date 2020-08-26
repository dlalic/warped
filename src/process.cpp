//
// Created by Dunja Lalic on 25/08/2020.
//

#include "interpolate.h"
#include "process.h"

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <random>
#include <algorithm>

enum Input {
    marker, end_tempo, s2b, b2s
};

static double beat_to_sample(std::vector<WarpMarker> &markers, double value) {
    std::sort(markers.begin(), markers.end(), [](WarpMarker a, WarpMarker b) {
        return a.beat_time < b.beat_time;
    });
    for (std::size_t i = 0; i < markers.size() - 1; ++i) {
        if (value > markers[i].beat_time && value < markers[i + 1].beat_time) {
            WarpMarker first = markers[i];
            WarpMarker second = markers[i + 1];
            return interpolate(value, first.beat_time, second.beat_time, first.sample_time, second.sample_time);
        }
    }
    return 0.0;
}

static double sample_to_beat(std::vector<WarpMarker> &markers, double value, double end_tempo) {
    std::sort(markers.begin(), markers.end(), [](WarpMarker a, WarpMarker b) {
        return a.sample_time < b.sample_time;
    });
    for (std::size_t i = 0; i < markers.size() - 1; ++i) {
        if (value > markers[i].sample_time && value < markers[i + 1].sample_time) {
            WarpMarker first = markers[i];
            WarpMarker second = markers[i + 1];
            return interpolate(value, first.sample_time, second.sample_time, first.beat_time, second.beat_time);
        }
    }
    WarpMarker second = markers[markers.size() - 1];
    double seconds = value - second.sample_time;
    return second.beat_time + seconds * end_tempo;
}

/*static void insert_sorted(std::vector<WarpMarker> &markers, const WarpMarker &marker) {
    auto it = upper_bound(markers.begin(), markers.end(), marker, [](auto &lhs, auto &rhs) {
        return lhs.sample_time < rhs.sample_time;
    });
    markers.insert(it, marker);
};

static std::tuple<WarpMarker, WarpMarker> closest(std::vector<WarpMarker> &markers, double value) {
    auto const lower = std::lower_bound(markers.begin(), markers.end(), value,
                                        [](WarpMarker lhs, WarpMarker rhs) -> bool {
                                            return lhs.beat_time < rhs.beat_time;
                                        });
    auto const upper = std::upper_bound(markers.begin(), markers.end(), value,
                                        [](WarpMarker lhs, WarpMarker rhs) -> bool {
                                            return lhs.beat_time < rhs.beat_time;
                                        });
    return std::make_tuple(*lower, *upper);
}*/

std::optional<double> process_line(const std::string &line, std::vector<WarpMarker> &markers, double end_tempo) {
    std::unordered_map<std::string, int> input_map = {
            {"marker",    Input::marker},
            {"end_tempo", Input::end_tempo},
            {"b2s",       Input::b2s},
            {"s2b",       Input::s2b},
    };

    std::string buffer;
    std::stringstream string_stream(line);
    std::vector<std::string> tokens;

    while (string_stream >> buffer)
        tokens.push_back(buffer);

    switch (input_map.at(tokens[0])) {
        case Input::marker: {
            double beat_time = stod(tokens[1]);
            double sample_time = stod(tokens[2]);
            markers.emplace_back(beat_time, sample_time);
            break;
        }
        case Input::end_tempo:
            return stod(tokens[1]);
        case Input::b2s: {
            double b2s_value = stod(tokens[1]);
            double b2s = beat_to_sample(markers, b2s_value);
            return b2s;
        }
        case Input::s2b: {
            double s2b_value = stod(tokens[1]);
            double s2b = sample_to_beat(markers, s2b_value, end_tempo);
            return s2b;
        }
        default:
            break;
    }
    return {};
}

void process(std::vector<WarpMarker> &markers, double end_tempo) {
    for (std::string line; std::cout << "" && std::getline(std::cin, line);) {
        if (!line.empty()) {
            std::optional<double> output = process_line(line, markers, end_tempo);
            if (output) {
                std::cout << *output << '\n';
            }
        }
    }
}

#ifdef ENABLE_DOCTEST_IN_LIBRARY

#include "doctest.h"

static double random_double() {
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_real_distribution<> dis(0.0, 100.0);
    return dis(gen);
}

TEST_CASE ("beat to sample and back") {
    std::vector<WarpMarker> markers;
    markers.emplace_back(0.0, 0.0);
    markers.emplace_back(1.0, 5.0);
    double expected = 0.5;
    double sample = beat_to_sample(markers, expected);
    double beat = sample_to_beat(markers, sample, 0.0);
            CHECK(expected == doctest::Approx(beat));
}

TEST_CASE ("process line: marker with random floats") {
    std::vector<WarpMarker> markers;
    auto value1 = random_double();
    auto value2 = random_double();
    auto line = "marker " + std::to_string(value1) + " " + std::to_string(value2);
    std::optional<double> output = process_line(line, markers, 0.0);
            CHECK_EQ(markers.size(), 1);
            CHECK(markers[0].beat_time == doctest::Approx(value1));
            CHECK(markers[0].sample_time == doctest::Approx(value2));
            CHECK_EQ(output, std::nullopt);
}

TEST_CASE ("process line: marker invalid") {
    std::vector<WarpMarker> markers;
            CHECK_THROWS(process_line("marker foo bar", markers, 0.0));
}

TEST_CASE ("process line: end_tempo with random value") {
    std::vector<WarpMarker> markers;
    auto value = random_double();
    auto line = "end_tempo " + std::to_string(value);
    std::optional<double> output = process_line(line, markers, 0.0);
            CHECK_EQ(markers.size(), 0);
            CHECK(*output == doctest::Approx(value));
}

TEST_CASE ("process line: end_tempo invalid") {
    std::vector<WarpMarker> markers;
            CHECK_THROWS(process_line("end_tempo foo", markers, 0.0));
}

TEST_CASE ("process line: b2s with predefined value") {
    std::vector<WarpMarker> markers;
    markers.emplace_back(0.0, 0.0);
    markers.emplace_back(1.0, 5.0);
    double expected = 2.5;
    std::optional<double> output = process_line("b2s 0.5", markers, 0.0);
            CHECK_EQ(markers.size(), 2);
            CHECK(expected == doctest::Approx(*output));
}

TEST_CASE ("process line: b2s invalid") {
    std::vector<WarpMarker> markers;
            CHECK_THROWS(process_line("b2s foo", markers, 0.0));
}

TEST_CASE ("process line: s2b with predefined value") {
    std::vector<WarpMarker> markers;
    markers.emplace_back(0.0, 0.0);
    markers.emplace_back(1.0, 5.0);
    double expected = 11.0;
    std::optional<double> output = process_line("s2b 6.0", markers, 10.0);
            CHECK_EQ(markers.size(), 2);
            CHECK(output == doctest::Approx(expected));
}

TEST_CASE ("process line: s2b invalid") {
    std::vector<WarpMarker> markers;
            CHECK_THROWS(process_line("b2s foo", markers, 0.0));
}

TEST_CASE ("process input") {
    std::ostringstream local_input;
    std::ostringstream local_output;
    auto cin_buff = std::cin.rdbuf();
    auto cout_buff = std::cout.rdbuf();
    std::cin.rdbuf(local_input.rdbuf());
    std::cout.rdbuf(local_output.rdbuf());

    std::istringstream input("b2s 0.5");
    std::cin.rdbuf(input.rdbuf());

    std::vector<WarpMarker> markers;
    markers.emplace_back(0.0, 0.0);
    markers.emplace_back(1.0, 5.0);
    process(markers, 0.0);

    std::cin.rdbuf(cin_buff);
    std::cout.rdbuf(cout_buff);

            CHECK(local_output.str() == "2.5\n");
}

#endif
