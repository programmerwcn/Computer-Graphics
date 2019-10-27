//
// Created by 吴小宁 on 2019/10/8.
//

#ifndef PROJECT1_CLIPPING_H
#define PROJECT1_CLIPPING_H
#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>


#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include "Polygon.h"

class Clipping {
const int WIN_LEFT_BIT_CODE = 0x1;
    const int WIN_RIGHT_BIT_CODE = 0x2;
    const int WIN_BOTTOM_BIT_CODE = 0x4;
    const int WIN_TOP_BIT_CODE = 0x8;
public:
    inline int inside(int code) {
        return (!code);
    }
    inline int reject (int code1, int code2) {
        return (code1 & code2);
    }
    inline int accept(int code1, int code2) {
        return !(code1 | code2);
    }

    GLubyte encode (Polygon::Point point, Polygon::Point winMin, Polygon::Point winMax);
    void swap_points (Polygon::Point * point1, Polygon::Point * point2 );
    void swap_codes (GLubyte * c1, GLubyte * c2);
    void line_clip_Coh_Suth(Polygon::Point winMin, Polygon::Point winMax, Polygon::Point p1, Polygon::Point p2);

};


#endif //PROJECT1_CLIPPING_H
