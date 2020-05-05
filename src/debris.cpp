//
// Created by Aidan on 4/18/2020.
//

#include "mylibrary/debris.h"

namespace mylibrary {

// The texture sheet for the debris animation. Only needs to be loaded once
// for all debris.
static TextureSheet debris_texture_sheet_;

Debris::Debris() {
  // Do nothing
}

Debris::Debris(b2World* world, int x, int y, float r, bool load_assets) {

  // Defining the body.
  b2BodyDef body_def;
  body_def.type = b2_dynamicBody;
  body_def.position.Set(x, y);
  body_def.angle = 0;
  body_ = world->CreateBody(&body_def);
  body_->SetFixedRotation(true);

  // Defining the shape.
  b2PolygonShape box_shape;
  box_shape.SetAsBox(r, r);
  b2FixtureDef box_fixture_def;
  box_fixture_def.shape = &box_shape;
  box_fixture_def.density = kDebrisDensity;

  // Adding the body to the world.
  body_->CreateFixture(&box_fixture_def);

  // Loading the animations if specified.
  if (load_assets) {
    LoadAnimation();
  }
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
  animation_.Start(kDebrisAnimationMs);
}

cinder::gl::TextureRef Debris::GetTexture() {
  return animation_.GetTexture();
}

void Debris::LoadTexture() {
  TextureSheet texture_sheet(kDebrisTextureSize, kDebrisTextureSize,
      kDebrisCoordinates, kDebrisTexturePath);
  debris_texture_sheet_ = texture_sheet;
}

}