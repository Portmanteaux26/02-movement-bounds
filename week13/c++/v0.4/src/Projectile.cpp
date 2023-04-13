#include "Projectile.h"

// constructor
Projectile::Projectile() {

	m_ProjectileShape.setSize(sf::Vector2f(2, 2));

}

void Projectile::shoot(float startX, float startY, float targetX, float targetY) {

	// check status of projectile
	// - still moving
	// - set initial position
	m_InFlight = true; 
	m_Posn.x = startX; 
	m_Posn.y = startY;

	// calculate gradient of projectile motion
	float gradient = (startX - targetX) / (startY - targetY);

	// gradient less than 1, set negative
	if (gradient < 0) {
		gradient *= -1;
	}

	// calculate ratio between x and y
	float ratioXY = m_ProjectileSpeed / (1 + gradient);

	// set position of projectile
	// - update per frame
	// - update relative to target selected
	m_ProjectileDistanceY = ratioXY;
	m_ProjectileDistanceX = ratioXY * gradient;

	// point projectile in correct direction to target
	if (targetX < startX) {
		m_ProjectileDistanceX *= -1;
	}
	if (targetY < startY) {
		m_ProjectileDistanceY *= -1;
	}

	// set max to stop indefinite projectile motion
	float range = 1000;
	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxY = startY + range;

	// position projectile for drawing
	m_ProjectileShape.setPosition(m_Posn);

}

void Projectile::stop() {
	m_InFlight = false;
}

// check projectile is active, in motion
bool Projectile::isInFlight() {
	return m_InFlight;
}

// get position during motion
FloatRect Projectile::getPosition() {
	return m_ProjectileShape.getGlobalBounds();
}

// get shape of current projectile
RectangleShape Projectile::getShape() {
	return m_ProjectileShape;
}

void Projectile::update(float elapsedTime) {
	// update projectile position
	m_Posn.x += m_ProjectileDistanceX * elapsedTime;
	m_Posn.y += m_ProjectileDistanceY * elapsedTime;
	// move projectile
	m_ProjectileShape.setPosition(m_Posn);
	// check projectile range
	if (m_Posn.x < m_MinX || m_Posn.x > m_MaxX || m_Posn.y < m_MinY || m_Posn.y > m_MaxY) {
		m_InFlight = false;
	}
}
