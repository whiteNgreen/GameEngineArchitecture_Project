#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <unordered_map>

//#include "GL/glew.h" //We use QOpenGLFunctions instead, so no need for Glew (or GLAD)!

//This class is pretty much a copy of the shader class at
//https://github.com/SonarSystems/Modern-OpenGL-Tutorials/blob/master/%5BLIGHTING%5D/%5B8%5D%20Basic%20Lighting/Shader.h
//which is based on stuff from http://learnopengl.com/ and http://open.gl/.

enum class ShaderType{
    plain,
    texture,
    phong,
    cubemap
};

//must inherit from QOpenGLFunctions_4_1_Core, since we use that instead of glfw/glew/glad
class Shader : protected QOpenGLFunctions_4_1_Core
{
public:
    // Constructor generates the shader on the fly
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath);

    ///Use the current shader
    void use( );

    ///Returns the program number for this shader
    GLuint getProgram() const;

    void setUniformMatrix(const std::string& uniform, QMatrix4x4& matrix);
    GLint getUniformMatrix(const std::string& uniform);

    void setShaderType(ShaderType mType){ type = mType; }
    bool shaderType(ShaderType mType){ return type == mType; }

    /* Kan sette opp alle GLint'ene i et unordered map for å sette dem opp manuelt
     * så jeg ikke ender opp med for mange for hver shader */
//    GLint  mMmatrixUniform              {-1};
//    GLint  mPmatrixUniform              {-1};
//    GLint  mVmatrixUniform              {-1};
//    GLint  mTextureUniform              {-1};

//    GLint mLightColorUniform            {-1};
//    GLint mObjectColorUniform           {-1};
//    GLint mAmbientLightStrengthUniform  {-1};
//    GLint mLightPositionUniform         {-1};
//    GLint mCameraPositionUniform        {-1};
//    GLint mSpecularStrengthUniform      {-1};
//    GLint mSpecularExponentUniform      {-1};
//    GLint mLightPowerUniform            {-1};

//    void setupPlainShader();
//    void setupTextureShader();
//    void setupPhongShader();
//    void setupSkyBoxShader();
private:
    ///The int OpenGL gives as a reference to this shader
    GLuint mProgram;

    ///The logger - Output Log in the application
    class Logger *mLogger{nullptr};

    std::unordered_map<std::string, GLint> mUniforms;

    ShaderType type;

public:
    static Shader* getPlainShader();
};


#endif
