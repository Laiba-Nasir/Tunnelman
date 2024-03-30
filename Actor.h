#ifndef ACTOR_H_
#define ACTOR_H_
#include "StudentWorld.h"
#include "GraphObject.h"
#include "GameConstants.h"

class StudentWorld;
// PARENT CLASSES
// GraphObject
class GameObject : public GraphObject {
private:
	int IMAGE_ID;
	int x;
	int y;
	Direction startDirection;
	float size = 1;
	unsigned int depth = 0;
	//StudentWorld* world; 
public:
	GameObject(int x, int y, const int IMAGE_ID, Direction startDirection, float size, int depth);
	virtual void doSomething() = 0;
	~GameObject();
	//virtual void doSomething(bool key, int ch) = 0;
	//virtual StudentWorld* getWorld(); 
};

// --------------- BOULDER CLASS -------------------------------
class Boulder : public GameObject {
private:
	bool isBoulderAlive = true;
	bool isBoulderInStableCondition;
	StudentWorld* world;
	int numTicks = 0;
public:
	bool getBoulderStatus() { return isBoulderAlive; }
	void setBoulderStatus(bool status) { this->isBoulderAlive = status; }
	int startX;
	int startY;
	Boulder(int x, int y, StudentWorld* world);
	void doSomething();

};

// --------------- PICKUP CLASS -------------------------------

class Pickups : public GameObject {
private:
	int IMAGE_ID;
	int x;
	int y;
	Direction startDirection;
	float size = 1;
	unsigned int depth = 0;
public:
	Pickups(int x, int y, const int IMAGE_ID, Direction startDirection, float size, int depth);
	virtual void doSomething();
};

class GoldNugget : public Pickups {
private:
public:
	GoldNugget(int x, int y);
	void doSomething();
};

class BarrelOfOil : public Pickups {
private:
	bool isBarrelAlive = true;
	StudentWorld* world;
public:
	BarrelOfOil(int x, int y, StudentWorld* world);
	void doSomething();
	bool getBarrelStatus() { return isBarrelAlive; }
	void setBarrelStatus(bool barrelStatus) { this->isBarrelAlive = barrelStatus; }
	bool lessThanOrEqualto4(int tmX, int tmY, int oilX, int oilY);
	bool lessThanOrEqualto3(int tmX, int tmY, int oilX, int oilY);
};

class SonarKit : public Pickups {
private:
public:
	SonarKit(int x, int y);
	void doSomething();
};

class WaterPool : public Pickups {
private:
public:
	WaterPool(int x, int y);
	void doSomething();
};

// CHILD CLASSES
// Earth
class Earth : public GameObject {
private:
	int x;
	int y;
public:
	Earth(int x, int y);
	virtual void doSomething();
	~Earth();
};

// Tunnelman
class Tunnelman : public GameObject {
private:
	int x;
	int y;
	StudentWorld* world;
	bool isPlayerAlive = true;
	int playerPts = 0; 
public:
	bool getPlayerStatus() { return isPlayerAlive; }
	void setPlayerStatus(bool status) {
		this->isPlayerAlive = status;
	}
	bool isBoulderPresent(int x, int y);
	Tunnelman(int x, int y, StudentWorld* world);
	~Tunnelman();
	void doAct(bool key, int ch);
	bool isPositionValid();
	StudentWorld* getWorld();
	void doSomething();
	int returnDirection();
	void addPlayerPts(int pts) { this->playerPts = playerPts + pts; }
};

#endif