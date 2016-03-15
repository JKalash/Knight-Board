//
//  main.cpp
//  KnightBoard
//
//  Created by Joseph Kalash on 3/11/16.
//  Copyright © 2016 Joseph Kalash. All rights reserved.
//
//
//
//  Recorded Time Working:
//  1. Friday 12: 6pm - 6.30pm
//  2. Friday 12: 9pm - 10pm
//  3. Saturday 2.15pm - 4pm & 6.30pm-8pm
//  4. Sunday   4pm - 7.30pm
//  5. Monday   1pm - 2pm
//  Total Time:2.5+1+1.75+1.5+3.5+1 = 11.25 hours

#include "Level3.h"
#include "Level4.h"
#include "Level5.h"

#define MAP_FILE_PATH "ENTER_PATH_TO_MAP_TXT_FILE"

int main(int argc, const char * argv[]) {
    
    while(true) {
        cout << "Level 3, 4 or 5? ";
        int level; cin >> level;
        if (level != 3 && level != 4 && level != 5)
            exit(0);
        
        if (0 == level)
            simulateLevels1To3();
        else if(4 == level)
            simulateLevel4(MAP_FILE_PATH);
        else simulateLevel5(MAP_FILE_PATH);
    }
    
    return 0;
}
