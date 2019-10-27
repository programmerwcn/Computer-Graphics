//
// Created by 吴小宁 on 2019/10/8.
//

#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>


#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include "ClippingPolygon.h"
#include "Polygon.h"
#include <math.h>

int double_equal(double a, double b);

GLint ClippingPolygon::inside(Polygon::Point p, ClippingPolygon::Boundary b, Polygon::Point wMin,
                              Polygon::Point wMax) {
    switch (b) {
        case Left:
            if (p.x < wMin.x) {
                return false;
            }
            break;
        case Right:
            if (p.x > wMax.x) {
                return false;
            }
            break;
        case Bottom:
            if (p.y < wMin.y) {
                return false;
            }
            break;
        case Top:
            if (p.y > wMax.y) {
                return false;
            }
            break;
    }
    return true;
}

GLint ClippingPolygon::cross(Polygon::Point p1, Polygon::Point p2, ClippingPolygon::Boundary winEdge,
                             Polygon::Point wMin, Polygon::Point wMax) {
    return  inside(p1, winEdge, wMin, wMax) != inside(p2, winEdge, wMin, wMax);
}

Polygon::Point ClippingPolygon::intersect(Polygon::Point p1, Polygon::Point p2, ClippingPolygon::Boundary winEdge,
                                          Polygon::Point wMin, Polygon::Point wMax) {
    Polygon::Point iPT;
    GLfloat m;

    if (!double_equal(p1.x, p2.x)) {
        m = (p2.y - p1.y) / (p2.x - p1.x);
    }

    switch (winEdge) {
        case Left:
            iPT.x = wMin.x;
            iPT.y = p2.y + (wMin.x - p2.x) * m;
            break;
        case Right:
            iPT.x = wMax.x;
            iPT.y = p2.y + (wMax.x - p2.x) * m;
            break;
        case Bottom:
            iPT.y = wMin.y;
            if (!double_equal(p1.x, p2.x)) {
                iPT.x = p2.x + (wMin.y - p2.y) / m;
            }
            else {
                iPT.x = p2.x;
            }
            break;
        case Top:
            iPT.y = wMax.y;
            if (!double_equal(p1.x, p2.x)) {
                iPT.x = p2.x + (wMax.y - p2.y) / m;
            }
            else {
                iPT.x = p2.x;
            }
            break;
    }
    return iPT;
}

int double_equal(double a, double b) {
    return (abs(a - b) < 0.0001);
}

void ClippingPolygon::clip_point(Polygon::Point p, ClippingPolygon::Boundary winEdge, Polygon::Point wMin,
                                 Polygon::Point wMax, vector<Polygon::Point> &p_out, int *cnt, vector<Polygon::Point *> &first,
                                 Polygon::Point *s) {
    Polygon::Point iPT;
    // no previous point for this boundary
    if (!first[winEdge]) {
        first[winEdge] = &p;
    }
    // has previous point for this boundary
    else {
        if (cross(p, s[winEdge], winEdge, wMin, wMax)) {
            iPT = intersect(p, s[winEdge], winEdge, wMin, wMax);
            if (winEdge < Top) {
                clip_point(iPT, (ClippingPolygon::Boundary)(winEdge + 1), wMin, wMax, p_out, cnt, first, s);
            }
            else {
                p_out.push_back(iPT);
            }
        }
    }

    // save p as most recent point for this clip boundary
    s[winEdge] = p;

    if (inside(p, winEdge, wMin, wMax)) {
        if (winEdge < Top) {
            clip_point(p, (ClippingPolygon::Boundary)(winEdge + 1), wMin, wMax, p_out, cnt, first, s);
        }
        else {
            p_out.push_back(p);
        }
    }
}


void ClippingPolygon::close_clip(Polygon::Point wMin, Polygon::Point wMax, vector<Polygon::Point> &p_out, GLint *cnt,
                                 vector<Polygon::Point *> &first, Polygon::Point *s) {
    Polygon::Point pt;
    Boundary winEdge;

    for (winEdge = Left; winEdge <= Top; winEdge = (ClippingPolygon::Boundary)(winEdge + 1)) {
        if (cross(s[winEdge], *first[winEdge], winEdge, wMin, wMax)) {
            Polygon::Point * p = first[winEdge];
            pt = intersect(s[winEdge], *p, winEdge, wMin, wMax);
            if (winEdge < Top) {
                clip_point(pt, (ClippingPolygon::Boundary)(winEdge + 1), wMin, wMax, p_out, cnt, first, s);
            }
            else {
                p_out.push_back(pt);
                (*cnt)++;
            }
        }
    }
}

GLint ClippingPolygon::polygon_clip_Suth_Hodg(Polygon::Point wMin, Polygon::Point wMax, GLint n, vector<Polygon::Point> pIN,
                                              vector<Polygon::Point> &pOUT) {
//    vector<Polygon::Point *> first;
//            Polygon::Point s[N_CLIP];
//            for (int i = 0; i < N_CLIP; i++) {
//                first.push_back(0);
//            }
//    GLint  k, cnt = 0;
//    for (k = 0; k < n; k++) {
//        clip_point(pIN[k], Left, wMin, wMax, pOUT, &cnt, first, s);
//    }
//    clip_point(pIN[0], Left, wMin, wMax, pOUT, &cnt, first, s);
//    close_clip(wMin, wMax, pOUT, &cnt, first, s);
//
//    return cnt;
Boundary boundary = Left;
vector<Polygon::Point> in, out;
int flag = 0;
for (int i = 0; i < 4; i++) {
    boundary = Boundary (i);
    if (i == 0) {
        in = pIN;
    }
    out = clipper(in, wMin, wMax, boundary);
    in = out;
}
pOUT = out;
    return (pOUT.size());

}

vector<Polygon::Point> ClippingPolygon::clipper(vector<Polygon::Point> pIn, Polygon::Point wMin, Polygon::Point wMax, Boundary b){
    if (pIn.size() == 0) {
        return pIn;
    }
    Polygon::Point start, end;
    vector<Polygon::Point> out;
    for (int i = 0; i < pIn.size(); i++) {
        Polygon::Point p = pIn[i];
        if (i == 0) {
            start = p;
        }
        else {
            end = p;
            get_out(start, end, b, wMin, wMax, out);
            start = p;
        }
    }
    start = pIn[pIn.size() - 1];
    end = pIn[0];
    get_out(start, end, b, wMin, wMax, out);
    return out;
}

void ClippingPolygon::get_out(Polygon::Point start, Polygon::Point end, ClippingPolygon::Boundary b,
                              Polygon::Point wMin, Polygon::Point wMax, vector<Polygon::Point> &out) {
    if (!inside(start, b, wMin, wMax) && inside(end, b, wMin, wMax)) {
        Polygon::Point iPT = intersect(start, end, b, wMin, wMax);
        out.push_back(iPT);
        out.push_back(end);
    }
        // case 2: in -> in
    else if (inside(start, b, wMin, wMax) && inside(end, b, wMin, wMax)) {
        out.push_back(end);
    }
        // case 3: in -> out
    else if (inside(start, b, wMin, wMax) && !inside(end, b, wMin, wMax)) {
        Polygon::Point iPT = intersect(start, end, b, wMin, wMax);
        out.push_back(iPT);
    }
}



