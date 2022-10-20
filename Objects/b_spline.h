#ifndef B_SPLINE_H
#define B_SPLINE_H

#include <visualobject.h>
#include "StandAlone/LoggerFunctions.h"
//#include "visualpoint.h"
#include "Objects/visualpoint.h"

class BSpline : public VisualObject
{
private:
    std::vector<std::unique_ptr<VisualPoint>> mVisualPoints;

public:
    BSpline();
    BSpline(Shader* shader);
    ~BSpline();

    bool bShowPoints{ true };
    void ShowPoints(bool b);

    int resolution{ 1 };
    int maxResolution{ 100 };

    /* -- B-SPLINE -- */
    int degree = 2;
    std::vector<QVector3D> mPoints;
    std::vector<float> mKnots;

    void NewPoint(const QVector3D& point);
    void MakeKnotVector();

    void MakeBSplineCurve();


    void DeleteCurve();

    float Bid(float t, int iteration, int degree);
    float Wid(float t, int iteration, int degree);

//    virtual void init() override;
    virtual void draw(QMatrix4x4& projectionMatrix, QMatrix4x4& viewMatrix) override;
};

#endif // B_SPLINE_H
