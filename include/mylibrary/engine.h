//
// Created by Aidan on 4/18/2020.
//

#ifndef FINALPROJECT_ENGINE_H
#define FINALPROJECT_ENGINE_H

#include <Box2D/Box2D.h>
#include <mylibrary/player.h>
#include <set>

namespace mylibrary {

const double kGravity = 5 * 9.8;

// How much time goes by in each step of the world
const float32 kTimeStep = 1.0f / 60.0f;

// These two parameters determine how much Box2D should "correct" velocity and
// position. More iterations = More accurate physics + Higher computational cost
const int32 kVelIterations = 8;
const int32 kPosIterations = 3;


const int kMoveSpeed = 15;

class Engine {
public:
  Engine(int screen_width, int screen_height);
  Engine();
  void Step();
  void KeyPress(int key_code);
  void KeyRelease(int key_code);
  void Draw();
  void Start();
  void End();
  bool IsRunning();

private:
  b2World* world_;
  Player player_;
  int screen_width_;
  int screen_height_;
  float32 m_to_px;
  float32 px_to_m;
  bool running_ = false;
  std::set<int> held_keys_;

  void UpdatePlayer();
  void CreateBoundaries();
  void DrawHitBoxes();

};

}

#endif //FINALPROJECT_ENGINE_H
