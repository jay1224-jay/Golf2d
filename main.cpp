#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
using namespace std;
#define msg(x) cout << x << endl;
#define pi 3.1415
#define WIDTH  800
#define HEIGHT 600

double getDistance(double x1, double y1, double x2, double y2) {
    return sqrt( pow(x1-x2, 2) + pow(y1-y2, 2) );
}

class Ball {

    public:
        double x, y, dx, dy, velocity, a, r;
        sf::CircleShape entity;

        Ball(double radius) {
            entity.setRadius(radius);
            r = radius;
            dx = 0;
            dy = 0;
            a = 0;
            velocity = 0;

            entity.setFillColor(sf::Color::Green);
        }

        void setPosition(double _x, double _y) {
            x = _x;
            y = _y;
            entity.setPosition({_x, _y});
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
                msg("stop")
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

            if ( x+2*r >= WIDTH || x <= 0 ) {
                dx *= -1;
            }
            if ( y+2*r >= HEIGHT || y <= 0 ) {
                dy *= -1;
            }

        }

        void toward(int _x, int _y) {
            a = -0.05;
            velocity = 5;
            dx = (_x - x);
            dy = (_y - y);
            double tmp = sqrt(dx*dx + dy*dy);
            dx *= velocity/tmp;
            dy *= velocity/tmp;
        }

        void shoot(double mouse_x, double mouse_y) {
            a = -0.05;
            velocity = getDistance(mouse_x, mouse_y, x+r, y+r) / 20;
            dx = (x + r - mouse_x);
            dy = (y + r - mouse_y);
            double tmp = sqrt(dx*dx + dy*dy);
            dx *= velocity/tmp;
            dy *= velocity/tmp;
            // msg(velocity);
        }

};

class Arrow {

    public:
        double x, y, width, height;
        sf::RectangleShape entity;

        Arrow(int _thickness) {
            width = _thickness;
            entity.setFillColor(sf::Color::Blue);
        }

        void pointTo(double x1, double y1, double x2, double y2) {
            // (x1, y1) is head, (x2, y2) is tail
            double angle;
            angle = -1*atan((x1-x2)/(y1-y2));
            msg(angle * 180 /pi)
            height = getDistance(x1, y1, x2, y2) * 0.75;
            entity.setSize({width, height});
            entity.setPosition({(x1+x2)/2 + (height/2 * sin(angle)), (y1+y2)/2 - (height/2 * cos(angle))});
            entity.setRotation(sf::radians(angle));

        }

};

enum {clicked, not_clicked, aiming, idle};

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "My window");
    window.setFramerateLimit(60); // call it once after creating the window

    Ball b(20);
    b.setPosition(300, 200);
    b.velocity = 0;
    b.dx = 0;
    b.dy = 0;
    b.a = -0.05;


    Arrow aimLine(10);


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

        window.clear(sf::Color::Black);

        // cout << b.isMoving() << endl;
        if ( player_status == idle ) {
            if ( status == clicked && !b.isMoving() ) {
                player_status = aiming;
            }
        } else if ( player_status == aiming ) {
            if ( status == clicked ) {
                mx = sf::Mouse::getPosition(window).x;
                my = sf::Mouse::getPosition(window).y;
                aimLine.pointTo(b.getCenter().first, b.getCenter().second, mx, my);
            } else if ( status == not_clicked ) {
                // shoot the ball
                b.shoot(mx, my);
                player_status = idle;
            }
            window.draw(aimLine.entity);

        }
        // msg(b.y)
        // update
        b.update();

        // draw
        window.draw(b.entity);


        window.display();


    }
}
