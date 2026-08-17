#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <math.h>

void drawRect(float x1, float y1, float x2, float y2)
{
    glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
}

void drawEllipse(float cx, float cy, float rx, float ry)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i < 20; i++)
    {
        float pi = 3.1416f;
        float angle = (i * 2 * pi) / 20;
        glVertex2f(cx + rx * cos(angle), cy + ry * sin(angle));
    }
    glEnd();
}

void drawSky()
{
    glBegin(GL_QUADS);
        glColor3f(0.20f, 0.80f, 0.75f);
        glVertex2f(0, 450);
        glVertex2f(800, 450);
        glColor3f(0.68f, 0.95f, 0.90f);
        glVertex2f(800, 165);
        glVertex2f(0, 165);
    glEnd();
}

void drawWave(float baseY, float amp, float freq, float phase)
{
    glColor3f(0.80f, 0.97f, 0.93f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (int x = 0; x <= 800; x += 20)
    {
        float y = baseY + amp * sin(x * freq + phase);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawHill(float baseY, float amp, float freq, float phase, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_QUAD_STRIP);
    for (int x = 0; x <= 800; x += 20)
    {
        float y = baseY + amp * sin(x * freq + phase);
        glVertex2f(x, y);
        glVertex2f(x, 160);
    }
    glEnd();
}

void drawGround()
{
    glColor3f(0.45f, 0.19f, 0.12f);
    drawRect(0, 0, 800, 110);

    glColor3f(0.55f, 0.25f, 0.16f);
    glLineWidth(1.5f);
    for (int line = 0; line < 3; line++)
    {
        float baseY = 25 + line * 30;
        glBegin(GL_LINE_STRIP);
        for (int x = 0; x <= 800; x += 25)
        {
            float y = baseY + 6 * sin(x * 0.03f + line);
            glVertex2f(x, y);
        }
        glEnd();
    }

    glColor3f(0.30f, 0.12f, 0.08f);
    for (int i = 0; i < 14; i++)
    {
        float px = 40 + i * 57;
        float py = 15 + 25 * fmod(i * 37, 90) / 90;
        drawEllipse(px, py, 4, 3);
    }

    glColor3f(0.88f, 0.78f, 0.52f);
    drawRect(0, 110, 800, 165);
}

void drawRock(float cx, float cy, float rx, float ry)
{
    glColor3f(0.62f, 0.47f, 0.28f);
    drawEllipse(cx, cy, rx, ry);
}

void drawCactus(float cx, float baseY, float scale)
{
    glColor3f(0.45f, 0.36f, 0.20f);
    drawRect(cx - 4 * scale, baseY, cx + 4 * scale, baseY + 55 * scale);

    glPushMatrix();
        glTranslatef(cx - 3 * scale, baseY + 30 * scale, 0);
        glRotatef(30, 0, 0, 1);
        drawRect(-3 * scale, 0, 3 * scale, 22 * scale);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(cx + 3 * scale, baseY + 38 * scale, 0);
        glRotatef(-20, 0, 0, 1);
        drawRect(-3 * scale, 0, 3 * scale, 20 * scale);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(cx + 2 * scale, baseY + 45 * scale, 0);
        glRotatef(-50, 0, 0, 1);
        drawRect(-2.5f * scale, 0, 2.5f * scale, 14 * scale);
    glPopMatrix();
}

void drawSmallCactus(float cx, float baseY, float scale)
{
    glColor3f(0.60f, 0.70f, 0.50f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glVertex2f(cx, baseY);
        glVertex2f(cx, baseY + 10 * scale);
        glVertex2f(cx, baseY + 6 * scale);
        glVertex2f(cx - 4 * scale, baseY + 9 * scale);
        glVertex2f(cx, baseY + 7 * scale);
        glVertex2f(cx + 4 * scale, baseY + 10 * scale);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawSky();
    drawWave(300, 8, 0.02f, 0.0f);
    drawWave(255, 6, 0.025f, 1.5f);

    drawHill(230, 25, 0.010f, 0.0f, 0.78f, 0.80f, 0.90f);
    drawHill(210, 20, 0.014f, 2.0f, 0.70f, 0.73f, 0.87f);

    drawSmallCactus(60, 150, 1.2f);
    drawSmallCactus(205, 148, 1.0f);
    drawSmallCactus(355, 150, 1.1f);
    drawSmallCactus(430, 147, 0.9f);
    drawSmallCactus(585, 150, 1.1f);
    drawSmallCactus(655, 148, 1.0f);
    drawSmallCactus(745, 150, 1.0f);

    drawGround();

    drawRock(70, 118, 10, 6);
    drawRock(230, 116, 9, 5);
    drawRock(390, 119, 10, 6);
    drawRock(540, 116, 9, 5);
    drawRock(680, 118, 10, 6);
    drawRock(760, 116, 8, 5);

    drawCactus(120, 110, 1.0f);
    drawCactus(290, 110, 0.9f);
    drawCactus(503, 110, 1.0f);
    drawCactus(690, 110, 0.95f);

    glFlush();
}

void init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 450);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 450);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Desert Scene");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
