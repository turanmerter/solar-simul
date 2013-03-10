//
//  main.cpp
//  Bilgisayar Proje II - Real-Time Computer Graphics
//  Project #I - Solar System Simulation
//
//  Merter TURAN - turanmerter@gmail.com
//  040080204
//
//  Necessaries
//  1 - Planets must orbit around the star.									IMPLEMENTED
//  2 - Planets must orbit around their own axis.							IMPLEMENTED
//  3 - Moons must orbit around their relative planets.						IMPLEMENTED
//  4 - Moons must orbit around their own axis								IMPLEMENTED
//  5 - Scene must be illuminated with the star as the only light source.	NOT IMPLEMENTED
//  6 - You should use perspective projection as the projection method.		IMPLEMENTED
//  7 - The scene must be rendered with Z-Buffer enabled					IMPLEMENTED
//  8 - You should use texture mapping for all the objects.					IMPLEMENTED
//  9 - Position your camera these actions can  seen easily.				IMPLEMENTED
//  10 - The project must  submitted on github.								IMPLEMENTED
//  Bonus
//  1 - Enable your camera follow your planets.								IMPLEMENTED
//  2 - Add a skybox to your scene.											IMPLEMENTED
	

#define LPI 3.141592653589793238462643383
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <OpenGL/gl.h> 
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <SOIL/SOIL.h>
/* for windows usage
#include <windows.h>
#include <gl/gl.h> 
#include <gl/glu.h>
#include <GLUT/glut.h>
#include "SOIL.h"
*/
GLdouble	angle = 0,				// all orbiting calculations use this as a base
			cameraAngleX = 45,		// angle of x axis of the default camera position
			cameraAngleY = 0,		// angle of y axis of the default camera position
			cameraDistance = 60;	// distance of the default camera position from 0,0,0

GLboolean	pause = false,			// movement of objects are on/off
			isLeftClicked = false,	// left mouse button is being pressed or not
			isRightClicked = false,	// right mouse button is being pressed or not
			earthCameraView = false,// user wants to change view point to behind the earth or not
			marsCameraView = false,	// user wants to change view point to behind the mars or not
			orbit = false,			// user wants to see orbit lines or not
			skybox = true;			// user wants to see skybox effect or not

GLuint		slices = 64,			// slice number of the spheres
			stacks = 64,			// stack number of the spheres
			sun,					// int holder for sun texture
			earth,					// int holder for earth texture
			moon,					// int holder for moon texture
			mars,					// int holder for mars texture
			mercury,				// int holder for mercury texture
			sky;					// int holder for skybox texture

