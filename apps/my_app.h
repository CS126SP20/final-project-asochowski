// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_


#include <cinder/app/App.h>
#include <Box2D/Box2D.h>
#include <set>

namespace myapp {

  class MyApp : public cinder::app::App {
  public:
    MyApp();
    void setup() override;
    void update() override;
    void draw() override;
    void keyDown(cinder::app::KeyEvent) override;
    void keyUp(cinder::app::KeyEvent) override;

    void DrawBodies();
    void CreateRect(double x, double y, double h, double w, double angle);
    void CreateBoundaries();
    b2Body* CreatePlayer(double x, double y, double h, double w, double angle);
  private:
    b2World* game_world;
    b2Body* player;
    std::set<int> held_keys_;
  };

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_