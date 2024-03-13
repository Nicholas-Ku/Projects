#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
	m_happyGuy = new HappyGuy(this, VIEW_WIDTH/2, VIEW_HEIGHT/2);
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	m_happyGuy->doSomething();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete m_happyGuy;
}
