# Development
---
# Week 1:
- **4/17/20**: Finished CMake and Box2D configuration, created a Box2D world and a simple OpenGL rendering function 
(without rotation).
- **4/18/20**: Consolidated the current functionality into engine.cc, created a movable player that is controllable with
 WASD and space, added OpenGL rendering that supports rotated objects, created debris objects that are deleted whenever 
 they collide with a static body or a player, the player can shoot a bullet by clicking and the bullet will move in the 
 direction of the mouse.
 - **4/19/20**: Bullets now delete debris in their path, and the player is knocked back whenever they shoot and added 
 debris spawning that increases in speed as the game goes on.
- **4/21/20**: The game ends when the player is hit by debris, adjusted debris spawning algorithm, and added tests for
the player movement, the engine, the bullet, and the debris.
- **4/24/20**: Added boundary walls.
---
# Week 2:
- **4/25/20**: Added game scoring and background texture
    - Need to add textures for all other objects
- **4/26/20**: Added animation class to handle changing textures, added TextureSheet class to parse images into multiple
textures, added rendering without bilinear filtering.
    - Need to correctly render transparent pixels, probably will use a shader to fix my current issue.
- **4/27/20**: Fixed transparency issues, added player animation that faces the mouse, added debris and bullet
animations.
- **4/28/20**: Changed the bullet animation to a spinning blade, fixed resolution scaling issues, and cleaned up magic
numbers.
- **4/29/20**: Added platform textures, added comments describing all functions and variables in every header file.
- **4/30/20**: Added more tests, added button to reset the game.
    - Going to add better GUI textures and possibly a main menu.
---
# Week 3:
- **5/1/20**: Added a SoundManager class in order to keep track of the music that is being played and that needs to be
looped. Also added sounds for whenever:
    - Debris collides with the "bullet"
    - There is a near miss
    - Game music, game over music, and potential menu music.
    
    The next things to do are add a better GUI/game over screen and add a main menu.
- **5/2/20**: Added a refined "game over" screen. It now includes the score and instructions on how to restart. I also
decided I will not be adding different GUI elements, as the current ones match the game over screen much better.
- **5/3/20**: Added a title screen that shows on start, and also is shown when the player hits ESC while playing. If
they hit ESC while in the title screen, the program terminates.
- **5/4/20**: Changed the font to fit with this project's aesthetic, changed the image loading method so that it accepts
the relative path of the image and not the absolute path, added content to the README.md file,