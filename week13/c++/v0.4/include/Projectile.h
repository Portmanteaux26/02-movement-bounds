#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Projectile {

	private:

		// check position of projectile
		Vector2f m_Posn; 

		// basic shape of projectile
		RectangleShape m_ProjectileShape; 

		// check if projectile is currently in motion
		bool m_InFlight = false;
		// projectile speed
		float m_ProjectileSpeed = 1000; 
		// distance projectile travels
		// - values on each axis derived from speed
		float m_ProjectileDistanceX; 
		float m_ProjectileDistanceY; 
		// define limits to projectile's flight
		// - ensures object stops in game
		// - doesn't keep moving indefinitely
		float m_MaxX;
		float m_MinX;
		float m_MaxY;
		float m_MinY;

	// public function prototypes
	public:
	
		// constructor
		Projectile();

		// stop projectile
		void stop();

		// return value of m_InFlight
		bool isInFlight();

		// fire a new projectile
		void shoot(float startX, float startY, float xTarget, float yTarget);

		// return position of projectile in game
		FloatRect getPosition();

		// return shape for projectile
		// - use for drawing projectile
		RectangleShape getShape();

		// update projectile per frame
		void update(float elapsedTime);

};
