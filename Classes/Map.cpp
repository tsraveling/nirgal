//
//  Map.cpp
//  nirgal-mobile
//
//  Created by Timothy Raveling on 12/13/17.
//

#include "Map.h"
#include "FractalNoise.h"

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
    
    // Generate the noise
    for (int x=0; x<MAP_XS; x++) {
        for (int y=0; y< MAP_YS; y++) {
            float noise = noisemaker->noise(float(x) * width_interval, float(y) * height_interval, z_value);
            
            // Set the noise grid
            noise_grid[x][y] = noise;
            
            // Keep track of the range so we can chop it off in the middle
            if (noise < min) min = noise;
            if (noise > max) max = noise;
        }
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
    
    // This will eventually insert the lander
    string insert_block[4] = {"1100",
                              "1100",
                              "0001",
                              "0110"};
    for (int x=0; x<4; x++) {
        for (int y=0; y<4; y++) {
            auto string = insert_block[y];
            auto val = string[x];
            this->grid[x][y] = (val == '0') ? marsDirt : solidRock;
        }
    }
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
