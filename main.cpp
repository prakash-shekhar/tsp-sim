#include <string>
#include <iostream>
#include "Simulation.h"
#include "cxxopts.hpp"

int main(int argc, char **argv) {
    cxxopts::Options options("tsp-sim", "Runs a 2D simulation of the Travelling Salesman Problem");

    options.add_options()
            ("width", "Width of the window.",
             cxxopts::value<int>()->default_value(std::to_string(Simulation::DEFAULT_WINDOW_WIDTH)))
            ("height", "Height of the window.",
             cxxopts::value<int>()->default_value(std::to_string(Simulation::DEFAULT_WINDOW_HEIGHT)))
            ("fullscreen", "Runs the simulation in a fullscreen window.")
            ("dark_scheme", "Uses a dark color scheme.")
            ("help", "Displays this help message.");

    auto result = options.parse(argc, argv);

    if (result["help"].as<bool>()) {
        std::cout << options.help() << std::endl;
        return EXIT_SUCCESS;
    }

    Simulation simulation(result["width"].as<int>(), result["height"].as<int>(), 
    result["fullscreen"].as<bool>(), result["dark_scheme"].as<bool>());
    simulation.run();

    return EXIT_SUCCESS;
}
