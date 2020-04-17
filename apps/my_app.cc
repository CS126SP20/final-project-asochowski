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
  CreateBoundaries();
}

void MyApp::setup() {

}

void MyApp::update() {
  game_world->Step(kTimeStep, kVelIterations, kPosIterations);
}

void MyApp::draw() {
  cinder::gl::enableAlphaBlending();
  cinder::gl::clear();

  DrawBodies();
}

void MyApp::keyDown(KeyEvent event) {
  b2Vec2 vel = player->GetLinearVelocity();
  switch (event.getCode()) {
    case KeyEvent::KEY_UP:
    case KeyEvent::KEY_SPACE:
    case KeyEvent::KEY_w: {
      if (player->GetContactList()) {
        player->ApplyForceToCenter(b2Vec2(0, -7000));
      }
      break;
    }
    case KeyEvent::KEY_DOWN:
    case KeyEvent::KEY_s: {
      break;
    }
    case KeyEvent::KEY_LEFT:
    case KeyEvent::KEY_a: {
      vel.x = -20;
      player->SetLinearVelocity(vel);
      break;
    }
    case KeyEvent::KEY_RIGHT:
    case KeyEvent::KEY_d: {
      vel.x = 20;
      player->SetLinearVelocity(vel);
      break;
    }
  }
}

void MyApp::keyUp(KeyEvent event) {
  b2Vec2 vel = player->GetLinearVelocity();
  switch (event.getCode()) {
    case KeyEvent::KEY_UP:
    case KeyEvent::KEY_w: {
      break;
    }
    case KeyEvent::KEY_DOWN:
    case KeyEvent::KEY_s: {
      break;
    }
    case KeyEvent::KEY_LEFT:
    case KeyEvent::KEY_a: {
      if (vel.x == -20) {
        vel.x = 0;
      }
      player->SetLinearVelocity(vel);
      break;
    }

    case KeyEvent::KEY_RIGHT:
    case KeyEvent::KEY_d: {
      if (vel.x == 20) {
        vel.x = 0;
      }
      player->SetLinearVelocity(vel);
      break;
    }
  }
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
      if (i == 0) {
        body_path.moveTo(kMToPx * (pos.x + vertex.x) + 400, kMToPx * (pos.y + vertex.y) + 400);
      } else {
        body_path.lineTo(kMToPx * (pos.x + vertex.x) + 400, kMToPx * (pos.y + vertex.y) + 400);
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

  b2PolygonShape box_shape;
  box_shape.SetAsBox(w, h);
  b2FixtureDef box_fixture_def;
  box_fixture_def.shape = &box_shape;
  box_fixture_def.density = 1;

  dynamic_body->CreateFixture(&box_fixture_def);
  return dynamic_body;
}


}  // namespace myapp
