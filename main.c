#ifdef _WIN32
#include <windows.h>
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include "player.h"

#define PI 3.14159265f

/* ==================================================================
   HOW LONG THE SCENE HAS BEEN RUNNING
   gTime counts up. Things move because they use it.
   ================================================================== */

static float gTime    = 0.0f;   /* seconds since start        */
static int   gPaused  = 0;
static float sunAngle = 0.0f;   /* how far the sun has turned */

/* Scroll an object so it leaves on the left and re-enters on the right.
   speed is in pixels/second; margin keeps it fully off-screen before wrapping. */
static float wrapX(float x0, float speed, float margin)
{
    float span = 800.0f + 2.0f * margin;
    float x = fmodf(x0 - speed * gTime, span);
    if (x < 0.0f) x += span;
    return x - margin;
}

/* ==================================================================
   SHAPE HELPERS
   The little shapes everything else is built from.
   ================================================================== */

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
    for (int i = 0; i < 28; i++)
    {
        float angle = (i * 2 * PI) / 28;
        glVertex2f(cx + rx * cos(angle), cy + ry * sin(angle));
    }
    glEnd();
}

/* ==================================================================
   THE SKY  -  the TOP of the picture
   sky, sun, clouds, birds
   ================================================================== */

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

/* The sun: a round body with pointy rays around it, slowly turning.

   glPushMatrix and glPopMatrix are like "save" and "put it back".
   Everything written between them gets moved and turned.
   Everything else in the picture is left alone. */
void drawSun(float cx, float cy)
{
    glPushMatrix();

    glTranslatef(cx, cy, 0);         /* go to where the sun sits */
    glRotatef(sunAngle, 0, 0, 1);    /* turn it round            */

    /* 12 pointy rays. Draw one ray, turn a little, draw the next. */
    glColor3f(1.0f, 0.85f, 0.35f);
    for (int i = 0; i < 12; i++)
    {
        glBegin(GL_TRIANGLES);
            glVertex2f(-6, 30);      /* bottom left of the ray  */
            glVertex2f( 6, 30);      /* bottom right of the ray */
            glVertex2f( 0, 52);      /* the sharp point         */
        glEnd();

        glRotatef(30, 0, 0, 1);      /* 12 rays x 30 degrees = a full circle */
    }

    /* the round body, drawn last so it sits on top of the rays */
    glColor3f(1.0f, 0.93f, 0.55f);
    drawEllipse(0, 0, 28, 28);

    glPopMatrix();
}

/* One cloud. It is just three white circles sitting next to each other,
   with the middle one lifted a bit so the top looks puffy. */
void drawCloud(float x, float y, float size)
{
    glColor3f(1.0f, 1.0f, 1.0f);

    drawEllipse(x - 20 * size, y,            18 * size, 12 * size);  /* left  */
    drawEllipse(x + 20 * size, y,            18 * size, 12 * size);  /* right */
    drawEllipse(x,             y +  6 * size, 24 * size, 16 * size); /* middle */
}

/* A row of clouds that drifts to the left forever.
   gap   = how far apart the clouds are
   speed = how fast they drift */
void drawCloudRow(float y, float size, float gap, float speed)
{
    /* enough clouds to fill the screen, plus a few spare ones
       waiting off the edges */
    int count = 800 / gap + 3;
    float rowLength = count * gap;

    for (int i = 0; i < count; i++)
    {
        /* line them up, then drift the whole row left as time passes */
        float x = i * gap - speed * gTime;

        /* when a cloud floats off the left side,
           send it back around to the right side */
        while (x < -gap)
            x = x + rowLength;

        drawCloud(x, y, size);
    }
}

/* One bird. It is two lines that make a "V" shape.
   wing = how high the wing tips are right now.

   GL_LINES draws the dots in PAIRS: the first two dots make one line,
   the next two dots make another line. So we need four dots for two wings. */
void drawBird(float x, float y, float size, float wing)
{
    glColor3f(0.28f, 0.36f, 0.42f);
    glLineWidth(2.0f);

    glBegin(GL_LINES);
        /* left wing: from the wing tip in to the body */
        glVertex2f(x - 8 * size, y + wing);
        glVertex2f(x,            y);

        /* right wing: from the body out to the wing tip */
        glVertex2f(x,            y);
        glVertex2f(x + 8 * size, y + wing);
    glEnd();
}

/* A row of birds flying to the left, flapping their wings.
   They stay at the same height. */
