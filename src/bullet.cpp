//
// Created by Aidan on 4/18/2020.
//

#include "mylibrary/bullet.h"

namespace mylibrary {

Bullet::Bullet(b2World* world, const b2Vec2& player_pos,
    const b2Vec2& target_pos) {

  // Calculating trajectory
  b2Vec2 trajectory(target_pos.x - player_pos.x,
      target_pos.y - player_pos.y);
  float two_norm = sqrt(pow(trajectory.x, 2)
      + pow(trajectory.y, 2));
  trajectory = b2Vec2(trajectory.x / two_norm, trajectory.y / two_norm);
  trajectory_ = trajectory;

  // Making bullet velocity and spawn position vectors
  b2Vec2 velocity = b2Vec2(trajectory.x * 300, trajectory.y * 300);
  b2Vec2 spawn_pos = b2Vec2(player_pos.x + 5 * trajectory.x,
      player_pos.y + 5 * trajectory.y);

  // Defining bullet body
  b2BodyDef body_def;
  body_def.type = b2_dynamicBody;
  body_def.position.Set(spawn_pos.x, spawn_pos.y);
  body_def.angle = 0;
  body_ = world->CreateBody(&body_def);
  body_->SetFixedRotation(false);

  b2PolygonShape box_shape;
  box_shape.SetAsBox(0.5,0.5);
  b2FixtureDef box_fixture_def;
  box_fixture_def.shape = &box_shape;
  box_fixture_def.density = 100000;

  body_->CreateFixture(&box_fixture_def);
  body_->SetLinearVelocity(velocity);
  body_->SetGravityScale(0);
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

}