//
// Created by 吴小宁 on 2019/10/4.
//

#ifndef PROJECT1_POLYGON_H
#define PROJECT1_POLYGON_H

#include <vector>
#include <list>
#include <string>

#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>


#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif
using namespace std;
class Polygon {

public:
    int id;
    vector<vector<int>> pixels;
    int point_num;
    int minY, maxY;
    struct Edge{
        double yMax;
        double k;
        double x;
        //Edge * next;
    };
    struct Point {
        float x;
        float y;
    };
    struct Poly_edge {
        Point up;
        Point down;
        Edge edge;
    };
    Point centroid;
    vector<Point> poly_points;
    vector<Poly_edge> poly_edges;
    vector<vector<Edge>> SET;
    vector<vector<Edge>> AEL;
    Polygon();

    void set_id(int id);

    void add_pixel(int x, int y);
    static void lineDDA(int x0, int y0, int xEnd, int yEnd);
    void draw_outline(string algorithm);
    static void lineBres(int x0, int y0, int xEnd, int yEnd);

    void buildPoly();
    void buildSET();
    void buildAET();
    void addNewEdge(int index);
    void removeOldEdge(int index);
    void updateX(int index);
    void fillScanLine(int index);
    void getMinMaxY();
    static void draw_pixel(int x, int y);
    void fillPolygon();
    void translate(GLfloat tx, GLfloat ty);
    void rotate(Point pivPT, GLdouble theta);
    void scale(Polygon::Point fixedPT, GLfloat sx, GLfloat sy);
    vector<Edge> edgeSort(vector<Edge> unsorted_edge);
    void compute_centroid();
};


#endif //PROJECT1_POLYGON_H
