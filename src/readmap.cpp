#include <iostream>
#include <tuple>
#include <vector>
#include <SFML/Graphics/Image.hpp>

using namespace std;

#define grassT  make_tuple(255, 255, 255)
#define wallT   make_tuple(0, 0, 0)
#define startT  make_tuple(168, 230, 29)
#define endT    make_tuple(237, 28, 36)

int getPixelType(sf::Color c) {

    int r, g, b;
    r = static_cast<int>(c.r);
    g = static_cast<int>(c.g);
    b = static_cast<int>(c.b);
    tuple<int, int, int> ct = make_tuple(r, g, b);

    if ( ct == grassT )
        return 1;
    else if ( ct == wallT ) 
        return 2;
    else if ( ct == startT ) 
        return 3;
    else if ( ct == endT ) 
        return 4;

    return -1;

}

bool visited[100][100];

int main() {

    
    const sf::Image image("map1.png");

    int w, h, mapw, maph;
    w = image.getSize().x;
    h = image.getSize().y;

    maph = h;
    mapw = w;

    sf::Color tmp;


    int count = 0;

    for ( int y = 0 ; y < h ; ++y ) {
        for ( int x = 0 ; x < w ; ++x ) {
            tmp = image.getPixel({x, y});

            if ( x == 43 && y == 26 ) {
                cout << "X";
                continue;
            }

            int type = getPixelType(tmp);
            if ( type == 1 ) {
                // grass
                cout << " ";
            } else if ( type == 2 ) {
                // wall
                cout << "w";
            } else if ( type == 3 ) {
                // start point
                cout << "S";
            } else if ( type == 4 ) {
                cout << "E";
            } else {
                cout << "x";
            }

        }
        cout << endl;
    }



    for ( unsigned int y = 0 ; y < maph ; ++y ) {
        for ( unsigned int x = 0 ; x < mapw ; ++x ) {

            if ( visited[y][x] ) {
                continue;
            }

            tmp = image.getPixel({x, y});

            int type = getPixelType(tmp);
            if ( type == 2 ) {
                // wall
                
                unsigned int wallx, wally, wallWidth, wallHeight;
                
                wallx = x;
                wally = y;

                for ( wallWidth = 0 ; x + wallWidth < mapw ; ++wallWidth ) {
                    if ( getPixelType(image.getPixel({x+wallWidth, y})) == 2 ) {
                        ;
                    } else {
                        break;
                    }
                }
                
                for ( wallHeight = 0 ; x + wallHeight < maph ; ++wallHeight ) {
                    if ( getPixelType(image.getPixel({x+wallWidth-1, y+wallHeight})) == 2 ) {
                        ;
                    } else {
                        break;
                    }
                }


                if ( wallWidth < 2 || wallHeight < 2 )
                    continue;
                printf("w: %d, %d, %d, %d\n", wallx, wally, wallWidth, wallHeight);

                ++count;

                // set visited

                for ( int _y = wally ; _y < wally + wallHeight ; ++_y )
                    for ( int _x = wallx ; _x < wallx + wallWidth ; ++_x )
                        visited[_y][_x] = true;
            }
        }
    }

    printf("number of walls: %d\n", ++count);





    return 0;
}
