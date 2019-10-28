//
// Created by 吴小宁 on 2019/10/21.
//

#include "Polygon.h"
#include <math.h>

#define ARR_LEN(array) ((sizeof(array)) / (sizeof(array[0])) )

Polygon::Polygon() {
}

/**
 * 1 0 0 0
 * 0 1 0 0
 * 0 0 1 0
 * 0 0 0 1
 * @param matIndent4x4
 */
void Polygon::matrix_set_identity(Matrix4x4 & matIdent){
    GLint row, col;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 4; col++) {
            matIdent[row][col] = (row == col);

        }
    }
}

/**
 * m2 = m1 * m2
 * @param m1
 * @param m2
 */
void Polygon::matrix_multiply(Matrix4x4 & m1, Matrix4x4 & m2) {
    int size = 4;
    GLint row, col;
    Matrix4x4 matTemp;

    for (row = 0; row < size; row ++) {
        for (col = 0; col < size; col ++) {
            GLfloat temp = 0;
            for(int i = 0; i < size; i++) {
                temp += m1[row][i] * m2[i][col];
            }
            matTemp[row][col] = temp;
        }
    }
    for (row = 0; row < size; row ++) {
        for (col = 0; col < size; col ++) {
            m2[row][col] = matTemp[row][col];
        }
    }
}


/**
 * Translate a 3D polygon according to the given vector
 * @param tx
 * @param ty
 * @param tz
 */
void Polygon::translate3D(GLfloat tx, GLfloat ty, GLfloat tz) {
initialize_mat_rot();
    Matrix4x4 mat_trans;
    matrix_set_identity(mat_trans);
    mat_trans[0][3] = tx;
    mat_trans[1][3] = ty;
    mat_trans[2][3] = tz;
    for (int i = 0; i < poly_points.size(); i++) {
        set_mat_rot(i);
        matrix_multiply(mat_trans, mat_rot);
        get_mat_rot(i);
    }
//for (int i = 0; i < poly_points.size(); i++) {
//    poly_points[i].x += tx;
//    poly_points[i].y += ty;
//    poly_points[i].z += tz;
//}
}

void Polygon::initialize_mat_rot() {
    for (int i = 0; i < 4; i++) {
         for (int j = 0; j < 4; j++) {
             mat_rot[i][j] = 0;
         }
    }
}

void Polygon::set_mat_rot(int i) {
    mat_rot [0][0] = poly_points[i].x;
    mat_rot[1][0] = poly_points[i].y;
    mat_rot[2][0] = poly_points[i].z;
    mat_rot[3][0] = 1;
}

void Polygon::get_mat_rot(int i) {
poly_points[i].x = mat_rot[0][0];
poly_points[i].y = mat_rot[1][0];
    poly_points[i].z = mat_rot[2][0];

}

/**
 * rotate polygon with given axis defined by p1 & p2 by angle
 * @param p1
 * @param p2
 * @param angle
 */
void Polygon::rotate3D(Polygon::Point p1, Polygon::Point p2, GLfloat angle) {
    Matrix4x4 mat_quat_rot;
    GLfloat axis_vector_length = sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y) + (p2.z - p1.z) * (p2.z - p1.z));
    GLfloat cos_A = cos(angle);
    GLfloat sin_A = sin(angle);
    GLfloat cos_C = 1 - cos(angle);

    // direction vector
    GLfloat ux = (p2.x - p1.x) / axis_vector_length;
    GLfloat uy = (p2.y - p1.y) / axis_vector_length;
    GLfloat uz = (p2.z - p1.z) / axis_vector_length;

    rotate_vector.x = ux;
    rotate_vector.y = uy;
    rotate_vector.z = uz;

    translate3D((0 - p1.x), (0 - p1.y), (0 - p1.z));

    matrix_set_identity(mat_quat_rot);

    mat_quat_rot[0][0] = ux * ux * cos_C + cos_A;
    mat_quat_rot[0][1] = ux * uy * cos_C - uz * sin_A;
    mat_quat_rot[0][2] = ux * uz * cos_C + uy * sin_A;
    mat_quat_rot[1][0] = uy * ux * cos_C + uz * sin_A;
    mat_quat_rot[1][1] = uy * uy * cos_C + cos_A;
    mat_quat_rot[1][2] = uy * uz * cos_C - ux * sin_A;
    mat_quat_rot[2][0] = uz * ux * cos_C - uy * sin_A;
    mat_quat_rot[2][1] = uz * uy * cos_C + ux * sin_A;
    mat_quat_rot[2][2] = uz * uz * cos_C + cos_A;

    for (int i = 0; i < poly_points.size(); i++) {
        set_mat_rot(i);
        matrix_multiply(mat_quat_rot, mat_rot);
        get_mat_rot(i);
    }

    translate3D(p1.x, p1.y, p1.z);
}

