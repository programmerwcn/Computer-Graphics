
#ifdef WIN32
#include <windows.h>
#endif

#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif

// other includes
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <ctype.h>
#include "Polygon.h"
#include "Polygon2D.h"

using namespace std;

/****set in main()****/
//the number of pixels in the grid
int grid_width;
int grid_height;

//the size of pixels sets the inital window height and width
//don't make the pixels too large or the screen size will be larger than
//your display size
float pixel_size;

/*Window information*/
int win_height;
int win_width;
int trans;

int half_tone;

vector<Polygon> polygons;
// file name
string dictionray = "../";
string file_name;

// functions
void init();
void idle();
void display();
void draw_pix(int x, int y);
void reshape(int width, int height);
void key(unsigned char ch, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void check();
vector<string> split(const string& str, const string& delim);
int read_file(string path);
void write_file(string path);
// bounding box
void initialize_bounding();
void set_bounding(Polygon polygon);
void apply_bounding(Polygon & polygon);
void normalize();




int main(int argc, char **argv)
{
    file_name = dictionray + "cube_and_icosahedron.txt";
    read_file(file_name);

    // half_tone
    half_tone = 1;
    //the number of pixels in the grid
    grid_width = 500;
    grid_height = 500;

    trans = 10;

    //the size of pixels sets the inital window height and width
    //don't make the pixels too large or the screen size will be larger than
    //your display size
    pixel_size = 1;
    for (int i = 0; i < polygons.size(); i++) {
        polygons[i].grid_height = grid_height;
        polygons[i].grid_width = grid_width;
        polygons[i].pixel_size = pixel_size;
        polygons[i].half_tone = half_tone;
        // normalize
        polygons[i].compute_facet_normal_vector();
        polygons[i].compute_point_normal_vector();
        polygons[i].compute_Phong_n();
    }




    // phong
    GLfloat  K = 20;
    glm::vec3 I_A(0, 0, 0);
    glm::vec3 I_L(1, 1, 1);
    glm::vec3 k_s(255, 255, 255);
    glm::vec3 f(0, 0, 1), x(1, 1, 1);
    for (int i = 0; i < polygons.size(); i++) {
        for (int j = 0; j < polygons[i].points.size(); j++) {
            polygons[i].compute_IP_PHONG(k_s, K, I_A, I_L, x, f, polygons[i].points[j]);
        }
    }
    for (Polygon::Facet facet: polygons[1].facets) {
        cout << "normal vectors: " << glm::to_string(facet.normal_vector) << endl;
    }
    for (Polygon::Point point: polygons[1].points) {
        cout << "colors: " << glm::to_string(point.I_P) << endl;
    }


    /*Window information*/
    win_height = grid_height * pixel_size;
    win_width = grid_width * pixel_size;

    /*Set up glut functions*/
    /** See https://www.opengl.org/resources/libraries/glut/spec3/spec3.html ***/

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    /*initialize variables, allocate memory, create buffers, etc. */
    //create window of size (win_width x win_height
    glutInitWindowSize(win_width,win_height);
    //windown title is "glut demo"
    glutCreateWindow("glut demo");

    /*defined glut callback functions*/
    glutDisplayFunc(display); //rendering calls here
    glutReshapeFunc(reshape); //update GL on window size change
    glutMouseFunc(mouse);     //mouse button events
    glutMotionFunc(motion);   //mouse movement events
    glutKeyboardFunc(key);    //Keyboard events
    glutIdleFunc(idle);       //Function called while program is sitting "idle"

    //initialize opengl variables
    init();

    //start glut event loop
    glutMainLoop();
    return 0;
}

/*initialize gl stufff*/
void init()
{
    //set clear color (Default background to white)
    glClearColor(1.0,1.0,1.0,1.0);
    //checks for OpenGL errors
    check();
}

//called repeatedly when glut isn't doing anything else
void idle()
{
    //redraw the scene over and over again
    glutPostRedisplay();
}

//this is where we render the screen
void display() {

    Polygon::Point p_origion;
    //clears the screen
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//    //clears the opengl Modelview transformation matrix
//    glLoadIdentity();

//background
    glViewport(0, 0, win_width, win_height);

for (int i = 0; i < grid_width; i++) {
    for (int j = 0; j < grid_height; j++) {
        glBegin(GL_POINTS);
        glColor3f(0, 0, 0);
        glVertex2f(i, j);
        glEnd();
    }
}

    //画分割线，分成四个视区
    glViewport(0, 0, win_width, win_height);
    glBegin(GL_LINES);
    glColor3f(255, 255, 255);
    glVertex2f(0, grid_height / 2);
    glVertex2f(grid_width, grid_height / 2);
    glVertex2f(grid_width / 2, 0);
    glVertex2f(grid_width / 2, grid_height);
    glEnd();

    if (file_name == "") {
        return;
    }

    //定义在左上角的区域, xy plane
    p_origion.x = 0;
    p_origion.y = win_height / 2;
    glColor3f(0, 0, 1);
    glViewport(p_origion.x, p_origion.y, win_width / 2, win_width / 2);
//    // test half_toning
//    Polygon2D polygon2D;
//    polygon2D.half_tone = 1;
//    Polygon2D::Point point1, point2, point3;
//    point1.x = 0;
//    point1.y = 0;
//    point1.I_P = glm::vec3(255, 0, 0);
//    point2.x = 100;
//    point2.y = 0;
//    point2.I_P = glm::vec3(0, 255,0);
//    point3.x = 0;
//    point3.y = 100;
//    point3.I_P = glm::vec3(0,0,255);
//    polygon2D.poly_points.push_back(point1);
//    polygon2D.poly_points.push_back(point2);
//    polygon2D.poly_points.push_back(point3);
////
//    polygon2D.fillPolygon();
 //   polygon2D.draw_pixel(point1);

    polygons[1].gouraud_shading(0);





    //定义在右上角的区域, xz plane
    p_origion.x = win_width / 2;
    p_origion.y = win_height / 2;
    glColor3f(0,0,1);
    glViewport(p_origion.x, p_origion.y, win_width/2, win_width/2);
    polygons[1].gouraud_shading(1);


    //定义在左下角的区域 yz plane
    p_origion.x = 0;
    p_origion.y = 0;
    p_origion.z = 0;
    glColor3f(0,0,1);
    glViewport(p_origion.x, p_origion.y, win_width/2, win_width/2);
    polygons[1].gouraud_shading(2);

    //blits the current opengl framebuffer on the screen
    glutSwapBuffers();
    //checks for opengl errors
    check();
}


//Draws a single "pixel" given the current grid size
//don't change anything in this for project 1
void draw_pix(int x, int y){
    glBegin(GL_POINTS);
    glColor3f(.2,.2,1.0);
    glVertex3f(x+.5,y+.5,0);
    glEnd();
}


/*Gets called when display size changes, including initial craetion of the display*/
void reshape(int width, int height)
{
    /*set up projection matrix to define the view port*/
    //update the ne window width and height
    win_width = width;
    win_height = height;

    //creates a rendering area across the window
    glViewport(0,0,width,height);
    // up an orthogonal projection matrix so that
    // the pixel space is mapped to the grid space
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,grid_width,0,grid_height,-10,10);

    //clear the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //set pixel size based on width, if the aspect ratio
    //changes this hack won't work as well
    pixel_size = width/(float)grid_width;

    //set pixel size relative to the grid cell size
    glPointSize(pixel_size);
    //check for opengl errors
    check();
}

