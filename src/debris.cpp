//
// Created by Aidan on 4/18/2020.
//

#include "mylibrary/debris.h"

namespace mylibrary {

Debris::Debris() {

}

Debris::Debris(b2World *world, int x, int y, float r) {
  b2BodyDef body_def;
  body_def.type = b2_dynamicBody;
  body_def.position.Set(x, y);
  body_def.angle = 0;
  body_ = world->CreateBody(&body_def);
  body_->SetFixedRotation(false);

  b2PolygonShape box_shape;
  box_shape.SetAsBox(r, r);
  b2FixtureDef box_fixture_def;
  box_fixture_def.shape = &box_shape;
  box_fixture_def.density = 10;

  body_->CreateFixture(&box_fixture_def);
  body_->SetAngularVelocity(5);
  body_->SetLinearVelocity(b2Vec2(0, 20));
}

Debris::~Debris() {
  body_->GetWorld()->DestroyBody(body_);
}

b2Body *Debris::GetBody() {
  return body_;
}

}