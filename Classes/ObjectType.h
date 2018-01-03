//
//  ObjectType.h
//  nirgal
//
//  Created by Timothy Raveling on 12/27/17.
//

#ifndef ObjectType_h
#define ObjectType_h

#define kNumberOfObjectTypes    10

enum ObjectType : char {
    
    crashCouch = 0,
    landerCrate = 1,
    waterStation = 2,
    landingComputer = 3,
    landingStrut = 4,
    atmosphereTank = 5,
    smallFoodStation = 6,
    landerSolarPanel = 7,
    suitPort = 8,
    landerLiquidTank = 9
};

#endif /* ObjectType_h */
