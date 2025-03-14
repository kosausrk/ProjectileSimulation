#include <GL/glut.h>
#include <cmath>
#include <iostream>

// Constants
const float g = 9.8f;         //gravity 
const float PI = 3.14159265f;
const int windowWidth = 800, windowHeight = 600;

// Arrow and bow properties
float arrowX = 0.0f, arrowY = 0.0f; //initial position (V_o)
float speed = 30.0f;                //initial speed (m/s)
float angle = 45.0f;                //initial angle 

bool isLaunched = false;            //Launch state
float elapsedTime = 0.0f;                  //Time for physics simulation

//drawing bow and arrow graphics 
void drawBow() {
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f); //Red color
    glVertex2f(-0.8f, -0.4f);    //Bottom of the bow
    glVertex2f(-0.8f, 0.4f);     //Top of the bow
    glEnd();
}

//arrow graphics
void drawArrow(float x, float y) {
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f); 
    glVertex2f(x, y);            
    glVertex2f(x - 0.2f, y);     
    glEnd();
}

//physics simulation 
void simulateArrowMotion() {
    if (isLaunched) {
        elapsedTime += 0.02f; 
        //projectile motion equations 
        arrowX = speed * cos(angle * PI / 180.0f) * elapsedTime;
        arrowY = speed * sin(angle * PI / 180.0f) * elapsedTime - 0.5f * g * elapsedTime * elapsedTime;

        //end when arrow hits ground 
        if (arrowY <= -1.0f) {
            isLaunched = false;
            arrowX = 0.0f;
            arrowY = 0.0f;
            elapsedTime = 0.0f;
        }
    }
}

//Display callback

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawBow();

    
    if (isLaunched) {
        drawArrow(arrowX - 0.8f, arrowY - 0.4f);
    } else {
        drawArrow(-0.6f, 0.0f);
    }

    glutSwapBuffers();
}

//timer 
void timer(int value) {
    simulateArrowMotion();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); //60 FPS 
}

//keyboard inputs 
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': //++ angle 
            if (!isLaunched && angle < 90.0f) angle += 1.0f;
            break;
        case 's': //-- angle
            if (!isLaunched && angle > 0.0f) angle -= 1.0f;
            break;
        case 'a': //-- speed
            if (!isLaunched && speed > 10.0f) speed -= 1.0f;
            break;
        case 'd'://++ speed
            if (!isLaunched && speed < 100.0f) speed += 1.0f;
            break;
        case ' ': //Launch the arrow
            if (!isLaunched) {
                isLaunched = true;
                elapsedTime = 0.0f;
            }
            break;
    }
}


void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); //coordinate system
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Bow and Arrow Simulation");

    //callbacks 
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0);

    //main loop 
    glutMainLoop();
    return 0;
}
