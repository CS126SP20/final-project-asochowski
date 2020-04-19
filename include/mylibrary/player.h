// Copyright (c) 2020 [Your Name]. All rights reserved.
#ifndef FINALPROJECT_MYLIBRARY_EXAMPLE_H_
#define FINALPROJECT_MYLIBRARY_EXAMPLE_H_

#include <Box2D/Box2D.h>


namespace mylibrary {

class Player {
public:
  explicit Player(b2World* world);
  Player();
  void SetVelocity(b2Vec2 velocity);
  b2Vec2 GetVelocity();
  void Jump();
  b2Body* GetBody();

private:
  b2Body* body_;

};


}  // namespace mylibrary


#endif // FINALPROJECT_MYLIBRARY_EXAMPLE_H_
