#include "texture.hpp"
#include <glad/glad.h>
#include <iostream>
#include "engine.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Red::TextureLoader::TextureLoader() {
    auto& engine = Red::Engine::get();
    this->directory = engine.basepath + "//images//";
}

Red::TextureLoader::~TextureLoader() {
    for (const auto& texture : textures_loaded) {
        glDeleteTextures(1, &texture.id);
    }
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
std::vector<Texture> Red::TextureLoader::loadMaterialTextures(std::string str, std::string typeName)
{
    std::vector<Texture> textures;
    // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
    bool skip = false;
    for (const auto& texture: textures_loaded)
    {
        if (texture.path == str)
        {
            textures.push_back(texture);
            skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
            break;
        }
    }
    if (!skip)
    {   // if texture hasn't been loaded already, load it
        Texture texture;
        texture.id = TextureFromFile(str, this->directory, false);
        texture.type = typeName;
        texture.path = str;
        textures.push_back(texture);
        textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
    }
    return textures;
}


unsigned int Red::TextureLoader::TextureFromFile(std::string path, const std::string& directory, bool gamma)
{
    auto filename = directory + "//" + path;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}