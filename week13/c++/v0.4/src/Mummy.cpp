#include "Mummy.h"
#include "Textures.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

void Mummy::spawn(float startX, float startY, int type, int seed) {

	switch (type) {

		case 0:
			// pharaoh
			m_Sprite = Sprite(Textures::GetTexture("../assets/graphics/pharaoh.png"));
			m_Speed = PHARAOH_SPEED;
			m_Health = PHARAOH_HEALTH;
			break;
		case 1:
			// vizier
			m_Sprite = Sprite(Textures::GetTexture("../assets/graphics/vizier.png"));
			m_Speed = VIZIER_SPEED;
			m_Health = VIZIER_HEALTH;
			break;
		case 2:
			// worker
			m_Sprite = Sprite(Textures::GetTexture("../assets/graphics/worker.png"));
			m_Speed = WORKER_SPEED;
			m_Health = WORKER_HEALTH;
			break;
	}

	// modify speed
	// - create speed modifier to replicate unique mob object
	srand((int)time(0) * seed);
	// range between 80 and 100
	float modifier = (rand() % MAX_CHANGE) + OFFSET;
	// as a fraction of 1
	modifier /= 100; 
	m_Speed *= modifier;

	// initialise location
	m_Posn.x = startX;
	m_Posn.y = startY;
	// set origin of mob object to its centre
	m_Sprite.setOrigin(25, 25);
	// set position of mob object
	m_Sprite.setPosition(m_Posn);

}

bool Mummy::hit() {

	// decrement health per hit
	m_Health--;

	// check health status
	if (m_Health < 0) {
		// mob object is dead
		// - update bool for alive
		m_Alive = false;
		// update texture for current mob object's sprite
		// - render as dead in arena...
		m_Sprite.setTexture(Textures::GetTexture("../assets/graphics/mob_dead.png"));
		
		return true;
	}

	// injured 
	// - health remaining, other hits can be recorded
	return false;

}

// Getter FNs
// return values to execution context

bool Mummy::isAlive() {
	return m_Alive;
}

FloatRect Mummy::getPosn() {
	return m_Sprite.getGlobalBounds();
}

Sprite Mummy::getSprite() {
	return m_Sprite;
}

// update FN
void Mummy::update(float elapsedTime, Vector2f playerLocation) {

	float playerX = playerLocation.x;
	float playerY = playerLocation.y;

	// update object position
	if (playerX > m_Posn.x) {
		m_Posn.x = m_Posn.x + m_Speed * elapsedTime;
	}

	if (playerY > m_Posn.y) {
		m_Posn.y = m_Posn.y + m_Speed * elapsedTime;
	}

	if (playerX < m_Posn.x) {
		m_Posn.x = m_Posn.x - m_Speed * elapsedTime;
	}

	if (playerY < m_Posn.y) {
		m_Posn.y = m_Posn.y - m_Speed * elapsedTime;
	}

	// Move the sprite
	m_Sprite.setPosition(m_Posn);
	// Face the sprite in the correct direction
	float angle = (atan2(playerY - m_Posn.y, playerX - m_Posn.x) * 180) / 3.141;
	m_Sprite.setRotation(angle);
}

