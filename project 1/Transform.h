//
// Created by 吴小宁 on 2019/10/10.
//

#ifndef PROJECT1_TRANSFORM_H
#define PROJECT1_TRANSFORM_H

#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include "Polygon.h"
class Transform {
public:
vector<Polygon::Point> translate_polygon(vector<Polygon::Point> verts, GLfloat tx, GLfloat ty);
    vector<Polygon::Point> rotate_polygon(vector<Polygon::Point> verts, Polygon::Point pivPT, GLdouble theta);
    vector<Polygon::Point> scale_polygon(vector<Polygon::Point> verts, Polygon::Point fixedPT, GLfloat sx, GLfloat sy);
};


#endif //PROJECT1_TRANSFORM_H
