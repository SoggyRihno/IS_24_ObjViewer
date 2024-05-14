#define STB_IMAGE_IMPLEMENTATION



#include "Texture.h"


Texture::Texture( std::string &path){
    filepath = path;
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    int width, height, bpp;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &bpp, 4);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else if (!std::filesystem::exists(path)) {
        std::cout << "Could not find image at : " << path << std::endl;
    } else {
        std::cout << "Failed to load texture at: " << path << std::endl;
    }
    stbi_image_free(data);
}

void Texture::Bind() const {
    glBindTexture(GL_TEXTURE_2D, ID);
}

Texture::Texture() {

}