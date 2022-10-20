#include "cubemap.h"
#include "stb_image/stb_image.h"

CubeMap::CubeMap(Shader* shader)
{
    m_shader = shader;

    mLogger = Logger::getInstance();
    std::vector<std::string> faces
    {
        "right.jpg",
        "left.jpg",
        "bottom.jpg",
        "top.jpg",
        "front.jpg",
        "back.jpg"
    };


    initializeOpenGLFunctions();

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

    int width, height, nrChannels;
    for (uint32_t i=0; i < faces.size(); i++)
    {
        std::string path{"../GEA_Project/Assets/skybox/" + faces[i]};
//        qDebug() << qPrintable(QString::fromStdString(faces[i]));
        unsigned char* data;
        data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
//        if(stbi_failure_reason())
//            mLogger->logText(stbi_failure_reason());
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            mLogger->logText("Cubemap tex failed to load at path: " + path);
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);



        // positions
      mVertices.push_back(Vertex{  -1.0f,  1.0f, -1.0f});
      mVertices.push_back(Vertex{  -1.0f, -1.0f, -1.0f});
      mVertices.push_back(Vertex{   1.0f, -1.0f, -1.0f});
      mVertices.push_back(Vertex{   1.0f, -1.0f, -1.0f});
      mVertices.push_back(Vertex{   1.0f,  1.0f, -1.0f});
      mVertices.push_back(Vertex{  -1.0f,  1.0f, -1.0f});

      mVertices.push_back(Vertex{  -1.0f, -1.0f,  1.0f});
      mVertices.push_back(Vertex{  -1.0f, -1.0f, -1.0f});
      mVertices.push_back(Vertex{  -1.0f,  1.0f, -1.0f});
      mVertices.push_back(Vertex{  -1.0f,  1.0f, -1.0f});
      mVertices.push_back(Vertex{  -1.0f,  1.0f,  1.0f});
      mVertices.push_back(Vertex{  -1.0f, -1.0f,  1.0f});

      mVertices.push_back(Vertex{   1.0f, -1.0f, -1.0f});
      mVertices.push_back(Vertex{   1.0f, -1.0f,  1.0f});
      mVertices.push_back(Vertex{   1.0f,  1.0f,  1.0f});
      mVertices.push_back(Vertex{   1.0f,  1.0f,  1.0f});
      mVertices.push_back(Vertex{   1.0f,  1.0f, -1.0f});
      mVertices.push_back(Vertex{   1.0f, -1.0f, -1.0f});

      mVertices.push_back(Vertex{  -1.0f, -1.0f,  1.0f});
      mVertices.push_back(Vertex{  -1.0f,  1.0f,  1.0f});
      mVertices.push_back(Vertex{   1.0f,  1.0f,  1.0f});
      mVertices.push_back(Vertex{   1.0f,  1.0f,  1.0f});
      mVertices.push_back(Vertex{   1.0f, -1.0f,  1.0f});
      mVertices.push_back(Vertex{  -1.0f, -1.0f,  1.0f});

      mVertices.push_back(Vertex{  -1.0f,  1.0f, -1.0f});
      mVertices.push_back(Vertex{   1.0f,  1.0f, -1.0f});
      mVertices.push_back(Vertex{   1.0f,  1.0f,  1.0f});
      mVertices.push_back(Vertex{   1.0f,  1.0f,  1.0f});
      mVertices.push_back(Vertex{  -1.0f,  1.0f,  1.0f});
      mVertices.push_back(Vertex{  -1.0f,  1.0f, -1.0f});

      mVertices.push_back(Vertex{  -1.0f, -1.0f, -1.0f});
      mVertices.push_back(Vertex{  -1.0f, -1.0f,  1.0f});
      mVertices.push_back(Vertex{   1.0f, -1.0f, -1.0f});
      mVertices.push_back(Vertex{   1.0f, -1.0f, -1.0f});
      mVertices.push_back(Vertex{  -1.0f, -1.0f,  1.0f});
      mVertices.push_back(Vertex{   1.0f, -1.0f,  1.0f});

      glGenVertexArrays( 1, &mVAO );
      glBindVertexArray( mVAO );

      //Vertex Buffer Object to hold vertices - VBO
      glGenBuffers( 1, &mVBO );
      glBindBuffer( GL_ARRAY_BUFFER, mVBO );

      //Vertex Buffer Object to hold vertices - VBO
      glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

      // 1rst attribute buffer : vertices
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0 )  );          // array buffer offset
      glEnableVertexAttribArray(0);

//      mMatrix.rotate(90, 0, 0, 1);
}

void CubeMap::init()
{
//    MatrixUniform = matrixUniform;  // Objektet selv holder styr på hvilken shader som skal brukes

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0 )  );          // array buffer offset
    glEnableVertexAttribArray(0);


    glBindVertexArray(0);
}

void CubeMap::draw(QMatrix4x4& projectionMatrix, QMatrix4x4& viewMatrix)
{
    glUseProgram(m_shader->getProgram());
    glDepthFunc(GL_LEQUAL);
//    glUniformMatrix4fv(m_shader->mMmatrixUniform, 1, GL_FALSE, mMatrix.constData());  // ??
//    m_shader->setUniformMatrix("skybox", mMatrix);                                    // ??
    m_shader->setUniformMatrix("projection", projectionMatrix);
    m_shader->setUniformMatrix("view", viewMatrix);


    glBindVertexArray( mVAO );
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);
    glBindVertexArray(0);
}

