#ifndef VISUALPOINT_H
#define VISUALPOINT_H

#include "visualobject.h"

class VisualPoint : public VisualObject
{
public:
    VisualPoint(Shader* shader, const QVector3D& Location, bool ShowPoint, float PointSize = 5.f)
    {
        setShader(shader);

        bShowPoint = ShowPoint;

        QVector3D color { 1, 1, 1 };
        mVertices.push_back(Vertex{Location, color});

        mPointSize = PointSize;

//        init();
    }
    ~VisualPoint(){}

    bool bShowPoint{ true };
    float mPointSize{ 1.f };

    void draw(QMatrix4x4 &projectionMatrix, QMatrix4x4 &viewMatrix) override
    {
        if (bShowPoint)
        {
            glUseProgram(getShader()->getProgram());
            getShader()->setUniformMatrix("mMatrix", mMatrix);
            getShader()->setUniformMatrix("pMatrix", projectionMatrix);
            getShader()->setUniformMatrix("vMatrix", viewMatrix);

            glBindVertexArray( mVAO );
            glDrawArrays(GL_POINTS, 0, 1);
            glPointSize(mPointSize);

            glBindVertexArray(0);
        }
    }

    void init() override
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
};

#endif // VISUALPOINT_H
