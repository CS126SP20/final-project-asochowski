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

const float kBulletRenderHeight = 100.0 / 1080.0;
const float kBulletRenderWidth = 100.0 / 1920.0;

const int kPlayerKnockbackForce = 50000;
const int kBulletVelocity = 75;
const float kSpawnDistance = 4.5f;
const float kBulletSize = 0.5f;
const float kBulletDensity = 100000;
const int kBulletAnimationMs = 75;

const std::string kBulletTexture = "C:/Users/Aidan/CLionProjects/Cinder/"
                                   "my-projects/final-project-asochowski/"
                                   "assets/blade.png";
const int kBulletTextureSize = 25;
const std::vector<mylibrary::Coordinate> kBulletCoordinates = {{0,0},
                                                               {1,0},
                                                               {2,0},
                                                               {3,0}};
static mylibrary::TextureSheet bullet_texture_sheet_;


namespace  mylibrary {

class Bullet {
public:
  Bullet();
  Bullet(b2World* world, const b2Vec2& player_pos, const b2Vec2& target_pos);
  ~Bullet();
  b2Body* GetBody();
  b2Vec2 GetTrajectory();
  int GetMilliseconds();
  cinder::gl::TextureRef GetTexture();
  static void LoadTexture();

private:
  b2Body* body_;
  b2Vec2 trajectory_;
  std::chrono::time_point<std::chrono::system_clock> spawn_time_;
  Animation animation_;

  void LoadAnimation();

};

}
#endif //FINALPROJECT_MYLIBRARY_BULLET_H
