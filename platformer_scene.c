#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>

// Window Size
#define WIDTH  800
#define HEIGHT 450

// Basic Rectangle
void drawRect(float x1, float y1, float x2, float y2)
{
    glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
}

// Basic Circle
void drawCircle(float cx, float cy, float r)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i < 40; i++)
    {
        float angle = 2.0f * 3.1415926f * i / 40.0f;
        glVertex2f(cx + r * cos(angle), cy + r * sin(angle));
    }
    glEnd();
}

// Sky
void drawSky()
{
    glBegin(GL_QUADS);
        glColor3f(0.10f, 0.45f, 0.90f);
        glVertex2f(0, HEIGHT);
        glVertex2f(WIDTH, HEIGHT);
        glColor3f(0.55f, 0.85f, 1.0f);
        glVertex2f(WIDTH, 0);
        glVertex2f(0, 0);
    glEnd();
}

// Sun
void drawSun()
{
    glColor3f(1.0f, 0.85f, 0.35f);
    drawCircle(700, 390, 50);

    glColor3f(1.0f, 0.95f, 0.45f);
    drawCircle(700, 390, 32);

    // Sun rays
    glColor3f(1.0f, 0.90f, 0.45f);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
        glVertex2f(700, 445); glVertex2f(700, 430);
        glVertex2f(700, 330); glVertex2f(700, 350);
        glVertex2f(665, 390); glVertex2f(630, 390);
        glVertex2f(755, 390); glVertex2f(770, 390);
        glVertex2f(660, 430); glVertex2f(650, 440);
        glVertex2f(740, 430); glVertex2f(750, 440);
    glEnd();
}

// Cloud
void drawCloud(float x, float y, float s)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(x, y, 22 * s);
    drawCircle(x + 25 * s, y + 10 * s, 30 * s);
    drawCircle(x + 52 * s, y, 22 * s);
    drawCircle(x + 27 * s, y - 7 * s, 26 * s);
    drawRect(x - 5 * s, y - 22 * s, x + 57 * s, y);
}

// Mountains
void drawMountain(float x, float y, float w, float h)
{
    // mountain
    glColor3f(0.38f, 0.68f, 0.70f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x, y);
        glVertex2f(x + w / 2, y + h);
        glVertex2f(x + w, y);
    glEnd();

    // snow cap
    glColor3f(0.70f, 0.87f, 0.84f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x + w * 0.3f, y + h * 0.45f);
        glVertex2f(x + w / 2, y + h);
        glVertex2f(x + w * 0.7f, y + h * 0.45f);
    glEnd();
}

void drawMountains()
{
    drawMountain(0, 115, 230, 145);
    drawMountain(160, 115, 260, 180);
    drawMountain(370, 115, 230, 140);
    drawMountain(540, 115, 300, 180);
}

// Beautiful Tree
void drawTree(float x, float y, float s)
{
    // trunk
    glColor3f(0.40f, 0.20f, 0.08f);
    drawRect(x - 10 * s, y, x + 10 * s, y + 70 * s);
    glColor3f(0.55f, 0.28f, 0.10f);
    drawRect(x - 5 * s, y, x + 4 * s, y + 65 * s);

    // branches
    glColor3f(0.40f, 0.20f, 0.08f);
    glLineWidth(6.0f);
    glBegin(GL_LINES);
        glVertex2f(x, y + 45 * s);          glVertex2f(x - 25 * s, y + 80 * s);
        glVertex2f(x, y + 50 * s);          glVertex2f(x + 25 * s, y + 80 * s);
        glVertex2f(x - 10 * s, y + 65 * s); glVertex2f(x - 40 * s, y + 100 * s);
        glVertex2f(x + 10 * s, y + 65 * s); glVertex2f(x + 40 * s, y + 100 * s);
    glEnd();

    // leaves - dark
    glColor3f(0.06f, 0.35f, 0.10f);
    drawCircle(x - 35 * s, y + 90 * s, 30 * s);
    drawCircle(x + 35 * s, y + 90 * s, 25 * s);
    drawCircle(x - 15 * s, y + 115 * s, 28 * s);
    drawCircle(x + 15 * s, y + 115 * s, 28 * s);
    drawCircle(x, y + 100 * s, 32 * s);
    drawCircle(x, y + 130 * s, 30 * s);

    // leaves - medium
    glColor3f(0.14f, 0.55f, 0.17f);
    drawCircle(x - 25 * s, y + 100 * s, 20 * s);
    drawCircle(x + 25 * s, y + 100 * s, 20 * s);
    drawCircle(x - 5 * s, y + 125 * s, 22 * s);
    drawCircle(x + 5 * s, y + 125 * s, 22 * s);

    // leaves - light
    glColor3f(0.30f, 0.72f, 0.22f);
    drawCircle(x - 15 * s, y + 110 * s, 12 * s);
    drawCircle(x + 15 * s, y + 110 * s, 12 * s);
    drawCircle(x, y + 140 * s, 12 * s);
}

