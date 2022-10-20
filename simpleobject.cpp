#include "simpleobject.h"

SimpleObject::SimpleObject()
{

}

SimpleObject::SimpleObject(Type type, Shader* shader)
    : m_type{type}
{
    if (type == Type::Cube)
        constructCube(1.f);

    else if (type == Type::Triangle)
        constructTriangle(1.f);
    else if (type == Type::Axis)
        constructAxis();

    m_shader = shader;
//    init();
}

SimpleObject::SimpleObject(Type type, float size, Shader* shader)
    : m_type{type}
{
    mMatrix.setToIdentity();
    if (type == Type::Cube)
        constructCube(size);
    else if (type == Type::Plane)
        constructPlane(size);
    else if (type == Type::Triangle)
        constructTriangle(size);

    m_shader = shader;
//    init();
}


void SimpleObject::MoveToHeight(QVector3D Zvec)
{
    float z =  Zvec.z() - mMatrix.column(3).z();
    mMatrix.translate(0,0,z);
    mMatrix *= mPosition;
}

void SimpleObject::init()
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

void SimpleObject::draw()
{
//    /* Change rendering for World Axis type */
//    if (m_type == Type::Axis){
////        glUniformMatrix4fv(m_shader->mMmatrixUniform, 1, GL_FALSE, mMatrix.constData());
//        m_shader->setUniformMatrix("mMatrix", mMatrix);
//        glBindVertexArray( mVAO );
//        glDrawArrays(GL_LINES, 0, mVertices.size());
//        glBindVertexArray(0);
//        return;
//    }

//    m_shader->setUniformMatrix("mMatrix", mMatrix);

//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, m_texture);
//    glBindVertexArray( mVAO );
//    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
//    glBindVertexArray(0);
    VisualObject::draw();
}

