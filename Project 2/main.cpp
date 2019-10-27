/*
 * Simple glut demo that can be used as a template for
 * other projects by sai kopparthi
 */


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

//other includes
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

// polygon
vector<Polygon> polygons;

// file name
string file_name;


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
void read_file(string path);
void write_file(string path);



int main(int argc, char **argv)
{

    file_name = "test_scene";
    read_file(file_name);
    Polygon::Point p1, p2;
    p1.x = 0;
    p1.y = 0;
    p1.z = 0;
    p2.x = 0;
    p2.y = 1;
    p2.z = 0;
    GLfloat angle = 0.78;
    for (int i = 0; i < polygons.size(); i++) {
        polygons[i].rotate3D(p1, p2, angle);
    }
    //the number of pixels in the grid
    grid_width = 100;
    grid_height = 100;

    trans = 10;

    //the size of pixels sets the inital window height and width
    //don't make the pixels too large or the screen size will be larger than
    //your display size
    pixel_size = 5;

    /*Window information*/
    win_height = grid_height*pixel_size;
    win_width = grid_width*pixel_size;

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


    //画分割线，分成四个视区
    glViewport(0, 0, win_width, win_height);
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(0, grid_height / 2);
    glVertex2f(grid_width, grid_height / 2);
    glVertex2f(grid_width / 2, 0);
    glVertex2f(grid_width / 2, grid_height);
    glEnd();

    //定义在左上角的区域, xy plane
    p_origion.x = 0;
    p_origion.y = win_height / 2;
    glColor3f(0, 0, 1);
    glViewport(p_origion.x, p_origion.y, win_width / 2, win_width / 2);
    for (int i = 0; i < polygons.size(); i++) {
        Polygon p = polygons[i];
        for (Polygon::Edge edge: p.poly_edges) {
            Polygon::Point p_start = p.poly_points[edge.start_p];
            Polygon::Point p_end = p.poly_points[edge.end_p];
            glBegin(GL_LINES);
            int x0 = trans + p_start.x * grid_width;
            int y0 = trans + p_start.y * grid_height;
            int x1 = trans + p_end.x * grid_width;
            int y1 = trans + p_end.y * grid_height;
            glVertex2f(x0, y0);
            glVertex2f(x1, y1);
            glEnd();
        }
    }


    //定义在右上角的区域, xz plane
    p_origion.x = win_width / 2;
    p_origion.y = win_height / 2;
    glColor3f(0,0,1);
    glViewport(p_origion.x, p_origion.y, win_width/2, win_width/2);
    for (int i = 0; i < polygons.size(); i++) {
        Polygon p = polygons[i];
        for (Polygon::Edge edge: p.poly_edges) {
            Polygon::Point p_start = p.poly_points[edge.start_p];
            Polygon::Point p_end = p.poly_points[edge.end_p];
            glBegin(GL_LINES);
            int x0 = trans + p_start.x * grid_width;
            int y0 = trans + p_start.z * grid_height;
            int x1 = trans + p_end.x * grid_width;
            int y1 = trans + p_end.z * grid_height;
            glVertex2f(x0, y0);
            glVertex2f(x1, y1);
            glEnd();
        }
    }
    //定义在左下角的区域 yz plane
    p_origion.x = 0;
    p_origion.y = 0;
    p_origion.z = 0;
    glColor3f(0,0,1);
    glViewport(p_origion.x, p_origion.y, win_width/2, win_width/2);
    for (int i = 0; i < polygons.size(); i++) {
        Polygon p = polygons[i];
        for (Polygon::Edge edge: p.poly_edges) {
            Polygon::Point p_start = p.poly_points[edge.start_p];
            Polygon::Point p_end = p.poly_points[edge.end_p];
                glBegin(GL_LINES);
                int x0 = trans + p_start.y * grid_width;
                int y0 = trans + p_start.z * grid_height;
                int x1 = trans + p_end.y * grid_width;
                int y1 = trans + p_end.z * grid_height;
                glVertex2f(x0, y0);
                glVertex2f(x1, y1);
                glEnd();
            }
        }


//    glBegin(GL_POLYGON);
//    glColor3f(0.0, 0.0, 1.0);
//    glVertex2f(0,0);
//    glVertex2f(10,10);

    //glEnd();

//
//

//

//
//    //定义在右下角
//    glColor3f(1.0, 1.0, 1.0);
//    glViewport(200, 0, 200, 200);
//    glBegin(GL_POLYGON);
//    glVertex2f(-0.5, -0.5);
//    glVertex2f(-0.5, 0.5);
//    glVertex2f(0.5, 0.5);
//    glVertex2f(0.5, -0.5);
//    glEnd();
//    glFlush();

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

//gets called when a key is pressed on the keyboard
void key(unsigned char ch, int x, int y)
{
    switch(ch)
    {
        default:
            //prints out which key the user hit
            printf("User hit the \"%c\" key\n",ch);
            break;
    }
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

void read_file(string path) {
    ifstream in(path);
    string line;
    Polygon current_polygon;
    int i = 0;
    int num_of_points = 0;
    int num_of_edges = 0;
    int poly_id = 1;

    // open failure
    if (! in.is_open())
    { cout << "Error opening file"; exit (1); }

    while (getline(in, line)) {
        if ((i == 0 && line != "") || line == "") {
            // read in num of polygons | read in empty lines
        } else {
            vector<string> current_line = split(line, " ");
            switch (current_line.size()) {
                case 1:
                    // read in num of polygons
                    if (num_of_points == 0) {
                        num_of_points = stoi(line);
                    }
                        // read in num of polygonss
                    else {
                        num_of_edges = stoi(line);
                    }
                    break;
                    // read in edges
                case 2:
                    Polygon::Edge current_edge;
                    current_edge.start_p = stoi(current_line[0]) - 1;
                    current_edge.end_p = stoi(current_line[1]) - 1;
                    current_polygon.poly_edges.push_back(current_edge);
                    num_of_edges--;
                    // end of polygon
                    if (num_of_edges == 0 && num_of_points) {
                        num_of_points == 0;
                        Polygon temp = current_polygon;
                        temp.id = poly_id;
                        poly_id++;
                        polygons.push_back(temp);
                        current_polygon.poly_points.clear();
                        current_polygon.poly_edges.clear();
                    }
                    break;
                    // read in poly_points
                case 3:
                    // set poly points
                    Polygon::Point current_point;
                    current_point.x = stof(current_line[0]);
                    current_point.y = stof(current_line[1]);
                    current_point.z = stof(current_line[2]);
                    current_polygon.poly_points.push_back(current_point);
                    break;
            }
        }
        i++;
    }
    in.close();
}

void write_file(string path) {
    ofstream out;
    out.open(path);
    out << polygons.size() << endl;
    out << "" << endl;
    for (Polygon p: polygons) {
        out << p.poly_points.size() << endl;
        for (Polygon::Point point: p.poly_points) {
            out << point.x << " " << point.y << " " << point.z << endl;
        }
        out << "" << endl;
    }
    out.close();
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

