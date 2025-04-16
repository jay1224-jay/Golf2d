// common.h
#ifndef COMMON_H
#define COMMON_H

#define msg(x) cout << x << endl;
#define pi 3.1415
#define WIDTH  800
#define HEIGHT 600

class Wall {

public:

    float x, y, width, height;
    sf::RectangleShape entity;

    Wall(float _x, float _y, float _width, float _height);
    Wall(void);

    void update(void);

    void setXY(int _x, int _y);
    void setWH(int _w, int _h);
    int CheckCollision(sf::CircleShape ball);

};


#endif // COMMON_H
