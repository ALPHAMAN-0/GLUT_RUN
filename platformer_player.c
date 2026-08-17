#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>

#define WIDTH 800
#define HEIGHT 450

// =====================================================
// CLOUD POSITIONS
// =====================================================

float cloudX1 = 55;
float cloudX2 = 335;
float cloudX3 = 570;
float cloudX4 = 720;

// =====================================================
// RECTANGLE
// =====================================================

void drawRect(float x1, float y1, float x2, float y2)
{
    glBegin(GL_QUADS);

    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);

    glEnd();
}

// =====================================================
// CIRCLE
// =====================================================

void drawCircle(float cx, float cy, float r)
{
    glBegin(GL_POLYGON);

    for (int i = 0; i < 40; i++)
    {
        float pi = 3.1415926f;
        float angle = 2.0f * pi * i / 40.0f;

        glVertex2f(cx + r * cos(angle),
                   cy + r * sin(angle));
    }

    glEnd();
}

// =====================================================
// SKY
// =====================================================

void drawSky()
{
    glBegin(GL_QUADS);

    glColor3f(0.08f, 0.50f, 0.90f);

    glVertex2f(0, 450);
    glVertex2f(800, 450);

    glColor3f(0.40f, 0.84f, 0.95f);

    glVertex2f(800, 90);
    glVertex2f(0, 90);

    glEnd();
}

// =====================================================
// SUN
// =====================================================

void drawSun()
{
    glColor3f(1.0f, 0.70f, 0.08f);
    drawCircle(690, 380, 38);

    glColor3f(1.0f, 0.84f, 0.18f);
    drawCircle(690, 380, 29);

    glColor3f(1.0f, 0.94f, 0.38f);
    drawCircle(690, 380, 21);

    glColor3f(1.0f, 0.78f, 0.10f);

    glLineWidth(3);

    glBegin(GL_LINES);

    glVertex2f(690, 430);
    glVertex2f(690, 416);

    glVertex2f(690, 344);
    glVertex2f(690, 330);

    glVertex2f(640, 380);
    glVertex2f(625, 380);

    glVertex2f(740, 380);
    glVertex2f(755, 380);

    glVertex2f(655, 415);
    glVertex2f(644, 426);

    glVertex2f(725, 415);
    glVertex2f(736, 426);

    glVertex2f(655, 345);
    glVertex2f(644, 334);

    glVertex2f(725, 345);
    glVertex2f(736, 334);

    glEnd();
}

// =====================================================
// BIG CLOUD
// =====================================================

void drawCloud(float x, float y, float s)
{
    // Cloud shadow

    glColor3f(0.78f, 0.90f, 0.95f);

    drawCircle(x - 30 * s, y, 24 * s);
    drawCircle(x, y + 8 * s, 32 * s);
    drawCircle(x + 32 * s, y + 2 * s, 27 * s);
    drawCircle(x + 58 * s, y, 22 * s);

    drawRect(x - 30 * s,
             y - 15 * s,
             x + 58 * s,
             y + 8 * s);

    // White part

    glColor3f(1.0f, 1.0f, 1.0f);

    drawCircle(x - 25 * s,
               y + 5 * s,
               20 * s);

    drawCircle(x,
               y + 15 * s,
               28 * s);

    drawCircle(x + 28 * s,
               y + 10 * s,
               25 * s);

    drawCircle(x + 52 * s,
               y + 5 * s,
               19 * s);

    drawRect(x - 25 * s,
             y - 5 * s,
             x + 52 * s,
             y + 8 * s);
}

// =====================================================
// MOUNTAIN
// =====================================================

void drawMountain(float x, float y, float w, float h)
{
    // Main mountain

    glColor3f(0.18f, 0.55f, 0.64f);

    glBegin(GL_TRIANGLES);

    glVertex2f(x, y);
    glVertex2f(x + w / 2, y + h);
    glVertex2f(x + w, y);

    glEnd();

    // Dark side

    glColor3f(0.10f, 0.42f, 0.53f);

    glBegin(GL_TRIANGLES);

    glVertex2f(x + w / 2, y + h);
    glVertex2f(x + w, y);
    glVertex2f(x + w * 0.52f, y);

    glEnd();

    // Snow

    glColor3f(0.86f, 0.94f, 0.95f);

    glBegin(GL_TRIANGLES);

    glVertex2f(x + w * 0.37f,
               y + h * 0.55f);

    glVertex2f(x + w / 2,
               y + h);

    glVertex2f(x + w * 0.63f,
               y + h * 0.55f);

    glEnd();
}

