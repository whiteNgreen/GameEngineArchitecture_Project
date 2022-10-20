#include "beziercurve.h"

BezierCurve::BezierCurve(Shader* shader)
{
    m_shader = shader;

    /* The control points for the bezier curve */
//    float y = - 18 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/38));
    mPoints.push_back(QVector3D{-2,  -1,  5});
//    y = - 18 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/38));
    mPoints.push_back(QVector3D{-1,  2,  5});
//    y = - 18 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/38));
    mPoints.push_back(QVector3D{ 1,  -1,  5});
//    y = - 18 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/38));
    mPoints.push_back(QVector3D{ 2,  1,  5});


    /* Creates the vertices for the bezier curve */
    for (float t = 0; t < 1; t+=0.01)
    {
        mVertices.push_back(Vertex{QVector3D{findBezierPoint(mPoints, t)}, QVector3D{0.6, 0.6, 1}});
    }

    for (GLuint i = 0; i < mVertices.size() - 1; i++){
        mIndices.push_back(i);
        mIndices.push_back(i+1);
    }
}

void BezierCurve::init()
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

    glGenBuffers ( 1, &mEAB );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void BezierCurve::draw()
{
//    glUniformMatrix4fv(m_shader->mMmatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glUseProgram(m_shader->getProgram());
    m_shader->setUniformMatrix("mMatrix", mMatrix);
//    m_shader->setUniformMatrix("pMatrix", projectionMatrix);
//    m_shader->setUniformMatrix("vMatrix", viewMatrix);

    glBindVertexArray( mVAO );
    glDrawElements(GL_LINES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void BezierCurve::draw(QMatrix4x4 &projectionMatrix, QMatrix4x4 &viewMatrix)
{
    glUseProgram(m_shader->getProgram());
    m_shader->setUniformMatrix("mMatrix", mMatrix);
    m_shader->setUniformMatrix("pMatrix", projectionMatrix);
    m_shader->setUniformMatrix("vMatrix", viewMatrix);

    glBindVertexArray( mVAO );
    glDrawElements(GL_LINES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

/* Finds the location on the curve using t == 0<=t<=1 using the deCasteljau algorithm.
 * Is only functional on deCasteljau bezier curves in the third degree d=3 */
QVector3D BezierCurve::findBezierPoint(const std::vector<QVector3D>& points, float t)
{
    QVector3D b01{t*points[0] + (1-t)*points[1]};
    QVector3D b11{t*points[1] + (1-t)*points[2]};
    QVector3D b21{t*points[2] + (1-t)*points[3]};

    QVector3D b02{t*b01 + (1-t)*b11};
    QVector3D b12{t*b11 + (1-t)*b21};

    QVector3D b03{t*b02 + (1-t)*b12};

    return b03;
}
