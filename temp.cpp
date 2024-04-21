

#include <bits/stdc++.h>
#include <GL/glut.h>
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"
#include "imageio.h"

using namespace std;

/*

How to run:

1. Create imageio.o file: g++ -c imageio.cpp -ltiff -lpng
2. TO RUN: g++ 1.cpp -lglut -lGLU -lGL imageio.o -ltiff -lpng

Keyboard Binding:

r/R 		Reset the cube to initial position
+/- 		Increase/Decrease diffuse intensity
a/s 		Zoom in/out
A/S 		Zoom in/out
Arrow keys 	Rotate the cube (Also change the direction of rotation)

Mouse Binding:

drag		Rotate the cube (Also change the direction of rotation)
Scrollwheel Zoom in/out 		

Idle:

Will rotate in the direction of the last update in change of direction of rotation of cube

*/

double rotate_x = 0, rotate_y = 0, zoom = 1;

unsigned char *image;
int width, height, bpp;

int texImageWidth;
int texImageHeight;

// load texture image
// Only works for .png or .tif images.  NULL is returned if errors occurred.
// loadImageRGA() is from imageio library downloaded from Internet.
// file -> image file to load
GLubyte *makeTexImage(char *file){
	int width, height;
	GLubyte *texImage;
	texImage = loadImageRGBA((char*) file, &width, &height);	
	texImageWidth = width;
	texImageHeight = height;
	return texImage;
}

// prints text
// ref. https://www.opengl.org/archives/resources/code/samples/glut_examples/examples/fontdemo.c
// prints the string with a translation (x, y, z), scaling factor s and rotation of rotate degrees along y axis
// pointsize define the width of the line and point, to give a effect of bold
void stroke_output(GLfloat x, GLfloat y, GLfloat z, GLfloat s, GLfloat pointsize, GLfloat rotate, string format){
	va_list args;
	string buffer = format;
	glPushMatrix();
	glTranslatef(x, y, z);
	glPointSize(pointsize);
	glLineWidth(pointsize);
	glRotatef(rotate, 0.0, 1.0, 0.0);
	glScalef(s, s, s);
	for (auto p: buffer)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, p);
	glPopMatrix();
}

// Defining positions of light to be placed at
GLfloat front_left_light_position[] = {-1.0, 0.0, 1.0, 0.0};
GLfloat front_right_light_position[] = {1.0, 0.0, 1.0, 0.0};
GLfloat back_left_light_position[] = {-1.0, 0.0, -1.0, 0.0};
GLfloat back_right_light_position[] = {1.0, 0.0, -1.0, 0.0};

// Defining material properties
// ref. https://www.opengl.org/archives/resources/code/samples/glut_examples/examples/lightlab.c
GLfloat brass_ambient[] =
{0.33, 0.22, 0.03, 1.0}, brass_diffuse[] =
{0.78, 0.57, 0.11, 1.0}, brass_specular[] =
{0.99, 0.91, 0.81, 1.0}, brass_shininess = 27.8;

GLfloat red_plastic_ambient[] =
{0.0, 0.0, 0.0}, red_plastic_diffuse[] =
{0.5, 0.0, 0.0}, red_plastic_specular[] =
{0.7, 0.6, 0.6}, red_plastic_shininess = 32.0;

GLfloat emerald_ambient[] =
{0.0215, 0.1745, 0.0215}, emerald_diffuse[] =
{0.07568, 0.61424, 0.07568}, emerald_specular[] =
{0.633, 0.727811, 0.633}, emerald_shininess = 76.8;

GLfloat slate_ambient[] =
{0.02, 0.02, 0.02}, slate_diffuse[] =
{0.02, 0.01, 0.01}, slate_specular[] =
{0.4, 0.4, 0.4}, slate_shininess = .78125;

float diffuse_intensity = 0.5;
int select_material = 0;

