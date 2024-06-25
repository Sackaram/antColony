

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cassert>
#include <vector>
#include <random>
#include <time.h>

// g++ -IC:/SFML/2.5.0/32bit/SFML-2.5.0/include -LC:/SFML/2.5.0/32bit/SFML-2.5.0/lib -o my_program main.cpp -lsfml-graphics -lsfml-window -lsfml-system


// while mouse button down = pause, and be able to drag nodes

constexpr int WINDOW_HEIGHT = 1600;
constexpr int WINDOW_WIDTH = 2400;
constexpr int NODE_RADIUS = 35;
constexpr int NR_OF_NODES = 7;
constexpr int WALL_PADDING = NODE_RADIUS * 2;
constexpr float PI = 3.14f;
constexpr int NODE_PADDING = NODE_RADIUS * 15;
constexpr int NR_OF_LINES = (NR_OF_NODES * (NR_OF_NODES - 1) / 2);
// nodes(nodes -1)
// ------------  = NR_OF_LINES
//      2

void drawNodes(sf::RenderWindow &window, std::vector<sf::CircleShape> &nodes)
{
    for (size_t i = 0; i < nodes.size(); i++)
    {
        window.draw(nodes[i]);
    }
}

void drawLines(sf::RenderWindow &window, std::vector<sf::CircleShape> &nodes, std::vector<sf::RectangleShape> &lines)
{

    for (size_t i = 0; i < nodes.size(); i++)
    {
        for (size_t j = i+1; j < nodes.size(); j++)
        {
            sf::Vector2f startPos(nodes[i].getPosition().x + NODE_RADIUS, nodes[i].getPosition().y + NODE_RADIUS);
            sf::Vector2f endPos(nodes[j].getPosition().x + NODE_RADIUS, nodes[j].getPosition().y + NODE_RADIUS);

            float length = std::hypot(endPos.x - startPos.x, endPos.y - startPos.y);
            float angle = std::atan2(endPos.y - startPos.y, endPos.x - startPos.x) * 180 / PI;

            lines[j].setPosition(startPos);
            lines[j].setSize(sf::Vector2f(length, 4.0f));
            lines[j].setRotation(angle);

            window.draw(lines[j]);
        }
    }
    
}

bool tooClose(std::vector<sf::CircleShape> &nodes, int x, int y)
{
    for (size_t i = 0; i < nodes.size(); i++)
    {
        float distance = std::sqrt(std::pow(nodes[i].getPosition().x - x, 2) + std::pow(nodes[i].getPosition().y - y, 2));
        if (distance < NODE_PADDING)
        {
            return true;
        }
    }

    return false;
}

int main()
{
    // assert(WINDOW_HEIGHT > 0 && WINDOWWIDTH > 0 && NR_OF_NODES > 0 && NODE_RADIUS > 0);
    std::random_device rd;
    std::mt19937 gen(std::time(nullptr));
    std::uniform_int_distribution<> xPos(NODE_RADIUS, WINDOW_WIDTH - NODE_RADIUS - WALL_PADDING);
    std::uniform_int_distribution<> yPos(NODE_RADIUS, WINDOW_HEIGHT - NODE_RADIUS - WALL_PADDING);

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "HELLO");

    std::vector<sf::CircleShape> nodes;
    for (size_t i = 0; i < NR_OF_NODES; i++)
    {
        sf::CircleShape node(NODE_RADIUS);
        int x = xPos(gen);
        int y = yPos(gen);
        while (tooClose(nodes, x, y))
        {
            x = xPos(gen);
            y = yPos(gen);
        }

        node.setPosition(x, y);
        if (i == 0)
        {
            node.setFillColor(sf::Color::Red);
        }
        node.setOutlineColor(sf::Color::Black);
        node.setOutlineThickness((int)(NODE_RADIUS / 6));
        node.setPointCount(600);
        nodes.push_back(node);
    }

    std::vector<sf::RectangleShape> lines;
    for (size_t i = 0; i < NR_OF_LINES; i++)
    {
        float thickness = 4.0f;
        sf::RectangleShape line(sf::Vector2f(1, thickness));
        line.setFillColor(sf::Color::Black);
        lines.push_back(line);
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        window.clear(sf::Color(34, 34, 34));
        // window.draw(lines[0]);
        drawLines(window, nodes, lines);
        drawNodes(window, nodes);
        window.display();
    }

    std::cout << "Hello there!" << std::endl;

    return 0;
}