void drawBirdRow(float y, float size, float gap, float speed)
{
    /* enough birds to fill the screen, plus a few spare ones
       waiting off the edges */
    int count = 800 / gap + 3;
    float rowLength = count * gap;

    for (int i = 0; i < count; i++)
    {
        /* line them up, then fly the whole row left as time passes */
        float x = i * gap - speed * gTime;

        /* when a bird flies off the left side,
           send it back around to the right side */
        while (x < -gap)
            x = x + rowLength;

        /* the wings go up and down */
        float wing = 5 * size * sin(gTime * 6 + i);

        drawBird(x, y, size, wing);
    }
}

/* ==================================================================
   THE MOUNTAINS  -  the MIDDLE of the picture
   triangles that stand still
   ================================================================== */

/* One mountain. It is a triangle cut down the middle:
   the left half is bright because the sun is on that side,
   and the right half is darker because it is in shadow. */
void drawMountain(float x, float bottom, float width, float height,
                  float r, float g, float b)
{
    float top = bottom + height;

    /* dark half, on the right */
    glColor3f(r * 0.72f, g * 0.72f, b * 0.72f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x,         top);       /* the peak         */
        glVertex2f(x + width, bottom);    /* bottom right     */
        glVertex2f(x,         bottom);    /* middle of bottom */
    glEnd();

    /* bright half, on the left */
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLES);
        glVertex2f(x,         top);       /* the peak         */
        glVertex2f(x,         bottom);    /* middle of bottom */
        glVertex2f(x - width, bottom);    /* bottom left      */
    glEnd();
}

/* All the mountains, drawn one at a time.
   Every line is one mountain, like this:
   drawMountain(x, bottom, width, height, red, green, blue) */
void drawMountains()
{
    /* back row: far away, so pale and tall */
    drawMountain(-60, 160, 110,  95, 0.80f, 0.73f, 0.70f);
    drawMountain(110, 160, 110,  68, 0.80f, 0.73f, 0.70f);
    drawMountain(280, 160, 110, 110, 0.80f, 0.73f, 0.70f);
    drawMountain(450, 160, 110,  78, 0.80f, 0.73f, 0.70f);
    drawMountain(620, 160, 110, 100, 0.80f, 0.73f, 0.70f);
    drawMountain(790, 160, 110,  62, 0.80f, 0.73f, 0.70f);

    /* middle row */
    drawMountain( 20, 160, 100,  70, 0.70f, 0.59f, 0.53f);
    drawMountain(180, 160, 100,  50, 0.70f, 0.59f, 0.53f);
    drawMountain(340, 160, 100,  82, 0.70f, 0.59f, 0.53f);
    drawMountain(500, 160, 100,  58, 0.70f, 0.59f, 0.53f);
    drawMountain(660, 160, 100,  75, 0.70f, 0.59f, 0.53f);
    drawMountain(820, 160, 100,  46, 0.70f, 0.59f, 0.53f);

    /* front row: close by, so darker and shorter */
    drawMountain(-30, 160, 95, 50, 0.58f, 0.45f, 0.37f);
    drawMountain(120, 160, 95, 35, 0.58f, 0.45f, 0.37f);
    drawMountain(270, 160, 95, 57, 0.58f, 0.45f, 0.37f);
    drawMountain(420, 160, 95, 40, 0.58f, 0.45f, 0.37f);
    drawMountain(570, 160, 95, 52, 0.58f, 0.45f, 0.37f);
    drawMountain(720, 160, 95, 32, 0.58f, 0.45f, 0.37f);
}

/* ==================================================================
   THE GROUND  -  the BOTTOM of the picture
   ground, rocks, cacti, tumbleweeds, skull
   ================================================================== */

void drawGround()
{
    glColor3f(0.45f, 0.19f, 0.12f);
    drawRect(0, 0, 800, 110);

    /* little stones scattered on the dirt */
    glColor3f(0.30f, 0.12f, 0.08f);
    for (int i = 0; i < 14; i++)
    {
        float px = wrapX(40 + i * 57.0f, 95.0f, 20.0f);
        float py = 15 + 25 * fmod(i * 37, 90) / 90;
        drawEllipse(px, py, 4, 3);
    }

    /* pale sand band the cacti stand on */
    glColor3f(0.88f, 0.78f, 0.52f);
    drawRect(0, 110, 800, 165);
}

void drawRock(float cx, float cy, float rx, float ry)
{
    glColor4f(0.35f, 0.20f, 0.10f, 0.22f);
    drawEllipse(cx + rx * 0.35f, cy - ry * 0.55f, rx, ry * 0.45f);
    glColor3f(0.62f, 0.47f, 0.28f);
    drawEllipse(cx, cy, rx, ry);
    glColor3f(0.74f, 0.60f, 0.38f);
    drawEllipse(cx - rx * 0.25f, cy + ry * 0.25f, rx * 0.45f, ry * 0.35f);
}

