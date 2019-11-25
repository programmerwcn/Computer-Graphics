//
// Created by 吴小宁 on 2019/11/6.
//

#include "Polygon2D.h"

#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <vector>
#include "glm/glm.hpp"
#include <math.h>

using namespace std;

void Polygon2D::buildPoly() {
    if (poly_edges.size() != 0) {
        poly_edges.clear();
    }
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
        current_edge.up = current_poly_edge.up;
        current_edge.down = current_poly_edge.down;
        current_edge.x = current_poly_edge.down.x;
        current_edge.k = (current_poly_edge.up.x - current_poly_edge.down.x) / (current_poly_edge.up.y - current_poly_edge.down.y);
        current_edge.I_P = current_edge.down.I_P;
        current_poly_edge.edge = current_edge;
        poly_edges.push_back(current_poly_edge);
    }
    getMinMaxY();
}

void Polygon2D::getMinMaxY() {
    minY = floor(poly_points[0].y);
    maxY = minY;
    for (int i = 0; i < point_num; i++) {
        if (poly_points[i].y > maxY) {
            maxY = ceil(poly_points[i].y);
        }
        if (poly_points[i].y < minY) {
            minY = floor(poly_points[i].y);
        }
    }
}



void Polygon2D::buildSET() {
    getMinMaxY();
    if (SET.size() != 0) {
        SET.clear();
    }
    if (SET.size() <= maxY - minY + 2) {
        for (int i = 0; i <= maxY - minY; i++) {
            vector<Edge> edges;
            SET.push_back(edges);
        }
    }
    for (int i = 0; i < point_num; i++) {
        int scan_line_index = round(poly_edges[i].down.y) - minY;
        SET[scan_line_index].push_back(poly_edges[i].edge);

    }
    // sorting
    for (int i = 0; i <= maxY - minY; i++) {
        SET[i] = edgeSort(SET[i]);
    }
}


