#include "StudentWorld.h"
#include <string>
#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <algorithm>
#include <vector>
#include <random>


GameWorld* createStudentWorld(std::string assetDir) {
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir) {

}

void StudentWorld::makeOil() {
	int currLevel = this->getLevel();
	int numOfOil = std::min(2 + currLevel, 21);
	int x, y;
	for (int i = 0; i < numOfOil; i++) {
		if (i % 2 == 0) {
			distributeInfield(x, y, true);
			vOil.push_back(new BarrelOfOil(x, y, this));
		}
		else {
			distributeInfield(x, y, false);
			vOil.push_back(new BarrelOfOil(x, y, this));
		}
	}
	for (int i = 0; i < vOil.size(); i++)
		this->vOil[i]->setVisible(false);
}

//////////////// TESTING 

void StudentWorld::setDisplayText() {
	int level = this->getLevel();
	int lives = this->getLives();
	//	int health = this->getCurrentHealth();  HOW TO GET CURRENT HEALTH 
	//	int squirt = this->getSquirtsLef();  HOW TO GET SQUIRTS, GOLD, BARRELS, SONAR, SCORE 

}
void StudentWorld::distributeInfield(int& x, int& y, bool randomBool) {
	std::random_device RD;
	int leftXx = 0, rightXx = 0, leftYy = 0, rightYy = 0;
	// compare vBoulders , and check all vectors 
	int vX, vY;


	if (randomBool == true) {
		while (true) {
			std::uniform_int_distribution<int> leftXd(3, 25);
			leftXx = leftXd(RD);

			std::uniform_int_distribution<int> leftYd(21, 55);
			leftYy = leftYd(RD);

			if (!inRestrictedArea(leftXx) && checkAllRaidus(leftXx, leftYy) == true) {
				break;
			}
		}
		x = leftXx;
		y = leftYy;
	}
	else {
		while (true) {
			std::uniform_int_distribution<int> rightXd(35, 58);
			rightXx = rightXd(RD);

			std::uniform_int_distribution<int> rightYd(21, 55);
			rightYy = rightYd(RD);

			if (!inRestrictedArea(rightXx) && checkAllRaidus(rightXx, rightYy) == true) {
				break;
			}
		}
		x = rightXx;
		y = rightYy;
	}

}

bool StudentWorld::checkAllRaidus(int x, int y) {
	int vX, vY;
	// repeat for every vector 
	bool validRad = false;
	if (vBoulder.size() == 0)
		return true;
	for (int i = 0; i < vBoulder.size(); i++) {
		vX = vBoulder[i]->getX();
		vY = vBoulder[i]->getY();
		if (moreThanRadiusOfSix(vX, vY, x, y) == true)
			return true;
	}
	/// after comparing ALL VECTORS 
	return false;
}



int StudentWorld::init() {
	makeEarth();
	this->player = new Tunnelman(30, 60, this);
	this->player->setVisible(true);
	makeBoulders();
	makeOil();
	return GWSTATUS_CONTINUE_GAME;
}



bool StudentWorld::isEarthUnder(int x, int y) {
	for (int x1 = 0; x1 < 4; x1++) {
		// if there's earth [visible or invisible[ AND its visible 
		if (earthArr[x + x1][y - 1] != nullptr && this->earthArr[x + x1][y - 1]->isVisible())
			return true;
	}
	return false;
}

void StudentWorld::makeBoulders() {
	int currLevel = this->getLevel();
	int numBoulders = std::min((currLevel / 2) + 2, 9);
	int x, y;
	for (int i = 0; i < numBoulders; i++) {
		if (i % 2 == 0) {
			distributeInfield(x, y, true);
			//make4by4(x, y); 
			vBoulder.push_back(new Boulder(x, y, this));
		}
		else {
			distributeInfield(x, y, false);
			//make4by4(x, y);
			vBoulder.push_back(new Boulder(x, y, this));
		}
	}
	for (int i = 0; i < vBoulder.size(); i++) {
		if (vBoulder[i]->getBoulderStatus() == true) {
			make4by4(vBoulder[i]->getX(), vBoulder[i]->getY());
			this->vBoulder[i]->setVisible(true);
		}
	}

}

bool StudentWorld::moreThanRadiusOfSix(int x1, int y1, int x2, int y2) {
	double rad = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	if (rad >= 6)
		return true;
	return false;
}

