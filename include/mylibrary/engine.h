//
// Created by Aidan on 4/18/2020.
//

#ifndef FINALPROJECT_ENGINE_H
#define FINALPROJECT_ENGINE_H

#include <Box2D/Box2D.h>
#include <mylibrary/player.h>
#include <mylibrary/debris.h>
#include <mylibrary/bullet.h>
#include <mylibrary/sound_manager.h>
#include <set>
#include <vector>
#include <chrono>
#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>
#include <experimental/filesystem>

namespace mylibrary {

const double kGravity = 5 * 9.8;
const float kPixelToMeterRatio = 96.0f;

// How much time goes by in each step of the world
const float32 kTimeStep = 1.0f / 60.0f;

// These two parameters determine how much Box2D should "correct" velocity and
// position. More iterations = More accurate physics + Higher computational cost
const int32 kVelIterations = 20;
const int32 kPosIterations = 20;

// Bullet constants
const int kMoveSpeed = 12;
const int kBulletLifetime = 2500;
const float kNearMissDistance = 10;

// Asset paths
const std::string kBackgroundImagePath = "C:/Users/Aidan/CLionProjects/Cinder/"
                                          "my-projects/final-project-asochowski"
                                          "/assets/bkgreen.png";
const std::string kPlatformImagePath = "C:/Users/Aidan/CLionProjects/Cinder/"
                                       "my-projects/final-project-asochowski"
                                       "/assets/prtgard.png";

// Fonts
const char kNormalFont[] = "Arial";
const char kBoldFont[] = "Arial Bold";
const char kDifferentFont[] = "Papyrus";

// Debris spawn rate equation variables
const int kIntercept = 1;
const int kSlope = 20;
const float kDebrisSize = 2.5f;
const float kDebrisSpawnHeightMultiplier = 0.0925925926f;

// Platform positioning
const float kPlatformDy = 13.0f;
const float kPlatformDx = 25.0f;
const float kPlatformWidth = 5.0f;
const float kPlatformHeight = 1.0f;


class Engine {
public:

  // Creates the game engine given the current screen width and height, in
  // pixels.
  Engine(int screen_width, int screen_height, bool load_assets);

  // Empty constructor of the engine.
  Engine();

  // Moves the physics engine by one "tick" (about 1/60 of a second). All
  // objects are moved to their updated positions in the b2World.
  void Step();

  // Registers that a key is being pressed down by the player, given the current
  // key code.
  void KeyPress(int key_code);

  // Registers that a key is released by the player, given the current key code.
  void KeyRelease(int key_code);

  // Draws all objects on the screen, including the background, the player, all
  // debris, all bullets, and the platforms.
  void Draw();

  // Starts the engine. If it is not started, Step() will not do anything.
  void Start();

  // Ends the game, but the game engine will continue to run.
  void End();

  // Checks whether or not the game engine is running.
  bool IsRunning();

  // Checks whether or not the current game (within the engine) is over.
  bool IsOver();

  // Tells the game engine where the mouse is currently located so the player
  // can face that direction. This position is given in pixels.
  void UpdateMousePos(const cinder::ivec2& px_pos);

  // Tells the engine that the player has clicked at this location, meaning the
  // player should shoot in that direction. This location is given in pixels.
  void Shoot(int x_px, int y_px);

  // Returns a reference to the player object.
  Player& GetPlayer();

  // Spawns debris at the location, which is in pixels. Returns a pointer to
  // this debris object.
  Debris* SpawnDebris(int x_px, int y_px);

  // Spawns a bullet at the location, which is given in pixels. Returns a
  // pointer to this bullet object.
  Bullet* SpawnBullet(int x_px, int y_px);

  // Returns the number of seconds since Start() was last called.
  int GetSecondsSinceStart();

  // Resets the game as if it were created new.
  void Reset();

  // Calculates the score given these values.
  int CalculateScore(int num_shot, int num_near_miss, double seconds);

private:

  // The world object that houses all the b2Body's and all the physics in the
  // game.
  b2World* world_;

  // The Player object that the player controls.
  Player player_;

  // The given screen width, in pixels.
  int screen_width_;

