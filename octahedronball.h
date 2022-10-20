#ifndef OCTAHEDRONBALL_H
#define OCTAHEDRONBALL_H

#include "visualobject.h"
#include "Objects/bakke.h"
#include "Objects/hoydekart.h"
#include "StandAlone/LoggerFunctions.h"
#include <unordered_map>
#include "Objects/b_spline.h"
//#include "renderwindow.h"


/* Vil prøve å sette opp kontaktpunkter for ballen */
struct SurfaceContactPoint
{
    unsigned int Index{};
    float FrictionConstant{};
    QVector3D Baryc{};
    QVector3D SurfacePosition{};
    QVector3D SurfaceNormal{};
};

class OctahedronBall : public VisualObject
{
/* Konstruksjonen av objektet og ballen */
private:
    /*class */Logger* mLogger{ nullptr };
    int FrameCount{};

    int m_rekursjoner;
    int m_indeks;
    float mRadius; // Radiusen til ballen
    void lagTriangel(const QVector3D& v1, const QVector3D& v2, const QVector3D& v3);
    void subDivide(const QVector3D& a, const QVector3D& b, const QVector3D&c, int n);
    void CalculateNormalofTriangle() override;


public:
    OctahedronBall(int recursions, float radius);
    ~OctahedronBall();

    void Reset(Bakke* bakken, const QVector3D& StartLocation, const QVector3D StartVelocity);
    void Reset();
    void MoveTo(const QVector3D& Location);

    /* Pre Sim Flytting */
    /* Når objektet flyttes på utenfor simuleringen */
    QVector3D mStartLocation{};

    void PreSim_MoveTo(const QVector3D& Location, Bakke* bakken);
    void PreSim_MoveTo(const QVector3D& Location);


    /* Brukes når ballen blir dratt rundt utenfor simuleringen */
    void SetSurfaceIndex(Bakke* bakken);

    float GetRadius() const { return mRadius; }


/* Bakken ballen skal være på */
private:
    Bakke* mBakken{nullptr};
    int mTriangleIndex{ -1 }; // Indeksen senteret er over

    std::unordered_map<unsigned int, SurfaceContactPoint> mSurfaceContactPoints;

/* Fysikk utregning */
private:

    float Gravity = 9.81f;  // m/s^2
    float Mass = 1.f;       // kg
    float Elasticity = 0.4f; // How much velocity is kept, along the surface normal, when it bounces

    QVector3D Acceleration_PreviousFrame;
    QVector3D Acceleration;
    float FrictionForce;
    QVector3D FrictionAcceleration;
    QVector3D NormalForce;

    QVector3D Velocity_PreviousFrame;
    QVector3D Velocity;     // m/s
    float Speed;

    QVector3D Position_PreviousFrame;
    QVector3D Position;

public:

    float GetElasticity() const { return Elasticity; }
    void SetElasticity(float value){ Elasticity = value; }

    /* Setter start verdier */
    void SetStartVelocity(const QVector3D StartVelocity);

    /* Kalkulerer fysikken. Inneholder all logikken som trengs til det */
    void CalculatePhysics(Bakke* bakken, float DeltaTime);
    void CalculateAcceleration(QVector3D SurfaceNormal, float FrictionForce, const int SurfaceIndex);
    void UpdateVelocity(float DeltaTime);

    void SetNormalForce(const QVector3D SurfaceNormal);

    float GetDistanceToSurface(const QVector3D Position, const QVector3D SurfacePosition, const QVector3D SurfaceNormal);

    void ProjectCollisionVelocity(const QVector3D n);

/* Rendering */
public:
    void init() override;
    void draw() override;
    void draw(QMatrix4x4& projectionMatrix, QMatrix4x4& viewMatrix) override;

/* -- B-SPLINE LOKASJONS KURVE -- */
public:

    bool bShowSplineCurve{ true };
    bool bShowSplinePoints{ true };
    void ShowSplinePoints(bool b);

    bool bFirstGroundHit{ false };

    void Update(float DeltaTime);

    std::unique_ptr<BSpline> LocationSpline;

    float SplineTimer{};
    float SplinePointTimeInterval{ 1.f };

    void MakeSplinePoint(const QVector3D& Location);
    bool bScheduleNewSplinePoint{ false };
    QVector3D ScheduleSplinePoint;

    void InitSpline();

/* Debug */
public:

};
#endif // OCTAHEDRONBALL_H