void StudentWorld::make4by4(int x, int y) {
	int x1, y1;
	for (x1 = 0; x1 < 4; x1++) {
		for (y1 = 0; y1 < 4; y1++) {
			if (earthArr[x + x1][y + y1] != nullptr) {
				this->earthArr[x + x1][y + y1]->setVisible(false);
			}
		}
	}
}

bool StudentWorld::inRestrictedArea(int x, int y) {
	int arr[8] = { 28, 29, 30, 31, 32, 33, 34, 35 };
	for (int i = 0; i < 8; i++) {
		if (x == arr[i]) {
			return true;
		}
	}
	return false;
}

int StudentWorld::move() {
	// HANDLES PLAYER INPUT
	int input = 0;
	bool move = getKey(input);
	player->doAct(move, input);

	deleteEarth(player->getX(), player->getY(), player->returnDirection());

	// call boulders doSomething 
	for (int i = 0; i < vBoulder.size() && vBoulder.size() != 0; i++) {
		vBoulder[i]->doSomething();
	}

	// call oil's doSomething 
	for (int i = 0; i < vOil.size(); i++) {
		vOil[i]->doSomething();
	}
	//if (this->player->getPlayerStatus() == false) {
	//	return GWSTATUS_PLAYER_DIED; 
	//}
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
	//deleteEarth();
	for (int y = 0; y < 64; y++) {
		for (int x = 0; x < 64; x++) {
			if (earthArr[x][y] != nullptr) {
				delete this->earthArr[x][y];
			}
		}
	}
	if (player != nullptr)
		delete this->player;
	for (int i = 0; i < vBoulder.size(); i++) {
		vBoulder[i]->setBoulderStatus(false);
		delete vBoulder[i];
	}
}

StudentWorld::~StudentWorld() {
	for (int y = 0; y < 64; y++) {
		for (int x = 0; x < 64; x++) {
			if (earthArr[x][y] != nullptr) {
				delete this->earthArr[x][y];
			}
		}
	}
	if (player != nullptr)
		delete this->player;
}



// 0 up, 1 down, 2 right, 3 left
void StudentWorld::deleteEarth(int x, int y, int dir) {
	bool playSound = false;
	// up
	//if (this->world->isBoulderPresent(x, y) == true) {
	//	return; 
	//}
	std::vector <int> xCor;
	std::vector<int> yCor;
	for (int i = 0; i < vBoulder.size(); i++) {
		xCor.push_back(vBoulder[i]->getX());
		yCor.push_back(vBoulder[i]->getY());
	}

	if (dir == 0) {
		for (int x1 = 0; x1 < 4; x1++) {
			for (int y2 = 0; y2 < 4; y2++) {
				if (earthArr[x + x1][y + y2] != nullptr) {
					earthArr[x + x1][y + y2]->setVisible(false);
				}
			}
		}
	}
	// down
	if (dir == 1) {
		for (int x1 = 0; x1 < 4; x1++) {
			if (earthArr[x + x1][y] != nullptr) {
				earthArr[x + x1][y]->setVisible(false);
			}
		}
	}
	// right
	if (dir == 2) {
		for (int x1 = 0; x1 < 4; x1++) {
			for (int y1 = 0; y1 < 4; y1++) {
				if (earthArr[x + x1][y + y1] != nullptr) {
					earthArr[x + x1][y + y1]->setVisible(false);
				}
			}
		}
	}
	// left
	if (dir == 3) {
		for (int y1 = 0; y1 < 4; y1++) {
			if (earthArr[x][y + y1] != nullptr) {
				earthArr[x][y + y1]->setVisible(false);
			}
		}
	}
}
void StudentWorld::makeEarth() {
	for (int y = 0; y < 60; y++) {
		for (int x = 0; x < 64; x++) {
			if (x != 30 && x != 31 && x != 33 && x != 32) {
				this->earthArr[x][y] = new Earth(x, y);
				this->earthArr[x][y]->setVisible(true);
			}
			else {
				if (y < 4) {
					this->earthArr[x][y] = new Earth(x, y);
					this->earthArr[x][y]->setVisible(true);
				}
				else {
					this->earthArr[x][y] = nullptr;
				}
			}
		}
	}
}

///  TESTERS FUNCTION
bool StudentWorld::inRestrictedArea(int x) {
	int arr[8] = { 28, 29, 30, 31, 32, 33, 34, 35 };
	for (int i = 0; i < 8; i++) {
		if (x == arr[i]) {
			return true;
		}
	}
	return false;
}