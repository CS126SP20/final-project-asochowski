//
// Created by Aidan on 4/18/2020.
//

#include <set>
#include <mylibrary/engine.h>
#include <cinder/app/KeyEvent.h>
#include <cinder/Path2d.h>
#include <vector>
#include <cinder/gl/gl.h>

namespace mylibrary {

//==============================================================================
// Public Functions
//==============================================================================

mylibrary::Engine::Engine(int screen_width, int screen_height) {
  world_ = new b2World(b2Vec2(0, kGravity));
  screen_width_ = screen_width;
  screen_height_ = screen_height;
  m_to_px = (float) screen_width / 96;
  px_to_m = 1.0f / m_to_px;
  Player player(world_);
  player_ = player;
  CreateBoundaries();
  start_time_ = std::chrono::system_clock::now();
}

Engine::Engine() {
  screen_height_ = 0;
  screen_width_ = 0;
}

void mylibrary::Engine::Step() {
  if (running_) {
    CheckDebrisSpawn();
    CheckBulletCollisions();
    CheckDebrisCollisions();
    UpdatePlayer();
    world_->Step(kTimeStep, kVelIterations, kPosIterations);
  }
}

void mylibrary::Engine::KeyPress(int key_code) {
  held_keys_.insert(key_code);
}

void mylibrary::Engine::KeyRelease(int key_code) {
  held_keys_.erase(key_code);
}

void Engine::Draw() {
  DrawHitBoxes();
}

void mylibrary::Engine::Start() {
  running_ = true;
  start_time_ = std::chrono::system_clock::now();
  last_debris_time_ = std::chrono::system_clock::now();
}

void mylibrary::Engine::End() {
  running_ = false;
}

bool mylibrary::Engine::IsRunning() {
  return running_;
}

void Engine::SpawnDebris(int x_px, int y_px) {
  b2Vec2 coords = PxCoordsToMeterCoords(b2Vec2(x_px, y_px));

  Debris* debris = new Debris(world_, coords.x, coords.y, 2.5);
  all_debris_.push_back(debris);
}

Bullet* Engine::SpawnBullet(int x_px, int y_px) {
  b2Vec2 target = PxCoordsToMeterCoords(b2Vec2(x_px, y_px));

  auto* bullet = new Bullet(world_, player_.GetBody()->GetPosition(), target);
  all_bullets_.push_back(bullet);
  return bullet;
}

//==============================================================================
// Private Functions
//==============================================================================

void mylibrary::Engine::CreateBoundaries() {

  // Creating the ground
  b2BodyDef ground_body_def;
  ground_body_def.position = b2Vec2(0.0f, 26.0f);
  ground_body_def.angle = 0;
  ground_body_def.type = b2_staticBody;
  b2Body* ground_body = world_->CreateBody(&ground_body_def);

  b2PolygonShape ground_box;
  ground_box.SetAsBox(45.0f, 1.0f);
  ground_body->CreateFixture(&ground_box, 0.0f);

  // Creating the two platforms
  b2PolygonShape platform_box;
  platform_box.SetAsBox(5.0f, 1.0f);

  for (int i = -1; i < 2; i += 2) {
    b2BodyDef platform_body_def;
    platform_body_def.position = b2Vec2(25.0f * i, 12.0f);
    platform_body_def.angle = 0;
    platform_body_def.type = b2_staticBody;
    b2Body* platform_body = world_->CreateBody(&platform_body_def);

    platform_body->CreateFixture(&platform_box, 0.0f);
  }
}

void Engine::UpdatePlayer() {
  bool press_w = held_keys_.find(cinder::app::KeyEvent::KEY_w)
      != held_keys_.end();
  bool press_a = held_keys_.find(cinder::app::KeyEvent::KEY_a)
                 != held_keys_.end();
  bool press_s = held_keys_.find(cinder::app::KeyEvent::KEY_s)
                 != held_keys_.end();
  bool press_d = held_keys_.find(cinder::app::KeyEvent::KEY_d)
                 != held_keys_.end();
  bool press_space = held_keys_.find(cinder::app::KeyEvent::KEY_SPACE)
                     != held_keys_.end();

  b2Vec2 current_velocity = player_.GetVelocity();

  // Setting player's left and right movements according to A and D.
  if (press_a && press_d) {
    player_.SetVelocity(b2Vec2(0, current_velocity.y));
  } else if (press_a) {
    player_.SetVelocity(b2Vec2(-kMoveSpeed, current_velocity.y));
  } else if (press_d) {
    player_.SetVelocity(b2Vec2(kMoveSpeed, current_velocity.y));
  } else {
    player_.SetVelocity(b2Vec2(0, current_velocity.y));
  }

  if (press_space) {
    player_.Jump();
  }
}

void Engine::DrawHitBoxes() {
  b2Body* current_body = world_->GetBodyList();
  while (current_body) {
    std::vector<b2Vec2> points;
    cinder::Path2d body_path;
    b2Vec2 pos = current_body->GetPosition();

    for (int i = 0; i < ((b2PolygonShape*)current_body->GetFixtureList()->
        GetShape())->GetVertexCount(); i++) {

      b2Vec2 vertex = ((b2PolygonShape*)current_body->GetFixtureList()->
          GetShape())->GetVertex(i);
      float32 theta = current_body->GetAngle();

      // Since Box2D only gives the fixture's original coordinates, the
      // current position, and the current angle, we can use a rotation matrix
      // to calculate the real vertex coordinate.
      b2Vec2 real_vertex(cos(theta)*vertex.x - sin(theta)*vertex.y,
                         sin(theta)*vertex.x + cos(theta)*vertex.y);
      float real_x = m_to_px * (pos.x + real_vertex.x)
          + (float) screen_width_ / 2;
      float real_y = m_to_px * (pos.y + real_vertex.y)
          + (float) screen_height_ / 2;

      if (i == 0) {
        body_path.moveTo(real_x, real_y);
      } else {
        body_path.lineTo(real_x, real_y);
      }
    }

    body_path.close();
    cinder::gl::draw(body_path);
    current_body = current_body->GetNext();
  }
}

void Engine::CheckDebrisCollisions() {
  for (int i = all_debris_.size() - 1; i >= 0; i--) {
    Debris* debris = all_debris_.at(i);
    if (debris->GetBody()->GetContactList()) {
      all_debris_.erase(all_debris_.begin()+i);
      delete debris;
      break;
    }
  }
}

void Engine::CheckBulletCollisions() {
  for (int i = all_bullets_.size() - 1; i >= 0; i--) {
    Bullet* bullet = all_bullets_.at(i);
    while (bullet->GetBody()->GetContactList()) {
      if (bullet->GetBody()->GetContactList()->other->GetType() == b2_staticBody) {
        all_bullets_.erase(all_bullets_.begin()+i);
        delete bullet;
        break;
      } else {
        break;
      }
    }
  }
}

void Engine::CheckDebrisSpawn() {
  std::chrono::time_point<std::chrono::system_clock> current_time =
      std::chrono::system_clock::now();
  auto time_since_last_spawn = current_time - last_debris_time_;
  auto spawn_interval = GetDebrisSpawnInterval();

  if (time_since_last_spawn >= spawn_interval) {
    SpawnDebris(rand() % screen_width_,-100);
    last_debris_time_ = std::chrono::system_clock::now();
  }
}

std::chrono::milliseconds Engine::GetDebrisSpawnInterval() {
  std::chrono::time_point<std::chrono::system_clock> current_time =
      std::chrono::system_clock::now();
  auto time_since_start = current_time - start_time_;
  int time_in_seconds = (std::chrono::duration_cast<std::chrono::seconds>
      (time_since_start)).count();

  double current_spawn_speed = 5 * log(.135 * time_in_seconds + 1);
  int seconds_between_spawns = 1000.0 / current_spawn_speed;

  std::chrono::milliseconds milliseconds(seconds_between_spawns);

  return milliseconds;
}


b2Vec2 Engine::PxCoordsToMeterCoords(b2Vec2 px_vec) {
  return b2Vec2((px_vec.x - (float) screen_width_ / 2) * px_to_m,
                (px_vec.y - (float) screen_height_ / 2) * px_to_m);
}

b2Vec2 Engine::MeterCoordsToPxCoords(b2Vec2 m_vec) {
  return b2Vec2( (m_to_px * m_vec.x) + (float) screen_width_ / 2,
                 (m_to_px * m_vec.y) + (float) screen_height_ / 2);
}

void Engine::Shoot(int x_px, int y_px) {
  Bullet* bullet = SpawnBullet(x_px, y_px);
  b2Vec2 bullet_trajectory = bullet->GetTrajectory();
  b2Vec2 player_knockback_impulse =
      b2Vec2(-bullet_trajectory.x, -bullet_trajectory.y);


}


}