// =====================================================
// MOUNTAINS
// =====================================================

void drawMountains()
{
    drawMountain(-80, 90, 190, 120);
    drawMountain(80, 90, 210, 145);
    drawMountain(275, 90, 180, 110);
    drawMountain(410, 90, 210, 145);
    drawMountain(635, 90, 200, 120);
}

// =====================================================
// BACKGROUND BUSH
// =====================================================

void drawBackgroundBush(float x, float y, float s)
{
    glColor3f(0.04f, 0.32f, 0.18f);

    drawCircle(x - 22 * s, y, 23 * s);
    drawCircle(x, y + 10 * s, 28 * s);
    drawCircle(x + 22 * s, y, 23 * s);

    glColor3f(0.08f, 0.45f, 0.20f);

    drawCircle(x - 12 * s, y + 7 * s, 16 * s);
    drawCircle(x + 13 * s, y + 8 * s, 17 * s);
}

// =====================================================
// TREE
// =====================================================

void drawTree(float x, float y, float s)
{
    // Trunk

    glColor3f(0.25f, 0.09f, 0.02f);

    glBegin(GL_POLYGON);

    glVertex2f(x - 20 * s, y);
    glVertex2f(x + 20 * s, y);
    glVertex2f(x + 14 * s, y + 115 * s);
    glVertex2f(x - 14 * s, y + 115 * s);

    glEnd();

    // Light trunk

    glColor3f(0.58f, 0.28f, 0.07f);

    glBegin(GL_POLYGON);

    glVertex2f(x - 7 * s, y);
    glVertex2f(x + 7 * s, y);
    glVertex2f(x + 7 * s, y + 110 * s);
    glVertex2f(x - 4 * s, y + 110 * s);

    glEnd();

    // Branches

    glColor3f(0.25f, 0.09f, 0.02f);

    glLineWidth(9);

    glBegin(GL_LINES);

    glVertex2f(x, y + 55 * s);
    glVertex2f(x - 52 * s, y + 120 * s);

    glVertex2f(x, y + 65 * s);
    glVertex2f(x + 52 * s, y + 120 * s);

    glVertex2f(x - 32 * s, y + 100 * s);
    glVertex2f(x - 68 * s, y + 140 * s);

    glVertex2f(x + 32 * s, y + 100 * s);
    glVertex2f(x + 68 * s, y + 140 * s);

    glEnd();

    // Dark leaves

    glColor3f(0.02f, 0.24f, 0.05f);

    drawCircle(x - 55 * s, y + 145 * s, 38 * s);
    drawCircle(x + 55 * s, y + 145 * s, 38 * s);
    drawCircle(x - 30 * s, y + 175 * s, 41 * s);
    drawCircle(x + 30 * s, y + 175 * s, 41 * s);
    drawCircle(x, y + 155 * s, 45 * s);

    // Green leaves

    glColor3f(0.06f, 0.48f, 0.10f);

    drawCircle(x - 52 * s, y + 145 * s, 30 * s);
    drawCircle(x + 52 * s, y + 145 * s, 30 * s);
    drawCircle(x - 27 * s, y + 173 * s, 33 * s);
    drawCircle(x + 27 * s, y + 173 * s, 33 * s);
    drawCircle(x, y + 153 * s, 36 * s);

    // Light leaves

    glColor3f(0.18f, 0.63f, 0.14f);

    drawCircle(x - 35 * s, y + 170 * s, 18 * s);
    drawCircle(x + 32 * s, y + 172 * s, 19 * s);
    drawCircle(x, y + 192 * s, 19 * s);

    // Highlights

    glColor3f(0.40f, 0.78f, 0.20f);

    drawCircle(x - 38 * s, y + 181 * s, 7 * s);
    drawCircle(x + 27 * s, y + 184 * s, 7 * s);
}

// =====================================================
// GRASS
// =====================================================

