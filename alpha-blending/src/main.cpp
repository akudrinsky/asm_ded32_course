//
//  main.cpp
//  alpha-blending
//
//  Created by Alexey Kudrinsky on 30.04.2020.
//  Copyright © 2020 Alexey Kudrinsky. All rights reserved.
//

#include "image_mixer.hpp"

#pragma GCC diagnostic error "-Wold-style-cast"

int main (int argc, const char* argv[]) {
    /*
    mix_bmp ("data/Table.bmp", "data/AskhatCat.bmp", "data/merged.bmp",
             280, 100);
     */
    compare_cycles comparator ("data/Table.bmp", "data/AskhatCat.bmp", "data/merged.bmp",
                               280, 100);
    
    return 0;
}
