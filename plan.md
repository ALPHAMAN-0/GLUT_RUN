# Putting the player back into main.c

A step-by-step guide. You write the code — this just tells you what to do and where.

---

## Before you start

**Close `main.c` in your editor, or reload it.**

Twice during our session the file on disk jumped back to an older version, which
undid finished work. That happens when an editor has the old text in its buffer
and saves over the file. Reload before you begin.

## What you already have

Two files are sitting in your folder, unused:

| file | what's in it |
|---|---|
| `player.h` | the list of what the player can do |
| `player.c` | the drawing and the animation (about 300 lines) |

Nothing in `main.c` mentions them any more. That's why the desert is empty.

`player.h` offers you three things:

```
drawPlayer(x, y)      draw her standing on that point
playerUpdate(dt)      move the run cycle and the jump along
playerJump()          start a jump
```

---

## Pick one of two ways

**Way A — use the `player.c` you already have.**
Quickest. She runs and jumps straightaway. But `player.c` is written in the old
style: lots of little helper functions, `glPushMatrix` for every arm and leg.
It will not match the simple style the rest of your file is now in.

**Way B — draw a simple player yourself, inside `main.c`.**
More work, but it will look like your `drawCactus` — plain boxes, no helpers,
no loops, no arrays, all in one file. If a teacher asks you to explain your
code, this is the one you'll be able to explain.

Do Way A first if you just want to see her again. You can always do Way B after.

---

# Way A — use the existing player.c

### Step 1 — put the include back

Near the top of `main.c`, just under `#include <stdlib.h>`, add a line that
includes `"player.h"`.

Use **double quotes**, not angle brackets. Angle brackets mean "a system
header"; quotes mean "a file sitting next to mine".

### Step 2 — draw her

Go to `display()` and find this part:

```c
/* foreground obstacles -- fastest layer */
drawSkull(...);
drawRock(...);
drawRock(...);
```

Add your drawing **after those three lines and before the tumbleweeds.**

Order matters. Anything you paint later covers what came before. Put her before
`drawGround()` and the sand will paint straight over her.

Wrap the call in `glPushMatrix()` / `glPopMatrix()` and between them:

- `glTranslatef` to move her to where she stands — she used to be at **x 170, y 30**
- `glScalef` to shrink her — she used to be at **0.75** on x and y
- then call `drawPlayer(0, 0)`

You pass `0, 0` because `glTranslatef` has already moved you to her spot.

**Why the push and pop?** They mean *save* and *put it back*. Without them the
move and the shrink would leak out and squash every tumbleweed after her.

### Step 3 — make her move

Go to `tick()`. Inside the `if (!gPaused)` block, next to `gTime += 0.016f;`,
call `playerUpdate` and pass it `0.016f`.

That `0.016` is how many seconds one frame lasts — about 1/60th.

### Step 4 — let her jump

Go to `keyboard()`. Add a line for the space bar, `' '`, that calls
`playerJump()`. Copy the shape of the `'p'` and `'r'` lines above it.

### Step 5 — build with both files

`player.c` is a second file, so the compiler needs to be told about it:

```bash
gcc main.c player.c -o main -framework GLUT -framework OpenGL -Wno-deprecated-declarations
```

Inside your dev container:

```bash
gcc main.c player.c -o app -lglut -lGLU -lGL && ./app
```

**Forgetting `player.c` here is the most common mistake.** You get an error
saying `drawPlayer` is undefined. The code is fine — you just didn't hand the
compiler the file it lives in.

---

# Way B — write a simple player in main.c

No second file. Delete nothing yet; leave `player.c` alone until Way B works.

### Step 1 — decide her size

She stands on a point. Work out roughly how tall in scene units, and how wide.
Around **60 tall and 20 wide** sits nicely next to a cactus (yours are 85 tall).

Sketch it on paper first with numbers on it. Every step after this is just
copying your sketch into `glVertex2f` calls.

### Step 2 — write the function

Put it in the **GROUND** section of `main.c`, next to `drawCactus` — she stands
on the ground, so that's where she belongs.

Give it the same shape as `drawCactus`:

```
void drawPlayer(float x, float bottom)
```

`x` is where she stands, `bottom` is the ground under her feet. Every corner
you write is measured from those two, exactly like the cactus.

### Step 3 — build her out of boxes

One `glBegin(GL_QUADS)` … `glEnd()` per body part, with `glColor3f` before each
to change colour. Suggested order, bottom to top:

1. two legs
2. body
3. two arms
4. head

Do **one box first** — just the body — and run it. Seeing one rectangle appear
in the right place tells you your numbers are right. Then add the rest.

For a round head, `drawEllipse(x, y, rx, ry)` already exists near the top of the
file. Using it is fine — it's a shape helper like `drawRect`, not a player
helper.

### Step 4 — draw her

Same as **Way A step 2**, but simpler: no `glPushMatrix` needed, because you
built her at the size you wanted. Just call `drawPlayer(170, 30);` in the same
place — after the foreground rocks, before the tumbleweeds.

### Step 5 — make her move (optional)

Everything that moves in your file uses `gTime`. To make her bob while running,
add to a `y` value something like:

```
a small number  ×  fabs(sin(gTime × a speed))
```

That is the same one-liner your tumbleweed uses to bounce. `fabs` flips the
bottom half of the wave upward so she hops instead of sinking into the ground.

### Step 6 — build

One file only, so nothing changes:

```bash
gcc main.c -o main -framework GLUT -framework OpenGL -Wno-deprecated-declarations
```

---

## Check it worked

- [ ] It compiles with **no errors**
- [ ] You can see her on the brown dirt, not buried in it
- [ ] She's in **front** of the sand band, **behind** or among the tumbleweeds
- [ ] She's not stretched or tiny — if she is, check `glScalef`
- [ ] Nothing *after* her in `display()` got squashed — that means a missing `glPopMatrix()`
- [ ] Way A only: space bar makes her jump

## If something goes wrong

| what you see | what it usually is |
|---|---|
| `undefined symbol: drawPlayer` | `player.c` missing from the gcc line (Way A) |
| `'player.h' file not found` | angle brackets instead of `"quotes"` |
| Everything after her is squashed or slid over | a missing `glPopMatrix()` |
| She's invisible | drawn before `drawGround()`, so the sand covers her |
| She's cut off at the bottom | her feet are below `y = 0`, off the window |
| She doesn't move | `playerUpdate` not called in `tick()` |

## Where she used to be

For reference — the exact spot she stood before:

```
position   x 170, y 30
scale      0.75
drawn      after the two foreground rocks, before the three tumbleweeds
```
