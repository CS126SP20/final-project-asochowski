//
// Created by Aidan on 4/18/2020.
//

#ifndef FINALPROJECT_MYLIBRARY_BULLET_H
#define FINALPROJECT_MYLIBRARY_BULLET_H

#include <Box2D/Box2D.h>
#include <chrono>


namespace  mylibrary {

class Bullet {
public:
  Bullet();
  Bullet(b2World* world, const b2Vec2& player_pos, const b2Vec2& target_pos);
  ~Bullet();
  b2Body* GetBody();
  b2Vec2 GetTrajectory();
  int GetMilliseconds();

private:
  b2Body* body_;
  b2Vec2 trajectory_;
  std::chrono::time_point<std::chrono::system_clock> spawn_time_;

};

}
#endif //FINALPROJECT_MYLIBRARY_BULLET_H
