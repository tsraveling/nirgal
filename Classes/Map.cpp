//
//  Map.cpp
//  nirgal-mobile
//
//  Created by Timothy Raveling on 12/13/17.
//

#include "Map.h"
#include "FractalNoise.h"

#pragma mark - AI functions

bool Map::isPassable(int x, int y, RoutingDirection direction) {
    
    // Check for walls
    int dx, dy;
    switch (direction) {
        case routingUp:
            if (this->wallGrid[x * 2][(y * 2) + 1] != empty)
                return false;
            dx = x;
            dy = y + 1;
            break;
        case routingRight:
            if (this->wallGrid[(x * 2) + 1][y * 2] != empty)
                return false;
            dx = x + 1;
            dy = y;
            break;
        case routingDown:
            if (this->wallGrid[x * 2][(y * 2) - 1] != empty)
                return false;
            dx = x;
            dy = y - 1;
            break;
        case routingLeft:
            if (this->wallGrid[(x * 2) - 1][y * 2] != empty)
                return false;
            dx = x - 1;
            dy = y;
            break;
    }
    
    if (dx < 0 || dy < 0 || dx >= MAP_XS || dy >= MAP_YS)
        return false;
    
    if (this->grid[dx][dy] == solidRock)
        return false;
    
    // TODO: Check for other astronauts, objects, etc.
    
    return true;
}

vector<MapCoord> Map::getNeighbors(int x, int y) {
    
    vector <MapCoord> ret;
    
    if (this->isPassable(x, y, routingUp))
        ret.push_back({.x = x, .y = y + 1});
    
    if (this->isPassable(x, y, routingRight))
        ret.push_back({.x = x + 1, .y = y});
    
    if (this->isPassable(x, y, routingDown))
        ret.push_back({.x = x, .y = y - 1});
    
    if (this->isPassable(x, y, routingLeft))
        ret.push_back({.x = x - 1, .y = y});
    
    return ret;
}

AstronautRoute *Map::computeRoute(Astronaut *astronaut, int dx, int dy) {
    
    printf("Going to %d, %d from %d, %d\n", dx, dy, astronaut->x, astronaut->y);
    
    // Get the start location
    MapCoord start = { .x = astronaut->x, .y = astronaut->y };
    
    // Set up the frontier
    vector<MapCoord> frontier;
    frontier.push_back(start);
    
    // Set up the "came from" array. The start point "comes from" itself, aka it's the end.
    map<MapCoord, MapCoord> came_from;
    came_from[start] = start;
    
    // If there's no path, there's no path
    bool found_path = false;
    
    // Now process the frontier recursively
    while (frontier.size() > 0) {
        
        // Get the first item in the frontier
        auto current = frontier.front();
        
        // printf("Evaluating point: %d, %d\n", current.x, current.y);
        
        // Early exit; we don't have to keep calculating once we've reached the destination point
        if (current.x == dx && current.y == dy) {
            found_path = true;
            break;
        }
        
        // Get the neighbors
        auto neighbors = this->getNeighbors(current.x, current.y);
        
        // Iterate through the neighbors
        for (MapCoord neighbor : neighbors) {
            
            // printf(" - neighbor (%d, %d): ",neighbor.x, neighbor.y);
            
            // Check to see if came_from has this item
            if (came_from.count(neighbor) == 0) {
                
                // printf("new");
                
                // If it's not in came_from, then add it to the frontier
                frontier.push_back(neighbor);
                
                // Set the current path
                came_from[neighbor] = current;
            } else {
                // printf("previously added");
            }
            // printf("\n");
        }
        
        // Remove the current item
        frontier.erase(frontier.begin());
    }
    
    // If we didn't find a path, return nil
    if (!found_path) {
        printf("No path to destination\n");
        return NULL;
    }
    
    printf("We've calculated the area and found the destination. Now computing path.\n");
    
    // Set up the route we'll return
    AstronautRoute *route = new AstronautRoute();
    
    // Now, generate the path. We start with the destination and work backwards.
    auto current = MapCoord { .x = dx, .y = dy };
    
    while (current != start) {
        
        // Get the next item in line
        auto next = came_from[current];
        
        // Figure out the direction
        RoutingDirection dir = routingUp;
        if (current.x > next.x)
            dir = routingRight;
        else if (current.x < next.x)
            dir = routingLeft;
        else if (current.y < next.y)
            dir = routingDown;
        
        // Add the path step
        route->steps.push_back(dir);
        
        // Step toward origin
        current = next;
    }
    
    // Reverse the step array
    std::reverse(route->steps.begin(), route->steps.end());
    
    // Just for the hell of it let's print this shit to console
    for (auto step : route->steps) {
        switch(step) {
            case routingUp:
                printf("up");
                break;
            case routingRight:
                printf("right");
                break;
            case routingDown:
                printf("down");
                break;
            case routingLeft:
                printf("left");
                break;
        }
        
        printf(" -> ");
    }
    printf("end\n");
    
    // Return the route for us to use
    return route;
}

