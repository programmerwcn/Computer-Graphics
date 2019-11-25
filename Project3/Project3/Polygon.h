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
    int id;
    int grid_width, grid_height, pixel_size;
    int half_tone;
    int mega_size = 3;
    struct Point {
        float x;
        float y;
        float z;
        glm::vec3 position;
        glm::vec3 normal_vector;
        // r, g, b, 0-255
        glm::vec3 I_P;
        glm::vec3 origin_I_p;
        GLfloat specularity;

    };
    struct Facet {
        GLfloat specularity;
        vector<int> points;
        glm::vec3 normal_vector;
        GLfloat depth;
    };

    vector<Facet> facets;
    vector<Point> points;
    glm::vec2 projection(int direction, glm::vec3 point_3d);
    void compute_facet_normal_vector();
    void compute_point_normal_vector();
    void compute_Phong_n();
    void compute_IP_PHONG(glm::vec3 k_s, GLfloat K, glm::vec3 I_A, GLfloat I_L, glm::vec3 light_pos, glm::vec3 f, Point &p);
    vector<Facet> sort_facet(int direction);
    void compute_depth(vector<Facet> &facets,int direction);
    void sort_facet_by_depth(vector<Facet> &facets);
    void back_face_culling(vector<Facet> &facets, int direction);
    void gouraud_shading(int direction, int phong, glm::vec3 f);
    void draw_pixel(int half_tone, int x, int y);

};


#endif //PROJECT3_POLYGON_H
