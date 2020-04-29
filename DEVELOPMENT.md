# Development
---
# Week 1:
- Finished CMake and Box2D configuration
- Created a Box2D world and a simple OpenGL rendering function (without rotation)
- Consolidated the current functionality into engine.cc
- Created a movable player that is controllable with WASD and space
- Added OpenGL rendering that supports rotated objects
- Created debris objects that are deleted whenever they collide with a static body or a player
- The player can shoot a bullet by clicking and the bullet will move in the direction of the mouse
- Bullets will delete debris in their path, and the player is knocked back whenever they shoot
- Added debris spawning that increases in speed as the game goes on
- The game ends when the player is hit by debris
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