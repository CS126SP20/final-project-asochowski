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