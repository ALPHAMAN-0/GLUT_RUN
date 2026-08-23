#ifdef _WIN32
#include <windows.h>
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include <math.h>

#include "player.h"

#define P_PI 3.14159265f

/* ---------------- state ---------------- */

static float gPhase = 0.0f;   /* run cycle, radians */
static float gLift  = 0.0f;   /* height above the ground */
static float gVel   = 0.0f;   /* vertical velocity */
static int   gAir   = 0;

#define P_GRAVITY  (-1500.0f)  /* units / s^2  */
#define P_JUMP_VEL   (470.0f)  /* units / s    */
#define P_CADENCE     (10.0f)  /* radians / s  */

void playerJump(void)
{
    if (!gAir)
    {
        gVel = P_JUMP_VEL;
        gAir = 1;
    }
}

float playerGroundOffset(void) { return gLift; }
int   playerIsAirborne(void)   { return gAir;  }

void playerUpdate(float dt)
{
    if (dt <= 0.0f) return;

    if (gAir)
    {
        /* legs stay tucked mid-air, so the run cycle holds where it was */
        gVel  += P_GRAVITY * dt;
        gLift += gVel * dt;
        if (gLift <= 0.0f)
        {
            gLift = 0.0f;
            gVel  = 0.0f;
            gAir  = 0;
        }
    }
    else
    {
        gPhase += P_CADENCE * dt;
        if (gPhase > 2.0f * P_PI) gPhase -= 2.0f * P_PI;
    }
}

/* ---------------- primitives ----------------
   Named apart from the scene's own helpers so the two files can link
   together without colliding. */

static void pRect(float x1, float y1, float x2, float y2)
{
    glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
}

static void pCircle(float cx, float cy, float r)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i < 40; i++)
    {
        float a = 2.0f * P_PI * i / 40.0f;
        glVertex2f(cx + r * cos(a), cy + r * sin(a));
    }
    glEnd();
}

/* ---------------- body parts ----------------
   Every part is authored around her feet at the origin, then placed by the
   caller. Rotations pivot at the hip / shoulder so the limbs swing. */

static void backLeg(void)
{
    /* shaded a touch darker than the front leg so the stride reads as depth */
    glColor3f(0.86f, 0.60f, 0.43f);
    glBegin(GL_QUADS);
        glVertex2f( 3,  5);
        glVertex2f(11,  5);
        glVertex2f(13, 38);
        glVertex2f( 5, 38);
    glEnd();

    glColor3f(0.13f, 0.03f, 0.09f);
    glBegin(GL_POLYGON);
        glVertex2f( 5, 6);
        glVertex2f(17, 6);
        glVertex2f(19, 2);
        glVertex2f( 7, 1);
    glEnd();
}

static void frontLeg(void)
{
    glColor3f(1.0f, 0.72f, 0.52f);
    glBegin(GL_QUADS);
        glVertex2f(-7,  5);
        glVertex2f( 1,  5);
        glVertex2f(-1, 40);
        glVertex2f(-9, 40);
    glEnd();

    glColor3f(0.18f, 0.05f, 0.12f);
    glBegin(GL_POLYGON);
        glVertex2f(-10, 6);
        glVertex2f(  2, 6);
        glVertex2f(  5, 2);
        glVertex2f( -9, 1);
    glEnd();
}

static void dress(void)
{
    glColor3f(0.95f, 0.22f, 0.48f);
    glBegin(GL_POLYGON);
        glVertex2f(-13, 43);
        glVertex2f(  8, 43);
        glVertex2f( 18,  8);
        glVertex2f(-20,  8);
    glEnd();

    glColor3f(1.0f, 0.40f, 0.62f);
    glBegin(GL_POLYGON);
        glVertex2f( -8, 40);
        glVertex2f(  4, 40);
        glVertex2f( 10, 12);
        glVertex2f(-12, 12);
    glEnd();

    /* bodice: the original art stopped at the waist and left the torso open
       between the skirt and the neck */
    glColor3f(0.95f, 0.22f, 0.48f);
    glBegin(GL_POLYGON);
        glVertex2f(-12, 42);
        glVertex2f(  7, 42);
        glVertex2f(  6, 62);
        glVertex2f( -7, 62);
    glEnd();

    glColor3f(1.0f, 0.40f, 0.62f);
    glBegin(GL_POLYGON);
        glVertex2f(-6, 46);
        glVertex2f( 1, 46);
        glVertex2f( 1, 60);
        glVertex2f(-5, 60);
    glEnd();

    glColor3f(0.75f, 0.10f, 0.30f);
    pRect(-11, 39, 8, 45);
}

