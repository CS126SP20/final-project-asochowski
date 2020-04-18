//
// Created by Aidan on 4/18/2020.
//

#ifndef FINALPROJECT_MYLIBRARY_DEBRIS_H_
#define FINALPROJECT_MYLIBRARY_DEBRIS_H_

#include <Box2D/Box2D.h>

namespace mylibrary {

class Debris {
public:
  Debris();
  Debris(b2World* world, int x, int y, float r);
  ~Debris();
  b2Body* GetBody();

private:
  b2Body* body_;
};

}

#endif //FINALPROJECT_MYLIBRARY_DEBRIS_H_
