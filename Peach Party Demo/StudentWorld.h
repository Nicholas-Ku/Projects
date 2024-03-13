#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <string>

class HappyGuy;

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();

private:
  HappyGuy* m_happyGuy;
};

#endif // STUDENTWORLD_H_