static void arm(void)
{
    glColor3f(1.0f, 0.72f, 0.52f);
    glBegin(GL_QUADS);
        glVertex2f( 6, 54);
        glVertex2f(13, 53);
        glVertex2f(21, 33);
        glVertex2f(14, 31);
    glEnd();

    pCircle(18, 30, 5);
}

static void hair(void)
{
    glColor3f(0.12f, 0.045f, 0.02f);
    pCircle( -4, 96, 18);
    pCircle(-16, 80, 15);
    pCircle(-19, 60, 14);
    pCircle(-18, 42, 12);
    pRect(-31, 42, -5, 80);

    glColor3f(0.32f, 0.12f, 0.05f);
    glLineWidth(3);
    glBegin(GL_LINES);
        glVertex2f(-13, 99);
        glVertex2f(-24, 58);
        glVertex2f( -7, 98);
        glVertex2f(-18, 48);
    glEnd();
}

static void head(void)
{
    glColor3f(1.0f, 0.72f, 0.52f);
    pRect(-4, 58, 5, 69);          /* neck */
    pCircle(2, 84, 18);            /* head */

    glColor3f(0.95f, 0.62f, 0.45f);
    pCircle(17, 83, 5);            /* ear */
}

static void face(void)
{
    glColor3f(1.0f, 0.72f, 0.52f);
    glBegin(GL_TRIANGLES);
        glVertex2f(18, 86);
        glVertex2f(27, 82);
        glVertex2f(18, 78);
    glEnd();

    /* eye nudged forward off the hairline, mouth back onto the jaw --
       at the original coordinates both landed outside the face */
    glColor3f(0.02f, 0.02f, 0.02f);
    pCircle(14, 89, 2.5f);

    glLineWidth(2);
    glBegin(GL_LINES);
        glVertex2f(15, 91);
        glVertex2f(19, 93);
    glEnd();

    glColor3f(0.65f, 0.10f, 0.18f);
    glBegin(GL_LINES);
        glVertex2f(13.0f, 75);
        glVertex2f(18.5f, 75);
    glEnd();

    glColor3f(1.0f, 0.15f, 0.42f);
    glLineWidth(3);
    glBegin(GL_LINES);
        glVertex2f(-16,  95);
        glVertex2f(  8, 100);
    glEnd();
}

/* ---------------- assembly ---------------- */

void drawPlayer(float x, float y)
{
    float swing = sin(gPhase);
    float bob   = gAir ? 0.0f : fabs(sin(gPhase * 2.0f)) * 2.0f;

    /* Airborne she holds a tuck: back leg forward, front leg trailing,
       arm thrown up. On the ground everything swings off the same phase. */
    float backAng  = gAir ?  22.0f :  swing * 20.0f;
    float frontAng = gAir ? -26.0f : -swing * 20.0f;
    float armAng   = gAir ? -38.0f : -swing * 17.0f;
    float hairAng  = gAir ?  10.0f :  swing *  4.0f + 3.0f;
    float lean     = gAir ?  -6.0f :  -4.0f;

    /* Shadow stays on the ground and shrinks as she rises. */
    float t = gLift / 120.0f;
    if (t > 1.0f) t = 1.0f;
    glColor4f(0.30f, 0.14f, 0.08f, 0.30f * (1.0f - 0.7f * t));
    glPushMatrix();
        glTranslatef(x, y + 3.0f, 0);
        glBegin(GL_POLYGON);
        for (int i = 0; i < 24; i++)
        {
            float a = 2.0f * P_PI * i / 24.0f;
            glVertex2f(20.0f * (1.0f - 0.45f * t) * cos(a),
                        5.0f * (1.0f - 0.45f * t) * sin(a));
        }
        glEnd();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(x, y + gLift + bob, 0);
        glRotatef(lean, 0, 0, 1);       /* running lean, pivoting at the feet */

        glPushMatrix();
            glTranslatef(9, 41, 0);
            glRotatef(backAng, 0, 0, 1);
            glTranslatef(-9, -41, 0);
            backLeg();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-5, 42, 0);
            glRotatef(frontAng, 0, 0, 1);
            glTranslatef(5, -42, 0);
            frontLeg();
        glPopMatrix();

        dress();
        head();

        glPushMatrix();
            glTranslatef(0, 92, 0);
            glRotatef(hairAng, 0, 0, 1);
            glTranslatef(0, -92, 0);
            hair();
        glPopMatrix();

        face();

        glPushMatrix();
            glTranslatef(9.5f, 54, 0);
            glRotatef(armAng, 0, 0, 1);
            glTranslatef(-9.5f, -54, 0);
            arm();
        glPopMatrix();

    glPopMatrix();
}