vector<Polygon2D::Edge> Polygon2D::edgeSort(vector<Edge> unsorted_edge) {
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
void Polygon2D::addNewEdge(int index) {
    for (Edge edge: SET[index]) {
        AEL[index].push_back(edge);
    }
}

/**
 * This method should remove old edged from the Active Edge List when scan-line moves
 * @param index index of current scan line
 */
void Polygon2D::removeOldEdge(int index) {
    for (int i = 0; i < AEL[index].size(); i++) {
        Edge edge = AEL[index][i];
        if (index + round(minY) == round(edge.yMax)) {
            AEL[index].erase(AEL[index].begin() + i);
            i--;
        }
    }
}

/**
 * This method should update the x of intersection points
 * @param index
 */
void Polygon2D::updateX(int index) {
    if (index < AEL.size() - 1) {
        for (Edge edge: AEL[index]) {
            edge.x += edge.k;
            edge.y += 1;
            if (edge.up.y == edge.down.y) {
                edge.I_P = edge.down.I_P;
            }
            else {
                edge.I_P = (float) ((edge.y - edge.down.y) / (edge.up.y - edge.down.y)) * edge.up.I_P +
                           (float) ((edge.up.y - edge.y) / (edge.up.y - edge.down.y)) * edge.down.I_P;
            }
            AEL[index + 1].push_back(edge);
        }
    }
}

/**
 * This method should fill the in-area of current scan-line
 * @param index
 */
void Polygon2D::fillScanLine(int index) {
    for (int i = 0; i < (int)(AEL[index].size() - 1); i+=2) {
        Edge left = AEL[index][i];
        Edge right = AEL[index][i+1];
        for (int x0 = left.x ; x0 <= right.x; x0++) {
            Point draw_p;
            draw_p.x = x0;
            draw_p.y = index + minY;
            if (right.x == left.x) {
                draw_p.I_P = left.I_P;
            }
            else {
                draw_p.I_P = (float) ((right.x - x0) / (right.x - left.x)) * left.I_P +
                             (float) ((x0 - left.x) / (right.x - left.x)) * right.I_P;
            }
            draw_pixel(draw_p);
        }
    }
}



void Polygon2D::draw_pixel(Point p) {
    mega_size = 3;
    GLfloat red = p.I_P.x / 255;
    GLfloat green = p.I_P.y / 255;
    GLfloat blue = p.I_P.z / 255;
    GLfloat pixel_x = p.x * mega_size, pixel_y = p.y * mega_size;

    if (!half_tone) {
        for (int i = 0; i < mega_size; i ++) {
            for (int j = 0; j < mega_size; j++) {
                glBegin(GL_POINTS);
                glColor3f(red, green, blue);
                glVertex2f(pixel_x + i, pixel_y + j);
                glEnd();
            }
        }

    }
        // half_toning on
    else {
        vector<Point> mega_points;
        for(int i = 0; i < mega_size; i++) {
            for (int j = 0; j < mega_size; j++) {
                Point mega_point;
                mega_point.x = pixel_x + i;
                mega_point.y = pixel_y + j;
                mega_point.on = 0;
                mega_points.push_back(mega_point);
            }
        }
        GLfloat max = red;
        if (green > max) {
            max = green;
        }
        if (blue > max) {
            max = blue;
        }
        int on = mega_size * mega_size * max;
        if (on < 0) {
            on = 0;
        }
        for (int i = 0; i < on; i++) {
            // randomly half-tone
            while (1) {
                int position = rand() % 9;
                if (!mega_points[position].on) {
                    glBegin(GL_POINTS);
                    glColor3f(1, 1, 1);
                    glVertex2f(mega_points[position].x, mega_points[position].y);
                    glEnd();
                    //mega_points[position].on = 1;
                    break;
                }
            }
//draw_mega_pixel(i + 1, 0, pixel_x, pixel_y, 1);
        }
//        int num_red = round(on * red / (red + green + blue));
//        int num_green = round(on * green / (red + green + blue));
//        int num_blue = round(on * blue / (red + green + blue));
//        // draw all red points
//        for (int i = 0; i < num_red; i++) {
//            while (1) {
//                int position = rand() % 9;
//                if (!mega_points[position].on) {
//                    glBegin(GL_POINTS);
//                    glColor3f(1,0,0);
//                    glVertex2f(mega_points[position].x, mega_points[position].y);
//                    glEnd();
//                    mega_points[position].on = 1;
//                    break;
//                }
//            }
//        }
//        // draw all green points
//        for (int i = 0; i < num_green; i++) {
//            while (1) {
//                int position = rand() % 9;
//                if (!mega_points[position].on) {
//                    glBegin(GL_POINTS);
//                    glColor3f(0,1,0);
//                    glVertex2f(mega_points[position].x, mega_points[position].y);
//                    glEnd();
//                    mega_points[position].on = 1;
//                    break;
//                }
//            }
//        }
//        // draw all blue points
//        for (int i = 0; i < num_blue; i++) {
//            while (1) {
//                int position = rand() % 9;
//                if (!mega_points[position].on) {
//                    glBegin(GL_POINTS);
//                    glColor3f(0,0,1);
//                    glVertex2f(mega_points[position].x, mega_points[position].y);
//                    glEnd();
//                    mega_points[position].on = 1;
//                    break;
//                }
//            }
//        }
    }
}

void Polygon2D::draw_mega_pixel(int pos, int color, float origin_x, float origin_y, float mega_pixel_size) {
    GLfloat x,y;
    switch (pos) {
        case 0:
            return;
        case 1:
        x = origin_x + mega_pixel_size;
        y = origin_y + mega_pixel_size;
            break;
        case 2:
            x = origin_x + mega_pixel_size * 2;
            y = origin_y + mega_pixel_size;
            break;
        case 3:
            x = origin_x + mega_pixel_size;
            y = origin_y + mega_pixel_size * 2;
            break;
        case 4:
            x = origin_x ;
            y = origin_y ;
            break;
        case 5:
            x = origin_x ;
            y = origin_y + mega_pixel_size;
            break;
        case 6:
            x = origin_x + mega_pixel_size * 2;
            y = origin_y ;
            break;
        case 7:
            x = origin_x + mega_pixel_size * 2;
            y = origin_y + mega_pixel_size * 2;
            break;
        case 8:
            x = origin_x ;
            y = origin_y + mega_pixel_size * 2;
            break;
        case 9:
            x = origin_x + mega_pixel_size;
            y = origin_y ;
            break;
    }
    glBegin(GL_POINTS);
    glColor3f(1,1,1);
    glVertex2f(x, y);
    glEnd();
//    switch (color) {
//        case 0:
//            glBegin(GL_POINTS);
//            glColor3f(1,0,0);
//            glVertex2f(x, y);
//            glEnd();
//            break;
//        case 1:
//            glBegin(GL_POINTS);
//            glColor3f(0,1,0);
//            glVertex2f(x, y);
//            glEnd();
//            break;
//        case 2:
//            glBegin(GL_POINTS);
//            glColor3f(0,0,1);
//            glVertex2f(x, y);
//            glEnd();
//            break;
//    }
}

void Polygon2D::fillPolygon() {
    buildPoly();
    buildSET();
    if (AEL.size() != 0) {
        AEL.clear();
    }
    if (AEL.size() <= maxY - minY + 2) {
        for (int i = 0; i <= maxY - minY; i++) {
            vector<Edge> edges;
            AEL.push_back(edges);
        }
    }
    for (int i = 0; i < maxY - minY; i++) {
        addNewEdge(i);
        removeOldEdge(i);
        AEL[i] = edgeSort(AEL[i]);
        fillScanLine(i);
        updateX(i);
    }

}