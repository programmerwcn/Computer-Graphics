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
#include "Clipping.h"
#include "ClippingPolygon.h"
#include <ctype.h>

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
#include "ModelGL.h"
#include "Util.h"
#include "ViewGL.h"

int choice;
int poly_id;
float tx, ty, sx, sy, theta;


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

ClippingPolygon clippingPolygon;
ModelGL myModelGL;
ViewGL viewGl;


Polygon::Point wMin, wMax;


void init();
void idle();
void display();
void draw_pix(int x, int y);
void reshape(int width, int height);
void key(unsigned char ch, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void check();
void read_file(string path);
void write_file(string path);




int main(int argc, char **argv)
{
    //the number of pixels in the grid
    grid_width = 100;
    grid_height = 100;

    //the size of pixels sets the inital window height and width
    //don't make the pixels too large or the screen size will be larger than
    //your display size
    pixel_size = 5;

    /*Window information*/
    win_height = grid_height*pixel_size;
    win_width = grid_width*pixel_size;

    wMin.x = 0;
    wMin.y = 0;
    wMax.x = win_width;
    wMax.y = win_height;

    viewGl.wMin = wMin;
    viewGl.wMax = wMax;


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
    read_file("../test_scene");
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
void display()
{
    //clears the screen
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    //clears the opengl Modelview transformation matrix
    glLoadIdentity();

//
//
switch (choice) {
    case 0:
        if (poly_id == 0) {
            for (Polygon p: myModelGL.polygons) {
                viewGl.raster(p);
            }
        }
        else {
            Polygon p = myModelGL.polygons[poly_id - 1];
            viewGl.raster(p);
        }
        break;
    case 1:
        if (poly_id == 0) {
    for (Polygon p: myModelGL.polygons) {
        viewGl.view_outline("DDA", p);
    }
}
        else {
    Polygon p = myModelGL.polygons[poly_id - 1];
    viewGl.view_outline("DDA",p);
}
        break;
    case 2:
        if (poly_id == 0) {
    for (Polygon p: myModelGL.polygons) {
        viewGl.view_outline("Bres", p);
    }
}
        else {
    Polygon p = myModelGL.polygons[poly_id - 1];
    viewGl.view_outline("Bres",p);
}
        break;
        // translate

    default:
        break;

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

//gets called when a key is pressed on the keyboard
void key(unsigned char ch, int x, int y)
{
    switch(ch)
    {
        case '0':
            poly_id = 0;
            choice = 0;
            cout << "Choose your manipulation choice.\n"
                 << "Press D: Show polygon's outline with DDA algorithm.\n"
                 << "Press B: Show polygon's outline with Bres algorithm.\n"
                 << "Press T: Translate the polygon.\n"
                 << "Press S: Scale the polygon with respond to its centroid. \n"
                 << "Press R: Rotate the polygon with respond to its centroid. \n"
                 << "Press C: Clipping the polygon with input wMin & wMax. \n"
                 << "Press Q: Quit the program.\n"
                 << endl;
//            viewGl.view_outline("DDA", myModelGL.polygons[0]);
            break;
        case '1':
            poly_id = 1;
            choice = 0;
            cout << "Choose your manipulation choice.\n"
                 << "Press D: Show polygon's outline with DDA algorithm.\n"
                 << "Press B: Show polygon's outline with Bres algorithm.\n"
                 << "Press T: Translate the polygon.\n"
                 << "Press S: Scale the polygon with respond to its centroid. \n"
                 << "Press R: Rotate the polygon with respond to its centroid. \n"
                 << "Press C: Clipping the polygon with input wMin & wMax. \n"
                 << "Press Q: Quit the program.\n"
                 << endl;
            break;
        case '2':
            poly_id = 2;
            choice = 0;
            cout << "Choose your manipulation choice.\n"
                 << "Press D: Show polygon's outline with DDA algorithm.\n"
                 << "Press B: Show polygon's outline with Bres algorithm.\n"
                 << "Press T: Translate the polygon.\n"
                 << "Press S: Scale the polygon with respond to its centroid. \n"
                 << "Press R: Rotate the polygon with respond to its centroid. \n"
                 << "Press C: Clipping the polygon with input wMin & wMax. \n"
                 << "Press Q: Quit the program.\n"
                 << endl;
            break;
        case '3':
            poly_id = 3;
            choice = 0;
            cout << "Choose your manipulation choice.\n"
                 << "Press D: Show polygon's outline with DDA algorithm.\n"
                 << "Press B: Show polygon's outline with Bres algorithm.\n"
                 << "Press T: Translate the polygon.\n"
                 << "Press S: Scale the polygon with respond to its centroid. \n"
                 << "Press R: Rotate the polygon with respond to its centroid. \n"
                 << "Press C: Clipping the polygon with input wMin & wMax. \n"
                 << "Press Q: Quit the program.\n"
                 << endl;
            break;
        case '4':
            poly_id = 4;
            choice = 0;
            cout << "Choose your manipulation choice.\n"
                 << "Press D: Show polygon's outline with DDA algorithm.\n"
                 << "Press B: Show polygon's outline with Bres algorithm.\n"
                 << "Press T: Translate the polygon.\n"
                 << "Press S: Scale the polygon with respond to its centroid. \n"
                 << "Press R: Rotate the polygon with respond to its centroid. \n"
                 << "Press C: Clipping the polygon with input wMin & wMax. \n"
                 << "Press Q: Quit the program.\n"
                 << endl;
            break;
        case '5':
            poly_id = 5;
            choice = 0;
            cout << "Choose your manipulation choice.\n"
                 << "Press D: Show polygon's outline with DDA algorithm.\n"
                 << "Press B: Show polygon's outline with Bres algorithm.\n"
                 << "Press T: Translate the polygon.\n"
                 << "Press S: Scale the polygon with respond to its centroid. \n"
                 << "Press R: Rotate the polygon with respond to its centroid. \n"
                 << "Press C: Clipping the polygon with input wMin & wMax. \n"
                 << "Press Q: Quit the program.\n"
                 << endl;
            break;
            // draw outline with DDA
        case 'D':
            choice = 1;
            cout << "Choose your manipulation choice.\n"
                 << "Press D: Show polygon's outline with DDA algorithm.\n"
                 << "Press B: Show polygon's outline with Bres algorithm.\n"
                 << "Press T: Translate the polygon.\n"
                 << "Press S: Scale the polygon with respond to its centroid. \n"
                 << "Press R: Rotate the polygon with respond to its centroid. \n"
                 << "Press C: Clipping the polygon with input wMin & wMax. \n"
                 << "Press Q: Quit the program.\n"
                 << endl;
            break;
            // draw outlinr with Bres
        case 'B':
            choice = 2;
            cout << "Choose your manipulation choice.\n"
                 << "Press D: Show polygon's outline with DDA algorithm.\n"
                 << "Press B: Show polygon's outline with Bres algorithm.\n"
                 << "Press T: Translate the polygon.\n"
                 << "Press S: Scale the polygon with respond to its centroid. \n"
                 << "Press R: Rotate the polygon with respond to its centroid. \n"
                 << "Press C: Clipping the polygon with input wMin & wMax. \n"
                 << "Press Q: Quit the program.\n"
                 << endl;
            break;
            //translate
        case 'T':
            cout << "Input the translate size in x" << endl;
            cin >> tx;
            cout << "Input translate size in y" << endl;
            cin >> ty;
            if (poly_id == 0) {
                for (int i = 0; i < myModelGL.polygons.size(); i++){
                    myModelGL.polygons[i].translate(tx, ty);
                }
            }
            else {
                myModelGL.polygons[poly_id - 1].translate(tx, ty);
            }
            write_file("../test_scene");
            cout << "Choose your manipulation choice.\n"
                 << "Press D: Show polygon's outline with DDA algorithm.\n"
                 << "Press B: Show polygon's outline with Bres algorithm.\n"
                 << "Press T: Translate the polygon.\n"
                 << "Press S: Scale the polygon with respond to its centroid. \n"
                 << "Press R: Rotate the polygon with respond to its centroid. \n"
                 << "Press C: Clipping the polygon with input wMin & wMax. \n"
                 << "Press Q: Quit the program.\n"
                 << endl;
            break;
            // scaling
        case 'S':

            cout << "Input scale size in x for polygon " << endl;
            cin >> sx;
            cout << "Input scale size in y for polygon " << endl;
            cin >> sy;
            if (poly_id == 0) {
                for (int i = 0; i < myModelGL.polygons.size(); i++){
                    myModelGL.polygons[i].compute_centroid();
                    myModelGL.polygons[i].scale(myModelGL.polygons[i].centroid, sx, sy);
                }
            }
            else {
                myModelGL.polygons[poly_id - 1].compute_centroid();
                myModelGL.polygons[poly_id - 1].scale(myModelGL.polygons[poly_id - 1].centroid, sx, sy);
            }
            write_file("../test_scene");
            cout << "Choose your manipulation choice.\n"
                 << "Press D: Show polygon's outline with DDA algorithm.\n"
                 << "Press B: Show polygon's outline with Bres algorithm.\n"
                 << "Press T: Translate the polygon.\n"
                 << "Press S: Scale the polygon with respond to its centroid. \n"
                 << "Press R: Rotate the polygon with respond to its centroid. \n"
                 << "Press C: Clipping the polygon with input wMin & wMax. \n"
                 << "Press Q: Quit the program.\n"
                 << endl;

            break;
        case 'R':
            cout << "Input rotate angle theta" << endl;
            cin >> theta;
            if (poly_id == 0) {
                for (int i = 0; i < myModelGL.polygons.size(); i++){
                    myModelGL.polygons[i].compute_centroid();
                    myModelGL.polygons[i].rotate(myModelGL.polygons[i].centroid, theta);
                }
            }
            else {
                myModelGL.polygons[poly_id - 1].compute_centroid();
                myModelGL.polygons[poly_id - 1].rotate(myModelGL.polygons[poly_id - 1].centroid, theta);
            }
            write_file("../test_scene");
            cout << "Choose your manipulation choice.\n"
                 << "Press D: Show polygon's outline with DDA algorithm.\n"
                 << "Press B: Show polygon's outline with Bres algorithm.\n"
                 << "Press T: Translate the polygon.\n"
                 << "Press S: Scale the polygon with respond to its centroid. \n"
                 << "Press R: Rotate the polygon with respond to its centroid. \n"
                 << "Press C: Clipping the polygon with input wMin & wMax. \n"
                 << "Press Q: Quit the program.\n"
                 << endl;
            break;

        case 'C':
            cout << "Input wMin x" << endl;
            cin >> viewGl.wMin.x;
            cout << "Input wMin y" << endl;
            cin >> viewGl.wMin.y;
            cout << "Input wMax x" << endl;
            cin >> viewGl.wMax.x;
            cout << "Input wMax y" << endl;
            cin >> viewGl.wMax.y;
            cout << "Choose your manipulation choice.\n"
                 << "Press D: Show polygon's outline with DDA algorithm.\n"
                 << "Press B: Show polygon's outline with Bres algorithm.\n"
                 << "Press T: Translate the polygon.\n"
                 << "Press S: Scale the polygon with respond to its centroid. \n"
                 << "Press R: Rotate the polygon with respond to its centroid. \n"
                 << "Press C: Clipping the polygon with input wMin & wMax. \n"
                 << "Press Q: Quit the program.\n"
                 << endl;

            break;

        case 'Q':
            exit(0);

        default:
            //prints out which key the user hit
            cout << "Choose the polygon to manipulate.\n"
                 << "Press 1-5 to choose the corresponding polygon.\n"
                 << "Press 0 to choose all polygons" << endl;
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
void view(Polygon polygon) {
    for(vector<int> pixel : polygon.pixels) {
        draw_pix(pixel[0], pixel[1]);
    }
}

void read_file(string path) {
    ifstream in(path);
    string line;
    Polygon current_polygon;
    int i = 0;
    int num_of_points = 1;
    int poly_id = 1;

    // open failure
    if (! in.is_open())
    { cout << "Error opening file"; exit (1); }

    while (getline(in, line)) {
if (i == 0 && line != "") {
    myModelGL.num_of_polygons = stoi(line);
}
else if (line == "") {
    i++;
}
else {
    i++;
    vector<string> current_line = Util::split(line, " ");
    if (current_line.size() == 1) {
        num_of_points = stoi(line);
        current_polygon.point_num = num_of_points;

    }
    else {
        // set poly points
        Polygon::Point current_point;
        current_point.x = stof(current_line[0]);
        current_point.y = stof(current_line[1]);
        current_polygon.poly_points.push_back(current_point);
        num_of_points -- ;
        // end of poly points
        if (num_of_points == 0) {
            Polygon temp = current_polygon;
            temp.id = poly_id;
            poly_id ++;
            myModelGL.polygons.push_back(temp);
            current_polygon.poly_points.clear();
            current_polygon.point_num = 0;
        }

    }

}
    }
    in.close();

}

void write_file(string path) {
ofstream out;
out.open(path);
out << myModelGL.polygons.size() << endl;
out << "" << endl;
for (Polygon p: myModelGL.polygons) {
    out << p.poly_points.size() << endl;
    for (Polygon::Point point: p.poly_points) {
        out << point.x << " " << point.y << endl;
    }
    out << "" << endl;
}
out.close();
}

