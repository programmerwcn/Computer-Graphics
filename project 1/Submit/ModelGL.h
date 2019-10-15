//
// Created by 吴小宁 on 2019/10/4.
//

#ifndef PROJECT1_MODELGL_H
#define PROJECT1_MODELGL_H

#include <vector>
#include <string>
#include "Polygon.h"
using namespace std;

class ModelGL {


public:

    int num_of_polygons;
    // constructors
    ModelGL();


    void set_num_of_polygons(int num);
    vector<Polygon> polygons;

};


#endif //PROJECT1_MODELGL_H
