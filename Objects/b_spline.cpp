#include "b_spline.h"
#include "visualpoint.h"

BSpline::BSpline()
{
    setShader(Shader::getPlainShader());
}

BSpline::BSpline(Shader* shader)
{
    setShader(shader);
}

BSpline::~BSpline()
{
    DeleteCurve();
}

void BSpline::ShowPoints(bool b)
{
    bShowPoints = b;
    for (auto& it : mVisualPoints)
    {
        it->bShowPoint = b;
    }
}

void BSpline::NewPoint(const QVector3D &point)
{
    mVertices.clear();
    mPoints.push_back(point);
    mKnots.clear();

    MakeKnotVector();

    MakeBSplineCurve();
    init();

    /* Adding visual point */
    mVisualPoints.push_back(std::make_unique<VisualPoint>(getShader(), point, bShowPoints));
}

void BSpline::MakeKnotVector()
{
    mKnots.clear();

    /* Hardkoder noen enkle skjøtevektorer for 2 og 3 punkter
     * Fra og med 4 punkter så bruker jeg den mer avanserte metoden */
    if (mPoints.size() == 2)
    {
        return;
    }
    /* Antar at b-splinen er av andre grad */
    if (mPoints.size() == 3)
    {
        mKnots = { 0, 0, 0, 1, 1, 1 };
        return;
    }

    /* Skjøtvektor størrelse = (n + d + 1) */
    int KnotPoints = mPoints.size() + degree + 1;
    float delvis = KnotPoints - ((degree + 1) * 2);
    float step = (1/(delvis + 1));

    for (int i{1}; i <= KnotPoints; i++)
    {
        /* Setter de første skjøtene til 0 */
        if (i <= (degree + 1))
        {
            mKnots.push_back(0);
        }
        /* Setter de siste skjøtene til 1 */
        else if (i > KnotPoints - (degree + 1))
        {
            mKnots.push_back(1);
        }
        /* Regner ut mellom skjøter */
        else
        {
            int j = i - (degree + 1);
            float knot = step * j;
            mKnots.push_back(knot);
        }
    }
}

void BSpline::MakeBSplineCurve()
{
    if (mPoints.size() == 2)
    {
        QVector3D color { 1, 1, 1 };
        mVertices.push_back({mPoints[0], color});
        mVertices.push_back({mPoints[1], color});

        return;
    }

    QVector3D color { 1, 1, 1 };
    for (int i{}; i <= maxResolution; i += resolution)
    {
        float t = float(i) / 100;
        QVector3D pos{};

        for (unsigned int j{}; j < mPoints.size(); j++)
        {
            pos += mPoints[j] * Bid(t, j, degree);
        }

        /* Jukser litt her for å sette siste vertex posisjonen til
         *  det siste punktet i 'points' vectoren, ellers så ender den på (0,0,0) */
        if (i == maxResolution)
        {
            pos = mPoints[mPoints.size() - 1];
        }
        mVertices.push_back({pos, color});

        LogVector("Pos : " + std::to_string(t) + " : ", pos);
    }
}

void BSpline::DeleteCurve()
{
    mVertices.clear();
    mPoints.clear();
    mKnots.clear();
    init();

    mVisualPoints.clear();
}

float BSpline::Bid(float t, int iteration, int degree)
{
    if (degree == 0)
    {
        if (mKnots[iteration] <= t && t < mKnots[iteration + 1])
        {
            return 1.f;
        }

        return 0.f;
    }

    return (Wid(t, iteration, degree) * Bid(t, iteration, degree - 1)) + ((1 - Wid(t, iteration + 1, degree)) * Bid(t, iteration + 1, degree - 1));
}

float BSpline::Wid(float t, int iteration, int degree)
{
    if (mKnots[iteration] < mKnots[iteration + degree])
    {
        return (t - mKnots[iteration]) / (mKnots[iteration + degree] - mKnots[iteration]);
    }

    return 0.f;
}

void BSpline::draw(QMatrix4x4 &projectionMatrix, QMatrix4x4 &viewMatrix)
{
    if (mPoints.size() > 1)
    {
        glUseProgram(getShader()->getProgram());
        getShader()->setUniformMatrix("mMatrix", mMatrix);
        getShader()->setUniformMatrix("pMatrix", projectionMatrix);
        getShader()->setUniformMatrix("vMatrix", viewMatrix);

        glBindVertexArray( mVAO );
        glDrawArrays(GL_LINE_STRIP, 0, mVertices.size());

        glBindVertexArray(0);
    }

    /* Draw Visaul Points */
    if (bShowPoints)    // Causes crash atm
    {
        for (const auto& it : mVisualPoints)
        {
            it->draw(projectionMatrix, viewMatrix);
        }
    }
}
