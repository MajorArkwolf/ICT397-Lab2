#pragma once

#include <vector>
#include <string>


struct Texture {
	std::string type;
	std::string path;
	unsigned int id = 0;
};

namespace Red {
	class TextureLoader {
	public:
		TextureLoader();
		~TextureLoader();
		std::vector<Texture>loadMaterialTextures(std::string str, std::string typeName = "texture_normal");
		unsigned int TextureFromFile(std::string path, const std::string& directory, bool gamma);
		std::vector<Texture> getTextureList() const { return textures_loaded; }
	private:
		std::vector<Texture> textures_loaded;
		std::string directory = "";
	};
}