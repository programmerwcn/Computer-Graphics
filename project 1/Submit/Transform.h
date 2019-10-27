//
// Created by 吴小宁 on 2019/10/10.
//

#ifndef PROJECT1_TRANSFORM_H
#define PROJECT1_TRANSFORM_H

#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>


#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif
#include "Polygon.h"
class Transform {
public:
static vector<Polygon::Point> translate_polygon(vector<Polygon::Point> verts, GLfloat tx, GLfloat ty);
    static vector<Polygon::Point> rotate_polygon(vector<Polygon::Point> verts, Polygon::Point pivPT, GLdouble theta);
    static vector<Polygon::Point> scale_polygon(vector<Polygon::Point> verts, Polygon::Point fixedPT, GLfloat sx, GLfloat sy);
};


#endif //PROJECT1_TRANSFORM_H