// Selects material to be shown on the screen
void material(){
	if(select_material == 0){
		glMaterialfv(GL_FRONT, GL_AMBIENT, brass_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, brass_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, brass_specular);
  		glMaterialf(GL_FRONT, GL_SHININESS, brass_shininess);
	}
	if(select_material == 1){
		glMaterialfv(GL_FRONT, GL_AMBIENT, emerald_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, emerald_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, emerald_specular);
  		glMaterialf(GL_FRONT, GL_SHININESS, emerald_shininess);
	}
	if(select_material == 2){
		glMaterialfv(GL_FRONT, GL_AMBIENT, red_plastic_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, red_plastic_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, red_plastic_specular);
  		glMaterialf(GL_FRONT, GL_SHININESS, red_plastic_shininess);
	}
	if(select_material == 3){
		glMaterialfv(GL_FRONT, GL_AMBIENT, slate_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, slate_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, slate_specular);
  		glMaterialf(GL_FRONT, GL_SHININESS, slate_shininess);
	}
}

// main function that will be called by the glutDisplayFunc()
void display(void){

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	material();	// selecting material

	GLfloat light_diffuse[] = {diffuse_intensity, diffuse_intensity, diffuse_intensity, 1.0}; 	// Varing Diffuse light intersity
	
	// Enabling Lights 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);

	// Transformations on the cube
	glLoadIdentity();
	glRotatef(rotate_y, 0.0, 1.0, 0.0);
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glScalef(zoom, zoom, zoom);

	// Printing at front face
	stroke_output(-0.35, 0.25, -0.5, 0.001, 5, 0, "sample");

	stroke_output(-0.45, -0.25, -0.5, 0.0015, 8, 0, "text");

	// Printing at back face
	
	stroke_output(0.35, 0.25, 0.5, 0.001, 5, 180, "sample");
	
	stroke_output(0.45, -0.25, 0.5, 0.0015, 8, 180, "text");
	

	// Eabling Texture Environment
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	// Defining faces of the cube and also the texture to be binded on the face
	// ref. https://www.wikihow.com/Make-a-Cube-in-OpenGL

	// Front
	glBegin(GL_QUADS);
		glVertex3f( 0.5, -0.5, -0.5);
		glVertex3f( 0.5,  0.5, -0.5);
		glVertex3f(-0.5,  0.5, -0.5);
		glVertex3f(-0.5, -0.5, -0.5);
	glEnd();

	// Back
	glBegin(GL_QUADS);
		glVertex3f( 0.5, -0.5, 0.5);
		glVertex3f( 0.5,  0.5, 0.5);
		glVertex3f(-0.5,  0.5, 0.5);
		glVertex3f(-0.5, -0.5, 0.5);
	glEnd();

	// Top
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(0.5, -0.5, -0.5);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(0.5,  0.5, -0.5);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(0.5,  0.5,  0.5);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(0.5, -0.5,  0.5);
	glEnd();

	// Bottom
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-0.5, -0.5,  0.5);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-0.5,  0.5,  0.5);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-0.5,  0.5, -0.5);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-0.5, -0.5, -0.5);
	glEnd();

	// Right
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f( 0.5,  0.5,  0.5);
		glTexCoord2f(1.0, 0.0);
		glVertex3f( 0.5,  0.5, -0.5);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-0.5,  0.5, -0.5);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-0.5,  0.5,  0.5);
	glEnd();

	// Left
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f( 0.5, -0.5, -0.5);
		glTexCoord2f(1.0, 0.0);
		glVertex3f( 0.5, -0.5,  0.5);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-0.5, -0.5,  0.5);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-0.5, -0.5, -0.5);
	glEnd();

	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

// Direction of rotation
int axis_x = 0, axis_y = 0;

// takes action on pressing keyboard key
void processKeys(unsigned char key, int x, int y){
	if(key == 'r' or key == 'R'){	// Reset
		rotate_x = 0;
		rotate_y = 0;
	}
	if(key == '+'){					// change intensity
		diffuse_intensity += 0.01;
	}
	if(key == '-'){
		diffuse_intensity -= 0.01;
	}
	// 2 ** (1 / 32) = 1.021897148
	if(key == 'a' or key == 'A'){	// Zoom in/out
		zoom *= 1.021897148;
	}
	if(key == 's' or key == 'S'){
		zoom /= 1.021897148;
	}
	diffuse_intensity = min(diffuse_intensity, (float)1.0);
	diffuse_intensity = max(diffuse_intensity, (float)0.0);
	glutPostRedisplay();
}

