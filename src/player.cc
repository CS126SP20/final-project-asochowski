// Copyright (c) 2020 [Your Name]. All rights reserved.

#include <mylibrary/player.h>

namespace mylibrary {

Player::Player(b2World* world) {
  b2BodyDef body_def;
  body_def.type = b2_dynamicBody;
  body_def.position.Set(0, 0);
  body_def.angle = 0;
  body_ = world->CreateBody(&body_def);
  body_->SetFixedRotation(true);

  b2PolygonShape box_shape;
  box_shape.SetAsBox(2, 2);
  b2FixtureDef box_fixture_def;
  box_fixture_def.shape = &box_shape;
  box_fixture_def.density = 1;

  body_->CreateFixture(&box_fixture_def);
}

Player::Player() {

}

void Player::SetVelocity(b2Vec2 velocity) {
  body_->SetLinearVelocity(velocity);
}

b2Vec2 Player::GetVelocity() {
  return body_->GetLinearVelocity();
}

void Player::Jump() {
  if (body_->GetContactList()) {
    body_->SetLinearVelocity(b2Vec2(body_->GetLinearVelocity().x, 0));
    body_->ApplyLinearImpulse(b2Vec2(0, -600),
                              body_->GetWorldCenter());
  }
}

b2Body* Player::GetBody() {
  return body_;
}

}  // namespace mylibrary