void drawGrass(float x1, float x2, float y)
{
    glColor3f(0.15f, 0.52f, 0.06f);

    glBegin(GL_TRIANGLES);

    for (float x = x1; x < x2; x += 12)
    {
        glVertex2f(x, y);
        glVertex2f(x + 4, y + 11);
        glVertex2f(x + 8, y);
    }

    glEnd();

    glColor3f(0.48f, 0.80f, 0.12f);

    glBegin(GL_TRIANGLES);

    for (float x = x1 + 6; x < x2; x += 24)
    {
        glVertex2f(x, y);
        glVertex2f(x + 4, y + 8);
        glVertex2f(x + 8, y);
    }

    glEnd();
}

// =====================================================
// BRICK
// =====================================================

void drawBrick(float x1, float y1, float x2, float y2)
{
    // Dark border

    glColor3f(0.36f, 0.11f, 0.035f);

    drawRect(x1, y1, x2, y2);

    // Main brick

    glColor3f(0.69f, 0.28f, 0.11f);

    drawRect(x1 + 2, y1 + 2, x2 - 2, y2 - 2);

    // Highlight

    glColor3f(0.82f, 0.37f, 0.16f);

    drawRect(x1 + 4, y2 - 6, x2 - 4, y2 - 3);

    // Shadow

    glColor3f(0.48f, 0.15f, 0.05f);

    drawRect(x1 + 3, y1 + 2, x2 - 3, y1 + 5);
}

// =====================================================
// GROUND
// =====================================================

void drawGround()
{
    // Brown ground

    glColor3f(0.45f, 0.17f, 0.07f);

    drawRect(0, 0, 800, 70);

    float brickWidth = 70;
    float brickHeight = 23;

    // Three rows of bricks

    for (int row = 0; row < 3; row++)
    {
        float y = row * brickHeight;

        float startX;

        if (row % 2 == 0)
            startX = -35;
        else
            startX = 0;

        for (float x = startX; x < 800; x += brickWidth)
        {
            drawBrick(x + 1,
                      y + 1,
                      x + brickWidth - 1,
                      y + brickHeight - 1);
        }
    }

    // Grass

    glColor3f(0.12f, 0.50f, 0.06f);

    drawRect(0, 70, 800, 92);

    glColor3f(0.45f, 0.80f, 0.12f);

    drawRect(0, 82, 800, 92);

    drawGrass(0, 800, 90);
}

// =====================================================
// BUSH
// =====================================================

void drawBush(float x, float y, float s)
{
    // Dark outline

    glColor3f(0.02f, 0.23f, 0.04f);

    drawCircle(x - 22 * s, y, 20 * s);
    drawCircle(x, y + 13 * s, 25 * s);
    drawCircle(x + 22 * s, y, 20 * s);

    // Main green

    glColor3f(0.07f, 0.47f, 0.10f);

    drawCircle(x - 20 * s, y + 3 * s, 16 * s);
    drawCircle(x, y + 15 * s, 21 * s);
    drawCircle(x + 20 * s, y + 3 * s, 16 * s);

    // Light green

    glColor3f(0.28f, 0.67f, 0.14f);

    drawCircle(x - 9 * s, y + 21 * s, 8 * s);
    drawCircle(x + 11 * s, y + 21 * s, 8 * s);
}

// =====================================================
// FLOWER
// =====================================================

void drawFlower(float x, float y, float r)
{
    glColor3f(1.0f, 0.55f, 0.72f);

    drawCircle(x - r, y, r);
    drawCircle(x + r, y, r);
    drawCircle(x, y - r, r);
    drawCircle(x, y + r, r);

    glColor3f(1.0f, 0.90f, 0.95f);

    drawCircle(x - r * 0.5f, y, r * 0.5f);
    drawCircle(x + r * 0.5f, y, r * 0.5f);

    glColor3f(1.0f, 0.82f, 0.08f);

    drawCircle(x, y, r * 0.55f);
}

// =====================================================
// FLOWERS
// =====================================================

void drawFlowers(float x, float y)
{
    drawFlower(x, y, 7);
    drawFlower(x + 22, y + 2, 5);
    drawFlower(x + 44, y, 6);
}

// =====================================================
// BOX
// =====================================================