// draw the sky box
void DrawSky(void) {

	GLfloat skyBoxDimension= 100.0;

	glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// make the movements change the position of the skybox
		if (earthCameraView) {
			glRotated(-90, 0, 1, 0); 
			glRotated(-angle, 0, 1, 0); 
		} else if (marsCameraView) {
			glRotated(-90, 0, 1, 0); 
			glRotated(angle, 0, 1, 0); 
		} else {
			glRotated(cameraAngleX, 1, 0, 0);
			glRotated(cameraAngleY, 0, 1, 0);
		}

		glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);

		// FRONT
		glBindTexture(GL_TEXTURE_2D, sky);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f(  skyBoxDimension, -skyBoxDimension, -skyBoxDimension );
			glTexCoord2f(1, 0);
			glVertex3f( -skyBoxDimension, -skyBoxDimension, -skyBoxDimension );
			glTexCoord2f(1, 1);
			glVertex3f( -skyBoxDimension,  skyBoxDimension, -skyBoxDimension );
			glTexCoord2f(0, 1);
			glVertex3f(  skyBoxDimension,  skyBoxDimension, -skyBoxDimension );
		glEnd();
 
		// LEFT
		glBindTexture(GL_TEXTURE_2D, sky);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f(  skyBoxDimension, -skyBoxDimension,  skyBoxDimension );
			glTexCoord2f(1, 0);
			glVertex3f(  skyBoxDimension, -skyBoxDimension, -skyBoxDimension );
			glTexCoord2f(1, 1);
			glVertex3f(  skyBoxDimension,  skyBoxDimension, -skyBoxDimension );
			glTexCoord2f(0, 1);
			glVertex3f(  skyBoxDimension,  skyBoxDimension,  skyBoxDimension );
		glEnd();
 
		// BACK
		glBindTexture(GL_TEXTURE_2D, sky);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f( -skyBoxDimension, -skyBoxDimension,  skyBoxDimension );
			glTexCoord2f(1, 0);
			glVertex3f(  skyBoxDimension, -skyBoxDimension,  skyBoxDimension );
			glTexCoord2f(1, 1);
			glVertex3f(  skyBoxDimension,  skyBoxDimension,  skyBoxDimension );
			glTexCoord2f(0, 1);
			glVertex3f( -skyBoxDimension,  skyBoxDimension,  skyBoxDimension );
 
		glEnd();
 
		// RIGHT
		glBindTexture(GL_TEXTURE_2D, sky);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f( -skyBoxDimension, -skyBoxDimension, -skyBoxDimension );
			glTexCoord2f(1, 0);
			glVertex3f( -skyBoxDimension, -skyBoxDimension,  skyBoxDimension );
			glTexCoord2f(1, 1);
			glVertex3f( -skyBoxDimension,  skyBoxDimension,  skyBoxDimension );
			glTexCoord2f(0, 1);
			glVertex3f( -skyBoxDimension,  skyBoxDimension, -skyBoxDimension );
		glEnd();
 
		// TOP
		glBindTexture(GL_TEXTURE_2D, sky);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 1);
			glVertex3f( -skyBoxDimension,  skyBoxDimension, -skyBoxDimension );
			glTexCoord2f(0, 0);
			glVertex3f( -skyBoxDimension,  skyBoxDimension,  skyBoxDimension );
			glTexCoord2f(1, 0);
			glVertex3f(  skyBoxDimension,  skyBoxDimension,  skyBoxDimension );
			glTexCoord2f(1, 1);
			glVertex3f(  skyBoxDimension,  skyBoxDimension, -skyBoxDimension );
		glEnd();
 
		// BOTTOM
		glBindTexture(GL_TEXTURE_2D, sky);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f( -skyBoxDimension, -skyBoxDimension, -skyBoxDimension );
			glTexCoord2f(0, 1);
			glVertex3f( -skyBoxDimension, -skyBoxDimension,  skyBoxDimension );
			glTexCoord2f(1, 1);
			glVertex3f(  skyBoxDimension, -skyBoxDimension,  skyBoxDimension );
			glTexCoord2f(1, 0);
			glVertex3f(  skyBoxDimension, -skyBoxDimension, -skyBoxDimension );
		glEnd();
 
		glPopAttrib();

    glPopMatrix();

}

// draw the orbit lines
void DrawOrbitLines(void) {

	glPushMatrix();
		// disable textures to use coloring
		glDisable(GL_TEXTURE_2D); 

		glRotated(90, 1, 0, 0);
		glColor3ub(255, 255, 255);

		// EARTH'S ORBIT
		glBegin(GL_LINE_LOOP);
			for (int ang = 0; ang < 360; ang += 360 / 100) {
				glVertex2d(cos(ang * LPI / 180) * 15.0, sin(ang * LPI / 180) * 15.0);
			}
		glEnd();

		// MARS' ORBIT
		glBegin(GL_LINE_LOOP);
			for (int ang = 0; ang < 360; ang += 360 / 100) {
				glVertex2d(cos(ang * LPI / 180) * 25.0, sin(ang * LPI / 180) * 25.0);
			}
		glEnd();

		glEnable(GL_TEXTURE_2D);

	glPopMatrix();
}

