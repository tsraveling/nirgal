//
//  Coord.h
//  nirgal
//
//  Created by Timothy Raveling on 1/3/18.
//

#ifndef Coord_h
#define Coord_h

#include <stdio.h>
#include <vector>

using namespace std;

struct Coord {
    float x,y;
    
    static Coord fromString(string coordString) {
        
        // Remove spaces from the coordinate string
        string str = coordString;
        size_t start = 0;
        while((start = str.find(" ", start)) != string::npos) {
            str.replace(start, 1, "");
        }
        
        // Get the string stream
        stringstream ss(coordString);
        vector<string> result;
        
        while (ss.good()) {
            string substr;
            getline(ss, substr, ',');
            result.push_back(substr);
        }
        
        // Findally, make it
        Coord ret = Coord();
        ret.x = 0;
        ret.y = 0;
        
        // Push the strings we've retrieved into the return values
        if (result.size() == 2) {
            stringstream xs(result[0]);
            xs >> ret.x;
            
            stringstream ys(result[1]);
            ys >> ret.y;
        }
        
        return ret;
    }
};

#endif /* Point_h */
