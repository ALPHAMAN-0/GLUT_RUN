#ifndef PLAYER_H
#define PLAYER_H

/* Side-view running girl.

   Drawn standing on (x, y): her feet rest on that point and she reaches up
   about 114 units and out about 31 units to either side. Any jump currently
   in progress is applied on top of y by drawPlayer itself. */
void drawPlayer(float x, float y);

/* Advance the run cycle and the jump arc. dt is in seconds. */
void playerUpdate(float dt);

/* Begin a jump. Ignored while she is already off the ground. */
void playerJump(void);

/* Height above the ground right now, 0 when grounded. */
float playerGroundOffset(void);

/* Non-zero while airborne. */
int playerIsAirborne(void);

#endif
