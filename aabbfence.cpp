#include "aabbfence.h"

AABBfence::AABBfence()
{

}

AABBfence::AABBfence(VisualObject *obj, Shader* shader)
{
    mVertices = obj->getVertices();
    mIndices = obj->getIndices();

    mMatrix.setToIdentity();
    mPosition.setToIdentity();
    mRotation.setToIdentity();

    m_shader = shader;
    init();
}

void AABBfence::init()
{
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

void AABBfence::draw()
{
    glUseProgram(m_shader->getProgram());
//    glUniformMatrix4fv(m_shader->getUniformMatrix("mMatrix"), 1, GL_FALSE, mMatrix.constData());
    m_shader->setUniformMatrix("mMatrix", mMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glBindVertexArray( mVAO );
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

std::vector<QVector2D> AABBfence::getAABBcollider()
{
    std::vector<QVector2D> boundary;

    QVector2D pos{ mMatrix.column(3).x(), mMatrix.column(3).y() };

    boundary.push_back(pos - QVector2D{-0.5, -1.5}); // A
    boundary.push_back(pos - QVector2D{ 0.5, -1.5}); // B
    boundary.push_back(pos - QVector2D{ 0.5,  1.5}); // C
    boundary.push_back(pos - QVector2D{-0.5,  1.5}); // D

    return boundary;
}
