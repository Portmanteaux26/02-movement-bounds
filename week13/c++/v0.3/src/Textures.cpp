#include "Textures.h"
// initial debugging option
#include <assert.h>

Textures* Textures::m_singleInstance = nullptr;

Textures::Textures() {

	assert(m_singleInstance == nullptr);
	m_singleInstance = this;

}

Texture& Textures::GetTexture(string const& filename) {

	// reference to m_Textures using m_singleInstance
	// - auto equivalent of map<string, Texture>
	auto& m = m_singleInstance->m_Textures;
	// create iterator to hold key/value pair
	// - auto equivalent of map<string, Texture>::iterator
	// - search for required key/value by filename
	auto keyValuePair = m.find(filename);
	if (keyValuePair != m.end()) {
		// return second part of key/value, the texture type value
		return keyValuePair->second;
	} else {
		// filename not found
		// - create new key/value pair using filename
		auto& texture = m[filename];
		// load texture from file 
		texture.loadFromFile(filename);

		// return texture to calling code
		return texture;
	}

}