void SimpleObject::draw(QMatrix4x4 &projectionMatrix, QMatrix4x4 &viewMatrix)
{
    glUseProgram(m_shader->getProgram());
    m_shader->setUniformMatrix("mMatrix", mMatrix);
    m_shader->setUniformMatrix("pMatrix", projectionMatrix);
    m_shader->setUniformMatrix("vMatrix", viewMatrix);

    glBindVertexArray( mVAO );

    if (m_type == Type::Axis){
        glDrawArrays(GL_LINES, 0, mVertices.size());
        glBindVertexArray(0);
        return;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

// Task 11: Billboard
/* Not a true billboard with the specified shader. Instead uses a plane that simply
 * rotates towards the incomming 3Dvector (camera position in this case) in the z axis */
void SimpleObject::rotateToCamera(QVector3D cameraPos)
{
    QVector3D pos{ mMatrix.column(3).x(), mMatrix.column(3).y(), mMatrix.column(3).z() };

    QVector3D forward{QVector3D{0, 1, 0}};
    QVector3D right{QVector3D{1, 0, 0}};

    QVector2D diff{cameraPos.toVector2D() - QVector2D{pos.toVector2D()}};

    QVector2D dir{};
    float dotForward = (dir.dotProduct(forward.toVector2D(), diff)/
            (forward.toVector2D().length() * diff.length()));
    float dotRight = (dir.dotProduct(right.toVector2D(), diff))/
            (right.toVector2D().length() * diff.length());

    float angle = acosf(dotForward);
    if (dotRight >= 0)
        angle *= -1;
    angle *= (180/M_PI);

    float r = angle - prevAngle;

    prevAngle = angle;

    mMatrix.rotate(r, 0, 0, 1);
}

bool SimpleObject::onCollision(VisualObject *collisionObject)
{
    destroy = true;
    return destroy;
}

void SimpleObject::constructCube(float i)
{
    // Første flate -x
    mVertices.push_back(Vertex{-i,  i, -i,    -1,0,0,    0,0});
    mVertices.push_back(Vertex{-i, -i, -i,    -1,0,0,    1,0});
    mVertices.push_back(Vertex{-i,  i,  i,    -1,0,0,    0,1});
    mVertices.push_back(Vertex{-i, -i,  i,    -1,0,0,    1,1});

    // Andre flate +x
    mVertices.push_back(Vertex{i, -i, -i,      1,0,0,    0,0});
    mVertices.push_back(Vertex{i,  i, -i,      1,0,0,    1,0});
    mVertices.push_back(Vertex{i, -i,  i,      1,0,0,    0,1});
    mVertices.push_back(Vertex{i,  i,  i,      1,0,0,    1,1});

    // Tredje flate -y
    mVertices.push_back(Vertex{-i, -i, -i,     0,-1,0,   0,0});
    mVertices.push_back(Vertex{ i, -i, -i,     0,-1,0,   1,0});
    mVertices.push_back(Vertex{-i, -i,  i,     0,-1,0,   0,1});
    mVertices.push_back(Vertex{ i, -i,  i,     0,-1,0,   1,1});

    // Fjerde flate +y
    mVertices.push_back(Vertex{ i, i, -i,      0,1,0,    0,0});
    mVertices.push_back(Vertex{-i, i, -i,      0,1,0,    1,0});
    mVertices.push_back(Vertex{ i, i,  i,      0,1,0,    0,1});
    mVertices.push_back(Vertex{-i, i,  i,      0,1,0,    1,1});

    // Femte flate -z
    mVertices.push_back(Vertex{ i, -i, -i,     0,0,-1,   0,0});
    mVertices.push_back(Vertex{-i, -i, -i,     0,0,-1,   1,0});
    mVertices.push_back(Vertex{ i,  i, -i,     0,0,-1,   0,1});
    mVertices.push_back(Vertex{-i,  i, -i,     0,0,-1,   1,1});

    // Sjette flate +z
    mVertices.push_back(Vertex{ i, -i,  i,     0,0,1,    0,0});
    mVertices.push_back(Vertex{-i, -i,  i,     0,0,1,    1,0});
    mVertices.push_back(Vertex{ i,  i,  i,     0,0,1,    0,1});
    mVertices.push_back(Vertex{-i,  i,  i,     0,0,1,    1,1});


    mIndices = {
        0,1,3,
        2,0,3,

        4,5,7,
        6,4,7,

        8,9,11,
        10,8,11,

        12,13,15,
        14,12,15,

        16,17,19,
        18,16,19,

        21,20,23,
        20,22,23
    };

}

void SimpleObject::constructPlane(float i)
{
    mVertices.push_back(Vertex{-i/2, 0, -i/2,     0, 1, 0,   1,0}); // A
    mVertices.push_back(Vertex{ i/2, 0, -i/2,     0, 1, 0,   0,0}); // B
    mVertices.push_back(Vertex{ i/2, 0,  i/2,     0, 1, 0,   0,1}); // C
    mVertices.push_back(Vertex{-i/2, 0,  i/2,     0, 1, 0,   1,1}); // D

    mIndices = {
        0, 1, 2,
        0, 2, 3
    };
}

void SimpleObject::constructTriangle(float i)
{
    mVertices.push_back(Vertex{-i,  0, -i/2.f,     0, 1, 0,     0,   0});
    mVertices.push_back(Vertex{ i,  0, -i/2.f,     1, 0, 0,     1,   0});
    mVertices.push_back(Vertex{ 0,  0,  i/2.f,     0, 0, 1,     0.5, 1});

    mIndices = { 0, 1, 2 };
}

void SimpleObject::constructAxis()
{
    mVertices.push_back(Vertex{ 10000,  0,  0,   1, 0, 0});
    mVertices.push_back(Vertex{-10000,  0,  0,   1, 0, 0});

    mVertices.push_back(Vertex{0,  10000,  0,   0, 1, 0});
    mVertices.push_back(Vertex{0, -10000,  0,   0, 1, 0});

    mVertices.push_back(Vertex{0, 0,  10000,   0, 0, 1});
    mVertices.push_back(Vertex{0, 0, -10000,   0, 0, 1});
}


void SimpleObject::PlaceObject(QVector3D location)
{
    mMatrix.translate(location);
}

void SimpleObject::PlaceObject(float x, float y)
{
    mMatrix.translate(x, y, 0);
}

void SimpleObject::PlaceObject(float x, float y, float z)
{
    mMatrix.translate(x, y, z);
    mPosition.translate(x, y, z);
}
