#ifndef STUDENT_H_
#define STUDENT_H_
#include <vector>
#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "StudentWorld.h"

class GameObject;
class StudentWorld;
class Earth;
class Tunnelman;
class Boulder;
class BarrelOfOil;

class StudentWorld : public GameWorld {
private:
	GameObject* earthArr[VIEW_WIDTH][VIEW_HEIGHT];
	Tunnelman* player;
	StudentWorld* world;
	std::vector <Boulder*> vBoulder;
	std::vector<BarrelOfOil*> vOil;
public:
	StudentWorld(std::string assetDir);
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	StudentWorld* getWorld();

	void setDisplayText();


	// helper fun's
	bool isEarthUnder(int x, int y);
	void make4by4(int x, int y);
	void makeBoulders();
	bool moreThanRadiusOfSix(int x1, int y1, int x2, int y2);
	bool inRestrictedArea(int x, int y);
	void makeEarth();
	void deleteEarth(int x, int y, int dir);
	void makeOil();

	// getter 
	std::vector<Boulder*> getBoulderV() {
		return vBoulder;
	}
	Tunnelman* getPlayer() {
		return player;
	}

	/////////////////// TESTERS 
	void distributeInfield(int& x, int& y, bool rand);
	bool checkAllRaidus(int x, int y);

	/////////////////// TESTERS 
	bool inRestrictedArea(int x);
};

#endif