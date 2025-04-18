#include <iostream>
#include <tuple>
#include <vector>
#include <SFML/Graphics.hpp>

#include "common.h"

using namespace std;


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
vector<Wall> mapInit(sf::Image img) {

    vector<Wall> walls;

    int mapw, maph;
    mapw = img.getSize().x;
    maph = img.getSize().y;

    int rscale = WIDTH / mapw;

    sf::Color tmp;
    for ( unsigned int y = 0 ; y < maph ; ++y ) {
        for ( unsigned int x = 0 ; x < mapw ; ++x ) {

            if ( visited[y][x] ) {
                continue;
            }

            tmp = img.getPixel({x, y});

            int type = getPixelType(tmp);
            if ( type == 2 ) {
                // wall
                
                unsigned int wallx, wally, wallWidth, wallHeight;
                
                wallx = x;
                wally = y;

                for ( wallWidth = 0 ; x + wallWidth < mapw ; ++wallWidth ) {
                    if ( getPixelType(img.getPixel({x+wallWidth, y})) == 2 ) {
                        ;
                    } else {
                        break;
                    }
                }
                
                for ( wallHeight = 0 ; x + wallHeight < maph ; ++wallHeight ) {
                    if ( getPixelType(img.getPixel({x+wallWidth-1, y+wallHeight})) == 2 ) {
                        ;
                    } else {
                        break;
                    }
                }

                printf("w: %d, %d, %d, %d\n", wallx, wally, wallWidth, wallHeight);

                if ( wallWidth < 2 || wallHeight < 2 )
                    continue;

                Wall tmp_wall(wallx*rscale, wally*rscale, wallWidth*rscale, wallHeight*rscale);
                walls.push_back(tmp_wall);

                // set visited

                for ( int _y = wally ; _y < wally + wallHeight ; ++_y )
                    for ( int _x = wallx ; _x < wallx + wallWidth ; ++_x )
                        visited[_y][_x] = true;
            }
        }
    }

    printf("number of walls: %ld\n", walls.size());

    return walls;
}
