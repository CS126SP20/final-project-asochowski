// Copyright (c) 2020 [Your Name]. All rights reserved.
#ifndef FINALPROJECT_MYLIBRARY_EXAMPLE_H_
#define FINALPROJECT_MYLIBRARY_EXAMPLE_H_

#include <Box2D/Box2D.h>
#include <chrono>
#include "mylibrary/texture_sheet.h"
#include "mylibrary/animation.h"

namespace mylibrary {

const float kPlayerRenderSize = 1 / 13.5;
const int kShootCooldown = 1000;
const int kTextureSize = 16;
const std::string kTexturePath = "C:/Users/Aidan/CLionProjects/Cinder/"
                                 "my-projects/final-project-asochowski/"
                                 "assets/mychar.png";

const std::vector<Coordinate> kRunLeftCoordinates = {{1,2},{2,2},
                                                 {3,2},{4,2},
                                                 {5,2},{0,2}};
const std::vector<Coordinate> kStandLeftCoordinates = {{0,2}};
const std::vector<Coordinate> kRiseLeftCoordinates = {{6,2}};
const std::vector<Coordinate> kFallLeftCoordinates = {{5,2}};
const std::vector<Coordinate> kRunRightCoordinates = {{1,3},{2,3},
                                                      {3,3},{4,3},
                                                      {5,3},{0,3}};
const std::vector<Coordinate> kStandRightCoordinates = {{0,3}};
const std::vector<Coordinate> kRiseRightCoordinates = {{6,3}};
const std::vector<Coordinate> kFallRightCoordinates = {{5,3}};
const std::vector<Coordinate> kDeadCoordinates = {{9,2}};


class Player {
public:
  explicit Player(b2World* world);
  Player();
  void SetVelocity(b2Vec2 velocity);
  b2Vec2 GetVelocity();
  void ApplyImpulse(b2Vec2 impulse);
  void Jump();
  b2Body* GetBody();
  bool IsBody(b2Body* body);
  bool CanShoot();
  float GetCooldownPercent();
  void Shoot();
  cinder::gl::TextureRef GetTexture(bool left);

  void Die();

private:
  b2Body* body_;
  std::chrono::time_point<std::chrono::system_clock> last_shot_time_;
  Animation run_left_animation_;
  Animation stand_left_animation_;
  Animation rise_left_animation_;
  Animation fall_left_animation_;
  Animation run_right_animation_;
  Animation stand_right_animation_;
  Animation rise_right_animation_;
  Animation fall_right_animation_;
  Animation dead_animation_;
  bool dead_ = false;

  void LoadAnimations();
  Animation* LoadAnimation(int texture_size,
                                   const std::vector<Coordinate>& coordinates);

};


}  // namespace mylibrary


#endif // FINALPROJECT_MYLIBRARY_EXAMPLE_H_
