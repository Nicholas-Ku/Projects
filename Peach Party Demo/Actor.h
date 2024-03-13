#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class HappyGuy : public GraphObject
{
  public:
    HappyGuy(StudentWorld* sw, int x, int y);
    virtual void doSomething();

  private:
    StudentWorld* m_world;
};

#endif // ACTOR_H_
