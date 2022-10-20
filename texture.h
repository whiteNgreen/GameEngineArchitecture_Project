#ifndef TEXTURE_H
#define TEXTURE_H

#include <QOpenGLFunctions_4_1_Core>

#include "vertex.h"

///Simple class for creating textures from a bitmap file.
class Texture : protected QOpenGLFunctions_4_1_Core
{
public:
    ///Makes a dummy texture
    Texture();  //calls makeDummyTexture()
    ///Reads bmp file given by filename - path is required
    Texture(const std::string &filename, bool alpha = 0);
    ///Returns the id OpenGL gives this texture
    GLuint id() const;
    ///Filename of the texture, with full path
    std::string textureFilename;

    Vertex vertexFromPixel();

    uint32_t textureID;
    uint32_t getTexture(const std::string& filename, bool alpha = 0);

private:
    GLubyte pixels[16];     // For the standard texture from the no-parameter constructor
    GLuint mId{0};          //Texture ID that OpenGL makes when glGenTextures is called
    ///Pointer to the pixels in the bitmap, starting at lower, left corner
    unsigned char *mBitmap{nullptr};
    ///The width of the image, in pixels
    int mColumns;
    ///The height of the image, in pixels
    int mRows;
    ///Number of bytes describing each pixel. Have to be 3 or 4.
    int mBytesPrPixel;
    ///Does this image use alpha channel?
    bool mAlphaUsed{false};

    ///Utility class Logger to write to Output Log in the application.
    class Logger *mLogger{nullptr};

    void stbImageTexture(const std::string& filename);

private:

    void makeDummyTexture();
};

#endif // TEXTURE_H