// takes action on pressing special keyboard key
// Rotate the cube (Also change the direction of rotation)
void specialKeys(int key, int x, int y){
	if(key == GLUT_KEY_LEFT) {
		rotate_y += 5;
		axis_y = 0;
	}
	if(key == GLUT_KEY_RIGHT){
		rotate_y -= 5;
		axis_y = 1;
	}
	if(key == GLUT_KEY_DOWN){
		rotate_x -= 5;
		axis_x = 1;
	}
	if(key == GLUT_KEY_UP){
		rotate_x += 5;
		axis_x = 0;
	}
	glutPostRedisplay();
}

int last_x = 0, last_y = 0;

// captures drag
// Rotate the cube (Also change the direction of rotation)
void drag(int x, int y){
	if(x > 500 or y > 500 or x < 0 or y < 0)
		return;
	if(abs(x - last_x) > 25 or abs(y - last_y) > 25){
		last_x = x;
		last_y = y;
		return;
	}
	rotate_x += (last_y - y + 0.0) * 0.5;
	rotate_y += (last_x - x + 0.0) * 0.5;
	if((last_y - y) > 0)
		axis_x = 0;
	if((last_y - y) < 0)
		axis_x = 1;
	if((last_x - x) > 0)
		axis_y = 0;
	if((last_x - x) < 0)
		axis_y = 1;
	last_x = x;
	last_y = y;
	glutPostRedisplay();
}

// captures Scrollwheel 
// Zoom in/out
void mouseClick(int btn, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		// 2 ** (1 / 32) = 1.021897148
		if(btn == 3){
			zoom *= 1.021897148;
		}
		if(btn == 4){
			zoom /= 1.021897148;
		}
	}
	glutPostRedisplay();
}

// Captures Idleness
// Adds motion to the cube
void idleCallBack(){
    // Spins the cube around the set axis.
	if(axis_x == 0)	rotate_x += 0.5;
	if(axis_x == 1)	rotate_x -= 0.5;
	if(axis_y == 0)	rotate_y += 0.5;
	if(axis_y == 1)	rotate_y -= 0.5;
    glutPostRedisplay();
}

// Loading texture using imageio
// ref. https://learnopengl.com/Getting-started/Textures
void load_texture(){
	// image = stbi_load("texture.jpg", &width, &height, &bpp, 3);
	unsigned int texture;
	glGenTextures(1, &texture);
	GLubyte *texImage = makeTexImage((char*)"color.png");
	if ( !texImage ) {
		printf("\nError reading image \n");
		return;
	}
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImageWidth, texImageWidth, 0, GL_RGBA, GL_UNSIGNED_BYTE, texImage);
	delete texImage;
}

// Initialize the camera and light position
void init(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glLightfv(GL_LIGHT0, GL_POSITION, front_left_light_position);
	glLightfv(GL_LIGHT1, GL_POSITION, front_right_light_position);
	glLightfv(GL_LIGHT1, GL_POSITION, back_left_light_position);
	glLightfv(GL_LIGHT1, GL_POSITION, back_right_light_position);

	glEnable(GL_LIGHTING);

	glMatrixMode(GL_MODELVIEW);

	gluLookAt(0.0, 0.0, 5.0,  /* eye is at (0,0,5) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.);      /* up is in positive Y direction */
}

// This function is being called whenever any option is selected in menu
void main_menu_select(int value){
	if(value == 666)
		exit(0);
	select_material = value;
	glutPostRedisplay();
}

// Create menu
void create_menu(){
	glutCreateMenu(main_menu_select);
		glutAddMenuEntry("Brass", 0);
		glutAddMenuEntry("Emerald", 1);
		glutAddMenuEntry("Red Plastic", 2);
		glutAddMenuEntry("Slate", 3);
		glutAddMenuEntry("Quit", 666);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv){

	// openGL functions
	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 500);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Cube");
	load_texture();
	init();
	create_menu();
	glutDisplayFunc(display);
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(specialKeys);
	glutMouseFunc(mouseClick);
	glutMotionFunc(drag);
	glutIdleFunc(idleCallBack);
	glutMainLoop();
	// stbi_image_free(image);
	return 0;
}