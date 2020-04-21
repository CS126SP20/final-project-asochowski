//
// Created by Aidan on 4/18/2020.
//

#ifndef FINALPROJECT_ENGINE_H
#define FINALPROJECT_ENGINE_H

#include <Box2D/Box2D.h>
#include <mylibrary/player.h>
#include <mylibrary/debris.h>
#include <mylibrary/bullet.h>
#include <set>
#include <vector>
#include <chrono>


namespace mylibrary {

const double kGravity = 5 * 9.8;

// How much time goes by in each step of the world
const float32 kTimeStep = 1.0f / 60.0f;

// These two parameters determine how much Box2D should "correct" velocity and
// position. More iterations = More accurate physics + Higher computational cost
const int32 kVelIterations = 20;
const int32 kPosIterations = 20;

const int kMoveSpeed = 15;
const int kBulletLifetime = 2500;

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
  void Shoot(int x_px, int y_px);
  Player& GetPlayer();
  void SpawnDebris(int x_px, int y_px);
  Bullet* SpawnBullet(int x_px, int y_px);

private:
  b2World* world_;
  Player player_;
  int screen_width_;
  int screen_height_;
  float32 m_to_px;
  float32 px_to_m;
  bool running_ = false;
  std::set<int> held_keys_;
  std::vector<Debris*> all_debris_;
  std::vector<Bullet*> all_bullets_;
  std::chrono::time_point<std::chrono::system_clock> start_time_;
  std::chrono::time_point<std::chrono::system_clock> last_debris_time_;

  void UpdatePlayer();
  void CreateBoundaries();
  void DrawHitBoxes();
  void CheckDebrisCollisions();
  void CheckBullets();
  void CheckDebrisSpawn();
  std::chrono::milliseconds GetDebrisSpawnInterval();
  b2Vec2 PxCoordsToMeterCoords(b2Vec2);
  b2Vec2 MeterCoordsToPxCoords(b2Vec2);

};

}

#endif //FINALPROJECT_ENGINE_H
