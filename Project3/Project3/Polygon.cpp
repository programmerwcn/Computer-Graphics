//
// Created by 吴小宁 on 2019/11/3.
//

#include "Polygon.h"
#include "Polygon2D.h"

#include "glm/glm.hpp"
/**
 *
 * @param k_a ambient reflection coefficient
 * @param k_d diffuse reflection coefficient
 * @param k_s specular reflection coefficient
 * @param K average distance between scene and light (constant)
 * @param n Phong constant (affecting the “size” of high lights)
 * @param I_A normalized ambient light intensity
 * @param I_L normalized light source intensity
 * @param light_pos
 * @param f view point
 * @param p
 */
void Polygon::compute_IP_PHONG(glm::vec3 k_s, GLfloat K, glm::vec3 I_A, GLfloat I_L, glm::vec3 light_pos, glm::vec3 f, Point &p) {
    glm::vec3 I_p;
    glm::vec3 k_d = glm::normalize(p.I_P);
    glm::vec3 max_I_P = I_A + I_L / (glm::distance(f, p.position) + K)  * (k_d  + k_s);

// vector l
    glm::vec3 l_vec = glm::normalize(light_pos - p.position);
// vector n
glm::vec3 n_vec = glm::normalize(p.normal_vector);
// vector r
glm::vec3 r_vec = 2 * glm::dot(n_vec, l_vec) * n_vec - l_vec;
// vector v
glm::vec3 v_vec = f - p.position;
if (!(v_vec.x == 0 && v_vec.y == 0 && v_vec.z == 0)) {
    v_vec = glm::normalize(v_vec);
}


// special cases
// 0. view from back
if (glm::dot(n_vec, v_vec) < 0) {
    I_p = I_A;
}
// 1. light source & viewer on different sides
else if ((glm::dot(n_vec, l_vec) > 0 && glm::dot(n_vec, v_vec) < 0) || (glm::dot(n_vec, l_vec) < 0 && glm::dot(n_vec, v_vec) > 0)) {
    I_p = I_A;
}
// 2. view is outside the reflection zone
else if (glm::dot(r_vec, v_vec) < 0) {
    I_p = I_A + I_L / (glm::distance(f, p.position) + K)  * k_d * (float)(glm::dot(l_vec, n_vec));
}
else {
I_p = I_A + I_L / (glm::distance(f, p.position) + K)  * ( k_d * (float)(glm::dot(l_vec, n_vec))   + k_s * (float)glm::pow(glm::dot(r_vec, v_vec),p.specularity ));
}
//p.I_P = I_p;

//if (!(I_p.x == 0 && I_p.y == 0 && I_p.z == 0)) {
//    I_p = glm::normalize(I_p);
//}
if (max_I_P.x > 1) {
    I_p.x = I_p.x / max_I_P.x;
}
//if (I_p.x < 0) {
//    I_p.x = 0;
//}
if (max_I_P.y > 1) {
    I_p.y = I_p.y / max_I_P.y;
}
if (max_I_P.z > 1) {
    I_p.z = I_p.z / max_I_P.z;
}
//if (I_p.x < 0) {
//    I_p.x = 0;
//}
//if(I_p.y < 0) {
//    I_p.y = 0;
//}
//if (I_p.z < 0) {
//    I_p.z = 0;
//}
p.I_P.x = I_p.x * 255;
    p.I_P.y = I_p.y * 255;
    p.I_P.z = I_p.z * 255;
}

void Polygon::compute_facet_normal_vector() {
    int n = facets.size();
    for (int i = 0; i < n; i++) {
        int point1 = facets[i].points[0];
        int point2 = facets[i].points[1];
        int point3 = facets[i].points[2];
        glm::vec3 temp1 = points[point2].position - points[point1].position;
        glm::vec3 temp2 = points[point3].position - points[point1].position;
        facets[i].normal_vector = glm::normalize(glm::cross(temp1, temp2));
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
        if (!(normal_vector.x == 0 && normal_vector.y == 0 && normal_vector.z == 0)) {
            normal_vector = glm::normalize(normal_vector);
        }
        points[i].normal_vector = normal_vector;
    }
}

/**
 * painter algorithm
 * @param direction
 * @return
 */
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
        // project to xy plane, front --> back
        case 0:
            view_vec.x = 0;
            view_vec.y = 0;
            view_vec.z = 1;
            break;
            // project to xz plane, up --> down
        case 1:
            view_vec.x = 0;
            view_vec.y = 1;
            view_vec.z = 0;
            break;

            // project to yz plane, right --> left
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
 * projects facets to xy, xz, yz plane
 */
void Polygon::gouraud_shading(int direction, int phong, glm::vec3 f) {
    // remove hidden surface
    vector<Facet> visible_facets = sort_facet(direction);

    for (int i = 0; i < visible_facets.size(); i++) {
        if (phong && glm::dot(visible_facets[i].normal_vector, f) < 0) {
            visible_facets.erase(visible_facets.begin() + i);
            i--;
        }
    }

    for (Facet facet: visible_facets) {
        Polygon2D temp_poly;
        temp_poly.pixel_size = pixel_size;
        temp_poly.half_tone = half_tone;
        // projection
        for (int point_num: facet.points) {
            Point point_3d = points[point_num];
            Polygon2D::Point point_2d;
            glm::vec2 point_2d_pos = projection(direction, point_3d.position);
            point_2d.x = point_2d_pos.x;
            point_2d.y = point_2d_pos.y;
            point_2d.I_P = point_3d.I_P;
            point_2d.on = 0;
            temp_poly.poly_points.push_back(point_2d);
        }
// rasterize
        temp_poly.fillPolygon();

    }
}

glm::vec2 Polygon::projection(int direction, glm::vec3 point_3d) {
    glm::vec2 point_2d;
    switch (direction) {
        // project to xy plane, front --> back
        case 0:
            point_2d.x = point_3d.x * grid_width / mega_size ;
            point_2d.y = point_3d.y * grid_height / mega_size;
            break;
            // project to xz plane, up --> down
        case 1:
            point_2d.x = point_3d.x * grid_width /mega_size;
            point_2d.y = point_3d.z * grid_height /mega_size;
            break;
            // project to yz plane, right --> left
        case 2:
            point_2d.x = point_3d.y * grid_width /mega_size;
            point_2d.y = point_3d.z * grid_height / mega_size;
            break;
    }
    return point_2d;
}

void Polygon::compute_Phong_n() {
    compute_facet_normal_vector();
    compute_point_normal_vector();
    for (int i = 0; i < points.size(); i++) {
        Point p = points[i];
        GLfloat specularity = 0;
        int num_of_facets = 0;
        for (Facet facet: facets) {
            for (int num_of_point: facet.points) {
                if (num_of_point == i) {
                    specularity += facet.specularity;
                    num_of_facets++;
                }
            }
        }
        points[i].specularity = specularity / num_of_facets;
//        GLfloat max_product = 0;
//        for (Facet facet: facets) {
//            if (glm::dot(facet.normal_vector, p.normal_vector) > max_product) {
//                // the most similar normal_vectors
//                max_product = glm::dot(facet.normal_vector, p.normal_vector);
//                points[i].specularity = facet.specularity;
//            }
        }
    }