// draw planets and moons
void DrawSystem(void) {

	GLUquadric* quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);

	// SUN
    glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, sun); // texture binding
		glRotated(-90, 1, 0, 0); // correcting axis direction for texture
		gluSphere( quadric , 5 , slices , stacks ); // object
	glPopMatrix();

	// FIRST PLANET: EARTH
	glPushMatrix();
		glRotated(angle, 0, 1, 0); // orbit around the sun
		glTranslatef(15.f, 0.f, 0.f); // set distance from sun
		glRotated(angle*5, 0, 1, 0); // orbit around own axis
		glBindTexture(GL_TEXTURE_2D, earth); // texture binding
		glRotated(-90, 1, 0, 0); // correcting axis direction for texture
		gluSphere( quadric , 1.5 , slices , stacks ); // object

		// FIRST PLANET'S FIRST MOON
		glPushMatrix();
			glRotated(90, 1, 0, 0); // correcting orbit direction
			glRotated(-10*angle, 0, 1, 0); // orbit around its planet
			glTranslatef(3.f, 0.f, 0.f); // set distance from its planet
			glRotated(-2*angle, 0, 1, 0); // orbit around own axis
			glBindTexture(GL_TEXTURE_2D, moon); // texture binding
			glRotated(-90, 1, 0, 0); // correcting axis direction for texture
			gluSphere( quadric , 0.75 , slices , stacks ); // object
		glPopMatrix();
	
		// FIRST PLANET'S SECOND MOON
		glPushMatrix();
			glRotated(90, 1, 0, 0); // correcting orbit direction
			glRotated(2*angle, 0, 1, 0); // orbit around its planet
			glTranslatef(5.f, 0.f, 0.f); // set distance from its planet
			glRotated(-3*angle, 0, 1, 0); // orbit around own axis
			glBindTexture(GL_TEXTURE_2D, mercury); // texture binding
			glRotated(-90, 1, 0, 0); // correcting axis direction for texture
			gluSphere( quadric , 0.25 , slices , stacks ); // object
		glPopMatrix();

	glPopMatrix();

	// SECOND PLANET: MARS
	glPushMatrix();
		glRotated(-angle, 0, 1, 0); // orbit around the sun
		glTranslatef(25.f, 0.f, 0.f); // set distance from sun
		glRotated(-angle*5, 0, 1, 0); // orbit around own axis
		glBindTexture(GL_TEXTURE_2D, mars); // texture binding
		glRotated(-90, 1, 0, 0); // correcting axis direction for texture
		gluSphere( quadric , 0.75 , slices , stacks ); // object
			
		// SECOND PLANET'S FIRST MOON
		glPushMatrix();
			glRotated(90, 1, 0, 0); // correcting orbit direction
			glRotated(10*angle, 0, 1, 0); // orbit around its planet
			glTranslatef(1.5f, 0.f, 0.f); // set distance from its planet
			glRotated(5*angle, 0, 1, 0); // orbit around own axis
			glBindTexture(GL_TEXTURE_2D, moon); // texture binding
			glRotated(-90, 1, 0, 0); // correcting axis direction for texture
			gluSphere( quadric , 0.25 , slices , stacks ); // object
		glPopMatrix();

	glPopMatrix();
}

// main draw function
// set perspective
// draw objects/lines/skybox
// adjust the camera view points
void Draw(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(55, 800/600, 1, 250);

	if (skybox)
		DrawSky();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (earthCameraView) {
		glRotated(-90, 0, 1, 0); 
		glTranslatef(-22.f, 0.f, 0.f);
		glRotated(-angle, 0, 1, 0); 
	} else if (marsCameraView) {
		glRotated(-90, 0, 1, 0); 
		glTranslatef(-30.f, 0.f, 0.f);
		glRotated(angle, 0, 1, 0); 
	} else {
		// default camera view point
		glTranslated(0, 0, -cameraDistance);
		glRotated(cameraAngleX, 1, 0, 0);
		glRotated(cameraAngleY, 0, 1, 0);
	}

	if (orbit)
		DrawOrbitLines();

	DrawSystem();
    glutSwapBuffers();
    
}

// catch keybord entries
void Key(unsigned char key, int x, int y) {

	switch (key) {
		case 27: // ESC
			exit(0);
            break;
        case 114: // r
			printf("[INFO]  Reset the angle\n");
            angle = 0;
            break;
		case 115: // s
			if (!skybox)
				printf("[INFO]  Skybox is visible\n");
			else
				printf("[INFO]  Skybox is invisible\n");
            skybox = !skybox;
            break;
		case 82: // R
			printf("[INFO]  Reset the camera position\n");
			earthCameraView = false;
			marsCameraView = false;
			cameraAngleX = 45;
			cameraAngleY = 0;
			cameraDistance = 60;
			break;
		case 111: // o
			if (!orbit)
				printf("[INFO]  Orbits are visible\n");
			else
				printf("[INFO]  Orbits are invisible\n");
            orbit = !orbit;
            break;
        case 32: // spacebar
			if (!pause)
				printf("[INFO]  Stop the movement\n");
			else
				printf("[INFO]  Start the movement\n");
            pause = !pause;
            break;
		case 49: // 1
			if (earthCameraView) {
				earthCameraView = false;
			} else if (marsCameraView) {
				printf("[INFO]  Earth Camera View\n");
				marsCameraView = false;
				earthCameraView = true;
			} else {
				printf("[INFO]  Earth Camera View\n");
				earthCameraView = true;
			}
			break;
		case 50: // 2
			if (marsCameraView) {
				marsCameraView = false;
			} else if (earthCameraView) {
				printf("[INFO]  Mars Camera View\n");
				earthCameraView = false;
				marsCameraView = true;
			} else {
				printf("[INFO]  Mars Camera View\n");
				marsCameraView = true;
			}
			break;
	}
}

