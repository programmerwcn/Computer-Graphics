
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

int half_tone, phong;
glm::vec3 I_A, f, light_pos, k_s;
GLfloat K, I_L;

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
void draw_poly();




int main(int argc, char **argv)
{
    file_name = dictionray + "cube_and_icosahedron.txt";
    read_file(file_name);

    // half_tone
    //half_tone = 1;
    // phong
    phong = 1;
    //the number of pixels in the grid
    grid_width = 500;
    grid_height = 500;

    trans = 10;

    //the size of pixels sets the inital window height and width
    //don't make the pixels too large or the screen size will be larger than
    //your display size
    pixel_size = 2;
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

        K = 2;
        I_A = glm::vec3(0.1, 0.1, 0.1);
        I_L = 1;
        k_s = glm::vec3(0.75,0.75,0.75);
        f = glm::vec3(0, 0, 1);
       light_pos = glm::vec3(0, 0, 2);



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

//draw_poly();
}

void draw_poly() {
    Polygon::Point p_origion;
    //clears the screen
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//    //clears the opengl Modelview transformation matrix
//    glLoadIdentity();

//background
if (phong) {

        for (int i = 0; i < polygons.size(); i++) {
            for (int j = 0; j < polygons[i].points.size(); j++) {
                polygons[i].compute_IP_PHONG(k_s, K, I_A, I_L, light_pos, f, polygons[i].points[j]);
            }
        }

        // draw back_ground
    glViewport(0, 0, win_width, win_height);
    glm::vec3 background;
    if (!(I_A.x == 0 && I_A .y == 0 && I_A.z == 0)) {
        background = glm::normalize(I_A);
    }
    for (int i = 0; i < grid_width; i++) {
        for (int j = 0; j < grid_height; j++) {
            glBegin(GL_POINTS);
            glColor3f(background.x, background.y, background.z);
            glVertex2f(i, j);
            glEnd();
        }
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
//    Polygon2D polygon2D;
//    polygon2D.half_tone = 1;
//    Polygon2D::Point point1, point2, point3;
//    point1.x = 20;
//    point1.y = 0;
//    point1.I_P = glm::vec3(120, 20, 0);
//    point2.x = 0;
//    point2.y = 30;
//    point2.I_P = glm::vec3(0, 200,0);
//    point3.x = 0;
//    point3.y = 0;
//    point3.I_P = glm::vec3(0,0,-100);
//    polygon2D.poly_points.push_back(point1);
//    polygon2D.poly_points.push_back(point2);
//    polygon2D.poly_points.push_back(point3);
//    polygon2D.fillPolygon();
//       polygon2D.draw_pixel(point1);

    for (Polygon polygon: polygons) {
        polygon.gouraud_shading(0, phong, f);
    }

    //定义在右上角的区域, xz plane
    p_origion.x = win_width / 2;
    p_origion.y = win_height / 2;
    glColor3f(0,0,1);
    glViewport(p_origion.x, p_origion.y, win_width/2, win_width/2);
    for (Polygon polygon: polygons) {
        polygon.gouraud_shading(1, phong, f);
    }


    //定义在左下角的区域 yz plane
    p_origion.x = 0;
    p_origion.y = 0;
    p_origion.z = 0;
    glColor3f(0,0,1);
    glViewport(p_origion.x, p_origion.y, win_width/2, win_width/2);
    for (Polygon polygon: polygons) {
        polygon.gouraud_shading(2, phong, f);
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
switch (ch) {
    case 'A':
        if (phong) {
            cout << "Please input the ambient color. ((0,0,0) for black and (1,1,1) for white.)\n" << endl;
            cout << "Input vector.x (from 0 to 1)" << endl;
            cin >> I_A.x;
            cout << "Input vector.y (from 0 to 1)" << endl;
            cin >> I_A.y;
            cout << "Input vector.z (from 0 to 1)" << endl;
            cin >> I_A.z;
            draw_poly();
        }
        break;
    case 'I':
        if(phong) {
            cout << "Please input the intensity of light source (from 0 to 1). " << endl;
            cin >> I_L;
            draw_poly();
        }
        break;
    case 'K':
        if (phong) {
            cout << "Please input the distance from light source to facets" << endl;
            cin >> K;
            draw_poly();
        }
        break;
    case 'E':
        if (phong) {
            cout << "Please input the location of eye.\n" << endl;
            cout << "Input vector.x " << endl;
            cin >> f.x;
            cout << "Input vector.y " << endl;
            cin >> f.y;
            cout << "Input vector.z" << endl;
            cin >> f.z;
            draw_poly();
        }
        break;
    case'L':
        if (phong) {
            cout << "Please input the location of light source.\n" << endl;
            cout << "Input vector.x " << endl;
            cin >> light_pos.x;
            cout << "Input vector.y " << endl;
            cin >> light_pos.y;
            cout << "Input vector.z" << endl;
            cin >> light_pos.z;
            draw_poly();
        }
        break;
    case 'C' :
        if (phong) {
            cout << "Please input the color of light source.\n" << endl;
            cout << "Input vector.x (from 0 to 1)" << endl;
            cin >> k_s.x;
            cout << "Input vector.y (from 0 to 1)" << endl;
            cin >> k_s.y;
            cout << "Input vector.z (from 0 to 1)" << endl;
            cin >> k_s.z;
            draw_poly();
        }
        break;
    case 'P':
        cout << "Do you want to turn on/off the PHONG model?\n"
                "Input 0 to turn it off and 1 to turn it on. \n"
             << endl;
        cin >> phong;
        if (phong) {
          cout << "Default: k_aI_a = (0.1,0.1,0.1), I_L = 1, K = 2, f = (0,0,1), x = (0,0,2), k_s = (0.75,0.75,0.75) \n"
          << "Press A to modify ambient color \n"
          << "Press I to modify light intensity \n"
          << "Press K to modify distance from light to facets \n"
          << "Press E to modify the location of eye(f) \n"
          << "Press L to modify the location of light source \n"
          << "Press C to modify the color of light source \n" << endl;
        }
        else {
            for (int i = 0; i < polygons.size(); i++) {
                for (int j = 0; j < polygons[i].points.size(); j++) {
                    polygons[i].points[j].I_P = polygons[i].points[j].origin_I_p;
                }
            }
        }
        draw_poly();
        break;

    case 'H':
        cout << "Do you want to turn on/off the half-toning?\n"
                "Input 0 to turn it off and 1 to turn it on. \n"
                << endl;
        cin >> half_tone;
        for (int i = 0; i < polygons.size(); i++) {
            polygons[i].half_tone = half_tone;
        }
        draw_poly();
        break;

    case 'F':
        cout << "Please input the file you would like to read in.('bunny.txt' or 'cube_and_icosahedron.txt') \n" << endl;
        cin >> file_name;
        polygons.clear();
        file_name = dictionray + file_name;
        read_file(file_name);

        // half_tone
        half_tone = 0;
        // phong
        phong = 0;
        //the number of pixels in the grid
        grid_width = 500;
        grid_height = 500;

        trans = 10;

        //the size of pixels sets the inital window height and width
        //don't make the pixels too large or the screen size will be larger than
        //your display size
        pixel_size = 2;
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
        draw_poly();
        break;

    case 'Q':
        exit(0);
        break;


    default:
        file_name = "cube_and_icosahedron.txt";
        file_name = dictionray + file_name;
        read_file(file_name);

        // half_tone
        half_tone = 0;
        // phong
        phong = 0;
        //the number of pixels in the grid
        grid_width = 500;
        grid_height = 500;

        trans = 10;

        //the size of pixels sets the inital window height and width
        //don't make the pixels too large or the screen size will be larger than
        //your display size
        pixel_size = 2;
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
        draw_poly();
        cout << "Press F to change read in file (The default one is 'cube_and_icosahedron.txt'). \n"
        << "Press P to turn on/off Phong model and modify it. \n"
        << "Press H to turn on/off half-toning model. \n"
        << "Press Q to quit. \n"
        << endl;
        break;

}


    //redraw the scene after keyboard input
    //glutPostRedisplay();
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
                    current_polygon.points[point_id].origin_I_p = I_P;
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
