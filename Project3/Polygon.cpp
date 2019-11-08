//
// Created by 吴小宁 on 2019/11/3.
//

#include "Polygon.h"

#include "glm/glm.hpp"
void Polygon::compute_IP_PHONG(GLfloat k_a, GLfloat k_d, GLfloat k_s, GLfloat K, int n, glm::vec3 I_A, glm::vec3 I_L,
           glm::vec3 light_pos, Polygon::Point &p) {
// vector l
    glm::vec3 l_vec = glm::normalize(light_pos - p.position);
// vector n
glm::vec3 n_vec = glm::normalize(p.normal_vector);
// vector r
glm::vec3 r_vec = 2 * glm::dot(l_vec, n_vec) * n_vec - l_vec;
}

void Polygon::compute_facet_normal_vector() {
    int n = facets.size();
    for (int i = 0; i < n; i++) {
        int point1 = facets[i].points[0];
        int point2 = facets[i].points[1];
        int point3 = facets[i].points[2];
        glm::vec3 temp1 = points[point2].position - points[point1].position;
        glm::vec3 temp2 = points[point3].position - points[point1].position;
        facets[i].normal_vector = glm::cross(temp1, temp2);
    }
}

void Polygon::compute_point_normal_vector() {
    int n = points.size();
    for (int i = 0; i < n; i++) {
        vector<glm::vec3> normal_vectors;
        for (Facet facet: facets) {
            for (int point_num: facet.points) {
                if (point_num == i) {
                    normal_vectors.push_back(facet.normal_vector);
                }
            }
        }
        glm::vec3 normal_vector;
        for (glm::vec3 temp: normal_vectors) {
            normal_vector = normal_vector + temp;
        }
        normal_vector.x = normal_vector.x / (int)normal_vectors.size();
        normal_vector.y = normal_vector.y / (int)normal_vectors.size();
        normal_vector.z = normal_vector.z / (int)normal_vectors.size();
        points[i].normal_vector = normal_vector;
    }
}

vector<Polygon::Facet> Polygon::sort_facet( int direction) {
    vector<Facet> visible_facets;
    // 1. back-face culling
    compute_facet_normal_vector();
    visible_facets = this->facets;
    back_face_culling(visible_facets, direction);
    // 2. painter algorithm, far away -> near
    compute_depth(visible_facets, direction);
    sort_facet_by_depth(visible_facets);

    return visible_facets;
}

/**
 * Compute the depth of every facet according to the direction
 * @param direction
 */
void Polygon::compute_depth(vector<Facet> &facets,int direction) {
    int n = facets.size();
    switch (direction) {
        // depth in z, view from front
        case 0:
            for (int i = 0; i < n; i++ ) {
                // find the min depth of all points that make up the facet
                GLfloat min_depth = 1 - points[facets[i].points[0]].position.z;
                for (int point: facets[i].points) {
                    GLfloat temp_depth = 1 - points[point].position.z;
                    if (temp_depth < min_depth) {
                        min_depth = temp_depth;
                    }
                }
                facets[i].depth = min_depth;
            }
            break;

            // depth in y, view from up
        case 1:
            for (int i = 0; i < n; i++ ) {
                // find the min depth of all points that make up the facet
                GLfloat min_depth = 1 - points[facets[i].points[0]].position.y;
                for (int point: facets[i].points) {
                    GLfloat temp_depth = 1 - points[point].position.y;
                    if (temp_depth < min_depth) {
                        min_depth = temp_depth;
                    }
                }
                facets[i].depth = min_depth;
            }
            break;

            // depth in x, view from right
        case 2:
            for (int i = 0; i < n; i++ ) {
                // find the min depth of all points that make up the facet
                GLfloat min_depth = 1 - points[facets[i].points[0]].position.x;
                for (int point: facets[i].points) {
                    GLfloat temp_depth = 1 - points[point].position.x;
                    if (temp_depth < min_depth) {
                        min_depth = temp_depth;
                    }
                }
                facets[i].depth = min_depth;
            }
            break;
    }
}

/**
 * Sort facets according to depth, with bubble sort, form large -> small
 * @param facets
 */
void Polygon::sort_facet_by_depth(vector<Facet> &facets) {
    int n = facets.size();
    for (int i = n - 1; i > 0; i--) {
        for (int j = 0; j < i; j++) {
            if (facets[j].depth < facets[j + 1].depth) {
                Facet temp = facets[j];
                facets[j] = facets[j + 1];
                facets[j + 1] = temp;
            }
        }
    }
}

/**
 * Remove back facets
 * @param facets
 */
void Polygon::back_face_culling(vector<Facet> &facets, int direction) {
    glm::vec3 view_vec;
    switch (direction) {
        // project to xy plane, view in front
        case 0:
            view_vec.x = 0;
            view_vec.y = 0;
            view_vec.z = 1;
            break;
            // project to xz plane, view in up
        case 1:
            view_vec.x = 0;
            view_vec.y = 1;
            view_vec.z = 0;
            break;

            // project to yz plane, view in right
        case 2:
            view_vec.x = 1;
            view_vec.y = 0;
            view_vec.z = 0;
            break;
    }
    for (int i = 0; i < facets.size(); i++) {
        // invisible
        if (glm::dot(facets[i].normal_vector, view_vec) < 0) {
            facets.erase(facets.begin() + i);
            i--;
        }
    }

}

/**
 * projects sorted facets to xy, yz, xz plane
 */
void Polygon::gouraud_shading(int direction, vector<Facet> &visible_facets) {
switch (direction) {
    // project from front to back, to xy plane
    case 0:
        for (Facet facet: visible_facets) {

        }
        break;

}
}