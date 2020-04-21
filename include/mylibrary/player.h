// Copyright (c) 2020 [Your Name]. All rights reserved.
#ifndef FINALPROJECT_MYLIBRARY_EXAMPLE_H_
#define FINALPROJECT_MYLIBRARY_EXAMPLE_H_

#include <Box2D/Box2D.h>
#include <chrono>


namespace mylibrary {

const int kShootCooldown = 1000;

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
  void Shoot();

private:
  b2Body* body_;
  std::chrono::time_point<std::chrono::system_clock> last_shot_time_;

};


}  // namespace mylibrary


#endif // FINALPROJECT_MYLIBRARY_EXAMPLE_H_
