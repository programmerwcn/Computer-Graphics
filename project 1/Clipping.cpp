//
// Created by 吴小宁 on 2019/10/8.
//

#include "Clipping.h"
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <math.h>

/**
 * This method should encode the input point to its 4-bit code deciding its position to window
 * @param point
 * @param winMin window position
 * @param winMax window position
 * @return
 */
GLubyte Clipping::encode(Polygon::Point point, Polygon::Point winMin, Polygon::Point winMax) {
    GLubyte code = 0x00;

    if (point.x < winMin.x) {
        code = code | WIN_LEFT_BIT_CODE;
    }
    if (point.x > winMax.x) {
        code = code | WIN_RIGHT_BIT_CODE;
    }
    if (point.x < winMin.y) {
        code = code | WIN_BOTTOM_BIT_CODE;
    }
    if (point.x > winMax.y) {
        code = code | WIN_TOP_BIT_CODE;
    }

    return code;
}
/**
 * This method should swap two codes stored in c1 & c2
 * @param c1
 * @param c2
 */
void Clipping::swap_codes(GLubyte *c1, GLubyte *c2) {
    GLubyte temp;
    temp = *c1;
    *c1 = *c2;
    *c2 = temp;
}

/**
 * This method should swap two points stores in p1&p2
 * @param point1
 * @param point2
 */
void Clipping::swap_points(Polygon::Point *point1, Polygon::Point *point2) {
    Polygon::Point temp;
    temp = *point1;
    *point1 = *point2;
    *point2 = temp;
}

void Clipping::line_clip_Coh_Suth(Polygon::Point winMin, Polygon::Point winMax, Polygon::Point p1, Polygon::Point p2) {
    GLubyte code1, code2;
    GLint plotLine = false;
    // slop
    GLfloat m;

    while (true) {
        code1 = encode(p1, winMin, winMax);
        code2 = encode(p2, winMin, winMax);

        if (accept(code1, code2)) {
            plotLine = true;
            break;
        }
        if (reject(code1,code2)) {
            break;
            }
        // make sure that p1 is outside the window
        if (inside(code1)) {
            swap_points(&p1, &p2);
            swap_codes(&code1, &code2);
        }

        // find intersection points
        if (p2.x != p1.x) {
            m = (p2.y - p1.y) / (p2.x - p1.x);
        }
        if (code1 & WIN_LEFT_BIT_CODE) {
            p1.y += (winMin.x - p1.x) * m;
            p1.x = winMin.x;
        }
        else if (code1 & WIN_RIGHT_BIT_CODE) {
            p1.y += (winMax.x - p1.x) * m;
            p1.x = winMax.x;
        }
        else if (code1 & WIN_BOTTOM_BIT_CODE) {
            // nonvertical lines
            if (p2.x != p1.x) {
                p1.x += (winMin.y - p1.y) / m;
            }
            p1.y = winMin.y;
        }
        else if (code1 & WIN_TOP_BIT_CODE){
            if (p2.x != p1.x) {
                p1.x += (winMax.y - p1.y) / m;
            }
            p1.y = winMax.y;
        }
    }

    if (plotLine) {
        Polygon::lineBres(round(p1.x), round(p1.y), round(p2.x), round(p2.y));
    }
}