#pragma mark - Game logic

void Map::tick(float time) {
    
}

#pragma mark - Utility and initialization

void Map::setTile(int x, int y, TerrainType newTile) {
    
    // Update the tile in question to the new terrain, as long as it's different
    if (this->grid[x][y] != newTile) {
        
        // Set the tile
        this->grid[x][y] = newTile;
    }
}

Map::Map() {
    
    // Randomize the RNG
    srand(time(NULL));
    
    // Mars dirt
    for (int x=0;x<MAP_XS;x++) {
        for (int y=0;y<MAP_YS;y++) {
            this->grid[x][y] = marsDirt;
        }
    }
    
    // Constants affecting the results
    float z_value = 0.72f;
    float chop_threshold = 0.5f;
    
    // Now blob in mars walls! Start with the noisemaker
    FractalNoise *noisemaker = new FractalNoise();
    float width_interval = 1.0f / float(MAP_XS);
    float height_interval = 1.0f / float(MAP_YS);
    float min = 0.0f;
    float max = 0.0f;
    
    // Set up the grid we can use to generate perlin noise on
    float noise_grid[MAP_XS][MAP_YS];
    
    int fallback_start_x = MAP_XS / 2;
    int fallback_start_y = MAP_YS / 2;
    
    // Generate the noise
    for (int x=0; x<MAP_XS; x++) {
        for (int y=0; y< MAP_YS; y++) {
            float noise = noisemaker->noise(float(x) * width_interval, float(y) * height_interval, z_value);
            
            // Set the noise grid
            noise_grid[x][y] = noise;
            
            // Keep track of the range so we can chop it off in the middle
            if (noise < min) min = noise;
            if (noise > max) {
                max = noise;
                
                fallback_start_x = x;
                fallback_start_y = y;
                
                if (x < MAP_XS - 10 && y < MAP_YS - 10 && x > 10 && y > 10) {
                    startX = x;
                    startY = y;
                }
            }
        }
    }
    
    // Use the fallback if the map has failed to generate an otherwise useful space
    if (startX == 0 && startY == 0) {
        startX = fallback_start_x;
        startY = fallback_start_y;
    }
    
    // Figure out the intermediate variables
    float diff = max - min;
    float threshold = min + (diff * chop_threshold);
    
    // Now generate the final map
    for (int x=0; x<MAP_XS; x++) {
        for (int y=0; y<MAP_YS; y++) {
            
            if (noise_grid[x][y] < threshold) {
                this->grid[x][y] = solidRock;
            }
        }
    }
    
    // This template will be used for the lander
    int lander_x = startX - 4;
    int lander_y = startY - 4;
    
    // TODO: remove this pathing debug bullshit
    
    string insert_block[8] = {"00000000",
        "00110100",
        "01110110",
        "01110111",
        "01110110",
        "01110010",
        "00111100",
        "00011000"};
    
    /*
    string insert_block[8] = {"00000000",
                              "00111100",
                              "01111110",
                              "01111110",
                              "01111110",
                              "01111110",
                              "00111100",
                              "00000000"};
     */
    
    // Modify the terrain and add the wall
    for (int x=0; x<8; x++) {
        for (int y=0; y<8; y++) {
            auto string = insert_block[y];
            auto val = string[x];
            this->grid[lander_x + x][lander_y + y] = (val == '0') ? marsDirt : metalFloor;
            
            if (x < 7 && val != string[x + 1]) {
                this->wallGrid[((lander_x + x) * 2) + 1][(lander_y + y) * 2] = metalWall;
            }
            
            if (y < 7 && val != insert_block[y + 1][x]) {
                this->wallGrid[(lander_x + x) * 2][((lander_y + y) * 2) + 1] = metalWall;
            }
        }
    }
    
    // Now add the objects
    /*
    // Suitports along the top
    this->addObject(new MapObject(suitPort, lander_x + 3, lander_y + 7, orientNormal));
    this->addObject(new MapObject(suitPort, lander_x + 4, lander_y + 7, orientNormal));
    
    // Landing struts
    this->addObject(new MapObject(landingStrut, lander_x + 1, lander_y + 6, orient270));
    this->addObject(new MapObject(landingStrut, lander_x + 6, lander_y + 6, orientNormal));
    this->addObject(new MapObject(landingStrut, lander_x + 1, lander_y + 1, orient180));
    this->addObject(new MapObject(landingStrut, lander_x + 6, lander_y + 1, orient90));
    
    // Water and food and oxygen stations
    this->addObject(new MapObject(waterStation, lander_x + 2, lander_y + 6, orientNormal));
    this->addObject(new MapObject(smallFoodStation, lander_x + 5, lander_y + 6, orientNormal));
    this->addObject(new MapObject(atmosphereTank, lander_x + 1, lander_y + 5, orientNormal));
    
    // Crash couches
    this->addObject(new MapObject(crashCouch, lander_x + 2, lander_y + 4, orientNormal));
    this->addObject(new MapObject(crashCouch, lander_x + 3, lander_y + 5, orientNormal));
    this->addObject(new MapObject(crashCouch, lander_x + 4, lander_y + 5, orientNormal));
    this->addObject(new MapObject(crashCouch, lander_x + 5, lander_y + 4, orientNormal));
    
    // Computers
    this->addObject(new MapObject(landingComputer, lander_x + 1, lander_y + 2, orient270));
    this->addObject(new MapObject(landingComputer, lander_x + 3, lander_y + 1, orient180));
    this->addObject(new MapObject(landingComputer, lander_x + 5, lander_y + 1, orient180));
    this->addObject(new MapObject(landingComputer, lander_x + 6, lander_y + 3, orient90));
    
    // Crates
    this->addObject(new MapObject(landerCrate, lander_x + 0, lander_y + 2, orientNormal));
    this->addObject(new MapObject(landerCrate, lander_x + 0, lander_y + 3, orientNormal));
    this->addObject(new MapObject(landerCrate, lander_x + 0, lander_y + 4, orientNormal));
    this->addObject(new MapObject(landerCrate, lander_x + 0, lander_y + 5, orientNormal));
    
    // Solar Panels
    this->addObject(new MapObject(landerSolarPanel, lander_x + 2, lander_y + 0, orientNormal));
    this->addObject(new MapObject(landerSolarPanel, lander_x + 3, lander_y + 0, orientNormal));
    this->addObject(new MapObject(landerSolarPanel, lander_x + 4, lander_y + 0, orientNormal));
    this->addObject(new MapObject(landerSolarPanel, lander_x + 5, lander_y + 0, orientNormal));
    
    // Tanks
    this->addObject(new MapObject(landerLiquidTank, lander_x + 7, lander_y + 2, orientNormal));
    this->addObject(new MapObject(landerLiquidTank, lander_x + 7, lander_y + 3, orientNormal));
    this->addObject(new MapObject(landerLiquidTank, lander_x + 7, lander_y + 4, orientNormal));
    this->addObject(new MapObject(landerLiquidTank, lander_x + 7, lander_y + 5, orientNormal));
     */
    
    // Astronauts
    this->astronauts.push_back(new Astronaut("John", "astronaut_01", lander_x + 2, lander_y + 4));
    this->astronauts.push_back(new Astronaut("Hiroko", "astronaut_02", lander_x + 3, lander_y + 5));
    this->astronauts.push_back(new Astronaut("Sax", "astronaut_04", lander_x + 4, lander_y + 5));
    this->astronauts.push_back(new Astronaut("Ann", "astronaut_03", lander_x + 5, lander_y + 4));
}

