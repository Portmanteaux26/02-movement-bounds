#pragma once
#ifndef TEXTURES_H
#define TEXTURES_H

#include <SFML/Graphics.hpp>
// map from STL
#include <map>

using namespace sf;
using namespace std;

class Textures { 

	private:

		// map container
		// - holds related pairs of String and Texture
		map<string, Texture> m_Textures;

		// pointer 
		// - same type as class itself
		// - single, only instance
		static Textures* m_singleInstance; 

	public:

		Textures();
		static Texture& GetTexture(string const& filename);

};

#endif


