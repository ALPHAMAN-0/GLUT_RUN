#include <GL/glut.h>

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0); glVertex2f(-0.5f, -0.5f);
        glColor3f(0, 1, 0); glVertex2f( 0.5f, -0.5f);
        glColor3f(0, 0, 1); glVertex2f( 0.0f,  0.5f);
    glEnd();
    glFlush();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("GLUT in Docker");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}