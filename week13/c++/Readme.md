### Readme - Game Dev - OOP Examples - Shooter Arena

Basic game dev example with C++ and SFML.

To build game demo,

* in root of project directory
	* create `build` directory
	* `cd` to `build` directory
	* issue command `cmake ..`
	* issue command `make`
	* start game `./STGArenaSpire`

To start game demo after opening game window,

* press **Enter** key
* then press **1** key
* mouse controls rotation of player sprite

#### Game outline

* v0.3
	* initial use of references, pointers, & STL
	* add class for holding textures, singleton pattern &c.
	* spawn many mob objects to game arena
		* add random movements for objects
		* object adjust to relative position of player
		* object animate and move around arena
		* ...
* v0.4
	* add class for shooting a weapon
		* projectile objects &c.
	* add focus for shooting weapon in game
	* ...
