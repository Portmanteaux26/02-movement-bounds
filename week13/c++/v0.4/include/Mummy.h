#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Mummy {

	private:

		// speed for each mummy type
		// - update to pharaoh, vizier, worker
		const float PHARAOH_SPEED = 40;
		const float VIZIER_SPEED = 80;
		const float WORKER_SPEED = 20;

		// strength of each mummy type
		const float PHARAOH_HEALTH = 5;
		const float VIZIER_HEALTH = 1;
		const float WORKER_HEALTH = 3;

		// force change in speed, as needed per mummy type
		const int MAX_CHANGE = 30; 
		const int OFFSET = 101 - MAX_CHANGE;

		// check position of mummy
		Vector2f m_Posn; 
		// sprite 
		Sprite m_Sprite;
		// defined speed for current mummy
		float m_Speed;
		// defined strength for current mummy
		float m_Health;

		// set if current mummy is alive or not...
		bool m_Alive;

	public:

		// handle as bullet hits a mummy object
		bool hit();
		// check if mummy object is alive
		bool isAlive();

		// spawn new mummy object
		void spawn(float startX, float startY, int type, int seed);
		// return rectangle as position in game
		FloatRect getPosn(); 
		// sprite copy to draw
		Sprite getSprite();

		// update mummy object per frame
		void update(float elapsedTime, Vector2f playerLocation);

};
