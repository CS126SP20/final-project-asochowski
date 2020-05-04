# [Quake Miner]

Author: [Aidan Sochowski]

---

  Quake Miner will be a 2D platformer where you play as a mining robot stuck inside of a cavern during an earthquake. 
  This earthquake causes debris to fall from the ceiling. The robot is trapped inside of the cavern and has to survive 
  as long as possible, while earning points along the way. You can earn points 3 different ways:

1. Destroying debris with your blaster.
1. Performing "near misses" with debris (happens when you get really close to falling debris but do not touch it).
1. Lasting as long as possible.

The game will become progressively harder as you go along, as more debris will fall and at varying velocity. The stage 
will have boundaries and be fixed to the entirety of the screen, but it will include platforms for the robot to take a
 bit of cover and stand on. The robot's blaster will have a cooldown between shots and will propel the robot backwards 
 after each shot in order to prevent the robot from sitting in place.

  I've done a few projects similar to this, however they have been simple games in Javascript that use the basic 
  functions of a physics library. As for this project, I plan on using [Box2D](https://github.com/erincatto/Box2D) 
  to handle the physics and [OpenGL in Cinder](https://libcinder.org/docs/guides/opengl/index.html) to handle the 
  graphics. Box2D links fairly well with Cinder, and I will do so using either of the two methods listed in 
  [this guide](http://niltoid.com/blog/cinder-box2d/). As for why I am interested in this project, I feel like a lot of 
  the projects I have done in and outside of class have lacked graphical components, and I feel like a simple 2D game 
  would be a good introduction into that. I think that making a fun game would help motivate me to see this project to 
  it's conclusion, and I think this concept sounds pretty fun.
  
  As for how I plan the to structure the timeline of this project, there are three main stages:
  
* **Week 1: Engine, Physics, and Player Input.** By the end of the first week, I plan on having a game engine that is 
functional to the point where the player can control a box on the screen with their keys. Box2D should make collision 
detection and movement fairly simple, so the main challenge will be implementing OpenGL to render these graphically. 
While the objects will not have textures at this point, it should be kept in mind that they will be implemented in the 
future.
* **Week 2: Game Functionality and Scoring.** By the end of the second week, I plan on implementing all of the game 
functionality. The player should be able to move and jump around the stage, shoot falling debris, and die on impact with
 debris. When the player dies, the score should be calculated and displayed on the screen. At this point, the game 
 should be fully functional but will lack all of the "bells and whistles."
* **Week 3: Bells and Whistles.** This is where I plan on adding better graphics and sounds to the game. If I am behind 
on my other objectives at this point, I plan to make up for that during this week. I plan on taking assets from free 
sources online to represent anything that needs to be represented graphically (Player, Platforms, Background, GUI, etc).
 If I have any extra time, I plan on adding any of the following features:
  * A start menu
  * Debris with varying point value
  * A leaderboard
  * Different player textures