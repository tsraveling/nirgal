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
    
}

string Map::floorFrameForTile(int x, int y) {
    return Map::frameForTile(this->grid[x][y]);
}

string Map::frameForTile(TerrainType tile) {
    
    if (tile == solidRock) {
        return "mars-wall-07.png";
    }
    
    if (tile == metalFloor) {
        return "ground_metal.png";
    }
    
    return "ground_mars.png";
}
