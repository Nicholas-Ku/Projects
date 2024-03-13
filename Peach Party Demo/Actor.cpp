#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

HappyGuy::HappyGuy(StudentWorld* sw, int x, int y)
 : GraphObject(IID_STAR_SQUARE, x, y), m_world(sw)
{
}

void HappyGuy::doSomething()
{
    switch (m_world->getAction(1))  // cause hitting TAB to send ACTION_ROLL
    {
      case ACTION_ROLL:
        m_world->playSound(SOUND_BOO_ACTIVATE);
        break;
    }
}
