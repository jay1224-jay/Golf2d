#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <string>

#include "common.h"

using namespace std;


Wall::Wall(float _x, float _y, float _width, float _height)
{
    x = _x;
    y = _y;
    width = _width;
    height = _height;

    entity = sf::RectangleShape({width, height});
    entity.setPosition({x, y});
    entity.setFillColor(sf::Color(153, 107, 0));
    entity.setOutlineColor(sf::Color::Black);
    entity.setOutlineThickness(2);
}

Wall::Wall(void)
{
    width = 100;
    height = 20;
    entity.setFillColor(sf::Color(153, 107, 0));
    entity.setOutlineColor(sf::Color::Black);
    entity.setOutlineThickness(2);
}

void Wall::update()
{
    entity.setPosition({x, y});
    entity.setSize({width, height});
}

void Wall::setXY(int _x, int _y)
{
    x = _x;
    y = _y;
    update();
}
void Wall::setWH(int _w, int _h)
{
    width = _w;
    height = _h;
    update();
}

int Wall::CheckCollision(sf::CircleShape ball)
{
    sf::Rect bbox = entity.getGlobalBounds();

    // ball
    int bx, by;
    int r = ball.getRadius() + 2 /*offset*/;
    bx = ball.getPosition().x+r;
    by = ball.getPosition().y+r;

    // top

    if (bbox.contains({bx, by-r}))
    {
        return 1; // change dy
    }

    // bottom
    if (bbox.contains({bx, by+r}))
    {
        return 1; // change dy
    }
    // left
    if (bbox.contains({bx-r, by}))
    {
        return 2; // change dx
    }
    // right
    if (bbox.contains({bx+r, by}))
    {
        return 2; // change dx
    }

    return 0;

}
