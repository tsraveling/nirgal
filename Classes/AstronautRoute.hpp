//
//  AstronautRoute.hpp
//  nirgal-mobile
//
//  Created by Timothy Raveling on 1/20/18.
//

#ifndef AstronautRoute_hpp
#define AstronautRoute_hpp

#include <stdio.h>
#include <vector>

using namespace std;

enum RoutingDirection : unsigned char {
    routingUp = 0,
    routingRight = 1,
    routingDown = 2,
    routingLeft = 3
};

class AstronautRoute {
public:
    vector <RoutingDirection> steps;
};

#endif /* AstronautRoute_hpp */
