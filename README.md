C++ 2D Collision Simulator using Raylib, by Nicolas Lemke.

https://www.youtube.com/watch?v=UoBc7cA5OOw <-- showcase video

This program uses a different approach to collision handling (in turn more efficient), see references for more.
Settings can be altered in the constant definitions

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1200
#define FPS 240
#define QNT_CIRCLES 10
The above need no explanation.

#define MAX_MASS 100
Defines the max mass that a ball can have, in a range between [1, MAX_MASS].
The radius of a ball is proportional to the mass (mass * 0.75).

#define MAX_VELOCITY_X 300
#define MAX_VELOCITY_Y 300
Defines the max velocity. Notice that the velocity vector is decomposed to X and Y.
The range of the max velocity is [-MAX_VELOCITY, MAX_VELOCITY]. *Velocity can be 0!

Limitations:
Obviously, too many objects, too big of a mass (radius) or too big of a velocity will entail machine/program limitations.
The same applies for invalid values.

References:

https://www.youtube.com/watch?v=RGzj-PF7D74
https://www.youtube.com/watch?v=eED4bSkYCB8&t=938s
https://www.youtube.com/watch?v=dJNFPv9Mj-Y
https://www.vobarian.com/collisions/2dcollisions2.pdf

TODO:

add images to circles
fix ball spawning one another bug
fix FPS stop bug
add sfx
