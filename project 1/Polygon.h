//
// Created by 吴小宁 on 2019/10/4.
//

#ifndef PROJECT1_POLYGON_H
#define PROJECT1_POLYGON_H

#include <vector>
#include <list>
using namespace std;
class Polygon {
int id;

public:
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
    vector<Point> poly_points;
    vector<Poly_edge> poly_edges;
    vector<vector<Edge>> SET;
    vector<vector<Edge>> AEL;
    Polygon();

    void set_id(int id);

    void add_pixel(int x, int y);
    static void lineDDA(int x0, int y0, int xEnd, int yEnd);
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
    vector<Edge> edgeSort(vector<Edge> unsorted_edge);
};


#endif //PROJECT1_POLYGON_H
