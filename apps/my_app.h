// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_


#include <cinder/app/App.h>
#include <Box2D/Box2D.h>
#include <mylibrary/engine.h>
#include <set>

using mylibrary::Engine;

namespace myapp {
  const int kWidth = 1280;
  const int kHeight = 720;
const bool kFullscreen = false;

  class MyApp : public cinder::app::App {
  public:
    MyApp();
    void setup() override;
    void update() override;
    void draw() override;
    void keyDown(cinder::app::KeyEvent) override;
    void keyUp(cinder::app::KeyEvent) override;
    void mouseDown(cinder::app::MouseEvent) override;
  private:
    Engine* engine_;
  };

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_