// Bush
void drawBush(float x, float y, float s)
{
    glColor3f(0.05f, 0.30f, 0.08f);
    drawCircle(x - 25 * s, y, 20 * s);
    drawCircle(x + 25 * s, y, 20 * s);
    drawCircle(x, y + 15 * s, 25 * s);

    glColor3f(0.14f, 0.55f, 0.17f);
    drawCircle(x - 15 * s, y + 5 * s, 15 * s);
    drawCircle(x + 15 * s, y + 5 * s, 15 * s);
    drawCircle(x, y + 20 * s, 18 * s);
}

// Grass
void drawGrass(float x1, float x2, float y)
{
    glColor3f(0.10f, 0.40f, 0.10f);
    drawRect(x1, y - 5, x2, y + 3);

    glColor3f(0.22f, 0.68f, 0.25f);
    glBegin(GL_TRIANGLES);
    for (float x = x1; x < x2; x += 14)
    {
        glVertex2f(x, y);
        glVertex2f(x + 3, y + 14);
        glVertex2f(x + 7, y);
    }
    glEnd();

    glColor3f(0.55f, 0.85f, 0.25f);
    glBegin(GL_TRIANGLES);
    for (float x = x1 + 5; x < x2; x += 25)
    {
        glVertex2f(x, y);
        glVertex2f(x + 4, y + 9);
        glVertex2f(x + 8, y);
    }
    glEnd();
}

// Flower
void drawFlower(float x, float y, float r)
{
    glColor3f(1.0f, 0.60f, 0.75f);
    drawCircle(x - r, y, r);
    drawCircle(x + r, y, r);
    drawCircle(x, y - r, r);
    drawCircle(x, y + r, r);

    glColor3f(1.0f, 0.82f, 0.10f);
    drawCircle(x, y, r * 0.6f);
}

// Brick ground band spanning the full window width, capped with grass
void drawGround(float y0, float y1)
{
    glColor3f(0.72f, 0.42f, 0.20f);
    drawRect(0, y0, WIDTH, y1);

    glColor3f(0.50f, 0.28f, 0.12f);
    glLineWidth(1.5f);
    float brickH = (y1 - y0) / 3.0f;
    for (int row = 0; row < 3; row++)
    {
        float by = y0 + row * brickH;
        float offset = (row % 2 == 0) ? 0.0f : 20.0f;

        glBegin(GL_LINES);
            glVertex2f(0, by); glVertex2f(WIDTH, by);
        glEnd();

        glBegin(GL_LINES);
        for (float x = offset; x < WIDTH; x += 40)
        {
            glVertex2f(x, by);
            glVertex2f(x, by + brickH);
        }
        glEnd();
    }

    drawGrass(0, WIDTH, y1);
}

// Floating brick platform, capped with grass
void drawPlatform(float x1, float x2, float y0, float y1)
{
    glColor3f(0.72f, 0.42f, 0.20f);
    drawRect(x1, y0, x2, y1);

    glColor3f(0.50f, 0.28f, 0.12f);
    glLineWidth(1.5f);
    float brickH = (y1 - y0) / 2.0f;
    for (int row = 0; row < 2; row++)
    {
        float by = y0 + row * brickH;
        float offset = (row % 2 == 0) ? 0.0f : 15.0f;

        glBegin(GL_LINES);
            glVertex2f(x1, by); glVertex2f(x2, by);
        glEnd();

        glBegin(GL_LINES);
        for (float x = x1 + offset; x < x2; x += 30)
        {
            glVertex2f(x, by);
            glVertex2f(x, by + brickH);
        }
        glEnd();
    }

    drawGrass(x1, x2, y1);
}

// Wooden crate
void drawCrate(float cx, float baseY, float s)
{
    float half = 22 * s;

    glColor3f(0.62f, 0.42f, 0.18f);
    drawRect(cx - half, baseY, cx + half, baseY + 2 * half);

    glColor3f(0.40f, 0.24f, 0.08f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(cx - half, baseY);
        glVertex2f(cx + half, baseY);
        glVertex2f(cx + half, baseY + 2 * half);
        glVertex2f(cx - half, baseY + 2 * half);
    glEnd();

    glBegin(GL_LINES);
        glVertex2f(cx - half, baseY);           glVertex2f(cx + half, baseY + 2 * half);
        glVertex2f(cx - half, baseY + 2 * half); glVertex2f(cx + half, baseY);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawSky();
    drawCloud(120, 390, 0.7f);
    drawCloud(330, 420, 0.55f);
    drawCloud(560, 350, 0.6f);
    drawSun();

    drawMountains();

    drawGround(0, 115);

    drawTree(110, 115, 1.3f);

    drawCrate(300, 115, 1.0f);
    drawBush(250, 133, 0.9f);

    drawPlatform(430, 560, 180, 205);
    drawCrate(495, 205, 1.1f);

    drawPlatform(640, 740, 150, 170);
    drawCrate(690, 170, 0.9f);

    drawBush(600, 133, 1.0f);
    drawBush(770, 131, 0.85f);

    drawFlower(70, 122, 5);
    drawFlower(180, 120, 4);
    drawFlower(370, 121, 4.5f);
    drawFlower(590, 120, 4);
    drawFlower(700, 122, 5);

    glFlush();
}

void init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Platformer Scene");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
