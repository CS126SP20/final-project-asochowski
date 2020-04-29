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
  body_->SetFixedRotation(true);

  b2PolygonShape box_shape;
  box_shape.SetAsBox(r, r);
  b2FixtureDef box_fixture_def;
  box_fixture_def.shape = &box_shape;
  box_fixture_def.density = 10;

  body_->CreateFixture(&box_fixture_def);
  body_->SetLinearVelocity(b2Vec2(0, 20));

  LoadAnimation();
}

Debris::~Debris() {
  body_->GetWorld()->DestroyBody(body_);

}

b2Body *Debris::GetBody() {
  return body_;
}

float Debris::GetDistanceFrom(b2Body *other_body) {
  b2Vec2 debris_pos = this->GetBody()->GetPosition();
  b2Vec2 other_pos = other_body->GetPosition();

  return sqrt(pow(other_pos.x - debris_pos.x,2) +
  pow(other_pos.y - debris_pos.y,2));
}

void Debris::SetNearMissed() {
  near_missed_ = true;
}

bool Debris::HasBeenNearMissed() {
  return near_missed_;
}

void Debris::LoadAnimation() {
  Animation animation(debris_texture_sheet_);
  animation_ = animation;
  animation_.Start(50);
}

cinder::gl::TextureRef Debris::GetTexture() {
  return animation_.GetTexture();
}

void Debris::LoadTexture() {
  std::vector<Coordinate> coordinates;
  for (int i = 0; i < kNumTextures; i++) {
    coordinates.push_back({i, 3});
  }
  TextureSheet texture_sheet(kDebrisSize, kDebrisSize, coordinates, kDebrisTexture);
  debris_texture_sheet_ = texture_sheet;
}

}