/**
 * compute the centroid of current polygon
 */
void Polygon::get_centroid() {
    GLfloat x = 0, y = 0, z = 0;
    for(Point point: poly_points) {
        x += point.x;
        y += point.y;
        z += point.z;
    }
    centroid.x = x / (GLfloat)poly_points.size();
    centroid.y = y / (GLfloat )poly_points.size();
    centroid.z = z / (GLfloat )poly_points.size();
}

/**
 * Scale the polygon according to centroid and given size
 * @param sx
 * @param sy
 * @param sz
 */
void Polygon::scale3D(GLfloat sx, GLfloat sy, GLfloat sz) {
    Matrix4x4  mat_scale;

    // initialize
    matrix_set_identity(mat_scale);
    mat_scale[0][0] = sx;
    mat_scale[0][3] = (1 - sx) * centroid.x;
    mat_scale[1][1] = sy;
    mat_scale[1][3] = (1 - sy) * centroid.y;
    mat_scale[2][2] = sz;
    mat_scale[2][3] = (1 - sz) * centroid.z;

    initialize_mat_rot();
    for (int i = 0; i < poly_points.size(); i ++) {
        set_mat_rot(i);
        matrix_multiply(mat_scale, mat_rot);
        get_mat_rot(i);
    }
}


void Polygon::draw_polygon(string plane, int grid_width, int grid_height) {
    if (plane == "xy") {
        for (Edge edge: poly_edges) {
            Point p_start = poly_points[edge.start_p];
            Point p_end = poly_points[edge.end_p];
            glBegin(GL_LINES);
            glColor3f(0,0,1);
            int x0 = (p_start.x * 0.8 + 0.1) * grid_width;
            int y0 = (p_start.y * 0.8 + 0.1) * grid_height;
            int x1 = (p_end.x * 0.8 + 0.1) * grid_width;
            int y1 = (p_end.y * 0.8 + 0.1) * grid_height;
            glVertex2f(x0, y0);
            glVertex2f(x1, y1);
            glEnd();
        }
    }
    else if (plane == "yz") {
        for (Edge edge: poly_edges) {
            Point p_start = poly_points[edge.start_p];
            Point p_end = poly_points[edge.end_p];
            glBegin(GL_LINES);
            glColor3f(0,0,1);
            int x0 = (p_start.y * 0.8 + 0.1) * grid_width;
            int y0 = (p_start.z * 0.8 + 0.1) * grid_height;
            int x1 = (p_end.y * 0.8 + 0.1) * grid_width;
            int y1 = (p_end.z * 0.8 + 0.1) * grid_height;
            glVertex2f(x0, y0);
            glVertex2f(x1, y1);
            glEnd();
        }
    }
    else if (plane == "xz") {
        for (Edge edge: poly_edges) {
            Point p_start = poly_points[edge.start_p];
            Point p_end = poly_points[edge.end_p];
            glBegin(GL_LINES);
            glColor3f(0,0,1);
            int x0 = (p_start.x * 0.8 + 0.1) * grid_width;
            int y0 = (p_start.z * 0.8 + 0.1) * grid_height;
            int x1 = (p_end.x * 0.8 + 0.1) * grid_width;
            int y1 = (p_end.z * 0.8 + 0.1) * grid_height;
            glVertex2f(x0, y0);
            glVertex2f(x1, y1);
            glEnd();
        }
    }
}

//void Polygon::clear_rotate_vector() {
//    rotate_vector.x = 0;
//    rotate_vector.y = 0;
//    rotate_vector.z = 0;
//}



