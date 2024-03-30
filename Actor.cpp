#include "GraphObject.h"
#include "GameConstants.h"
#include "Actor.h"
#include "StudentWorld.h"
#include <vector>

enum Direction { none, up, down, left, right };


// --------------------- class GameObject -------------------
GameObject::GameObject(int x, int y, const int IMAGE_ID, Direction startDirection, float size, int depth)
	:GraphObject(IMAGE_ID, x, y, startDirection, size, depth) {
	this->x = x;
	this->y = y;
	this->IMAGE_ID = IMAGE_ID;
	this->startDirection = startDirection;
	this->size = size;
	this->depth = depth;
	//this->world = world;
}

GameObject::~GameObject() {

}

// ------------------ BOULDER CLASS ------------------------
Boulder::Boulder(int startX, int startY, StudentWorld* world)
	: GameObject(startX, startY, TID_BOULDER, down, 1, 1) {
	this->startX = startX;
	this->startY = startY;
	this->isBoulderAlive = true;
	this->isBoulderInStableCondition = true;
	this->world = world;
}

void Boulder::doSomething() {

	bool isEarthBeneath = this->world->isEarthUnder(this->getX(), this->getY());
	if (isBoulderAlive == false)
		return; 
	if (isBoulderAlive == true && isEarthBeneath == true) {
		this->isBoulderAlive = true;
		this->isBoulderInStableCondition = true;
		return;
	}
	else if (isBoulderAlive == true && isEarthBeneath == false) {
		// wait for 30 ticks 
		if (numTicks >= 30) {
			//this->isBoulderAlive = false;
			this->isBoulderInStableCondition = false;
			// must go into falling state
			moveTo(this->getX(), this->getY() - 1);

			// case1 - falling below y = -1 
			if (this->getY() == -1) {
				this->setVisible(false);
				this->isBoulderAlive = false; 
				return;
			}
			// case2 - falling on top of Earth 
			if (this->world->isEarthUnder(this->getX(), this->getY()) == true) {
				this->world->playSound(SOUND_FALLING_ROCK);
				this->setVisible(false);
				this->isBoulderAlive = false;
				return; 
			}

			// case3 - runs on top of player = player dead 
			if (this->getY() == this->world->getPlayer()->getY() + 4) {
				this->setVisible(false);
				this->isBoulderAlive = false;
				this->world->playSound(SOUND_PLAYER_GIVE_UP);
				this->world->getPlayer()->setVisible(false); 
				this->world->getPlayer()->setPlayerStatus(false);
				return; 
			}

			// case4 - boulder gets on top of boulder -- to check 
			std::vector<Boulder*> vB = this->world->getBoulderV();
			for (int i = 0; i < vB.size(); i++) {
				if (vB[i]->isBoulderAlive == true && this->getY() == vB[i]->getY() + 4) {
					vB[i]->setVisible(false);
					vB[i]->setBoulderStatus(false); 
				}
			}

		}
		else {
			numTicks++;
			return;
		}
	}
	return;
}

// --------------------- PICKUP CLASS  ------------------

Pickups::Pickups(int x, int y, const int IMAGE_ID, Direction startDirection, float size, int depth)
	: GameObject(x, y, IMAGE_ID, startDirection, size, depth) {
	this->x = x;
	this->y = y;
	this->IMAGE_ID = IMAGE_ID;
	this->startDirection = startDirection;
	this->size = size;
	this->depth = depth;
}

void Pickups::doSomething() {
	return;
}

GoldNugget::GoldNugget(int x, int y) : Pickups(x, y, TID_GOLD, right, 1, 2) {

}

void GoldNugget::doSomething() {
	return;
}

SonarKit::SonarKit(int x, int y) : Pickups(x, y, TID_SONAR, right, 1, 2) {

}

void SonarKit::doSomething() {
	return;
}

BarrelOfOil::BarrelOfOil(int x, int y, StudentWorld* world)
	: Pickups(x, y, TID_BARREL, right, 1, 2) {
	this->isBarrelAlive = true;
	this->world = world;
}

void BarrelOfOil::doSomething() {

	// case1 - barrel is dead = do nothing 
	if (!isBarrelAlive)
		return;

	// if (barrel is NOT visible AND TM is <=4 units away) 
	//		-> make itself visisble and return 
	int tmX = this->world->getPlayer()->getX();
	int tmY = this->world->getPlayer()->getY();
	int oilX = this->getX();
	int oilY = this->getY();
	if (!this->isVisible() && lessThanOrEqualto4(tmX, tmY, oilX, oilY)) {
		this->setVisible(true);
		return;
	}
	if (this->isVisible() && lessThanOrEqualto3(tmX, tmY, oilX, oilY)) {
		this->isBarrelAlive = false;
		this->world->playSound(SOUND_FOUND_OIL);
		this->setVisible(false);
		this->world->getPlayer()->addPlayerPts(1000);
		// Increase score by 1000 pts 
	}

}
bool BarrelOfOil::lessThanOrEqualto3(int tmX, int tmY, int oilX, int oilY) {
	double rad = sqrt(pow(tmX - oilX, 2) + pow(tmY - oilY, 2));
	if (rad <= 3)
		return true;
	return false;
}
bool BarrelOfOil::lessThanOrEqualto4(int tmX, int tmY, int oilX, int oilY) {
	double rad = sqrt(pow(tmX - oilX, 2) + pow(tmY - oilY, 2));
	if (rad <= 4)
		return true;
	return false;
}