void drawBox(float x1, float y1, float x2, float y2)
{
    // Main box

    glColor3f(0.88f, 0.43f, 0.07f);

    drawRect(x1, y1, x2, y2);

    // Border

    glColor3f(0.30f, 0.08f, 0.02f);

    glLineWidth(4);

    glBegin(GL_LINE_LOOP);

    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);

    glEnd();

    // X pattern

    glBegin(GL_LINES);

    glVertex2f(x1 + 5, y1 + 5);
    glVertex2f(x2 - 5, y2 - 5);

    glVertex2f(x1 + 5, y2 - 5);
    glVertex2f(x2 - 5, y1 + 5);

    glEnd();

    // Nails

    glColor3f(0.20f, 0.05f, 0.01f);

    drawCircle(x1 + 6, y1 + 6, 3);
    drawCircle(x2 - 6, y1 + 6, 3);
    drawCircle(x1 + 6, y2 - 6, 3);
    drawCircle(x2 - 6, y2 - 6, 3);
}

// =====================================================
// BEAUTIFUL GIRL - SIDE VIEW
// =====================================================

void drawPlayer(float x, float y)
{
    // =================================================
    // BACK LEG
    // =================================================

    glColor3f(1.0f, 0.72f, 0.52f);

    glBegin(GL_QUADS);

    glVertex2f(x + 3, y + 5);
    glVertex2f(x + 11, y + 5);
    glVertex2f(x + 13, y + 38);
    glVertex2f(x + 5, y + 38);

    glEnd();

    // =================================================
    // FRONT LEG
    // =================================================

    glBegin(GL_QUADS);

    glVertex2f(x - 7, y + 5);
    glVertex2f(x + 1, y + 5);
    glVertex2f(x - 1, y + 40);
    glVertex2f(x - 9, y + 40);

    glEnd();

    // =================================================
    // BACK SHOE
    // =================================================

    glColor3f(0.18f, 0.05f, 0.12f);

    glBegin(GL_POLYGON);

    glVertex2f(x + 5, y + 6);
    glVertex2f(x + 17, y + 6);
    glVertex2f(x + 19, y + 2);
    glVertex2f(x + 7, y + 1);

    glEnd();

    // =================================================
    // FRONT SHOE
    // =================================================

    glBegin(GL_POLYGON);

    glVertex2f(x - 10, y + 6);
    glVertex2f(x + 2, y + 6);
    glVertex2f(x + 5, y + 2);
    glVertex2f(x - 9, y + 1);

    glEnd();

    // =================================================
    // DRESS
    // =================================================

    glColor3f(0.95f, 0.22f, 0.48f);

    glBegin(GL_POLYGON);

    glVertex2f(x - 13, y + 43);
    glVertex2f(x + 8, y + 43);
    glVertex2f(x + 18, y + 8);
    glVertex2f(x - 20, y + 8);

    glEnd();

    // Dress highlight

    glColor3f(1.0f, 0.40f, 0.62f);

    glBegin(GL_POLYGON);

    glVertex2f(x - 8, y + 40);
    glVertex2f(x + 4, y + 40);
    glVertex2f(x + 10, y + 12);
    glVertex2f(x - 12, y + 12);

    glEnd();

    // =================================================
    // DRESS WAIST
    // =================================================

    glColor3f(0.75f, 0.10f, 0.30f);

    drawRect(x - 11, y + 39, x + 8, y + 45);

    // =================================================
    // NECK
    // =================================================

    glColor3f(1.0f, 0.72f, 0.52f);

    drawRect(x - 4, y + 58, x + 5, y + 69);

    // =================================================
    // HEAD
    // =================================================

    glColor3f(1.0f, 0.72f, 0.52f);

    drawCircle(x + 2, y + 84, 18);

    // =================================================
    // EAR
    // =================================================

    glColor3f(0.95f, 0.62f, 0.45f);

    drawCircle(x + 17, y + 83, 5);

    // =================================================
    // LONG HAIR
    // =================================================

    glColor3f(0.12f, 0.045f, 0.02f);

    drawCircle(x - 4, y + 96, 18);

    drawCircle(x - 16, y + 80, 15);
    drawCircle(x - 19, y + 60, 14);
    drawCircle(x - 18, y + 42, 12);

    drawRect(x - 31, y + 42, x - 5, y + 80);

    // =================================================
    // HAIR HIGHLIGHT
    // =================================================

    glColor3f(0.32f, 0.12f, 0.05f);

    glLineWidth(3);

    glBegin(GL_LINES);

    glVertex2f(x - 13, y + 99);
    glVertex2f(x - 24, y + 58);

    glVertex2f(x - 7, y + 98);
    glVertex2f(x - 18, y + 48);

    glEnd();

    // =================================================
    // NOSE
    // =================================================

    glColor3f(1.0f, 0.72f, 0.52f);

    glBegin(GL_TRIANGLES);

    glVertex2f(x + 18, y + 86);
    glVertex2f(x + 27, y + 82);
    glVertex2f(x + 18, y + 78);

    glEnd();

    // =================================================
    // EYE
    // =================================================

    glColor3f(0.02f, 0.02f, 0.02f);

    drawCircle(x + 11, y + 89, 2.5);

    // =================================================
    // EYELASH
    // =================================================

    glLineWidth(2);

    glBegin(GL_LINES);

    glVertex2f(x + 12, y + 91);
    glVertex2f(x + 16, y + 93);

    glEnd();

    // =================================================
    // MOUTH
    // =================================================

    glColor3f(0.65f, 0.10f, 0.18f);

    glBegin(GL_LINES);

    glVertex2f(x + 20, y + 74);
    glVertex2f(x + 25, y + 74);

    glEnd();

    // =================================================
    // ARM
    // =================================================

    glColor3f(1.0f, 0.72f, 0.52f);

    glBegin(GL_QUADS);

    glVertex2f(x + 6, y + 40);
    glVertex2f(x + 13, y + 39);
    glVertex2f(x + 21, y + 19);
    glVertex2f(x + 14, y + 17);

    glEnd();

    // =================================================
    // HAND
    // =================================================

    drawCircle(x + 18, y + 16, 5);

    // =================================================
    // HAIR BAND
    // =================================================

    glColor3f(1.0f, 0.15f, 0.42f);

    glLineWidth(3);

    glBegin(GL_LINES);

    glVertex2f(x - 16, y + 95);
    glVertex2f(x + 8, y + 100);

    glEnd();
}

