#include <GL/glut.h>

#include <iostream>

float *PixelBuffer;
void display();
void recordPixel(int, int, int, int, float, float, float, float*);
int main(int argc, char *argv[])
{
	//allocate new pixel buffer, need initialization!!
	PixelBuffer = new float[200 * 200 * 3];

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	//set window size to 200 * 200
	glutInitWindowSize(200, 200);
	//set window position
	glutInitWindowPosition(100, 100);

	//create and set main window title
	int MainWindow = glutCreateWindow("Hello Graphics!!");
	glClearColor(0, 0, 0, 0); //clears the buffer of OpenGL

        int locX, locY;
        float red, green, blue;

	//query user for point and color
	std::cin >> locX >> locY >> red >> green >> blue;
	std::cout << locX << std::endl << locY << std::endl << blue << std::endl;
        //record in pixel buffer the pixel the user chose
	recordPixel(200, 200, locX, locY, red, green, blue, PixelBuffer);

	//sets display function
	glutDisplayFunc(display);

	glutMainLoop();//main display loop, will display until terminate
	return 0;
}

//main display loop, this function will be called again and again by OpenGL
void display()
{
	//Misc.
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//draws pixel on screen, width and height must match pixel buffer dimension
	glDrawPixels(200, 200, GL_RGB, GL_FLOAT, PixelBuffer);

	//window refresh
	glFlush();

	return;
}

//primitive, unsafe method of altering pixelbuffer w/o good data encapsulation
void recordPixel(int sizeX, int sizeY, 
                 int locX,  int locY,
                 float red, float green, float blue,
                 float* pixelBuffer)
{
	pixelBuffer[3 * (locX + locY * sizeX)    ] = red;
	pixelBuffer[3 * (locX + locY * sizeX) + 1] = green;
	pixelBuffer[3 * (locX + locY * sizeX) + 2] = blue;
	
	return;
}
