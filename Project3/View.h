//
// Created by 吴小宁 on 2019/11/3.
//

#ifndef PROJECT3_VIEW_H
#define PROJECT3_VIEW_H

#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include "Polygon.h"
#include "glm/glm.hpp"
class View {
public:
    GLfloat k_a, k_d, k_s, K, I_A, I_L;
    int n;
    glm::vec3 light_source_pos;

};


#endif //PROJECT3_VIEW_H
