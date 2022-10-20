#include "light.h"


Light::Light(Shader* shader)
{
    m_shader = shader;

    mVertices.push_back(Vertex{-0.25f, 0.25f, -0.25f,     0.8, 0.8, 0.3,      0,0});
    mVertices.push_back(Vertex{-0.25f, -0.25f, -0.25f,     0.8, 0.8, 0.3,     1,0});
    mVertices.push_back(Vertex{0, 0, 0.25f,                0.8, 0.8, 0.3,     0.5,0.5});
    mVertices.push_back(Vertex{0.25f, -0.25f, -0.25f,      0.8, 0.8, 0.3,     0.5,0.5});

    mIndices = {
        0,1,2,
        1,3,2,
        3,0,2,
        0,3,1
    };

    mMatrix.setToIdentity();

    mMatrix.translate(0, -2, 0);
}

// Task: 3 Sun ----------------------------------------------------------------------------------------------
Light::Light(VisualObject *obj, Shader* shader)
{
    m_shader = shader;

    /* Gets the vertices and indexes of any VisualObject
     * Will use this to get data from an imported mesh */
    mVertices = obj->getVertices();
    mIndices = obj->getIndices();

    /* Overrides the objects normals to use them as vertex colors
     * during rendering
     * Sets the color to yellow */
    for (auto& it : mVertices)
    {
        it.setNormal(0.8f, 0.8f, 0.3f);
    }

    mMatrix.setToIdentity();

    mMatrix.translate(0, -2, 0);
}

void Light::init()
{
//    MatrixUniform = matrixUniform;

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

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glGenBuffers ( 1, &mEAB );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Light::draw(QMatrix4x4& projectionMatrix, QMatrix4x4& viewMatrix)
{
    glUseProgram(m_shader->getProgram());
    m_shader->setUniformMatrix("mMatrix", mMatrix);
    m_shader->setUniformMatrix("pMatrix", projectionMatrix);
    m_shader->setUniformMatrix("vMatrix", viewMatrix);

    glBindVertexArray( mVAO );
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Light::MoveTo(const QVector3D &Location)
{
    QVector3D Move = Location - mMatrix.column(3).toVector3D();
    mMatrix.translate(Move);
    Position = mMatrix.column(3).toVector3D();
}
