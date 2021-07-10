#include "Simulation.h"
#include "iostream"

#define INF ((unsigned)~0)

Simulation::Simulation(int window_width, int window_height,
                       bool fullscreen, bool dark_scheme)
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    if (fullscreen)
    {
        window.create(sf::VideoMode(desktop.width, desktop.height, desktop.bitsPerPixel), "TSP Simulation",
                      sf::Style::Fullscreen);
    }
    else
    {
        window.create(sf::VideoMode(window_width, window_height, desktop.bitsPerPixel), "TSP Simulation", sf::Style::Titlebar);
    }

    window.setFramerateLimit(FRAME_RATE);

    this->window_width = fullscreen ? desktop.width : window_width;
    this->window_height = fullscreen ? desktop.height : window_height;
    this->max_stops = max_stops;
    this->dark_scheme = dark_scheme;
}

Simulation::~Simulation() = default;

void Simulation::run()
{
    while (window.isOpen())
    {
        if (handle_input())
            break;
        render();
    }
}

void Simulation::add_stop(float x, float y)
{
    Stop b = Stop{x, y, index};
    sf::CircleShape shape(10);

    shape.setPosition(x, y);
    shape.setFillColor(index == 1 ? sf::Color::Green : sf::Color::Red);

    index++;
    Simulation::stops.push_back(b);
    shapes.emplace_back(shape);
    for (int i = 0; i < index - 1; i++)
    {
        sf::VertexArray line(sf::Lines, 2);
        line[0].position = sf::Vector2f(b.x + 10, b.y + 10);
        line[1].position = sf::Vector2f(Simulation::stops[i].x + 10, Simulation::stops[i].y + 10);
        line[0].color = dark_scheme ? sf::Color::White : sf::Color::Black;
        line[1].color = dark_scheme ? sf::Color::White : sf::Color::Black;
        // lines[index-2] = {};
        lines[index-2].resize(i+1);
        lines[index-2][i] = line;
    }
}

void Simulation::render()
{
    window.clear(dark_scheme ? sf::Color::Black : sf::Color::White);

    for (int i = 0; i < shapes.size(); ++i)
    {
        window.draw(shapes[i]);
    }
    for (int i = 0; i < texts.size(); i++)
    {
        window.draw(texts[i]);
    }
    for (auto const& x : lines)
    {
        for (sf::VertexArray line : x.second){
            window.draw(line);
        }
    }

    window.display();
}

bool Simulation::handle_input()
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
            return true;
        }
        if (event.mouseButton.button == sf::Mouse::Right && event.type == sf::Event::MouseButtonReleased)
        {
            sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
            add_stop(mouse_position.x, mouse_position.y);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        {
            stops.clear();
            shapes.clear();
            lines.clear();
            index = 1;
            run();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
        {
            nearest_neighbor();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            window.close();
            std::exit(0);
        }
    }
    return false;
}

void Simulation::nearest_neighbor(){
    int siz = stops.size();
    int graph[siz][siz];
    for (int i = 0; i < siz; i++)
    {
        for (int j = 0; j < siz; j++)
        {
            graph[i][j] = graph[j][i] = getEuclideanDistance(stops[i], stops[j]);
        }
    }
    std::vector<int> path;
    std::set<int> visited;
    path.push_back(0);
    visited.insert(0);
    for(int i = 0; i < siz; i++){
        int orig[siz];
        std::copy(graph[i], graph[i]+siz, orig);
        std::sort(graph[i], graph[i]+siz);
        int tmpidx = 0;
        while(visited.find(std::find(orig, orig+siz, graph[i][tmpidx]) - orig) != visited.end()){
            tmpidx++;
        }
        path.push_back(std::find(orig, orig+siz, graph[i][tmpidx]) - orig);
        visited.insert(std::find(orig, orig+siz, graph[i][tmpidx]) - orig);
    }
    path.back() = 0;
    for(int i=0; i<path.size()-1; i++){
        int j=i+1;
        if(path[j]>path[i]){
            lines[path[j]][path[i]][0].color = sf::Color::Green;
            lines[path[j]][path[i]][1].color = sf::Color::Green;
        }
        else{
            lines[path[i]][path[j]][0].color = sf::Color::Green;
            lines[path[i]][path[j]][1].color = sf::Color::Green;
        }
        std::cout << path[i] << " " << path[j] << "  ";
        
    }
    std::cout << std::endl;
}

int Simulation::getEuclideanDistance(Stop c1, Stop c2)
{
    int dx = pow((float)(c1.x - c2.x), 2);
    int dy = pow((float)(c1.y - c2.y), 2);
    return floor((float)(sqrt(dx + dy) + .5));
}