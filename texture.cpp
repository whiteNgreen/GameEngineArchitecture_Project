#include <iostream>
#include <fstream>
#include <string>
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#include "texture.h"
#include "logger.h" //only needed for logging to our Output Log

Texture::Texture()
{
    mLogger = Logger::getInstance();    // Get instance of logger

    initializeOpenGLFunctions();

    //Make small dummy texture
    makeDummyTexture();
}

Texture::Texture(const std::string& filename, bool alpha/*=0*/): QOpenGLFunctions_4_1_Core()
{
    mLogger = Logger::getInstance();
    textureFilename = filename;
    initializeOpenGLFunctions();

    mAlphaUsed = alpha;
    textureID = getTexture(filename);
}

void Texture::stbImageTexture(const std::string &filename)
{
    std::string filepath = "../GEA_Project/Assets/" + filename;

    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
    if(stbi_failure_reason())
        mLogger->logText(stbi_failure_reason());
    if (data)
    {

//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        if(mAlphaUsed == false)                     //no alpha in this texture
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
        else{
            //alpha is present, so we set up an alpha channel
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,  GL_BGRA, GL_UNSIGNED_BYTE, data);
            mLogger->logText("Imported with alpha");
        }
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else
    {
        mLogger->logText("Failed to load texture " + filename);
    }
}

uint32_t Texture::getTexture(const std::string &filename, bool alpha/* = 0*/)
{
    std::string filepath = "../GEA_Project/Assets/" + filename;

    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else
    {
        mLogger->logText("Failed to load texture " + filename);
    }

//    GLint tmp = static_cast<GLint>(texture);
    return texture;
}

GLuint Texture::id() const
{
    return mId;
}


void Texture::makeDummyTexture()
{
    mLogger->logText("Making dummy texture");
    for (int i=0; i<16; i++)
        pixels[i] = 0;
    pixels[0] = 255;
    pixels[4] = 255;
    pixels[8] = 255;
    pixels[9] = 255;
    pixels[10] = 255;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &mId);
    glBindTexture(GL_TEXTURE_2D, mId);
    mLogger->logText("Texture id: " + std::to_string(mId));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 reinterpret_cast<const GLvoid*>(pixels));
}
