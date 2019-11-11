//
// Created by 吴小宁 on 2019/11/6.
//

#ifndef PROJECT3_POLYGON2D_H
#define PROJECT3_POLYGON2D_H

#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include "glm/glm.hpp"
#include <vector>

using namespace std;
class Polygon2D {
public:
    int id;
    int half_tone;
    float pixel_size, mega_size;
    vector<vector<int>> pixels;
    int point_num;
    int minY, maxY;
    struct Point {
        float x;
        float y;
        glm::vec3 I_P;
        int on;
        int color;
    };
    struct Edge{
        double yMax;
        double k;
        double x,y;
        glm::vec3 I_P;
        Point up, down;
    };
    struct Poly_edge {
        Point up;
        Point down;
        Edge edge;
    };
    vector<Point> poly_points;
    vector<Poly_edge> poly_edges;
    vector<vector<Edge>> SET;
    vector<vector<Edge>> AEL;

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
    void draw_pixel(Point p);
    void draw_mega_pixel(int pos, int color, int origin_x, int origin_y, float mega_pixel_size);
    void fillPolygon();
    vector<Edge> edgeSort(vector<Edge> unsorted_edge);

};


#endif //PROJECT3_POLYGON2D_H