// catch keybord direction entries
void SpecialKey(int key, int x, int y) {

	if (!earthCameraView && !marsCameraView) {
		switch (key) {
			case GLUT_KEY_LEFT:
				cameraAngleY++;
				break;
			case GLUT_KEY_RIGHT:
				cameraAngleY--;
				break;
			case GLUT_KEY_UP:
				cameraAngleX++;
				break;
			case GLUT_KEY_DOWN:
				cameraAngleX--;
				break;
		}
	}
}

// catch mouse clicks
void Mouse(int button, int state, int x, int y) {

	if (!earthCameraView && !marsCameraView) {
		if (state == 0) {
			if (button == 0) {
				isLeftClicked = true;
			} else if (button == 2) {
				isRightClicked = true;
			}
		} else if (state == 1) {
			if (button == 0) {
				isLeftClicked = false;
			} else if (button == 2) {
				isRightClicked = false;
			}
		} 
	}
}

void timerCallback (int value) {

	if(isLeftClicked && cameraDistance > 10.0) {
        cameraDistance -= 0.5;
    }
    if(isRightClicked && cameraDistance < 225.0) {
        cameraDistance += 0.5;
    }

	if( !pause ) {
        angle += 0.1;
	}
	if (angle == 360.0) {
		angle = 0.0;
	}
    
    glutTimerFunc(10, timerCallback, 0);
    glutPostRedisplay();
}

GLint LoadGLTexture(const char *filename, int width, int height, bool sky = false) {

    GLuint _texture;
    
    _texture = SOIL_load_OGL_texture
	(
     filename,
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT 
     );
    
    if(_texture == 0) {
	    printf( "[ERROR] SOIL loading error: '%s'\n", SOIL_last_result() );
    }
	
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  

	if (sky) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	} else {    
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
    
    return _texture;
}

void LoadTextures() {

	sun = LoadGLTexture("sun.jpg",512,512);
	earth = LoadGLTexture("earth.jpg",1024,512);
	moon = LoadGLTexture("moon.jpg",256,256);
	mars = LoadGLTexture("mars.jpg",512,256);
	mercury = LoadGLTexture("mercury.jpg",1024,512);
	sky = LoadGLTexture("sky.jpg",1440,1440, true);
}

void printInstructions() {

	printf("Bilgisayar Proje II - Real-Time Computer Graphics\n");
	printf("Project #I - Solar System Simulation\n\n");
	printf("Commands:\n");
	printf("-------------\n");
	printf("    r           - reset the orbit angle to 0\n");
	printf("    R           - reset the camera position to default\n");
	printf("    o           - switch orbit visibility\n");
	printf("    s           - switch skybox visibility\n");
	printf("    1           - switch to earth camera view\n");
	printf("    2           - switch to mars camera view\n");
	printf("    Spacebar    - start/stop the movement\n");
	printf("    ESC         - exit the simulation\n\n");
	printf("Commands that work only in default camera position:\n");
	printf("--------------------------------------------------------\n");
	printf("    Left Key    - move camera to left\n");
	printf("    Right Key   - move camera to right\n");
	printf("    Up Key      - move camera to up\n");
	printf("    Down Key    - move camera to down\n");
	printf("    Left Mouse  - zoom in\n");
	printf("    Right Mouse - zoom out\n\n");
	printf("Simulation has been started...\n\n");
}

int main (int argc, char ** argv) {

    GLenum type;
    
	glutInit(&argc, argv);
	glutInitWindowSize(800,600);
	type = GLUT_RGB;
	type |= GLUT_DOUBLE;
	type |= GLUT_DEPTH;
    type |= GLUT_MULTISAMPLE;
	glutInitDisplayMode(type);
	glutCreateWindow("Project I");

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(Draw);
	glutKeyboardFunc(Key);
    glutSpecialFunc(SpecialKey);
	glutMouseFunc(Mouse);

	LoadTextures();
	printInstructions();

	timerCallback(0);
    glutMainLoop();

    return 0;
}

