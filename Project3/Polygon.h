//
// Created by 吴小宁 on 2019/11/3.
//

#ifndef PROJECT3_POLYGON_H
#define PROJECT3_POLYGON_H

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
class Polygon {
public:
    struct Point {
        float x;
        float y;
        float z;
        glm::vec3 position;
        glm::vec3 normal_vector;
        // r, g, b, 0-255
        glm::vec3 I_P;
        glm::vec3 grb_color;
    };
    struct Facet {
        vector<int> points;
        glm::vec3 normal_vector;
        GLfloat depth;
    };

    vector<Facet> facets;
    vector<Point> points;
    void compute_facet_normal_vector();
    void compute_point_normal_vector();
    void compute_IP_PHONG(GLfloat k_a, GLfloat k_d, GLfloat k_s, GLfloat K, int n, glm::vec3 I_A, glm::vec3 I_L, glm::vec3 light_pos, Point &p);
    vector<Facet> sort_facet(int direction);
    void compute_depth(vector<Facet> &facets,int direction);
    void sort_facet_by_depth(vector<Facet> &facets);
    void back_face_culling(vector<Facet> &facets, int direction);
    void gouraud_shading(int direction, vector<Facet> &visible_facets);
    void draw_pixel(int half_tone, int x, int y);

};


#endif //PROJECT3_POLYGON_H
