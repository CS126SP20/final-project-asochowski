// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <cinder/Rand.h>
#include <catch2/catch.hpp>
#include <Box2D/Box2D.h>
#include <mylibrary/engine.h>
#include <cinder/app/KeyEvent.h>

using mylibrary::Engine;
using mylibrary::Player;

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

TEST_CASE("", "[]") {

}