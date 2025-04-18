#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include "common.h"
#define msg(x) cout << x << endl;
#define pi 3.1415
#define WIDTH  800
#define HEIGHT 600

using namespace std;


float getDistance(float x1, float y1, float x2, float y2) {
    return sqrt( pow(x1-x2, 2) + pow(y1-y2, 2) );
}

class Ball {

    public:
        float x, y, dx, dy, velocity, a, r;
        sf::CircleShape entity;

        Ball(float radius) {
            entity.setRadius(radius);
            r = radius;
            dx = 0;
            dy = 0;
            a = 0;
            velocity = 0;

            entity.setFillColor(sf::Color::Green);
        }

        void setPosition(sf::Vector2f point) {
            x = point.x;
            y = point.y;
            entity.setPosition({x, y});
        }

        void setColor(int code) {
            sf::Color c;
            switch (code) {

            case 1:
                c = sf::Color::Green;
                break;
            case 2:
                c = sf::Color::Blue;
                break;
            case 3:
                c = sf::Color::Red;
                break;
            case 4:
                c = sf::Color::White;
            }
            entity.setFillColor(c);
        }

        pair<int, int> getCenter() {
            return make_pair(x+r, y+r);
        }

        int isMoving() {
            return !(dx == 0 && dy == 0);
        }

        void update() {
            x += dx;
            y += dy;
            entity.setPosition({x, y});

            if ( abs(velocity) < abs(a)*2 ) {

                dx = 0;
                dy = 0;
                velocity = 0;
                a = 0;
            } else {
                if ( velocity ) {
                    dx *= (velocity + a)/velocity;
                    dy *= (velocity + a)/velocity;
                    velocity += a;
                }
            }

            if ( x+2*r + 1>= WIDTH || x <= 1 ) {
                dx *= -1;
            }
            if ( y+2*r + 1 >= HEIGHT || y <= 1 ) {
                dy *= -1;
            }

        }

        void toward(int _x, int _y) {
            a = -0.05;
            velocity = 5;
            dx = (_x - x);
            dy = (_y - y);
            float tmp = sqrt(dx*dx + dy*dy);
            dx *= velocity/tmp;
            dy *= velocity/tmp;
        }

        void shoot(float mouse_x, float mouse_y) {
            a = -0.05;
            velocity = min(getDistance(mouse_x, mouse_y, x+r, y+r) / 30, 20.f);
            dx = (x + r - mouse_x);
            dy = (y + r - mouse_y);
            float tmp = sqrt(dx*dx + dy*dy);
            dx *= velocity/tmp;
            dy *= velocity/tmp;
            // msg(velocity);
        }

};

class Arrow {

    public:
        float x, y, width, height;
        sf::RectangleShape entity;

        Arrow(int _thickness) {
            width = _thickness;
            entity.setFillColor(sf::Color::Blue);
        }

        void pointTo(float x1, float y1, float x2, float y2) {
            // (x1, y1) is head, (x2, y2) is tail
            float angle;
            angle = -1*atan((x1-x2)/(y1-y2));
            height = getDistance(x1, y1, x2, y2);
            float vx = x1-x2, vy = y1-y2;
            entity.setSize({width, height});
            entity.setPosition({static_cast<float>((x1+x2)/2 + (height/2 * sin(angle)) + ( width/2 * (1 ? vy < 0 : 1) * (vy) / sqrt(pow(vx, 2)+pow(vy, 2)))),
                    static_cast<float>((y1+y2)/2 - (height/2 * cos(angle)) + ( width/2 * (1 ? vy < 0 : 1) * (vx) / sqrt(pow(vx, 2)+pow(vy, 2))))});
            entity.setRotation(sf::radians(angle));

        }

};

class Hole {
    public:

        float radius;
        sf::CircleShape entity;

        Hole(float _radius) {
            radius = _radius;
            entity.setRadius(radius);
            entity.setFillColor(sf::Color::Black);
        }

        bool checkIn(Ball b) {
            float d = getDistance(b.x, b.y, entity.getPosition().x, entity.getPosition().y);

            if ( d <= radius ) {
                return 1;
            }

            return 0;
        }


};


enum {clicked, not_clicked, aiming, idle};

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "My window");
    window.setFramerateLimit(60);

    Ball b(10);
    b.velocity = 0;
    b.dx = 0;
    b.dy = 0;
    b.a = -0.0001;
    b.setColor(4); // white

    Hole hole(15);


    Arrow aimLine(5);
    aimLine.entity.setFillColor(sf::Color(250, 150, 100));


    // === Map Initialization ===


    const sf::Image img("map1.png");

    int mapw, maph;
    mapw = img.getSize().x;
    maph = img.getSize().y;

    float mapscale = WIDTH/mapw;

    sf::Vector2f StartPoint, HolePoint;

    sf::Color tmp;
    for ( unsigned int y = 0 ; y < maph ; ++y ) {
        for ( unsigned int x = 0 ; x < mapw ; ++x ) {
            tmp = img.getPixel({x, y});
            int type = getPixelType(tmp);
            if ( type == 3 ) {
                msg(x);
                msg(y);
                StartPoint = { x*mapscale, y*mapscale };
            } else if ( type == 4 ) {
                HolePoint = { x*mapscale, y*mapscale };
            }
        }
    }

    // 
    // use for another use
    b.setPosition(StartPoint);


    hole.entity.setPosition(HolePoint);

    vector<Wall> walls = mapInit(img); // define in map.cpp

    // === Map Initialization ===

    int strike_number = 0;
    sf::Font myfont("arial.ttf");
    sf::Text countText(myfont);
    countText.setCharacterSize(24);
    countText.setFillColor(sf::Color::White);


    int status = not_clicked, pre_status, player_status = idle;

    int mx, my;


    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (event->is<sf::Event::MouseButtonPressed>()) {
                status = clicked;
            }
            else if (event->is<sf::Event::MouseButtonReleased>())
                status = not_clicked;
        }

        window.clear(sf::Color(11, 112, 38));


        // condition
        if ( player_status == idle ) {
            if ( status == clicked && !b.isMoving() ) {
                player_status = aiming;
            }
        } else if ( player_status == aiming ) {
            if ( status == clicked ) {
                mx = sf::Mouse::getPosition(window).x;
                my = sf::Mouse::getPosition(window).y;
                aimLine.pointTo(2 * b.getCenter().first - mx, 2 * b.getCenter().second - my, b.getCenter().first, b.getCenter().second);
            } else if ( status == not_clicked ) {
                // shoot the ball
                b.shoot(mx, my);
                player_status = idle;
                ++strike_number;
            }
            window.draw(aimLine.entity);

        }

        // collision
        int collision_signal;
        for ( int i = 0 ; i < walls.size() ; ++i ) {
            if ( collision_signal = walls[i].CheckCollision(b.entity) ) {
                if ( collision_signal == 1 ) {
                    // change dy
                    b.dy *= -1;
                } else if ( collision_signal == 2 ) {
                    b.dx *= -1;
                }
                break;
            }
        }



        // update
        b.update();

        // check hole
        if ( hole.checkIn(b) ) {
            b.velocity = 0;
            b.entity.setPosition(StartPoint);
            strike_number = 0;
        }

        countText.setString("Strike number: " + to_string(strike_number));

        // draw
        window.draw(b.entity);
        window.draw(hole.entity);
        window.draw(countText);

        for ( int i = 0 ; i < walls.size(); ++i ) {
            window.draw(walls[i].entity);
        }


        window.display();


    }
}
