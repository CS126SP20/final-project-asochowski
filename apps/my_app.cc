// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/draw.h>

using std::vector;

namespace myapp {

  using cinder::app::KeyEvent;

  MyApp::MyApp() {
    Engine engine(kWidth, kHeight, true);
    engine_ = engine;
    engine_.Start();
  }

  void MyApp::setup() {
    setFullScreen(kFullscreen);
  }

  void MyApp::update() {
    engine_.Step();
  }

  void MyApp::draw() {
    cinder::gl::clear();
    cinder::gl::enableAlphaBlending();

    engine_.UpdateMousePos(getWindow()->getMousePos());
    engine_.Draw();
  }

  void MyApp::keyDown(KeyEvent event) {
    engine_.KeyPress(event.getCode());
  }

  void MyApp::keyUp(KeyEvent event) {
    engine_.KeyRelease(event.getCode());
  }

  void MyApp::mouseDown(cinder::app::MouseEvent event) {
    if (event.isLeftDown()) {
      engine_.Shoot(event.getX(), event.getY());
    }
  }

}  // namespace myapp