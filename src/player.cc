// Copyright (c) 2020 [Your Name]. All rights reserved.

#include <mylibrary/player.h>
#include <chrono>

namespace mylibrary {

Player::Player(b2World* world, bool load_assets) {
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
  last_shot_time_ = std::chrono::system_clock::now();

  if (load_assets) {
    LoadAnimations();
  }
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
    body_->ApplyLinearImpulse(b2Vec2(0, -kJumpForce),
                              body_->GetWorldCenter());
  }
}

b2Body* Player::GetBody() {
  return body_;
}

void Player::ApplyImpulse(b2Vec2 impulse) {
  body_->ApplyLinearImpulse(impulse, body_->GetWorldCenter());
}

bool Player::IsBody(b2Body* body) {
  return body == body_;
}

bool Player::CanShoot() {
  return std::chrono::system_clock::now() - last_shot_time_
  > std::chrono::milliseconds(kShootCooldown);
}

void Player::Shoot() {
  last_shot_time_ = std::chrono::system_clock::now();
}

float Player::GetCooldownPercent() {
  if (CanShoot()) {
    return 1;
  } else {
    return (float) (std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now() - last_shot_time_)).count()
        / (float) kShootCooldown;
  }
}

cinder::gl::TextureRef Player::GetTexture(bool left) {
  if (dead_) {
    return dead_animation_.GetTexture();
  } else if (body_->GetLinearVelocity().y > 0) {
    if (left) {
      return fall_left_animation_.GetTexture();
    }
    return fall_right_animation_.GetTexture();
  } else if (body_->GetLinearVelocity().y < 0) {
    if (left) {
      return rise_left_animation_.GetTexture();
    }
    return rise_right_animation_.GetTexture();
  } else if (abs(body_->GetLinearVelocity().x) > kRunAnimationSpeedThreshold) {
    if (left) {
      return run_left_animation_.GetTexture();
    }
    return run_right_animation_.GetTexture();
  } else {
    if (left) {
      return stand_left_animation_.GetTexture();
    }
    return stand_right_animation_.GetTexture();
  }
}

void Player::LoadAnimations() {
  run_left_animation_ = *LoadAnimation(kPlayerTextureSize,
      kRunLeftCoordinates);
  run_left_animation_.Start(kPlayerAnimationMs);

  run_right_animation_ = *LoadAnimation(kPlayerTextureSize,
      kRunRightCoordinates);
  run_right_animation_.Start(kPlayerAnimationMs);

  rise_left_animation_ = *LoadAnimation(kPlayerTextureSize,
      kRiseLeftCoordinates);
  rise_left_animation_.Start(kPlayerAnimationMs);

  rise_right_animation_ = *LoadAnimation(kPlayerTextureSize,
      kRiseRightCoordinates);
  rise_right_animation_.Start(kPlayerAnimationMs);

  fall_left_animation_ = *LoadAnimation(kPlayerTextureSize,
      kFallLeftCoordinates);
  fall_left_animation_.Start(kPlayerAnimationMs);

  fall_right_animation_ = *LoadAnimation(kPlayerTextureSize,
      kFallRightCoordinates);
  fall_right_animation_.Start(kPlayerAnimationMs);

  stand_left_animation_ = *LoadAnimation(kPlayerTextureSize,
      kStandLeftCoordinates);
  stand_left_animation_.Start(kPlayerAnimationMs);

  stand_right_animation_ = *LoadAnimation(kPlayerTextureSize,
      kStandRightCoordinates);
  stand_right_animation_.Start(kPlayerAnimationMs);

  dead_animation_ = *LoadAnimation(kPlayerTextureSize,
      kDeadCoordinates);
  dead_animation_.Start(kPlayerAnimationMs);
}

Animation* Player::LoadAnimation(int texture_size,
    const std::vector<Coordinate>& coordinates) {
  TextureSheet texture_sheet(texture_size, texture_size, coordinates,
      kPlayerTexturePath);
  Animation* animation = new Animation(texture_sheet);
  return animation;
}

void Player::Die() {
  dead_ = true;
}

void Player::Reset() {
  body_->SetTransform(b2Vec2(0,0),body_->GetAngle());
  body_->SetLinearVelocity(b2Vec2_zero);
  body_->SetAwake(true);
  dead_ = false;
  last_shot_time_ = std::chrono::system_clock::now();
}

}  // namespace mylibrary