  // The given screen height, in pixels.
  int screen_height_;

  // The conversion factor from meters to pixels, given the current resolution.
  float32 m_to_px;

  // The conversion factor from pixels to meters, given the current resolution.
  float32 px_to_m;

  // Whether or not the game engine is running.
  bool running_ = false;

  // Whether or not the game is over (the player is dead).
  bool over_ = false;

  // A set of key codes that stores what keys are currently being held down.
  // When a key is released, it is removed from this list.
  std::set<int> held_keys_;

  // A vector storing pointers to all of the debris objects currently in the
  // world.
  std::vector<Debris*> all_debris_;

  // A vector storing pointers to all of the bullet objects currently in the
  // world.
  std::vector<Bullet*> all_bullets_;

  // A vector storing pointers to all of the platform bodies currently in the
  // world.
  std::vector<b2Body*> all_platforms_;

  // The time at which Start() was last called.
  std::chrono::time_point<std::chrono::system_clock> start_time_;

  // The time at which SpawnDebris() was last called.
  std::chrono::time_point<std::chrono::system_clock> last_debris_time_;

  // The number of debris the player shot this game.
  int num_debris_shot_ = 0;

  // The number of debris that the player has has "near misses" with this game.
  int num_near_missed_ = 0;

  // The current calculated score of the player.
  int score_ = 0;

  // The current mouse position of the player.
  cinder::ivec2 mouse_pos_;

  // The background texture reference.
  cinder::gl::TextureRef background_texture_;

  // The platform texture reference.
  cinder::gl::TextureRef platform_texture_;

  // Whether or not assets will be loaded from this engine.
  bool load_assets_;

  // The sound manager for the game.
  SoundManager sound_manager_;

  // Tells the player object what keys are currently being pressed, and applies
  // forces in the appropriate directions given the current keys being pressed.
  void UpdatePlayer();

  // Created the boundaries of the map, including the outer walls and the
  // platforms.
  void CreateBoundaries();

  // Draws the hitboxes (the b2Body's) of all of the bodies in the world.
  void DrawHitBoxes();

  // Draws the GUI of the game.
  void DrawGui();

  // Draws the background texture.
  void DrawBackground();

  // Draws the animation of all of the current debris bodies.
  void DrawDebris();

  // Draws the animation of the player body.
  void DrawPlayer();

  // Draws the animation of all of the current bullet bodies.
  void DrawBullets();

  // Draws the platform textures.
  void DrawPlatforms();

  // Checks all of the debris collisions, and deletes them if they are not
  // colliding with other debris.
  void CheckDebrisCollisions();

  // Checks all of the bullets to see if they are colliding with anything. If
  // they are colliding with debris, it will increment the score. If they are
  // colliding with a static body, then it will delete the bullet. If the bullet
  // has been in the world for greater that kBulletLifetime seconds, then it
  // deletes the bullet.
  void CheckBullets();

  // Checks whether or not a debris should be spawned, given the current spawn
  // interval.
  void CheckDebrisSpawn();

  // Gets the current interval at which debris should be spawning, determined
  // by the equation: seconds = kIntercept + (time in seconds) / kSlope
  std::chrono::milliseconds GetDebrisSpawnInterval();

  // Checks whether or not the player is close enough to any debris to count
  // as a "near miss". If it does count as one, then the score is incremented
  // and the debris is marked as already been near missed.
  void CheckNearMisses();

  // Checks whether or not the player is pressing the reset key.
  void CheckReset();

  // Calculates the score given the current number of near misses, the time, and
  // the number of debris that have been shot.
  void UpdateScore();

  // Pre-loads all of the textures. This includes every texture but the player's
  // (the player's texture is loaded when it is constructed).
  void LoadTextures();

  // Safely deletes all of the entities (debris and bullets) from the map.
  void ClearEntities();

  // Converts a pixel coordinate to a meter coordinate.
  b2Vec2 PxCoordsToMeterCoords(b2Vec2);

  // Converts a meter coordinate to a pixel coordinate.
  b2Vec2 MeterCoordsToPxCoords(b2Vec2);

};

}

#endif //FINALPROJECT_ENGINE_H
