// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/draw.h>

using std::vector;

namespace myapp {

  using cinder::app::KeyEvent;

// How much time goes by in each step of the world
  const float32 kTimeStep = 1.0f / 60.0f;

// These two parameters determine how much Box2D should "correct" velocity and
// position. More iterations = More accurate physics + Higher computational cost
  const int32 kVelIterations = 8;
  const int32 kPosIterations = 3;

  const float32 kMToPx = 20.0f;
  const float32 kPxToM = 1.0f / kMToPx;

  MyApp::MyApp() {
    cinder::gl::enableDepthWrite();
    cinder::gl::enableDepthRead();

    b2Vec2 gravity(0, 3 * 9.8);
    game_world = new b2World(gravity);
    player = CreatePlayer(0,0,1,1,0);
    CreateRect(0,1,1,1,2);
    CreateBoundaries();
  }

  void MyApp::setup() {

  }

  void MyApp::update() {

    if (held_keys_.find(KeyEvent::KEY_a) != held_keys_.end() &&
    held_keys_.find(KeyEvent::KEY_d) != held_keys_.end()) {
      player->SetLinearVelocity(b2Vec2(0,player->GetLinearVelocity().y));
    } else if (held_keys_.find(KeyEvent::KEY_a) != held_keys_.end()) {
      player->SetLinearVelocity(b2Vec2(-10,player->GetLinearVelocity().y));
    } else if (held_keys_.find(KeyEvent::KEY_d) != held_keys_.end()) {
      player->SetLinearVelocity(b2Vec2(10,player->GetLinearVelocity().y));
    } else {
      player->SetLinearVelocity(b2Vec2(0,player->GetLinearVelocity().y));
    }

    if (held_keys_.find(KeyEvent::KEY_SPACE) != held_keys_.end()
    && player->GetContactList()) {
      player->ApplyForceToCenter(b2Vec2(player->GetLinearVelocity().x, -1500));
    }


    game_world->Step(kTimeStep, kVelIterations, kPosIterations);
  }

  void MyApp::draw() {
    cinder::gl::enableAlphaBlending();
    cinder::gl::clear();

    DrawBodies();
  }

  void MyApp::keyDown(KeyEvent event) {
    held_keys_.insert(event.getCode());
  }

  void MyApp::keyUp(KeyEvent event) {
    held_keys_.erase(event.getCode());
  }



  void MyApp::DrawBodies() {
    b2Body* current_body = game_world->GetBodyList();
    while (current_body) {
      vector<b2Vec2> points;
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

        if (i == 0) {
          body_path.moveTo(kMToPx * (pos.x + real_vertex.x) + 400,
              kMToPx * (pos.y + real_vertex.y) + 400);
        } else {
          body_path.lineTo(kMToPx * (pos.x + real_vertex.x) + 400,
              kMToPx * (pos.y + real_vertex.y) + 400);
        }
      }
      body_path.close();

      cinder::gl::drawSolid(body_path);

      current_body = current_body->GetNext();
    }
  }

  void MyApp::CreateRect(double x, double y, double h, double w, double angle) {
    b2BodyDef body_def;
    body_def.type = b2_dynamicBody;
    body_def.position.Set(x, y);
    body_def.angle = angle;
    b2Body* dynamic_body = game_world->CreateBody(&body_def);


    b2PolygonShape box_shape;
    box_shape.SetAsBox(w, h);
    b2FixtureDef box_fixture_def;
    box_fixture_def.shape = &box_shape;
    box_fixture_def.density = 1;

    dynamic_body->CreateFixture(&box_fixture_def);
  }

  void MyApp::CreateBoundaries() {
    b2BodyDef groundBodyDef;
    groundBodyDef.position = b2Vec2(0.0f, 20.0f);
    groundBodyDef.angle = 0;
    groundBodyDef.type = b2_staticBody;
    b2Body* groundBody = game_world->CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(100.0f, 1.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);
  }

  b2Body *MyApp::CreatePlayer(double x, double y, double h, double w, double angle) {
    b2BodyDef body_def;
    body_def.type = b2_dynamicBody;
    body_def.position.Set(x, y);
    body_def.angle = angle;
    b2Body* dynamic_body = game_world->CreateBody(&body_def);
    dynamic_body->SetFixedRotation(true);

    b2PolygonShape box_shape;
    box_shape.SetAsBox(w, h);
    b2FixtureDef box_fixture_def;
    box_fixture_def.shape = &box_shape;
    box_fixture_def.density = 1;

    dynamic_body->CreateFixture(&box_fixture_def);
    return dynamic_body;
  }


}  // namespace myapp