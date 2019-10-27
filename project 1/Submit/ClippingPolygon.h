//
// Created by 吴小宁 on 2019/10/8.
//

#ifndef PROJECT1_CLIPPINGPOLYGON_H
#define PROJECT1_CLIPPINGPOLYGON_H

#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>


#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include "Polygon.h"

const GLint N_CLIP = 4;
class ClippingPolygon {
    typedef enum {
        Left,
        Right,
        Bottom,
        Top
    } Boundary;
public:
    GLint inside(Polygon::Point p, Boundary b, Polygon::Point wMin, Polygon::Point wMax);

    GLint cross(Polygon::Point p1, Polygon::Point p2, Boundary winEdge, Polygon::Point wMin, Polygon::Point wMax);

    Polygon::Point
    intersect(Polygon::Point p1, Polygon::Point p2, Boundary winEdge, Polygon::Point wMin, Polygon::Point wMax);

    void clip_point(Polygon::Point p, Boundary winEdge, Polygon::Point wMin, Polygon::Point wMax, vector<Polygon::Point> &p_out,
                    int *cnt, vector<Polygon::Point *> &first, Polygon::Point *s);

    void
    close_clip(Polygon::Point wMin, Polygon::Point wMax, vector<Polygon::Point> &p_out, GLint *cnt, vector<Polygon::Point *> &first,
               Polygon::Point *s);

    vector<Polygon::Point> clipper(vector<Polygon::Point> pIn, Polygon::Point wMin, Polygon::Point wMax, Boundary b);

    void get_out(Polygon::Point start, Polygon::Point end, Boundary b, Polygon::Point wMin, Polygon::Point wMax, vector<Polygon::Point> &out);
    GLint polygon_clip_Suth_Hodg(Polygon::Point wMin, Polygon::Point wMax, GLint n, vector<Polygon::Point> pIN,
                                 vector<Polygon::Point> &pOUT);
};


#endif //PROJECT1_CLIPPINGPOLYGON_H