// =====================================================
// DISPLAY
// =====================================================

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // SKY

    drawSky();

    // SUN

    drawSun();

    // MOVING CLOUDS

    drawCloud(cloudX1, 350, 1.0f);
    drawCloud(cloudX2, 395, 0.9f);
    drawCloud(cloudX3, 340, 1.0f);
    drawCloud(cloudX4, 305, 0.75f);

    // MOUNTAINS

    drawMountains();

    // BACKGROUND BUSHES

    drawBackgroundBush(40, 95, 0.75f);
    drawBackgroundBush(220, 95, 0.70f);
    drawBackgroundBush(400, 95, 0.75f);
    drawBackgroundBush(570, 95, 0.75f);
    drawBackgroundBush(750, 95, 0.70f);

    // TREE

    drawTree(135, 92, 0.70f);

    // GROUND

    drawGround();

    // BUSHES

    drawBush(280, 92, 0.65f);
    drawBush(350, 92, 0.60f);
    drawBush(520, 92, 0.65f);
    drawBush(750, 92, 0.65f);

    // FLOWERS

    drawFlowers(45, 103);
    drawFlowers(280, 103);
    drawFlowers(520, 103);
    drawFlowers(735, 103);

    // BOX

    drawBox(570, 92, 625, 147);

    // GIRL

    drawPlayer(200, 92);

    glutSwapBuffers();
}

// =====================================================
// CLOUD MOVEMENT
// =====================================================

void update(int value)
{
    // Move clouds from LEFT to RIGHT

    cloudX1 += 0.5f;
    cloudX2 += 0.7f;
    cloudX3 += 0.6f;
    cloudX4 += 0.8f;

    // Bring clouds back from the left

    if (cloudX1 > 850)
        cloudX1 = -100;

    if (cloudX2 > 850)
        cloudX2 = -100;

    if (cloudX3 > 850)
        cloudX3 = -100;

    if (cloudX4 > 850)
        cloudX4 = -100;

    glutPostRedisplay();

    glutTimerFunc(16, update, 0);
}

// =====================================================
// INITIALIZATION
// =====================================================

void init()
{
    glClearColor(0.20f, 0.70f, 0.95f, 1.0f);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluOrtho2D(0, WIDTH, 0, HEIGHT);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
}

// =====================================================
// MAIN
// =====================================================

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(WIDTH, HEIGHT);

    glutInitWindowPosition(50, 50);

    glutCreateWindow("2D Platformer Game");

    init();

    glutDisplayFunc(display);

    // Start cloud animation

    glutTimerFunc(0, update, 0);

    glutMainLoop();

    return 0;
}
