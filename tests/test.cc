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
  Engine engine(1280, 720, false);
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
  Engine engine(1280, 720, false);
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
  Engine engine(1280, 720, false);
  Player& player = engine.GetPlayer();
  engine.Start();

  // Need to move about 2 seconds ahead because the player starts in the air
  for (int i = 0; i < 120; i++) {
    engine.Step();
  }

  SECTION("Debris hitting player kills player and R resets game") {
    engine.SpawnDebris(640,360);

    // Lets the debris fall on the player
    for (int i = 0; i < 120; i++) {
      engine.Step();
    }

    REQUIRE(engine.IsOver());

    engine.KeyPress(cinder::app::KeyEvent::KEY_r);
    engine.Step();

    REQUIRE(!engine.IsOver());
    REQUIRE(engine.IsRunning());
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

TEST_CASE("Shooting Cooldown", "[player][engine][bullet]") {
  Engine engine(1280, 720, false);
  Player& player = engine.GetPlayer();
  engine.Start();

  // Need to move about 2 seconds ahead because the player starts in the air
  for (int i = 0; i < 120; i++) {
    engine.Step();
  }

  SECTION("Player can't shoot right away") {
    REQUIRE(player.GetCooldownPercent() > 0);
  }

  SECTION("Player can shoot after a second") {
    // Wait a second of time
    auto start_time = std::chrono::system_clock::now();
    while (std::chrono::system_clock::now() - start_time <
           std::chrono::seconds(1)) {
      // Just wait a second because the shot has a 1 second starting cooldown
    }

    REQUIRE(player.GetCooldownPercent() == 1.0f);
  }

  SECTION("Shooting resets cooldown") {
    // Wait a second of time
    auto start_time = std::chrono::system_clock::now();
    while (std::chrono::system_clock::now() - start_time <
           std::chrono::seconds(1)) {
      // Just wait a second because the shot has a 1 second starting cooldown
    }

    player.Shoot();
    REQUIRE(player.GetCooldownPercent() == 0);
  }
}

TEST_CASE("Score Calculation", "[engine]") {
  Engine engine(1280, 720, false);
  Player& player = engine.GetPlayer();
  engine.Start();

  int random_1 = rand() % 100000;
  int random_2 = rand() % 100000;
  int random_3 = rand() % 100000;

  SECTION("Game starts with zero score") {
    REQUIRE(engine.CalculateScore(0, 0, 0) == 0);
  }

  SECTION("No score if no debris shot or near misses") {
    REQUIRE(engine.CalculateScore(0, 0, 10000) == 0);
  }

  SECTION("Debris shot are worth a base of 2") {
    REQUIRE(engine.CalculateScore(random_1, 0, exp(1) - 1.1) == random_1 * 2);
  }

  SECTION("Near misses are worth a base of 1") {
    REQUIRE(engine.CalculateScore(0, random_2, exp(1) - 1.1) == random_2);
  }

  SECTION("Score is sum of debris score and near miss score") {
    REQUIRE(engine.CalculateScore(random_1, random_2, exp(1) - 1.1) ==
    random_2 + random_1 * 2);
  }

  SECTION("Score is multiplied by ln(seconds + 1.1)") {
    REQUIRE(engine.CalculateScore(random_1, random_2, random_3) ==
            (int) ((random_2 + random_1 * 2) * log(random_3 + 1.1)));
  }
}