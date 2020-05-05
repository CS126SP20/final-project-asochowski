//
// Created by Aidan on 4/18/2020.
//

#include "mylibrary/bullet.h"

namespace mylibrary {

// The texture sheet for the bullet animation. Only needs to be loaded once
// for all bullets.
static TextureSheet bullet_texture_sheet_;

Bullet::Bullet(b2World* world, const b2Vec2& player_pos,
    const b2Vec2& target_pos, bool load_assets) {

  // Calculating trajectory
  b2Vec2 trajectory(target_pos.x - player_pos.x,
      target_pos.y - player_pos.y);
  float two_norm = sqrt(pow(trajectory.x, 2)
      + pow(trajectory.y, 2));
  trajectory = b2Vec2(trajectory.x / two_norm, trajectory.y / two_norm);
  trajectory_ = trajectory;

  // Making bullet velocity and spawn position vectors
  b2Vec2 velocity = b2Vec2(trajectory.x * kBulletVelocity,
      trajectory.y * kBulletVelocity);
  b2Vec2 spawn_pos = b2Vec2(player_pos.x + kSpawnDistance * trajectory.x,
      player_pos.y + kSpawnDistance * trajectory.y);

  // Defining bullet body
  b2BodyDef body_def;
  body_def.type = b2_dynamicBody;
  body_def.position.Set(spawn_pos.x, spawn_pos.y);
  body_def.angle = 0;
  body_ = world->CreateBody(&body_def);
  body_->SetFixedRotation(false);

  // Defining bullet shape.
  b2PolygonShape box_shape;
  box_shape.SetAsBox(kBulletSize, kBulletSize);
  b2FixtureDef box_fixture_def;
  box_fixture_def.shape = &box_shape;
  box_fixture_def.density = kBulletDensity;

  // Adding the body to the world with no gravity at a fixed speed.
  body_->CreateFixture(&box_fixture_def);
  body_->SetLinearVelocity(velocity);
  body_->SetGravityScale(0);
  body_->SetBullet(true);

  spawn_time_ = std::chrono::system_clock::now();

  // Load the animations, if specified.
  if (load_assets) {
    LoadAnimation();
  }
}

Bullet::Bullet() {

}

Bullet::~Bullet() {
  body_->GetWorld()->DestroyBody(body_);
}

b2Body* Bullet::GetBody() {
  return body_;
}

b2Vec2 Bullet::GetTrajectory() {
  return trajectory_;
}

int Bullet::GetMilliseconds() {
  int time_in_ms = (std::chrono::duration_cast<std::chrono::milliseconds>
      (std::chrono::system_clock::now() - spawn_time_)).count();
  return time_in_ms;
}

void Bullet::LoadTexture() {
  TextureSheet texture_sheet(kBulletTextureSize, kBulletTextureSize,
      kBulletCoordinates, kBulletTexturePath);
  bullet_texture_sheet_ = texture_sheet;
}

cinder::gl::TextureRef Bullet::GetTexture() {
  return animation_.GetTexture();
}

void Bullet::LoadAnimation() {
  Animation animation(bullet_texture_sheet_);
  animation_ = animation;
  animation_.Start(kBulletAnimationMs);}
}