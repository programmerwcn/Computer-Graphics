//
// Created by 吴小宁 on 2019/10/4.
//

#include "ModelGL.h"
#include "math.h"
#include <string>
#include <iostream>
#include <fstream>

ModelGL::ModelGL() {
    this->num_of_polygons = 0;

}

void ModelGL::set_num_of_polygons(int num)
{
num_of_polygons = num;
}



