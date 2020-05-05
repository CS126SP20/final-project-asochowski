//
// Created by Aidan on 4/18/2020.
//

#ifndef FINALPROJECT_MYLIBRARY_BULLET_H
#define FINALPROJECT_MYLIBRARY_BULLET_H

#include <Box2D/Box2D.h>
#include <chrono>
#include <string>
#include "mylibrary/texture_sheet.h"
#include "mylibrary/animation.h"

namespace  mylibrary {

// The render size of the bullet.
const float kBulletRenderHeight = 100.0 / 1080.0;
const float kBulletRenderWidth = 100.0 / 1920.0;

// Constants that pertain to Box2D physics
const int kPlayerKnockbackForce = 50000;
const int kBulletVelocity = 75;
const float kSpawnDistance = 4.5f;
const float kBulletSize = 0.5f;
const float kBulletDensity = 100000;
const int kBulletAnimationMs = 75;

// The bullet texture's path, size, and coordinates.
const std::string kBulletTexturePath = "images/blade.png";
const int kBulletTextureSize = 25;
const std::vector<mylibrary::Coordinate> kBulletCoordinates = {{0,0},
                                                               {1,0},
                                                               {2,0},
                                                               {3,0}};

// An object representing a bullet object in the world. Houses a b2Body and all
// other variables pertaining to each bullet.
class Bullet {

public:

  // The default bullet constructor.
  Bullet();

  // The standard bullet constructor. Spawns a bullet given the players position
  // and sets its velocity so it will eventually get to the target position.
  Bullet(b2World* world, const b2Vec2& player_pos, const b2Vec2& target_pos,
      bool load_assets);

  // The destructor for the bullet.
  ~Bullet();

  // Returns a pointer to the b2Body of the bullet.
  b2Body* GetBody();

  // Returns the b2Vec2 that the bullet is following. Will be a vector with a
  // magnitude of 1.
  b2Vec2 GetTrajectory();

  // Gets the amount of milliseconds the bullet has been active.
  int GetMilliseconds();

  // Gets the current texture in the bullet animation.
  cinder::gl::TextureRef GetTexture();

  // Loads the texture of the bullet object. MUST BE CALLED BEFORE ANY BULLET
  // is created.
  static void LoadTexture();

private:

  // The b2Body representing the bullet in the world.
  b2Body* body_;

  // The trajectory of the bullet.
  b2Vec2 trajectory_;

  // The time at which the bullet was spawned into the world.
  std::chrono::time_point<std::chrono::system_clock> spawn_time_;

  // The animation of the bullet.
  Animation animation_;

  // Creates the animation object for the specific bullet, and starts it.
  void LoadAnimation();

};

}
#endif //FINALPROJECT_MYLIBRARY_BULLET_H
