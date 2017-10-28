#define STB_IMAGE_IMPLEMENTATION
#include <GLFW/glfw3.h>
#include <math.h>
#include<GL\GL.H>
#include <stdlib.h>
#include <GL/glut.h>
#include <stb_image.h>
#include <string>

using namespace std;

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius);
void draw(int x);
GLuint DrawImage(int turn);
int x = 1;
float colors[10][3] =
{
	{ 1, 0, 0 },
	{ 1, 0.635, 0 },
	{ 1, 0.965, 0 },
	{ 0.118, 1, 0 },
	{ 0, 1, 0.918 },
	{ 0, 0.141, 1 },
	{ 0.514, 0.004, 0.702 },
	{ 1, 0, 0.776 },
	{ 1, 0, 0.471 },
	{ 1, 0, 0 },
};
int main(void)
{
	GLFWwindow *window;
	if (!glfwInit())
		return -1;
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Draw Circles", NULL, NULL);
	if (!window)
		return -1;
	glfwMakeContextCurrent(window);

	glViewport(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
	glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
	glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
	glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1); // essentially set coordinate system
	glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
	glLoadIdentity(); // same as above comment
					  // Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		glEnable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor3f(1.0f, 1.0f, 1.0f);
		double time = 30;
		while (time >= 0.0f) {
			draw(x);
			glColor3f(1.0f, 1.0f, 1.0f);
			GLuint texture;
			texture = DrawImage(x);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);  // Bottom Left Of The Texture and Quad
			glTexCoord2f(1.0f, 0.0f); glVertex3f(500.0f, 0.0f, 0.0f);  // Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f(500.0f, 700.0f, 0.0f);  // Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 700.0f, 0.0f);  // Top Left Of The Texture and Quad
			glEnd();
			glDisable(GL_TEXTURE_2D);
			time--;
		}
		if (x < 10)
			x++;
		// Swap front and back buffers
		glfwSwapBuffers(window);
		// Poll for and process events
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
void drawCircle(GLfloat xP, GLfloat y, GLfloat z, GLfloat radius)
{
	GLfloat twicePi = 2.0f * 3.14;
	GLfloat circleVerticesX[363];
	GLfloat circleVerticesY[363];
	GLfloat circleVerticesZ[363];
	static int count = 1;

	glColor3f(colors[x - 1][0], colors[x - 1][1], colors[x - 1][2]);
	circleVerticesX[0] = xP;
	circleVerticesY[0] = y;
	circleVerticesZ[0] = z;

	for (int i = 1; i < 363; i++) {
		circleVerticesX[i] = xP + (radius * cos(i *  twicePi / 360));
		circleVerticesY[i] = y + (radius * sin(i * twicePi / 360));
		circleVerticesZ[i] = z;
	}
	GLfloat allCircleVertices[(363) * 3];
	for (int i = 0; i <363; i++) {
		allCircleVertices[i * 3] = circleVerticesX[i];
		allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
		allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, allCircleVertices);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 363);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void draw(int x) {
	GLfloat margin, radius, YP = 400;
	radius = (x <= 5 ? (0.25*0.8*SCREEN_WIDTH) / x : (0.25*0.8*SCREEN_WIDTH) / 5);
	margin = (x <= 5 ? (0.5*0.2*SCREEN_WIDTH) / x : (0.5*0.2*SCREEN_WIDTH) / 5);
	for (int i = 1; i <= x; i++) {
		if (i <= 5)
			drawCircle((i)*margin + 2 * radius*(i - 0.5) + 580, YP, 0, radius);
		else
			drawCircle((i - 5)*margin + 2 * radius*(i - 5.5) + 580, YP - 120, 0, radius);
	}
}

GLuint DrawImage(int turn) {
	int width, height, nrChannels;
	GLuint texture;
	string location = "Pics/" + to_string(turn) + ".jpg";
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(location.c_str(), &width, &height, &nrChannels, 0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	return texture;
}