#include <cmath>
#include <ctime>
#include <GL/glut.h>
#define M_PI 3.14159265358979323846

// Constants for the clock dimensions
const float CLOCK_RADIUS = 1.0f;
float cx = 0.0, cy = 0.0;
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;

// Function to draw a circle (clock face)
void drawCircle(float radius) {
    glLineWidth(8.0f);
    glBegin(GL_LINE_LOOP);
    glColor3f(0.5f, 0.5f, 0.5f);
    for (int i = 0; i < 360; ++i) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(radius * cos(angle), radius * sin(angle));
    }
    glEnd();
}

// Function to draw clock numbers
void drawClockNumbers() {
    for (int i = 1; i <= 12; ++i) {
        // Adjust angle to start from 12 at the top and move clockwise
        float angle = (90 - i * 30) * M_PI / 180.0f; // Convert to radians

        // Calculate positions slightly inside the clock face
        float x = 0.8f * CLOCK_RADIUS * cos(angle); // Adjust position for larger font
        float y = 0.8f * CLOCK_RADIUS * sin(angle);

        //Set the position for the number
        glRasterPos2f(x, y);

        //Draw the number character by character
        if (i == 12) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '1');
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '2');
        }
        //11, 12
        else if (i >= 10) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '1');
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0' + (i - 10));
        }
        //from 1 to 9
        else {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0' + i);
        }
    }
}

// Function to draw a hand of the clock
void drawHand(float length, float thickness, float angle) {
    glLineWidth(thickness);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(length * cos(angle), length * sin(angle));
    glEnd();
}

// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Draw the clock face
    glColor3f(0.8f, 0.6f, 0.2f);
    drawCircle(CLOCK_RADIUS);

    // Draw clock numbers
    glColor3f(1.0f, 1.0f, 1.0f);
    drawClockNumbers();

    // Get the current time
    time_t now = time(0);
    tm localTime;
    if (localtime_s(&localTime, &now) != 0) {
        return;
    }

    // Corrected angle calculations with reversed direction
    float secondAngle = -1 * (localTime.tm_sec * M_PI / 30.0f - M_PI / 2.0f);
    float minuteAngle = -1 * ((localTime.tm_min + localTime.tm_sec / 60.0f) * M_PI / 30.0f - M_PI / 2.0f);
    float hourAngle = -1 * (((localTime.tm_hour % 12) + localTime.tm_min / 60.0f) * M_PI / 6.0f - M_PI / 2.0f);

    // Draw the hour hand
    glColor3f(1.0f, 1.0f, 1.0f);
    drawHand(0.5f, 3.5f, hourAngle);

    // Draw the minute hand
    glColor3f(1.0f, 1.0f, 1.0f);
    drawHand(0.7f, 4.0f, minuteAngle);

    // Draw the second hand
    glColor3f(0.58f, 0.035f, 0.035f);
    drawHand(0.7f, 2.0f, secondAngle);

    glFlush();
}

// Reshape callback
void reshape(int w, int h) {
    // defualt constrianst -1 1 from x-axis and -1 1 from y-axis
    if (h == 0) h = 1;
    float aspectRatio = (float)w / (float)h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w >= h) {
        gluOrtho2D(-CLOCK_RADIUS * aspectRatio, CLOCK_RADIUS * aspectRatio, -CLOCK_RADIUS, CLOCK_RADIUS);
    }
    else {
        gluOrtho2D(-CLOCK_RADIUS, CLOCK_RADIUS, -CLOCK_RADIUS / aspectRatio, CLOCK_RADIUS / aspectRatio);
    }

    glMatrixMode(GL_MODELVIEW);
}

// Timer callback
void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(1000, timer, 0);
}

// Main function
int main(int argc, char** argv) {
    //GLUT Init
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    //Create Window
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("OpenGL Analog Clock");

    //Call Back Functions 
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(1000, timer, 0);

    //Color Of Window
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    //Main Loop
    glutMainLoop();
    return 0;
}
