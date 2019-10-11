//
// Created by 吴小宁 on 2019/10/4.
//

#include <iostream>
#include "Polygon.h"
#include <math.h>
#include <vector>
#include <list>
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>
using namespace std;


Polygon::Polygon() {
    this->id = 0;

}

void Polygon::set_id(int id) {
    this->id = id;
}

/**
 * This function should add pixels to the model
 * @param x
 * @param y
 */
void Polygon::add_pixel(int x, int y) {
    vector<int> pixel;
    pixel.push_back(x);
    pixel.push_back(y);
    pixels.push_back(pixel);

}

/**
 * This function should generate a line with given points with DDA algorithm
 * @param x0 start point
 * @param y0 start point
 * @param xEnd end point
 * @param yEnd end point
 */
void Polygon::lineDDA(int x0, int y0, int xEnd, int yEnd) {
    int dx = xEnd - x0, dy = yEnd - y0, steps, k;
    float xIncrement, yIncrement, x = x0, y = y0;

    if (fabs(dx) > fabs(dy)) {
        steps = fabs(dx);
    }
    else {
        steps = fabs(dy);
    }
    xIncrement = float (dx) / steps;
    yIncrement = float (dy) / steps;

    draw_pixel(round(x), round(y));
    for (k = 0; k < steps; k++) {
        x += xIncrement;
        y += yIncrement;
        draw_pixel(round(x), round(y));
    }
}

/**
 * This function should generate a line with given points with Bresenham algorithm
 * @param x0 start point
 * @param y0 start point
 * @param xEnd end point
 * @param yEnd end point
 */
void Polygon::lineBres(int x0, int y0, int xEnd, int yEnd) {
    int dx = fabs(xEnd - x0), dy = fabs(yEnd - y0);
    int x, y, p;
    float k;
    if (x0 != xEnd) {
        k = (yEnd - y0) / (xEnd - x0);
    }
    // |k| < 1
    if (x0 != xEnd && fabs(k) < 1.0) {
        p = 2 * dy - dx;

        // determine which endpoint to use as start point
        if (x0 > xEnd) {
            x = xEnd;
            y = yEnd;
            xEnd = x0;
        } else {
            x = x0;
            y = y0;
        }
        draw_pixel(x, y);

        while (x < xEnd) {
            x++;
            if (p < 0) {
                p += 2 * dy;
            } else {
                y++;
                p += 2 * dy - 2 * dx;
            }
            draw_pixel(x, y);
        }
    }
    // |k|>=1
    else {
        p = 2 * dx - dy;

        if (y0 > yEnd) {
            x = xEnd;
            y = yEnd;
            yEnd = y0;
        } else {
            x = x0;
            y = y0;
        }
        draw_pixel(x, y);

        while (y < yEnd) {
            y++;
            if (p < 0) {
                p += 2 * dx;
            } else {
                x++;
                p += 2 * dx - 2 * dy;
            }
            draw_pixel(x, y);
        }
    }

}

void Polygon::buildPoly() {
    point_num = poly_points.size();
    Poly_edge current_poly_edge;
    Edge current_edge;
    for (int i = 0; i < point_num; i++) {
        if (i < point_num - 1) {
            if (poly_points[i].y > poly_points[i + 1].y) {
                current_poly_edge.up = poly_points[i];
                current_poly_edge.down = poly_points[i + 1];
            } else {
                current_poly_edge.up = poly_points[i + 1];
                current_poly_edge.down = poly_points[i];
            }
        }
        else {
            Point start = poly_points[0];
            Point end = poly_points[poly_points.size() - 1];
            if (start.y > end.y) {
                current_poly_edge.up = start;
                current_poly_edge.down = end;
            }
            else {
                current_poly_edge.up = end;
                current_poly_edge.down = start;
            }
        }
        current_edge.yMax = current_poly_edge.up.y;
        current_edge.x = current_poly_edge.down.x;
        current_edge.k = (current_poly_edge.up.x - current_poly_edge.down.x) / (current_poly_edge.up.y - current_poly_edge.down.y);
        current_poly_edge.edge = current_edge;
        poly_edges.push_back(current_poly_edge);
    }
    getMinMaxY();
}

void Polygon::getMinMaxY() {
    minY = poly_points[0].y;
    maxY = minY;
    for (int i = 0; i < point_num; i++) {
        if (poly_points[i].y > maxY) {
            maxY = poly_points[i].y;
        }
        if (poly_points[i].y < minY) {
            minY = poly_points[i].y;
        }
    }
}



void Polygon::buildSET() {
    getMinMaxY();
    for (int i = 0; i < maxY - minY; i++) {
        vector<Edge> edges;
        SET.push_back(edges);
    }
    for (int i = 0; i < point_num; i++) {
        int scan_line_index = poly_edges[i].down.y - minY;
        SET[scan_line_index].push_back(poly_edges[i].edge);
    }
    // sorting
    for (int i = 0; i < maxY - minY; i++) {
        SET[i] = edgeSort(SET[i]);
        }
    }


vector<Polygon::Edge> Polygon::edgeSort(vector<Edge> unsorted_edge) {
   int size = unsorted_edge.size();
   for (int i = 0; i < size - 1; i++) {
       for (int j = 0; j < size - 1 - i; j++) {
           if (unsorted_edge[j].x > unsorted_edge[j + 1].x) {
               Edge temp = unsorted_edge[j + 1];
               unsorted_edge[j + 1] = unsorted_edge[j];
               unsorted_edge[j] = temp;
           }
       }
   }
    return unsorted_edge;
}

/**
 * This method should add new lines into the Active Edge List
 * @param index the index of current scan line
 */
void Polygon::addNewEdge(int index) {
    for (Edge edge: SET[index]) {
        AEL[index].push_back(edge);
    }
}

/**
 * This method should remove old edged from the Active Edge List when scan-line moves
 * @param index index of current scan line
 */
void Polygon::removeOldEdge(int index) {
    for (int i = 0; i < AEL[index].size(); i++) {
        Edge edge = AEL[index][i];
        if (index + minY == edge.yMax) {
            AEL[index].erase(AEL[index].begin() + i);
            i--;
        }
    }
}

/**
 * This method should update the x of intersection points
 * @param index
 */
void Polygon::updateX(int index) {
    if (index < AEL.size() - 1) {
        for (Edge edge: AEL[index]) {
            edge.x += edge.k;
            AEL[index + 1].push_back(edge);
        }
    }
}

/**
 * This method should fill the in-area of current scan-line
 * @param index
 */
void Polygon::fillScanLine(int index) {
    for (int i = 0; i < AEL[index].size() - 1; i+=2) {
        for (int x0 = AEL[index][i].x; x0 < AEL[index][i+1].x; x0++) {
            draw_pixel(x0, index + minY);
        }
    }
}

void Polygon::draw_pixel(int x, int y){
    glBegin(GL_POINTS);
    glColor3f(.2,.2,1.0);
    glVertex3f(x+.5,y+.5,0);
    glEnd();
}

void Polygon::fillPolygon() {
    buildPoly();
    buildSET();
    for (int i = 0; i < maxY - minY; i++) {
        vector<Edge> edges;
        AEL.push_back(edges);
    }
    for (int i = 0; i < maxY - minY; i++) {
        addNewEdge(i);
        removeOldEdge(i);
        AEL[i] = edgeSort(AEL[i]);
        fillScanLine(i);
        updateX(i);
    }
}