void key(unsigned char ch, int x, int y) {



    //redraw the scene after keyboard input
    glutPostRedisplay();
}



//gets called when a mouse button is pressed
void mouse(int button, int state, int x, int y)
{
    //print the pixel location, and the grid location
    printf ("MOUSE AT PIXEL: %d %d, GRID: %d %d\n",x,y,(int)(x/pixel_size),(int)((win_height-y)/pixel_size));
    switch(button)
    {
        case GLUT_LEFT_BUTTON: //left button
            printf("LEFT ");
            break;
        case GLUT_RIGHT_BUTTON: //right button
            printf("RIGHT ");
        default:
            printf("UNKNOWN "); //any other mouse button
            break;
    }
    if(state !=GLUT_DOWN)  //button released
        printf("BUTTON UP\n");
    else
        printf("BUTTON DOWN\n");  //button clicked

    //redraw the scene after mouse click
    glutPostRedisplay();
}

//gets called when the curser moves accross the scene
void motion(int x, int y)
{
    //redraw the scene after mouse movement
    glutPostRedisplay();
}

//checks for any opengl errors in the previous calls and
//outputs if they are present
void check()
{
    GLenum err = glGetError();
    if(err != GL_NO_ERROR)
    {
        printf("GLERROR: There was an error %s\n",gluErrorString(err) );
        exit(1);
    }
}

