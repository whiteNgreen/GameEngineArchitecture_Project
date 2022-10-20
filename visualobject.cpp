#include "visualobject.h"

VisualObject::VisualObject()
{

}

VisualObject::~VisualObject()
{
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

void VisualObject::init()
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

void VisualObject::draw()
{
    /* Om annet ikke er spesifisert så antar vi at den bruker 'plainshader' */
    m_shader->setUniformMatrix("mMatrix", mMatrix);

    glUseProgram(m_shader->getProgram());

    glBindVertexArray( mVAO );
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}

void VisualObject::draw(QMatrix4x4 &projectionMatrix, QMatrix4x4 &viewMatrix)
{
    glUseProgram(m_shader->getProgram());
    m_shader->setUniformMatrix("mMatrix", mMatrix);
    m_shader->setUniformMatrix("pMatrix", projectionMatrix);
    m_shader->setUniformMatrix("vMatrix", viewMatrix);

    glBindVertexArray( mVAO );

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void VisualObject::drawLines(QMatrix4x4 &projectionMatrix, QMatrix4x4 &viewMatrix)
{
    glUseProgram(m_shader->getProgram());
    m_shader->setUniformMatrix("mMatrix", mMatrix);
    m_shader->setUniformMatrix("pMatrix", projectionMatrix);
    m_shader->setUniformMatrix("vMatrix", viewMatrix);

    glBindVertexArray( mVAO );
    glDrawElements(GL_LINE_LOOP, mIndices.size(), GL_UNSIGNED_INT, nullptr);
//    glDrawArrays(GL_LINE_LOOP, 0, mVertices.size());

    glBindVertexArray(0);
}

Position2D VisualObject::getPosition2D()
{
    return std::pair<float, float>{mMatrix.column(3).x(), mMatrix.column(3).y()};
}

QVector2D VisualObject::getPositionVector2D()
{
    return mMatrix.column(3).toVector2D();
}

QVector3D VisualObject::getPositionVector3D()
{
    return mMatrix.column(3).toVector3D();
}

void VisualObject::CalculateNormalofTriangle(Vertex &v1, Vertex &v2, Vertex &v3)
{
    QVector3D ab = v2.getPosition() - v1.getPosition();
    QVector3D ac = v3.getPosition() - v1.getPosition();
//    QVector3D n = QVector3D::crossProduct(ab, ac);
    QVector3D n = QVector3D::crossProduct(ac, ab);
    n.normalize();

    v1.setNormal(n);
    v2.setNormal(n);
    v3.setNormal(n);
}


