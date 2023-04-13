#include "Arena.h"
#include "Mummy.h"

// pointer to Mummy object
Mummy* createMob(int numMummies, IntRect arena) {

	Mummy* mummies = new Mummy[numMummies];
	
	int maxY = arena.height - 20;
	int minY = arena.top + 20;

	int maxX = arena.width - 20;
	int minX = arena.left + 20;

	for (int i = 0; i < numMummies; i++) {

		// check side to spawn mob object
		srand((int)time(0) * i);
		int side = (rand() % 4);
		float x, y;

		switch (side) {
			case 0:
				// left
				x = minX;
				y = (rand() % maxY) + minY;
				break;
			case 1:
				// right
				x = maxX;
				y = (rand() % maxY) + minY;
				break;
			case 2:
				// top
				x = (rand() % maxX) + minX;
				y = minY;
				break;
			case 3:
				// bottom
				x = (rand() % maxX) + minX;
				y = maxY;
				break;
		}

		// pharaoh, vizier, worker
		srand((int)time(0) * i * 2);
		int type = (rand() % 3);

		// spawn new mob object to array
		mummies[i].spawn(x, y, type, i);
	}

	// return array of spawned mob objects
	return mummies;
}	