WaterPool::WaterPool(int x, int y) : Pickups(x, y, TID_WATER_POOL, right, 1, 2) {

}

void WaterPool::doSomething() {
	return;
}


// --------------------- class Tunnelman ----------------
Tunnelman::Tunnelman(int x, int y, StudentWorld* world) :GameObject(x, y, TID_PLAYER, right, 1, 0) {
	x = 30;
	y = 60;
	this->world = world;
	this->x = x;
	this->y = y;
}
Tunnelman::~Tunnelman() {}

void Tunnelman::doSomething() {
	return;
}

bool Tunnelman::isBoulderPresent(int x, int y) {
	// First Check Boulder x And y , then check if the boulder is Visible 
	std::vector <Boulder*> vBoulder = this->world->getBoulderV();
	for (int i = 0; i < vBoulder.size(); i++) {
		if ((vBoulder[i]->getX() == x || vBoulder[i]->getX() + 1 == x || vBoulder[i]->getX() + 2 == x || vBoulder[i]->getX() + 3 == x) &&
			(vBoulder[i]->getY() + 1 == y || vBoulder[i]->getY() + 2 == y || vBoulder[i]->getY() + 3 == y)
			&& vBoulder[i]->getBoulderStatus() == true) {
			return true;
		}
	}
	return false;
}

void Tunnelman::doAct(bool key, int ch) {
	std::vector <Boulder*> vBoulder = this->world->getBoulderV();
	if (key) {
		switch (ch) {
		case KEY_PRESS_LEFT:
			// y+1 ====> y+3
			// Set direction first then check collisions on directioin
			setDirection(left);
			if (x > 0
				// One block left of the player and all of the player y values
				&& !isBoulderPresent(x - 1, y)
				&& !isBoulderPresent(x - 1, y + 1)
				&& !isBoulderPresent(x - 1, y + 2)
				&& !isBoulderPresent(x - 1, y + 3)
				&& !isBoulderPresent(x - 1, y + 4)) {
				x -= 1;
			}
			break;
		case KEY_PRESS_RIGHT:
			// for x, its always x +4 
			setDirection(right);
			if (x + 4 < VIEW_WIDTH
				// One block right of player and all of the player y values
				&& !isBoulderPresent(x + 4, y)
				&& !isBoulderPresent(x + 4, y + 1)
				&& !isBoulderPresent(x + 4, y + 2)
				&& !isBoulderPresent(x + 4, y + 3)
				&& !isBoulderPresent(x + 4, y + 4)) {
				x += 1;
			}
			break;
		case KEY_PRESS_UP:  // NOT WORKING 
			setDirection(up);
			if (y + 4 < 64
				// One block above the player and all of the x values
				&& !isBoulderPresent(x, y + 4)
				&& !isBoulderPresent(x + 1, y + 4)
				&& !isBoulderPresent(x + 2, y + 4)
				&& !isBoulderPresent(x + 3, y + 4)) {
				y += 1;
			}
			break;
		case KEY_PRESS_DOWN: // NOT WORKING 
			setDirection(down);
			if (y > 0
				// One block below and all of the x values 
				&& !isBoulderPresent(x, y)
				&& !isBoulderPresent(x, y - 1)
				&& !isBoulderPresent(x + 1, y - 1)
				&& !isBoulderPresent(x + 2, y - 1)
				&& !isBoulderPresent(x + 3, y - 1)) {
				y -= 1;
			}
			break;
		default:
			return;
			break;
		}
		moveTo(x, y);
	}
	else {
		return;
	}
	return;
}

int Tunnelman::returnDirection() {
	if (this->getDirection() == up) {
		return 0;
	}
	if (this->getDirection() == down) {
		return 1;
	}
	if (this->getDirection() == right) {
		return 2;
	}
	if (this->getDirection() == left) {
		return 3;
	}
	return 0;
}

StudentWorld* Tunnelman::getWorld() {
	return world;
}

// --------------------- class Earth  -------------------
Earth::Earth(int x, int y) : GameObject(x, y, TID_EARTH, right, 0.25, 3) {
	this->x = x;
	this->y = y;
}
Earth::~Earth() {}
void Earth::doSomething() {
	// Earth doSometihng does nothing 
	return;
}