/* A big cactus. It is just five green boxes:
   one tall box standing up, and two arms made of two boxes each.
   Every box is four corners: bottom-left, bottom-right, top-right, top-left. */
void drawCactus(float x, float bottom)
{
    glColor3f(0.20f, 0.47f, 0.26f);

    /* the tall middle */
    glBegin(GL_QUADS);
        glVertex2f(x - 9, bottom);
        glVertex2f(x + 9, bottom);
        glVertex2f(x + 9, bottom + 85);
        glVertex2f(x - 9, bottom + 85);
    glEnd();

    /* left arm, going across */
    glBegin(GL_QUADS);
        glVertex2f(x - 28, bottom + 33);
        glVertex2f(x -  9, bottom + 33);
        glVertex2f(x -  9, bottom + 44);
        glVertex2f(x - 28, bottom + 44);
    glEnd();

    /* left arm, going up */
    glBegin(GL_QUADS);
        glVertex2f(x - 28, bottom + 33);
        glVertex2f(x - 17, bottom + 33);
        glVertex2f(x - 17, bottom + 66);
        glVertex2f(x - 28, bottom + 66);
    glEnd();

    /* right arm, going across */
    glBegin(GL_QUADS);
        glVertex2f(x +  9, bottom + 48);
        glVertex2f(x + 28, bottom + 48);
        glVertex2f(x + 28, bottom + 59);
        glVertex2f(x +  9, bottom + 59);
    glEnd();

    /* right arm, going up */
    glBegin(GL_QUADS);
        glVertex2f(x + 17, bottom + 48);
        glVertex2f(x + 28, bottom + 48);
        glVertex2f(x + 28, bottom + 77);
        glVertex2f(x + 17, bottom + 77);
    glEnd();
}

/* A little cactus. Three small boxes: a middle one and two stubby arms. */
void drawSmallCactus(float x, float bottom)
{
    glColor3f(0.25f, 0.49f, 0.29f);

    /* the middle */
    glBegin(GL_QUADS);
        glVertex2f(x - 5, bottom);
        glVertex2f(x + 5, bottom);
        glVertex2f(x + 5, bottom + 22);
        glVertex2f(x - 5, bottom + 22);
    glEnd();

    /* left arm, standing up */
    glBegin(GL_QUADS);
        glVertex2f(x - 13, bottom + 4);
        glVertex2f(x -  6, bottom + 4);
        glVertex2f(x -  6, bottom + 17);
        glVertex2f(x - 13, bottom + 17);
    glEnd();

    /* right arm, standing up */
    glBegin(GL_QUADS);
        glVertex2f(x +  6, bottom + 6);
        glVertex2f(x + 13, bottom + 6);
        glVertex2f(x + 13, bottom + 19);
        glVertex2f(x +  6, bottom + 19);
    glEnd();
}

/* tumbleweed: tangled ball that rolls (spins with travel) and bounces */
void drawTumbleweed(float cx, float groundY, float r, float speed)
{
    float bounce = fabs(sin(gTime * speed / (r * 2.4f))) * (r * 0.7f);
    float cy     = groundY + r + bounce;
    float angle  = -(gTime * speed) / r * (180.0f / PI);

    glColor4f(0.30f, 0.14f, 0.08f, 0.30f);
    drawEllipse(cx, groundY + 2, r * (1.0f - bounce / (r * 2.5f)), r * 0.28f);

    glPushMatrix();
        glTranslatef(cx, cy, 0);
        glRotatef(angle, 0, 0, 1);

        glColor3f(0.52f, 0.38f, 0.20f);
        glLineWidth(1.6f);
        glBegin(GL_LINES);
        for (int i = 0; i < 14; i++)
        {
            float a = i * 2 * PI / 14;
            glVertex2f(-r * 0.85f * cos(a), -r * 0.85f * sin(a));
            glVertex2f( r * cos(a),          r * sin(a));
        }
        glEnd();

        glColor3f(0.63f, 0.48f, 0.26f);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 14; i++)
        {
            float a  = i * 2 * PI / 14;
            float rr = r * ((i % 3) ? 1.0f : 0.68f);
            glVertex2f(rr * cos(a), rr * sin(a));
        }
        glEnd();
    glPopMatrix();
}

