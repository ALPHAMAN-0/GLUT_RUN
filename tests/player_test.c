/* Tests for the jump logic in player.c (playerJump / playerUpdate).
 *
 * player.c also holds the OpenGL drawing code, so the test has to be linked
 * with GLUT although it never opens a window. From the repository root:
 *
 *   macOS:
 *     gcc tests/player_test.c player.c -o tests/player_test.out -framework GLUT -framework OpenGL -Wno-deprecated-declarations && ./tests/player_test.out
 *   Linux (the dev container):
 *     gcc tests/player_test.c player.c -o tests/player_test.out -lglut -lGLU -lGL -lm && ./tests/player_test.out
 */
#include <stdio.h>

#include "../player.h"

static int checks = 0;
static int failures = 0;

#define CHECK(cond)                                           \
    do {                                                      \
        checks++;                                             \
        if (!(cond)) {                                        \
            failures++;                                       \
            printf("FAIL line %d: %s\n", __LINE__, #cond);    \
        }                                                     \
    } while (0)

int main(void)
{
    const float dt = 1.0f / 60.0f;   /* one frame at 60 fps */
    float peak = 0.0f;
    int frames = 0;
    int i;

    /* she starts on the ground, and time alone does not lift her */
    CHECK(!playerIsAirborne());
    CHECK(playerGroundOffset() == 0.0f);
    playerUpdate(dt);
    CHECK(!playerIsAirborne());
    CHECK(playerGroundOffset() == 0.0f);

    /* a jump leaves the ground with the next update */
    playerJump();
    CHECK(playerIsAirborne());
    playerUpdate(dt);
    CHECK(playerGroundOffset() > 0.0f);

    /* a second jump in mid-air is ignored: the arc stays that of one jump
       (470^2 / (2 * 1500) is about 74 units; a double jump would go higher) */
    playerJump();
    while (playerIsAirborne() && frames < 600) {
        if (playerGroundOffset() > peak) peak = playerGroundOffset();
        playerUpdate(dt);
        frames++;
    }
    CHECK(!playerIsAirborne());            /* she lands again ...            */
    CHECK(playerGroundOffset() == 0.0f);   /* ... exactly on the ground      */
    CHECK(peak > 65.0f && peak < 75.0f);
    CHECK(frames > 30 && frames < 45);     /* about 0.63 s in the air        */

    /* zero or negative time steps change nothing */
    playerJump();
    playerUpdate(0.0f);
    playerUpdate(-1.0f);
    CHECK(playerIsAirborne());
    CHECK(playerGroundOffset() == 0.0f);

    for (i = 0; i < 600 && playerIsAirborne(); i++) playerUpdate(dt);
    CHECK(!playerIsAirborne());

    printf("%d/%d checks passed\n", checks - failures, checks);
    return failures == 0 ? 0 : 1;
}
