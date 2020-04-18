//
// Created by Aidan on 4/18/2020.
//

#ifndef FINALPROJECT_ENGINE_H
#define FINALPROJECT_ENGINE_H

#include <Box2D/Box2D.h>
#include <mylibrary/player.h>

namespace mylibrary {

const double kGravity = 3 * 9.8;


class Engine {
public:
  Engine(int screen_width, int screen_height);
  void Step();
  void KeyPressUpdate(std::set<int> keys_pressed);
  void DrawBodies();
  void DrawPlayer();
  void Start();
  void End();
  bool IsRunning();

private:
  b2World* world_;
  Player player_;
  int screen_width_;
  int screen_height_;
  bool running_ = false;

  void CreateBoundaries();

};

}

#endif //FINALPROJECT_ENGINE_H