/* bleached skull half-buried in the foreground dirt */
void drawSkull(float cx, float cy, float s)
{
    glColor4f(0.30f, 0.14f, 0.08f, 0.28f);
    drawEllipse(cx, cy - 5 * s, 13 * s, 3.5f * s);
    glColor3f(0.90f, 0.87f, 0.78f);
    drawEllipse(cx, cy + 2 * s, 9 * s, 7 * s);
    drawRect(cx - 4 * s, cy - 6 * s, cx + 4 * s, cy + 2 * s);
    glColor3f(0.86f, 0.82f, 0.72f);
    drawEllipse(cx - 12 * s, cy + 6 * s, 5 * s, 3 * s);   /* horns */
    drawEllipse(cx + 12 * s, cy + 6 * s, 5 * s, 3 * s);
    glColor3f(0.24f, 0.20f, 0.16f);
    drawEllipse(cx - 4 * s, cy + 3 * s, 2.2f * s, 2.4f * s);
    drawEllipse(cx + 4 * s, cy + 3 * s, 2.2f * s, 2.4f * s);
}

/* ==================================================================
   PUTTING IT ALL TOGETHER
   paint sky first, ground last, then run the window
   ================================================================== */

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawSky();
    drawSun(660, 385);

    /* clouds drift behind everything -- higher ones are farther, so slower */
    /* high clouds: small and slow */
    drawCloudRow(405, 0.7f, 320,  5.0f);
    /* middle clouds: big and medium speed */
    drawCloudRow(345, 1.1f, 270,  9.0f);
    /* low clouds: medium and fastest */
    drawCloudRow(288, 0.8f, 230, 14.0f);

    /* two flocks of birds at different heights */
    drawBirdRow(375, 1.0f, 190, 26.0f);
    drawBirdRow(315, 0.8f, 240, 20.0f);

    drawMountains();

    /* little cacti far away */
    drawSmallCactus(wrapX( 60, 20.0f, 40.0f), 150);
    drawSmallCactus(wrapX(175, 20.0f, 40.0f), 148);
    drawSmallCactus(wrapX(290, 20.0f, 40.0f), 151);
    drawSmallCactus(wrapX(405, 20.0f, 40.0f), 149);
    drawSmallCactus(wrapX(520, 20.0f, 40.0f), 150);
    drawSmallCactus(wrapX(635, 20.0f, 40.0f), 148);
    drawSmallCactus(wrapX(750, 20.0f, 40.0f), 151);

    drawGround();

    /* mid layer on the sand band */
    for (int i = 0; i < 6; i++)
        drawRock(wrapX(70 + i * 138.0f, 60.0f, 30.0f), 116 + 2 * (i % 2), 10 - (i % 2), 6 - (i % 2));

    /* little cacti on the sand */
    drawSmallCactus(wrapX(150, 60.0f, 30.0f), 112);
    drawSmallCactus(wrapX(355, 60.0f, 30.0f), 112);
    drawSmallCactus(wrapX(560, 60.0f, 30.0f), 112);
    drawSmallCactus(wrapX(765, 60.0f, 30.0f), 112);

    /* big cacti */
    drawCactus(wrapX(120, 60.0f, 70.0f), 110);
    drawCactus(wrapX(310, 60.0f, 70.0f), 110);
    drawCactus(wrapX(500, 60.0f, 70.0f), 110);
    drawCactus(wrapX(690, 60.0f, 70.0f), 110);

    /* foreground obstacles -- fastest layer */
    drawSkull(wrapX(300, 150.0f, 60.0f), 60, 1.0f);
    drawRock(wrapX(640, 150.0f, 60.0f), 30, 16, 9);
    drawRock(wrapX(120, 150.0f, 60.0f), 78, 12, 7);
    
    glPushMatrix();
        glTranslatef(170, 30, 0);
        glScalef(0.75f, 0.75f, 1.0f);
        drawPlayer(0, 0);
    glPopMatrix();

    drawTumbleweed(wrapX(500,  150.0f, 60.0f), 18, 15, 150.0f);
    drawTumbleweed(wrapX(180,  185.0f, 60.0f), 46, 20, 185.0f);
    drawTumbleweed(wrapX(720,  120.0f, 60.0f), 88, 13, 120.0f);
    
    
    glutSwapBuffers();
}

void tick(int)
{
    if (!gPaused)
    {
        gTime += 0.016f;
        playerUpdate(0.016f);

        sunAngle += 0.3f;                /* turn the sun a little */
        if (sunAngle > 360.0f)
            sunAngle -= 360.0f;
    }
    glutPostRedisplay();
    glutTimerFunc(16, tick, 0);
    
}

void keyboard(unsigned char key, int, int)
{ 
    if (key == 27) exit(0);          /* Esc */
    if (key == 'p' || key == 'P') gPaused = !gPaused;
    if (key == 'r' || key == 'R') gTime = 0.0f;
    if (key == ' ') playerJump();      /* Space */
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 450);
    glMatrixMode(GL_MODELVIEW);
}

void init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 450);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 450);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Desert Scene");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, tick, 0);
    glutMainLoop();
    return 0;
}