void Map::addObject(MapObject *object) {
    this->objects.push_back(object);
}

void Map::removeObject(MapObject *object) {
}

void Map::regenerateTiles(int x1, int y1, int x2, int y2) {
    
    // This will be used to null overlay tiles
    string nope = "";
    
    for (int x = x1; x <= x2; x++) {
        for (int y = y1; y <= y2; y++) {
            
            // First we have to decide if an overlay blend is required
            bool blend = false;
            
            // If it's solid rock, it might be
            if (this->grid[x][y] == solidRock) {
                
                // What's around this tile?
                bool empty[3][3];
                for (int map_x=0; map_x<3; map_x++) {
                    for (int map_y=0; map_y<3; map_y++) {
                        
                        empty[map_x][map_y] = false;
                        
                        int real_x = (x - 1) + map_x;
                        int real_y = (y - 1) + map_y;
                        
                        if (real_x >=0 && real_y >= 0 && real_x < MAP_XS && real_y < MAP_YS) {
                            
                            if (this->grid[real_x][real_y] != solidRock) {
                                empty[map_x][map_y] = true;
                                blend = true;
                            }
                        }
                    }
                }
                
                // If anything is next to this tile other than more rock, it's a blend.
                if (blend) {
                    
                    string pattern = "";
                    pattern += empty[1][2] ? "0" : "1"; // Top
                    pattern += empty[2][1] ? "0" : "1"; // Right
                    pattern += empty[1][0] ? "0" : "1"; // Bottom
                    pattern += empty[0][1] ? "0" : "1"; // Left
                    
                    if (pattern == "0110") {
                        this->baseTile[x][y] = Map::frameForTile(this->grid[x - 1][y + 1]);
                        if (empty[2][0])
                            this->overlayTile[x][y] = "mars-wall-04.png";
                        else
                            this->overlayTile[x][y] = "mars-wall-01.png";
                    } else if (pattern == "0111") {
                        this->baseTile[x][y] = Map::frameForTile(this->grid[x][y + 1]);
                        
                        if (empty[0][0])
                            this->overlayTile[x][y] = "mars-wall-01.png";
                        else if (empty[2][0])
                            this->overlayTile[x][y] = "mars-wall-03.png";
                        else
                            this->overlayTile[x][y] = "mars-wall-02.png";
                        
                    } else if (pattern == "0011") {
                        this->baseTile[x][y] = Map::frameForTile(this->grid[x][y + 1]);
                        if (empty[0][0])
                            this->overlayTile[x][y] = "mars-wall-04.png";
                        else
                            this->overlayTile[x][y] = "mars-wall-03.png";
                    } else if (pattern == "1110") {
                        this->baseTile[x][y] = Map::frameForTile(this->grid[x - 1][y]);
                        
                        if (empty[2][2])
                            this->overlayTile[x][y] = "mars-wall-01.png";
                        else if (empty[2][0])
                            this->overlayTile[x][y] = "mars-wall-11.png";
                        else
                            this->overlayTile[x][y] = "mars-wall-06.png";
                    } else if (pattern == "1011") {
                        this->baseTile[x][y] = Map::frameForTile(this->grid[x + 1][y]);
                        
                        if (empty[0][2])
                            this->overlayTile[x][y] = "mars-wall-03.png";
                        else if (empty[0][0])
                            this->overlayTile[x][y] = "mars-wall-13.png";
                        else
                            this->overlayTile[x][y] = "mars-wall-08.png";
                    } else if (pattern == "1100") {
                        this->baseTile[x][y] = Map::frameForTile(this->grid[x - 1][y - 1]);
                        if (empty[2][2])
                            this->overlayTile[x][y] = "mars-wall-04.png";
                        else
                            this->overlayTile[x][y] = "mars-wall-11.png";
                    } else if (pattern == "1101") {
                        this->baseTile[x][y] = Map::frameForTile(this->grid[x][y - 1]);
                        
                        if (empty[0][2])
                            this->overlayTile[x][y] = "mars-wall-11.png";
                        else if (empty[2][2])
                            this->overlayTile[x][y] = "mars-wall-13.png";
                        else
                            this->overlayTile[x][y] = "mars-wall-12.png";
                    } else if (pattern == "1001") {
                        this->baseTile[x][y] = Map::frameForTile(this->grid[x + 1][y - 1]);
                        if (empty[0][2])
                            this->overlayTile[x][y] = "mars-wall-04.png";
                        else
                            this->overlayTile[x][y] = "mars-wall-13.png";
                    } else {
                        
                        bool pillar = false;
                        
                        string diag = "";
                        diag += empty[0][2] ? "0" : "1"; // Upper left
                        diag += empty[2][2] ? "0" : "1"; // Upper right
                        diag += empty[0][0] ? "0" : "1"; // Lower left
                        diag += empty[2][0] ? "0" : "1"; // Lower right
                        
                        if (pattern == "1111") {
                            
                            if (diag == "1110") {
                                this->baseTile[x][y] = Map::frameForTile(this->grid[x + 1][y - 1]);
                                this->overlayTile[x][y] = "mars-wall-09.png";
                            } else if (diag == "1101") {
                                this->baseTile[x][y] = Map::frameForTile(this->grid[x - 1][y - 1]);
                                this->overlayTile[x][y] = "mars-wall-10.png";
                            } else if (diag == "1011") {
                                this->baseTile[x][y] = Map::frameForTile(this->grid[x + 1][y + 1]);
                                this->overlayTile[x][y] = "mars-wall-14.png";
                            } else if (diag == "0111") {
                                this->baseTile[x][y] = Map::frameForTile(this->grid[x - 1][y + 1]);
                                this->overlayTile[x][y] = "mars-wall-15.png";
                            } else if (diag == "0011") {
                                this->baseTile[x][y] = Map::frameForTile(this->grid[x - 1][y + 1]);
                                this->overlayTile[x][y] = "mars-wall-02.png";
                            } else if (diag == "0101") {
                                this->baseTile[x][y] = Map::frameForTile(this->grid[x - 1][y + 1]);
                                this->overlayTile[x][y] = "mars-wall-06.png";
                            } else if (diag == "1010") {
                                this->baseTile[x][y] = Map::frameForTile(this->grid[x + 1][y - 1]);
                                this->overlayTile[x][y] = "mars-wall-08.png";
                            } else if (diag == "1100") {
                                this->baseTile[x][y] = Map::frameForTile(this->grid[x + 1][y - 1]);
                                this->overlayTile[x][y] = "mars-wall-12.png";
                            } else if (diag == "0001") {
                                this->baseTile[x][y] = Map::frameForTile(this->grid[x + 1][y - 1]);
                                this->overlayTile[x][y] = "mars-wall-01.png";
                            } else if (diag == "0010") {
                                this->baseTile[x][y] = Map::frameForTile(this->grid[x + 1][y - 1]);
                                this->overlayTile[x][y] = "mars-wall-03.png";
                            } else if (diag == "0100") {
                                this->baseTile[x][y] = Map::frameForTile(this->grid[x + 1][y - 1]);
                                this->overlayTile[x][y] = "mars-wall-11.png";
                            } else if (diag == "1000") {
                                this->baseTile[x][y] = Map::frameForTile(this->grid[x + 1][y + 1]);
                                this->overlayTile[x][y] = "mars-wall-13.png";
                            } else {
                                pillar = true;
                            }
                        } else {
                            pillar = true;
                        }
                        
                        if (pillar) {
                            
                            // We have to figure out which is the empty adjacent space
                            if (empty[0][0])
                                this->baseTile[x][y] = Map::frameForTile(this->grid[x - 1][y - 1]);
                            else if (empty[2][0])
                                this->baseTile[x][y] = Map::frameForTile(this->grid[x + 1][y - 1]);
                            else if (empty[0][2])
                                this->baseTile[x][y] = Map::frameForTile(this->grid[x - 1][y + 1]);
                            else
                                this->baseTile[x][y] = Map::frameForTile(this->grid[x + 1][y + 1]);
                            
                            this->overlayTile[x][y] = "mars-wall-04.png";
                        }
                    }
                 
                    if (this->baseTile[x][y] == "rock-interior.png") {
                        this->baseTile[x][y] = "ground_mars.png";
                    }
                }
            }
            
            // If it's not a blend, just use the default tile
            if (!blend) {
                this->baseTile[x][y] = Map::frameForTile(this->grid[x][y]);
                this->overlayTile[x][y] = nope;
            }
        }
        printf("\n\n");
    }
}

string Map::frameForTile(TerrainType tile) {
    
    if (tile == solidRock) {
        return "rock-interior.png";
    }
    
    if (tile == metalFloor) {
        return "ground_metal.png";
    }
    
    return "ground_mars.png";
}
