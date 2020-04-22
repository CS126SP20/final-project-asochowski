// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <cinder/Rand.h>
#include <catch2/catch.hpp>
#include <Box2D/Box2D.h>
#include <mylibrary/engine.h>
#include <cinder/app/KeyEvent.h>
#include <chrono>

using mylibrary::Engine;
using mylibrary::Player;
using mylibrary::Bullet;
using mylibrary::Debris;

TEST_CASE("Player Movement", "[player][engine][input]") {
  Engine engine(1280, 720);
  Player& player = engine.GetPlayer();
  engine.Start();

  // Need to move about 2 seconds ahead because the player starts in the air
  for (int i = 0; i < 120; i++) {
    engine.Step();
  }

  SECTION("Move Left") {
    engine.KeyPress(cinder::app::KeyEvent::KEY_a);
    for (int i = 0; i < 5; i++) {
      engine.Step();
    }
    REQUIRE(player.GetVelocity().x < 0);
  }

  SECTION("Move Right") {
    engine.KeyPress(cinder::app::KeyEvent::KEY_d);
    for (int i = 0; i < 5; i++) {
      engine.Step();
    }
    REQUIRE(player.GetVelocity().x > 0);
  }

  SECTION("Jump") {
    engine.KeyPress(cinder::app::KeyEvent::KEY_SPACE);
    for (int i = 0; i < 5; i++) {
      engine.Step();
    }
    REQUIRE(player.GetVelocity().y < 0);
  }

}

TEST_CASE("Bullet", "[bullet][player][engine]") {
  Engine engine(1280, 720);
  Player& player = engine.GetPlayer();
  engine.Start();

  // Need to move about 2 seconds ahead because the player starts in the air
  for (int i = 0; i < 120; i++) {
    engine.Step();
  }

  // Wait a second of time
  auto start_time = std::chrono::system_clock::now();
  while (std::chrono::system_clock::now() - start_time <
  std::chrono::seconds(1)) {
    // Just wait a second because the bullet has a 1 second starting cooldown
  }

  SECTION("Shoot downwards moves player") {
    engine.Shoot(640, 1000000);
    for (int i = 0; i < 5; i++) {
      engine.Step();
    }
    REQUIRE(player.GetVelocity().y < 0);
  }

  SECTION("Shoot sideways moves player") {
    engine.Shoot(0, 720);
    for (int i = 0; i < 5; i++) {
      engine.Step();
    }
    REQUIRE(player.GetVelocity().x > 0);
  }

  SECTION("Bullet has correct trajectory") {
    Bullet* bullet = engine.SpawnBullet(640, 0);
    b2Vec2 trajectory = bullet->GetTrajectory();

    REQUIRE(trajectory.x == 0);
    REQUIRE(trajectory.y == -1);
  }

}

TEST_CASE("Debris", "[debris][engine]") {
  Engine engine(1280, 720);
  Player& player = engine.GetPlayer();
  engine.Start();

  // Need to move about 2 seconds ahead because the player starts in the air
  for (int i = 0; i < 120; i++) {
    engine.Step();
  }

  SECTION("Debris hitting player ends game") {
    engine.SpawnDebris(640,360);

    // Lets the debris fall on the player
    for (int i = 0; i < 120; i++) {
      engine.Step();
    }

    REQUIRE(!engine.IsRunning());
  }

  SECTION("Debris is alive until collision") {
    Debris* debris = engine.SpawnDebris(20, 0);

    // Lets the debris fall to the ground
    while (!debris->GetBody()->GetContactList()) {
      engine.Step();
      REQUIRE(debris->GetBody()->IsActive());
    }
  }
}