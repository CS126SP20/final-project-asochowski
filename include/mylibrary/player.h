// Copyright (c) 2020 [Your Name]. All rights reserved.
#ifndef FINALPROJECT_MYLIBRARY_EXAMPLE_H_
#define FINALPROJECT_MYLIBRARY_EXAMPLE_H_

#include <Box2D/Box2D.h>
#include <chrono>
#include "mylibrary/texture_sheet.h"
#include "mylibrary/animation.h"

namespace mylibrary {

// The render size multiplier of the player.
const float kPlayerRenderHeight = 80.0 / 1080.0;
const float kPlayerRenderWidth = 80.0 / 1920.0;

// Player shot cooldown and jump force.
const int kShootCooldown = 1000;
const int kJumpForce = 600;

// Animation delay and speed threshold.
const int kPlayerAnimationMs = 100;
const int kRunAnimationSpeedThreshold = 5;

// The texture size and path.
const int kPlayerTextureSize = 16;
const std::string kPlayerTexturePath = "images/player.png";

// All of the different sets of coordinates for each animation.
const std::vector<Coordinate> kRunLeftCoordinates = {{1,2},
                                                     {2,2},
                                                     {3,2},
                                                     {4,2},
                                                     {5,2},
                                                     {0,2}};
const std::vector<Coordinate> kStandLeftCoordinates = {{0,2}};
const std::vector<Coordinate> kRiseLeftCoordinates = {{6,2}};
const std::vector<Coordinate> kFallLeftCoordinates = {{5,2}};
const std::vector<Coordinate> kRunRightCoordinates = {{1,3},
                                                      {2,3},
                                                      {3,3},
                                                      {4,3},
                                                      {5,3},
                                                      {0,3}};
const std::vector<Coordinate> kStandRightCoordinates = {{0,3}};
const std::vector<Coordinate> kRiseRightCoordinates = {{6,3}};
const std::vector<Coordinate> kFallRightCoordinates = {{5,3}};
const std::vector<Coordinate> kDeadCoordinates = {{9,2}};

// The object representing the player in the game. Houses a b2Body and can be
// moved around. Also keeps track of shooting cooldown and its own animations.
class Player {
public:

  // The main constructor for the player. Creates the player in the b2World.
  explicit Player(b2World* world, bool load_assets);

  // The default constructor for the player.
  Player();

  // Sets the player's current velocity to a b2Vec2.
  void SetVelocity(b2Vec2 velocity);

  // Gets the current velocity of the player.
  b2Vec2 GetVelocity();

  // Applies an impulse to he player body.
  void ApplyImpulse(b2Vec2 impulse);

  // Makes the player jump.
  void Jump();

  // Returns a pointer to the player's b2Body.
  b2Body* GetBody();

  // Checks whether or not a given body is the player's body.
  bool IsBody(b2Body* body);

  // Checks whether or not the player can shoot (if the cooldown timer is at 0).
  bool CanShoot();

  // Returns the percent that the cooldown has left.
  float GetCooldownPercent();

  // Resets the shoot cooldown.
  void Shoot();

  // Gets the current texture, given the current direction it is facing. If left
  // is true, return the texture that faces left. If false, return the right
  // texture.
  cinder::gl::TextureRef GetTexture(bool left);

  // Set the player to be dead.
  void Die();

  // Resets the player to it's original state.
  void Reset();

private:

  // The b2Body pointer representing the player in the b2World.
  b2Body* body_;

  // The last time that a Shoot() was called.
  std::chrono::time_point<std::chrono::system_clock> last_shot_time_;

  // The animation corresponding to the player running left.
  Animation run_left_animation_;

  // The animation corresponding to the player standing facing left.
  Animation stand_left_animation_;

  // The animation corresponding to the player rising facing left.
  Animation rise_left_animation_;

  // The animation corresponding to the player falling facing left.
  Animation fall_left_animation_;

  // The animation corresponding to the player running facing right.
  Animation run_right_animation_;

  // The animation corresponding to the player standing facing right.
  Animation stand_right_animation_;

  // The animation corresponding to the player rising facing right.
  Animation rise_right_animation_;

  // The animation corresponding to the player falling facing right.
  Animation fall_right_animation_;

  // The animation corresponding to the player dead.
  Animation dead_animation_;

  // Whether or not the player is dead.
  bool dead_ = false;

  // Loads all of the animatons of the player.
  void LoadAnimations();

  // Loads an animation, given a texture size and the coordinates of the
  // animation. Returns a pointer to the animation object.
  Animation LoadAnimation(int texture_size,
      const std::vector<Coordinate>& coordinates);

};


}  // namespace mylibrary


#endif // FINALPROJECT_MYLIBRARY_EXAMPLE_H_
