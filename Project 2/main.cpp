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

int poly_id;
char choice;

struct Bounding_Box {
    GLfloat x_min, x_max, y_min, y_max, z_min, z_max;
};
Bounding_Box boundingBox;

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
file_name = "test_scene";
read_file(file_name);
normalize();
poly_id = 0;

//    for (int i = 0; i < polygons.size(); i++) {
//        polygons[i].scale3D(1, 1, 1);
//        polygons[i].initialize_bounding();
//        polygons[i].set_bounding();
//        polygons[i].apply_bounding();
//    }

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
    if (polygons.size() == 0) {
        return;
    }
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

    if (file_name == "") {
        return;
    }

    //定义在左上角的区域, xy plane
    p_origion.x = 0;
    p_origion.y = win_height / 2;
    glColor3f(0, 0, 1);
    glViewport(p_origion.x, p_origion.y, win_width / 2, win_width / 2);
    if (poly_id == 0) {
        // draw rotate_vector
        if (choice == 'R') {
            glBegin(GL_LINES);
            glColor3f(1.0, 0.0, 0.0);
            glVertex2f(polygons[0].rotate_vector.x * grid_width, polygons[0].rotate_vector.y * grid_height);
            glVertex2f(0, 0);
            glEnd();
        }

        for (int i = 0; i < polygons.size(); i++) {
            Polygon p = polygons[i];
            p.draw_polygon("xy", grid_width, grid_height);
        }
    }
    else {
        if (choice == 'R') {
            glBegin(GL_LINES);
            glColor3f(1.0, 0.0, 0.0);
            glVertex2f(polygons[poly_id - 1].rotate_vector.x * grid_width,
                       polygons[poly_id - 1].rotate_vector.y * grid_height);
            glVertex2f(0, 0);
            glEnd();
        }

        Polygon p = polygons[poly_id -1];
        p.draw_polygon("xy", grid_width, grid_height);
    }


    //定义在右上角的区域, xz plane
    p_origion.x = win_width / 2;
    p_origion.y = win_height / 2;
    glColor3f(0,0,1);
    glViewport(p_origion.x, p_origion.y, win_width/2, win_width/2);
    if (poly_id == 0) {
        // draw rotate_vector
        if (choice == 'R') {
            glBegin(GL_LINES);
            glColor3f(1.0, 0.0, 0.0);
            glVertex2f(polygons[0].rotate_vector.x * grid_width, polygons[0].rotate_vector.z * grid_height);
            glVertex2f(0, 0);
            glEnd();
        }

        for (int i = 0; i < polygons.size(); i++) {
            Polygon p = polygons[i];
            p.draw_polygon("xz", grid_width, grid_height);
        }
    }
    else {
        if (choice == 'R') {
            glBegin(GL_LINES);
            glColor3f(1.0, 0.0, 0.0);
            glVertex2f(polygons[poly_id - 1].rotate_vector.x * grid_width,
                       polygons[poly_id - 1].rotate_vector.z * grid_height);
            glVertex2f(0, 0);
            glEnd();
        }

        Polygon p = polygons[poly_id -1];
        p.draw_polygon("xz", grid_width, grid_height);
    }
    //定义在左下角的区域 yz plane
    p_origion.x = 0;
    p_origion.y = 0;
    p_origion.z = 0;
    glColor3f(0,0,1);
    glViewport(p_origion.x, p_origion.y, win_width/2, win_width/2);
    if (poly_id == 0) {
        // draw rotate_vector
        if (choice == 'R') {
            glBegin(GL_LINES);
            glColor3f(1.0, 0.0, 0.0);
            glVertex2f(polygons[0].rotate_vector.y * grid_width, polygons[0].rotate_vector.z * grid_height);
            glVertex2f(0, 0);
            glEnd();
        }

        for (int i = 0; i < polygons.size(); i++) {
            Polygon p = polygons[i];
            p.draw_polygon("yz", grid_width, grid_height);
        }
    }
    else {
        if (choice == 'R') {
            glBegin(GL_LINES);
            glColor3f(1.0, 0.0, 0.0);
            glVertex2f(polygons[poly_id - 1].rotate_vector.y * grid_width,
                       polygons[poly_id - 1].rotate_vector.z * grid_height);
            glVertex2f(0, 0);
            glEnd();
        }

        Polygon p = polygons[poly_id -1];
        p.draw_polygon("yz", grid_width, grid_height);
    }

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
    choice = ch;
    switch (ch) {
        case '0':
         if (polygons.size() == 0) {
             break;
         }
            poly_id = 0;
            normalize();
            cout << "Choose your manipulation choice.\n"
                 << "Press T: Translate the polygon.\n"
                 << "Press S: Scale the polygon with respond to its centroid. \n"
                 << "Press R: Rotate the polygon with respond to its centroid. \n"
                 << "Press Q: Quit the program.\n"
                 << "Press F: Specify input file. \n"
                 << endl;
            break;
        case '1':
            if (polygons.size() < 1) {
                break;
            }
            poly_id = 1;
            normalize();
            cout << "Choose your manipulation choice.\n"
                 << "Press T: Translate the polygon.\n"
                 << "Press S: Scale the polygon with respond to its centroid. \n"
                 << "Press R: Rotate the polygon with respond to its centroid. \n"
                 << "Press Q: Quit the program.\n"
                 << "Press F: Specify input file. \n"
                 << endl;
            break;
        case '2':
            if (polygons.size() < 2) {
                break;
            }
            poly_id = 2;
            normalize();
            cout << "Choose your manipulation choice.\n"
                 << "Press T: Translate the polygon.\n"
                 << "Press S: Scale the polygon with respond to its centroid. \n"
                 << "Press R: Rotate the polygon with respond to its centroid. \n"
                 << "Press Q: Quit the program.\n"
                 << "Press F: Specify input file. \n"
                 << endl;
            break;
        case '3':
            if (polygons.size() < 3) {
                break;
            }
            poly_id = 3;
            normalize();
            cout << "Choose your manipulation choice.\n"
                 << "Press T: Translate the polygon.\n"
                 << "Press S: Scale the polygon with respond to its centroid. \n"
                 << "Press R: Rotate the polygon with respond to its centroid. \n"
                 << "Press Q: Quit the program.\n"
                 << "Press F: Specify input file. \n"
                 << endl;
            break;
            //translate
        case 'T':
            if (polygons.size() == 0) {
                break;
            }
            GLfloat tx, ty, tz;
            cout << "Input the translate size in x" << endl;
            cin >> tx;
            cout << "Input translate size in y" << endl;
            cin >> ty;
            cout << "Input translate size in z" << endl;
            cin >> tz;
            if (poly_id == 0) {
                for (int i = 0; i < polygons.size(); i++) {
                    polygons[i].translate3D(tx, ty, tz);
                }
            } else {
                polygons[poly_id - 1].translate3D(tx, ty, tz);
            }
            write_file(file_name);
            normalize();
            cout << "Choose your manipulation choice.\n"
                 << "Press T: Translate the polygon.\n"
                 << "Press S: Scale the polygon with respond to its centroid. \n"
                 << "Press R: Rotate the polygon with respond to its centroid. \n"
                 << "Press Q: Quit the program.\n"
                 << "Press F: Specify input file. \n"
                 << endl;
            break;
            // scaling
        case 'S':
            if (polygons.size() == 0) {
                break;
            }
            GLfloat sx, sy, sz;
            cout << "Input scale size in x for " << endl;
            cin >> sx;
            cout << "Input scale size in y for " << endl;
            cin >> sy;
            cout << "Input scale size in z for " << endl;
            cin >> sz;
            if (poly_id == 0) {
                for (int i = 0; i < polygons.size(); i++) {
                    polygons[i].get_centroid();
                    polygons[i].scale3D(sx, sy, sz);
                }
            } else {
                polygons[poly_id - 1].get_centroid();
                polygons[poly_id - 1].scale3D(sx, sy, sz);
            }
            write_file(file_name);
            normalize();
            cout << "Choose your manipulation choice.\n"
                 << "Press T: Translate the polygon.\n"
                 << "Press S: Scale the polygon with respond to its centroid. \n"
                 << "Press R: Rotate the polygon with respond to its centroid. \n"
                 << "Press Q: Quit the program.\n"
                 << "Press F: Specify input file. \n"
                 << endl;

            break;
        case 'R':
            if (polygons.size() == 0) {
                break;
            }
            {
            GLfloat theta;
            Polygon::Point p1, p2;
            string string1;
            string coordinates[3];
            cout << "Input rotate angle theta" << endl;
            cin >> theta;
            cout << "Input p1.x" << endl;
            cin >> p1.x;
            cout << "Input p1.y" << endl;
            cin >> p1.y;
            cout << "Input p1.z" << endl;
            cin >> p1.z;
            cout << "Input p2.x" << endl;
            cin >> p2.x;
            cout << "Input p2.y" << endl;
            cin >> p2.y;
            cout << "Input p2.z" << endl;
            cin >> p2.z;
            if (poly_id == 0) {
                for (int i = 0; i < polygons.size(); i++) {
                    polygons[i].get_centroid();
                    polygons[i].rotate3D(p1, p2, theta);
                }
            } else {
                polygons[poly_id - 1].get_centroid();
                polygons[poly_id - 1].rotate3D(p1, p2, theta);
            }
            write_file(file_name);
            normalize();
            cout << "Choose your manipulation choice.\n"
                 << "Press T: Translate the polygon.\n"
                 << "Press S: Scale the polygon with respond to its centroid. \n"
                 << "Press R: Rotate the polygon with respond to its centroid. \n"
                 << "Press Q: Quit the program.\n"
                 << "Press F: Specify input file. \n"
                 << endl;
            break;
        }
        case 'Q':
            exit(0);
            break;

        case 'F':
            polygons.clear();
            // specify input file
            cout << "Please input the file name('test_scene' or 'bunny_scene') that stores your polygons. \n"
                 << "The file should be under the 'cmake-build-debug' dictionary. \n "
                 << endl;
            cin >> file_name;
            if (!read_file(file_name)) {
                cout << "Cannot find your file." << endl;
                break;
            }

        default:
            //prints out which key the user hit
            if (polygons.size() > 0) {
                cout << "Choose the polygon to manipulate.\n"
                     << "Press 1-" << polygons.size() << " to choose the corresponding polygon.\n"
                     << "Press 0 to choose all polygons" << endl;
            }
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

int read_file(string path) {
    ifstream in(path);
    string line;
    Polygon current_polygon;
    int i = 0;
    int num_of_points = 0;
    int num_of_edges = 0;
    int poly_id = 1;

    // open failure
    if (! in.is_open())
    { return 0;}

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
    return 1;
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
        out << p.poly_edges.size() << endl;
        for (Polygon::Edge edge: p.poly_edges) {
            out << (edge.start_p + 1) << " " << (edge.end_p + 1) << endl;
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


/**
 * initialize bounding box
 */
void initialize_bounding() {
    boundingBox.x_min = 0;
    boundingBox.x_max = 1;
    boundingBox.y_min = 0;
    boundingBox.y_max = 1;
    boundingBox.z_min = 0;
    boundingBox.z_max = 1;
}
void set_bounding(Polygon polygon) {
    for (Polygon::Point point: polygon.poly_points) {
        if (point.x < boundingBox.x_min) {
            boundingBox.x_min = point.x;
        }
        if (point.x > boundingBox.x_max) {
            boundingBox.x_max = point.x;
        }
        if (point.y < boundingBox.y_min) {
            boundingBox.y_min = point.y;
        }
        if (point.y > boundingBox.y_max) {
            boundingBox.y_max = point.y;
        }
        if (point.z < boundingBox.z_min) {
            boundingBox.z_min = point.z;
        }
        if (point.z > boundingBox.z_max) {
            boundingBox.z_max = point.z;
        }
    }
}

/**
 * apply normalization according to bounding box
 */
void apply_bounding(Polygon & polygon) {
    for (int i = 0; i < polygon.poly_points.size(); i++) {
        Polygon::Point point = polygon.poly_points[i];
        polygon.poly_points[i].x = (point.x - boundingBox.x_min) / (boundingBox.x_max - boundingBox.x_min);
        polygon.poly_points[i].y = (point.y - boundingBox.y_min) / (boundingBox.y_max - boundingBox.y_min);
        polygon.poly_points[i].z = (point.z - boundingBox.z_min) / (boundingBox.z_max - boundingBox.z_min);

    }

}

/**
 * according to bounding box
 */
void normalize() {
    initialize_bounding();
    if (poly_id == 0) {
        for (int i = 0; i < polygons.size(); i++) {
            set_bounding(polygons[i]);
        }
        for (int i = 0; i < polygons.size(); i++) {
            apply_bounding(polygons[i]);
        }
    }
    else {
        set_bounding(polygons[poly_id - 1]);
        apply_bounding(polygons[poly_id - 1]);
    }

    cout << "bounding box: [" << boundingBox.x_min << " ," << boundingBox.x_max << "] * ["
         << boundingBox.y_min << " ," << boundingBox.y_max << "] * ["
         << boundingBox.z_min << " ," << boundingBox.z_max << "]" << endl;

}

