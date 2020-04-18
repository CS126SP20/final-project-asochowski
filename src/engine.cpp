//
// Created by Aidan on 4/18/2020.
//

#include <set>
#include "mylibrary/engine.h"

mylibrary::Engine::Engine(int screen_width, int screen_height) {
  world_ = new b2World(b2Vec2(0, kGravity));
  screen_width_ = screen_width;
  screen_height_ = screen_height;

  Player player(world_);
  player_ = player;
}

void mylibrary::Engine::Step() {

}

void mylibrary::Engine::KeyPressUpdate(std::set<int> keys_pressed) {

}

void mylibrary::Engine::DrawBodies() {

}

void mylibrary::Engine::DrawPlayer() {

}

void mylibrary::Engine::Start() {

}

void mylibrary::Engine::End() {

}

bool mylibrary::Engine::IsRunning() {
  return false;
}

void mylibrary::Engine::CreateBoundaries() {

}

