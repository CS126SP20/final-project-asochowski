// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/draw.h>
#include <cinder/app/Window.h>

using std::vector;

namespace myapp {

  using cinder::app::KeyEvent;

  MyApp::MyApp() {
    engine_ = new Engine(kWidth, kHeight, true);
  }

  void MyApp::setup() {

  }

  void MyApp::update() {
    engine_->Step();
  }

  void MyApp::draw() {
    cinder::gl::clear();
    cinder::gl::enableAlphaBlending();

    engine_->UpdateMousePos(getWindow()->getMousePos());
    engine_->Draw();
  }

  void MyApp::keyDown(KeyEvent event) {
    if (event.getCode() == KeyEvent::KEY_ESCAPE && !engine_->IsRunning()) {
      quit();
    }

    engine_->KeyPress(event.getCode());
  }

  void MyApp::keyUp(KeyEvent event) {
    engine_->KeyRelease(event.getCode());
  }

  void MyApp::mouseDown(cinder::app::MouseEvent event) {
    if (event.isLeftDown()) {
      engine_->Click(event.getX(), event.getY());
    }
  }

}  // namespace myapp