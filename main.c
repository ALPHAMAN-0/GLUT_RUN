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

/* ---------------- animation state ---------------- */
static float gTime   = 0.0f;   /* seconds since start */
static int   gPaused = 0;

/* Scroll an object so it leaves on the left and re-enters on the right.
   speed is in pixels/second; margin keeps it fully off-screen before wrapping. */
static float wrapX(float x0, float speed, float margin)
{
    float span = 800.0f + 2.0f * margin;
    float x = fmodf(x0 - speed * gTime, span);
    if (x < 0.0f) x += span;
    return x - margin;
}

/* ---------------- primitives ---------------- */
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

/* vertical capsule: flat bottom at baseY, rounded top, total height h */
void drawCapsule(float cx, float baseY, float w, float h)
{
    float r = w * 0.5f;
    drawRect(cx - r, baseY, cx + r, baseY + h - r);
    drawEllipse(cx, baseY + h - r, r, r);
}

/* ---------------- sky ---------------- */
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

void drawSun(float cx, float cy)
{
    glColor4f(1.0f, 0.95f, 0.70f, 0.18f);
    drawEllipse(cx, cy, 54, 54);
    glColor4f(1.0f, 0.95f, 0.70f, 0.28f);
    drawEllipse(cx, cy, 38, 38);
    glColor3f(1.0f, 0.97f, 0.80f);
    drawEllipse(cx, cy, 24, 24);
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

/* One bird. It is just two lines that make a "V" shape.
   wing = how high the wing tips are right now. */
void drawBird(float x, float y, float size, float wing)
{
    glColor3f(0.28f, 0.36f, 0.42f);
    glLineWidth(2.0f);

    glBegin(GL_LINE_STRIP);
        glVertex2f(x - 8 * size, y + wing);   /* left wing tip  */
        glVertex2f(x,            y);          /* the body       */
        glVertex2f(x + 8 * size, y + wing);   /* right wing tip */
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

/* ---------------- terrain ---------------- */

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

void drawGround()
{
    glColor3f(0.45f, 0.19f, 0.12f);
    drawRect(0, 0, 800, 110);

    /* streaks in the foreground dirt -- these scroll fastest (closest layer) */
    float off = gTime * 95.0f;
    glColor3f(0.55f, 0.25f, 0.16f);
    glLineWidth(1.5f);
    for (int line = 0; line < 3; line++)
    {
        float baseY = 25 + line * 30;
        glBegin(GL_LINE_STRIP);
        for (int x = 0; x <= 800; x += 12)
        {
            float y = baseY + 6 * sin((x + off) * 0.03f + line);
            glVertex2f(x, y);
        }
        glEnd();
    }

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

/* ---------------- cacti ---------------- */

/* an arm: out from the trunk by `reach` (negative = left), then up by `rise` */
static void cactusArm(float sx, float sy, float reach, float rise, float w)
{
    float r  = w * 0.5f;
    float ex = sx + reach;
    drawRect((sx < ex ? sx : ex), sy - r, (sx < ex ? ex : sx), sy + r);
    drawEllipse(ex, sy, r, r);
    drawRect(ex - r, sy, ex + r, sy + rise - r);
    drawEllipse(ex, sy + rise - r, r, r);
}

/* saguaro: rounded trunk + two bent arms, ribbed and shaded */
void drawCactus(float cx, float baseY, float s)
{
    float w = 17 * s;
    float h = 80 * s;

    glColor4f(0.35f, 0.20f, 0.10f, 0.22f);
    drawEllipse(cx + 10 * s, baseY + 3 * s, 26 * s, 5 * s);

    glColor3f(0.20f, 0.47f, 0.26f);
    drawCapsule(cx, baseY, w, h);
    cactusArm(cx - 3 * s, baseY + 33 * s, -21 * s, 30 * s, 12 * s);
    cactusArm(cx + 3 * s, baseY + 47 * s,  23 * s, 25 * s, 11 * s);

    /* lit edge on the left */
    glColor3f(0.30f, 0.62f, 0.34f);
    drawRect(cx - 7.5f * s, baseY, cx - 5.0f * s, baseY + h - 10 * s);

    /* ribs */
    glColor3f(0.13f, 0.33f, 0.19f);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
        glVertex2f(cx - 2 * s, baseY + 3 * s);  glVertex2f(cx - 2 * s, baseY + h - 11 * s);
        glVertex2f(cx + 3 * s, baseY + 3 * s);  glVertex2f(cx + 3 * s, baseY + h - 11 * s);
        glVertex2f(cx - 24 * s, baseY + 40 * s); glVertex2f(cx - 24 * s, baseY + 57 * s);
        glVertex2f(cx + 26 * s, baseY + 54 * s); glVertex2f(cx + 26 * s, baseY + 66 * s);
    glEnd();

    /* spines along the trunk */
    glColor3f(0.78f, 0.82f, 0.60f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < 7; i++)
    {
        float y = baseY + (8 + i * 9) * s;
        glVertex2f(cx - w * 0.5f, y);          glVertex2f(cx - w * 0.5f - 3 * s, y + 2 * s);
        glVertex2f(cx + w * 0.5f, y + 4 * s);  glVertex2f(cx + w * 0.5f + 3 * s, y + 6 * s);
    }
    glEnd();

    /* blooms on the crowns */
    glColor3f(0.97f, 0.88f, 0.52f);
    drawEllipse(cx, baseY + h - 2 * s, 3.5f * s, 3.0f * s);
    drawEllipse(cx - 24 * s, baseY + 62 * s, 3.0f * s, 2.6f * s);
}

/* stubby barrel cactus with a flower */
void drawBarrelCactus(float cx, float baseY, float s)
{
    glColor3f(0.23f, 0.44f, 0.27f);
    drawEllipse(cx, baseY + 8 * s, 8 * s, 10 * s);
    glColor3f(0.32f, 0.58f, 0.34f);
    drawEllipse(cx - 2.5f * s, baseY + 9 * s, 3.0f * s, 6 * s);
    glColor3f(0.14f, 0.31f, 0.19f);
    glLineWidth(1.2f);
    glBegin(GL_LINES);
        glVertex2f(cx - 4 * s, baseY + 3 * s); glVertex2f(cx - 4 * s, baseY + 14 * s);
        glVertex2f(cx + 1 * s, baseY + 1 * s); glVertex2f(cx + 1 * s, baseY + 17 * s);
        glVertex2f(cx + 5 * s, baseY + 3 * s); glVertex2f(cx + 5 * s, baseY + 14 * s);
    glEnd();
    glColor3f(0.93f, 0.48f, 0.36f);
    drawEllipse(cx + 1 * s, baseY + 18 * s, 2.6f * s, 2.2f * s);
}

/* prickly pear: overlapping pads */
void drawSmallCactus(float cx, float baseY, float s)
{
    glColor3f(0.25f, 0.49f, 0.29f);
    drawEllipse(cx, baseY + 9 * s, 7 * s, 10 * s);
    drawEllipse(cx - 8 * s, baseY + 17 * s, 5.5f * s, 6.5f * s);
    drawEllipse(cx + 8 * s, baseY + 15 * s, 5.0f * s, 6.0f * s);
    glColor3f(0.33f, 0.60f, 0.35f);
    drawEllipse(cx - 2 * s, baseY + 10 * s, 2.5f * s, 5 * s);
    glColor3f(0.85f, 0.35f, 0.30f);
    drawEllipse(cx - 8 * s, baseY + 23 * s, 1.8f * s, 2.2f * s);
    drawEllipse(cx + 9 * s, baseY + 20 * s, 1.6f * s, 2.0f * s);
}

/* ---------------- moving obstacles ---------------- */

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

/* ---------------- scene ---------------- */
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

    /* background greenery -- slow parallax */
    for (int i = 0; i < 7; i++)
        drawSmallCactus(wrapX(60 + i * 115.0f, 20.0f, 40.0f), 148 + (i % 3), 0.95f + 0.1f * (i % 3));

    drawGround();

    /* mid layer on the sand band */
    for (int i = 0; i < 6; i++)
        drawRock(wrapX(70 + i * 138.0f, 60.0f, 30.0f), 116 + 2 * (i % 2), 10 - (i % 2), 6 - (i % 2));

    for (int i = 0; i < 4; i++)
        drawBarrelCactus(wrapX(150 + i * 205.0f, 60.0f, 30.0f), 112, 1.0f + 0.15f * (i % 2));

    for (int i = 0; i < 4; i++)
        drawCactus(wrapX(120 + i * 190.0f, 60.0f, 70.0f), 110, 0.9f + 0.1f * (i % 2));

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