int read_file(string path) {
    ifstream in(path);
    string line;
    Polygon current_polygon;
    int i = 0;
    int num_of_points = 0;
    int num_of_facets = 0;
    int poly_id = 1;
    int point_id, facet_id, positions_read, colors_read;

    // open failure
    if (! in.is_open())
    { return 0;}

    while (getline(in, line)) {
        if ((i == 0 && line != "") || line == "") {
            // read in num of polygons | read in empty lines
        } else {
            vector<string> current_line = split(line, " ");
            if (current_line.size() == 1) {
                // read in num of vertices
                if (num_of_points == 0) {
                    num_of_points = stoi(line);
                    positions_read = 0;
                    colors_read = 0;
                    point_id = 0;
                }
                    // read in num of facets
                else if (num_of_facets == 0) {
                    num_of_facets = stoi(line);
                    facet_id = 0;
                }
                    // read in facet specularity
                else if (num_of_facets != 0){
                    current_polygon.facets[facet_id].specularity = stof(line);
                    facet_id ++;
                    // the end of reading in a polygon
                    if (facet_id == num_of_facets) {
                        num_of_points = 0;
                        num_of_facets = 0;
                        Polygon temp = current_polygon;
                        temp.id = poly_id;
                        poly_id++;
                        polygons.push_back(temp);
                        current_polygon.points.clear();
                        current_polygon.facets.clear();
                    }
                }

            }

            else {
                // first, read in points positions until finished all points
                if (positions_read != num_of_points) {
                    Polygon::Point current_point;
                    current_point.x = stof(current_line[0]);
                    current_point.y = stof(current_line[1]);
                    current_point.z = stof(current_line[2]);
                    glm::vec3 pos(current_point.x, current_point.y, current_point.z);
                    current_point.position = pos;
                    current_polygon.points.push_back(current_point);
                    positions_read ++;
                }
                // then, read in points colors until finished all points
                else if (colors_read != num_of_points) {
                    glm::vec3 I_P;
                    I_P.x = stof(current_line[0]);
                    I_P.y = stof(current_line[1]);
                    I_P.z = stof(current_line[2]);
                    current_polygon.points[point_id].I_P = I_P;
                    point_id ++;
                    colors_read ++;
                }
                // finally, read in facets
                else {
                    Polygon::Facet current_facet;
                    current_facet.points.push_back(stoi(current_line[0]) - 1);
                    current_facet.points.push_back(stoi(current_line[1]) - 1);
                    current_facet.points.push_back(stoi(current_line[2]) - 1);
                    current_polygon.facets.push_back(current_facet);

                }
            }
        }
        i++;
    }
    in.close();
    return 1;


}

void write_file(string path) {
//    ofstream out;
//    out.open(path);
//    out << polygons.size() << endl;
//    out << "" << endl;
//    for (Polygon p: polygons) {
//        out << p.poly_points.size() << endl;
//        for (Polygon::Point point: p.poly_points) {
//            out << point.x << " " << point.y << " " << point.z << endl;
//        }
//        out << p.poly_edges.size() << endl;
//        for (Polygon::Edge edge: p.poly_edges) {
//            out << (edge.start_p + 1) << " " << (edge.end_p + 1) << endl;
//        }
//        out << "" << endl;
//    }
//    out.close();
}

vector<string> split(const string& str, const string& delim) {
    vector<string> res;
    if("" == str) return res;
    //先将要切割的字符串从string类型转换为char*类型
    char * strs = new char[str.length() + 1] ; //不要忘了
    strcpy(strs, str.c_str());

    char * d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while(p) {
        string s = p; //分割得到的字符串转换为string类型
        res.push_back(s); //存入结果数组
        p = strtok(NULL, d);
    }

    return res;
}
