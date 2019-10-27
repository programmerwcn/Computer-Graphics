//
// Created by 吴小宁 on 2019/10/21.
//

#ifndef PROJECT2_POLYGON_H
#define PROJECT2_POLYGON_H


#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <vector>
#include <list>
#include <string>
#include <vector>
using namespace std;

class Polygon {

public:
    int id;

    struct Point {
        float x;
        float y;
        float z;
    };
    struct Edge {
        int start_p;
        int end_p;
    };
    typedef GLfloat Matrix4x4 [4][4] ;

    Matrix4x4 mat_rot;

    Point centroid;
    vector<Point> poly_points;
    vector<Edge> poly_edges;

    Polygon();

    void set_mat_rot(int i);
    void initialize_mat_rot();
    void get_mat_rot(int i);

    void matrix_set_identity(Matrix4x4 & matIdent);
    void matrix_multiply(Matrix4x4 & m1, Matrix4x4 & m2);

    void translate3D(GLfloat tx, GLfloat ty, GLfloat tz);
    void rotate3D(Point p1, Point p2, GLfloat angle);


};






#endif //PROJECT2_POLYGON_H
