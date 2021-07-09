#ifndef BOIDS_SIMULATION_H
#define BOIDS_SIMULATION_H

#include <vector>
#include <set>
#include <cmath>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

// #include "algorithm.h"


class Simulation {
private:

    sf::RenderWindow window;
    int window_width, window_height, max_stops;
    std::vector<sf::CircleShape> shapes;
    std::vector<std::vector<sf::VertexArray>> lines;
    int index = 1;
    bool dark_scheme;
    struct Stop
    {
        float x;
        float y;
        int index;
    };
    std::vector<Stop> stops;

    void add_stop(float x, float y);

    void render();

    bool handle_input();

    int getEuclideanDistance(Stop c1, Stop c2);

    void nearest_neighbor();



public:
    constexpr static int FRAME_RATE = 60;
    constexpr static int DEFAULT_WINDOW_WIDTH = 900;
    constexpr static int DEFAULT_WINDOW_HEIGHT = 500;

    Simulation(int window_width = DEFAULT_WINDOW_WIDTH, int window_height = DEFAULT_WINDOW_HEIGHT, bool fullscreen = false, bool dark_scheme = false);
    
    ~Simulation();

    void run();

};

#endif