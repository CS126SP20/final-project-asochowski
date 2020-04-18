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
}

Engine::Engine() {
  screen_height_ = 0;
  screen_width_ = 0;
}

void mylibrary::Engine::Step() {
  if (running_) {
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
}

void mylibrary::Engine::End() {
  running_ = false;
}

bool mylibrary::Engine::IsRunning() {
  return running_;
}

//==============================================================================
// Private Functions
//==============================================================================

void mylibrary::Engine::CreateBoundaries() {
  b2BodyDef groundBodyDef;
  groundBodyDef.position = b2Vec2(0.0f, 20.0f);
  groundBodyDef.angle = 0;
  groundBodyDef.type = b2_staticBody;
  b2Body* groundBody = world_->CreateBody(&groundBodyDef);

  b2PolygonShape groundBox;
  groundBox.SetAsBox(100.0f, 1.0f);
  groundBody->CreateFixture(&groundBox, 0.0f);
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


}