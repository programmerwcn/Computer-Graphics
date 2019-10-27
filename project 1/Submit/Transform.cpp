//
// Created by 吴小宁 on 2019/10/10.
//

#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>


#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include "Transform.h"
#include "Polygon.h"
#include <math.h>

vector<Polygon::Point> Transform::translate_polygon(vector<Polygon::Point> verts, GLfloat tx, GLfloat ty) {
    vector<Polygon::Point> pOUT;
    for (Polygon::Point p: verts) {
        Polygon::Point temp;
        temp.x = p.x + tx;
        temp.y = p.y + ty;
        pOUT.push_back(temp);
    }
    return pOUT;
}

vector<Polygon::Point> Transform::rotate_polygon(vector<Polygon::Point> verts, Polygon::Point pivPT, GLdouble theta) {
    vector<Polygon::Point> pOUT;
    for (Polygon::Point p: verts) {
        Polygon::Point temp;
        temp.x = pivPT.x + (p.x - pivPT.x) * cos(theta) - (p.y - pivPT.y) * sin(theta);
        temp.y = pivPT.y + (p.x - pivPT.x) * sin(theta) + (p.y - pivPT.y) * cos(theta);
        pOUT.push_back(temp);
    }
    return pOUT;
}

vector<Polygon::Point> Transform::scale_polygon(vector<Polygon::Point> verts, Polygon::Point fixedPT, GLfloat sx,
                                                GLfloat sy) {
    vector<Polygon::Point> pOUT;
    for (Polygon::Point p: verts) {
        Polygon::Point temp;
        temp.x = p.x * sx + fixedPT.x * (1 - sx);
        temp.y = p.y * sy + fixedPT.y * (1 - sy);
        pOUT.push_back(temp);
    }
    return pOUT;
}