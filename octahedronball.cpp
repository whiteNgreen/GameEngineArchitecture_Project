#include "octahedronball.h"
#include "logger.h"

OctahedronBall::OctahedronBall(int recursions, float m_radius)
    : m_rekursjoner(recursions), m_indeks(0), mRadius(m_radius)
{
    mLogger = Logger::getInstance();

    mVertices.reserve(3*8*pow(4, m_rekursjoner));

    /* Size of ball */
    QVector3D v0{    0,     0,   mRadius};
    QVector3D v1{ mRadius,     0,      0};
    QVector3D v2{    0,  mRadius,      0};
    QVector3D v3{-mRadius,     0,      0};
    QVector3D v4{    0, -mRadius,      0};
    QVector3D v5{    0,     0,  -mRadius};

    /* Subdivide ball */
    subDivide(v0, v1, v2, m_rekursjoner);
    subDivide(v0, v2, v3, m_rekursjoner);
    subDivide(v0, v3, v4, m_rekursjoner);
    subDivide(v0, v4, v1, m_rekursjoner);
    subDivide(v5, v2, v1, m_rekursjoner);
    subDivide(v5, v3, v2, m_rekursjoner);
    subDivide(v5, v4, v3, m_rekursjoner);
    subDivide(v5, v1, v4, m_rekursjoner);

    CalculateNormalofTriangle();

    /* BSpline objektet */
    LocationSpline = std::make_unique<BSpline>();
}

OctahedronBall::~OctahedronBall()
{
    LocationSpline = nullptr;
}

void OctahedronBall::Reset(Bakke* bakken, const QVector3D& StartLocation, const QVector3D StartVelocity)
{
    /* Move to start location */
    PreSim_MoveTo(StartLocation, bakken);

    /* Reset Movement Variables */
    SetStartVelocity(StartVelocity);
    Acceleration_PreviousFrame *= 0.f;
    Acceleration *= 0.f;

    /* Reset Debug Variables */
    FrameCount = 0;

    /* Delete B-Spline */
    LocationSpline->DeleteCurve();
    bFirstGroundHit = false;
    SplineTimer = 0.f;
}

void OctahedronBall::Reset()
{
    PreSim_MoveTo(mStartLocation);

    SetStartVelocity({0,0,0});
    Acceleration_PreviousFrame *= 0.f;
    Acceleration *= 0.f;

    /* Reset Debug Variables */
    FrameCount = 0;

    /* Delete B-Spline */
    LocationSpline->DeleteCurve();
    bFirstGroundHit = false;
    SplineTimer = 0.f;
}

void OctahedronBall::MoveTo(const QVector3D &Location)
{
    Position_PreviousFrame = Position;

    QVector3D Move = Location - mMatrix.column(3).toVector3D();
    mMatrix.translate(Move);
    Position = mMatrix.column(3).toVector3D();
}

void OctahedronBall::PreSim_MoveTo(const QVector3D &Location, Bakke *bakken)
{
    mStartLocation = Location;
    MoveTo(Location);
    SetSurfaceIndex(bakken);
}

void OctahedronBall::PreSim_MoveTo(const QVector3D &Location)
{
    mStartLocation = Location;
    MoveTo(Location);
}

void OctahedronBall::SetSurfaceIndex(Bakke* bakken)
{
    if (!bakken)
    {
        mTriangleIndex = 0;
        return;
    }

    int tmpIndex{};
    QVector3D SurfacePosition{};
    QVector3D SurfaceNormal{};
    QVector3D Baryc{};
    QVector3D ObjectPosition{ Position };
    float radius{ mRadius };


    tmpIndex = bakken->BarySentricCoordinate( ObjectPosition, Baryc, SurfacePosition, SurfaceNormal );

    /* Sjekk om ballen er oppå flata */
    float HeightDifferenceObjectSurface = GetDistanceToSurface( ObjectPosition, SurfacePosition, SurfaceNormal ) - radius;

    /* Hvis ballen er under flata så korrigerer vi høyden i z-aksen til å matche surface */
    if (HeightDifferenceObjectSurface >= 0.001f)
    {
        tmpIndex = 0;
    }

    mTriangleIndex = tmpIndex;
}

void OctahedronBall::SetStartVelocity(const QVector3D StartVelocity)
{
    Velocity_PreviousFrame = StartVelocity;
    Velocity = StartVelocity;
}


void OctahedronBall::CalculatePhysics(Bakke* bakken, float DeltaTime)
//void OctahedronBall::CalculatePhysics(HoydeKart* bakken, float DeltaTime)
{
    FrameCount++;
    mLogger->logText("Frame: " + std::to_string(FrameCount));

    /* Object Position Variables */
    QVector3D ObjectPosition{ Position };
    float radius{ mRadius };    // Som Ball, Kollisjon tar med radius
//    float radius{ 0.f };  // Som partikkel, Kollisjonen er ved senter
    QVector3D TranslationAdjustment{};

    /* Surface Variables */
    QVector3D SurfacePosition;
    QVector3D SurfaceNormal{};
    QVector3D Baryc;
    int FinalTriangleIndex{ -1 };
    int tmpIndex{ -1 };
    float HeightDifferenceObjectSurface;
    float FrictionConstant{};


    /* 1: Posisjonen og om den er på en overflate */
    /* Få info om overflaten ballen er over */
    tmpIndex = bakken->BarySentricCoordinate( ObjectPosition, Baryc, SurfacePosition, SurfaceNormal);
    LogValue("Friction Constant", FrictionConstant);

    /* Normal force */
    SetNormalForce(SurfaceNormal);
    LogVector("NormalForce: ", NormalForce);

    /* Friction Force */
    FrictionForce = FrictionConstant * NormalForce.length();
    mLogger->logText("FrictionForce: " + std::to_string(FrictionForce));


    /* Sjekk om ballen er oppå flata */
    HeightDifferenceObjectSurface = GetDistanceToSurface(ObjectPosition, SurfacePosition, SurfaceNormal) - radius;
    mLogger->logText("HeightDifference: " + std::to_string(HeightDifferenceObjectSurface));

    /* Hvis ballen er under flata så korrigerer vi høyden i z-aksen til å matche surface */
    if (HeightDifferenceObjectSurface <= 0.001f)
    {
        FinalTriangleIndex = tmpIndex;
    }
    /* Hvis ballen er over flaten så skal den falle fritt nedover */
    else
    {
        FinalTriangleIndex = -1;
    }


    /* 2: Akselerasjonen */
    CalculateAcceleration(SurfaceNormal, FrictionForce, FinalTriangleIndex);
    LogVector("Acceleration", Acceleration);


    /* 3: Hastigheten */
    UpdateVelocity(DeltaTime);
    LogVector("Velocity", Velocity);
    mLogger->logText("Velocity Size: " + std::to_string(Velocity.length()));


    QVector3D SurfaceNormal2{};    // Hvis den kommer innenfor en ny trekant
    QVector3D SurfacePosition2{};

    /* 4: Den nye posisjonen */
    /* Juster posisjonen etter hastigheten */
    ObjectPosition += Velocity * DeltaTime;

    /* Sjekk de barysentriske koordinatene for den nye posisjonen */
    tmpIndex = bakken->BarySentricCoordinate( ObjectPosition, Baryc, SurfacePosition2, SurfaceNormal2);


    /* 5: Er den nye posisjonen gyldig? Nei?: Gjør justeringer */
    /* Sjekk om ballen er oppå flata */
    HeightDifferenceObjectSurface = GetDistanceToSurface(ObjectPosition, SurfacePosition2, SurfaceNormal2) - radius;
    mLogger->logText("HeightDifference: " + std::to_string(HeightDifferenceObjectSurface));
    LogVector("SurfaceNormal2: ", SurfaceNormal2);

    /* Hvis ballen er under flata så korrigerer vi høyden i z-aksen til å matche surface */
    if (HeightDifferenceObjectSurface <= 0.001f)
    {
//        Log("BALL PÅ TREKANT");
        /* Treffer en trekant etter fritt fall */
        if (mTriangleIndex == -1)
        {
            mLogger->logText("LANDED ON TRIANGLE: " + std::to_string(tmpIndex) + ", FROM FREEFALL");


            /* Må finne hvor mye som skal tas fra hastigheten
             * Hvis ballen lander på bakken mellom frames så duger ikke dette
             * må se forrige posisjon og hastigheten og bestemme hvor lang tid det ville tatt for å røre bakken
             * FINN BetweenDeltaTime
             * Velocity -= Acceleration * BetweenDeltaTime;
             * */
            Velocity -= Acceleration * DeltaTime;

            /* Finn vinkelen mellom Velocity * -1 og Surface Normal.
             * Nye velocity er da Velocity * -1 og rotert theta*2 mot og forbi Surface Normal */
            float dotprod{ QVector3D::dotProduct(Velocity.normalized(), SurfaceNormal * -1.f) };
            LogValue("Velocity to SurfaceNormal Dotproduct: ", dotprod);

            CalculateAcceleration(SurfaceNormal, FrictionForce, tmpIndex);

            if (dotprod != 1)
            {
                /* Projeksjonen av Velocity på b (orthogonal til n på vei mot Velocity)
                 * For å kunne finne hastigheten en vektor burde ha parallelt med bakken når den lander     //vProj
                 * OG for å finne hastigheten ballen skal ha perpendikulært med bakken                      //hProj
                 * */
                ProjectCollisionVelocity(SurfaceNormal);
            }
            else
            {
                mLogger->logText("BOUNCE STRAIGHT UP ALONG NORMAL!");
                Velocity = (Velocity * -1) * Elasticity;
            }

            UpdateVelocity(DeltaTime);
            ObjectPosition += Velocity * DeltaTime;


            /* Første gangen ballen lander på bakken */
            if (!bFirstGroundHit)
            {
//                MakeSplinePoint(ObjectPosition);
                bScheduleNewSplinePoint = true;
                ScheduleSplinePoint = ObjectPosition;
                bFirstGroundHit = true;
            }
        }

        /* Treffer kula en ny trekant? */
        else if (tmpIndex != mTriangleIndex && (tmpIndex != -1 && mTriangleIndex != -1))
        {
            /* Oppdater velocity */
            mLogger->logText("CAME OVER TO A NEW TRIANGLE!");   // Husker ikke forrige triangel så begge surfacenormals er (0,0,1)

            /* Hovedproblemet er at ballen (ikke som partikkel) ikke har
             * kontroll på kontaktpunktene den ville hatt over hele overflaten
             * når den går mellom trekanter
             * */
            ProjectCollisionVelocity(SurfaceNormal2);   // Mer nøyaktig overgang mellom overflater

            UpdateVelocity(DeltaTime);

            HeightDifferenceObjectSurface = GetDistanceToSurface(ObjectPosition, SurfacePosition2, SurfaceNormal2) - radius;
        }

        TranslationAdjustment += SurfaceNormal2 * -1.f * HeightDifferenceObjectSurface; // Hvorfor må jeg gange med -1?!
        ObjectPosition += TranslationAdjustment;

        LogVector("TranslationAdjustment", TranslationAdjustment);


        FinalTriangleIndex = tmpIndex;
    }


    LogValue("Temp Index: ", tmpIndex);
    LogValue("Triangle Index: ", mTriangleIndex);
    LogValue("FinalTIndex: ", FinalTriangleIndex);


    /* 6: Sett den endelige posisjonen */
    MoveTo(ObjectPosition);
    mTriangleIndex = FinalTriangleIndex;


    mLogger->logText("");
}


void OctahedronBall::CalculateAcceleration(QVector3D SurfaceNormal, float FrictionForce, const int SurfaceIndex)
{
    Acceleration_PreviousFrame = Acceleration;

    /* If ball is free falling */
    if (SurfaceIndex == -1)
    {
        Acceleration = {0, 0, -Gravity};
        FrictionAcceleration *= 0;

//        return;
    }
    /* else If ball is on a surface */
    else
    {
        float nx{ SurfaceNormal.x() };
        float ny{ SurfaceNormal.y() };
        float nz{ SurfaceNormal.z() };
        Acceleration = QVector3D{nx * nz * Gravity, ny * nz * Gravity, (nz * nz * Gravity) - Gravity};

        /* Calculate the friction force/acceleration */
        {
            /* Finn retningen av friksjonskraften */
            FrictionAcceleration = QVector3D::crossProduct(SurfaceNormal, QVector3D::crossProduct(Velocity, SurfaceNormal)) * -1.f;
            FrictionAcceleration.normalize();

            FrictionAcceleration *= FrictionForce * SurfaceNormal.z();
        }
//        return;
    }
    LogVector("Friction Acceleration: ", FrictionAcceleration);
}

void OctahedronBall::UpdateVelocity(float DeltaTime)
{
    Velocity_PreviousFrame = Velocity;

    /* Calculate new velocity */
    QVector3D NewVelocity = Velocity + (Acceleration * DeltaTime);

    /* Kalkulere ny hastighet fra friksjonkraften. Som kan ikke føre til at hastigheten vil kunne gå i motsatt retning */
    QVector3D FrictionVelocityChange = (FrictionAcceleration * DeltaTime);

    /* Hvis forandringen i hastigheten fra friksjonen får objektet til å gå bakover så vil det heller bare stoppe og gå til 0 */
    if (FrictionVelocityChange.length() > NewVelocity.length())
    {
        FrictionVelocityChange *= NewVelocity.length()/FrictionVelocityChange.length();

    }
    LogVector("FrictionVelocityChange", FrictionVelocityChange);
    NewVelocity += FrictionVelocityChange;

    Velocity = NewVelocity;

/* 'Fix' Velocity under certain conditions */
    /* If velocity is extremely small set it to 0 */
    if (Velocity.length() < 0.00001f)
    {
        Velocity *= 0.f;
    }


    static float timeStill{};
    /* Hvis ballen står stille og hastigheten ikke har forandret seg
     *  så regnes det med at ballen er stillestående på en flat bakke
     *  vi setter Velocity = 0
     *  */
    if (Position == Position_PreviousFrame && Velocity == Velocity_PreviousFrame)
    {
        Velocity *= 0.f;
    }

    /* Hvis ballen har stått stille på en flat overflate over lenre tid, men
     * hastigheten nedover er teknisk sett større enn kravet tidligere
     * */
    else if (Position.z() == Position_PreviousFrame.z() && Velocity.toVector2D().length() <= 0.1f)
    {
        timeStill += DeltaTime;
        if (timeStill > 2.0f)
        {
            Velocity *= 0.f;
            timeStill = 0.f;
        }
    }
    else
    {
        timeStill = 0.f;
    }
}

void OctahedronBall::SetNormalForce(const QVector3D SurfaceNormal)
{
    NormalForce = SurfaceNormal * Gravity * SurfaceNormal.z();
}

float OctahedronBall::GetDistanceToSurface(const QVector3D Position, const QVector3D SurfacePosition, const QVector3D SurfaceNormal)
{
    /* Gets a vector with the length to the surface */
    QVector3D Y{(Position - SurfacePosition) * SurfaceNormal};

    /* Checks if this is below or above the surface with the dotprod of Y and SurfaceNormal */
    float dotprod = QVector3D::dotProduct(SurfaceNormal, Y) / (SurfaceNormal.length() + Y.length());
    if (dotprod > 0) { dotprod = 1; }
    else if (dotprod < 0) { dotprod = -1; }
    return Y.length() * dotprod;
}

void OctahedronBall::ProjectCollisionVelocity(const QVector3D n)
{
    /* Projeksjonen av Velocity på b (orthogonal til n på vei mot Velocity)
     * For å kunne finne hastigheten en vektor burde ha parallelt med bakken når den lander     //vProj
     * OG for å finne hastigheten ballen skal ha perpendikulært med bakken                      //hProj
     * */
    QVector3D Vc{ Velocity };    // Inkommende Hastigheten
    QVector3D V{};                  // Ny Hastighet
//    QVector3D n{ SurfaceNormal }; // SurfaceNormal
    QVector3D B{ QVector3D::crossProduct(n, QVector3D::crossProduct(Vc.normalized(), n)) };
    QVector3D vProj = (QVector3D::dotProduct(Vc,B)/QVector3D::dotProduct(B,B))*B;
    QVector3D hProj = (QVector3D::dotProduct(Vc,n)/QVector3D::dotProduct(n,n))*n;
    V = (vProj + (n * hProj.length() * Elasticity));
    Velocity = V;
}

void OctahedronBall::subDivide(const QVector3D &a, const QVector3D &b, const QVector3D &c, int n)
{
    if (n>0)
    {
        QVector3D v1 = a+b; v1.normalize(); v1 *= mRadius;
        QVector3D v2 = a+c; v2.normalize(); v2 *= mRadius;
        QVector3D v3 = c+b; v3.normalize(); v3 *= mRadius;
        subDivide(a, v1, v2, n-1);
        subDivide(c, v2, v3, n-1);
        subDivide(b, v3, v1, n-1);
        subDivide(v3, v2, v1, n-1);
    }
    else
    {
        lagTriangel(a,b,c);
    }
}

void OctahedronBall::CalculateNormalofTriangle()
{
    for (unsigned int it{0}; it < mVertices.size(); it+=3)
    {
        QVector3D v1{ mVertices[it+1].getPosition() - mVertices[it].getPosition() };
        QVector3D v2{ mVertices[it+2].getPosition() - mVertices[it].getPosition() };
        QVector3D normal = QVector3D::crossProduct(v1, v2);
        normal.normalize();

        mVertices[it].setNormal(normal);
        mVertices[it+1].setNormal(normal);
        mVertices[it+2].setNormal(normal);
    }
}

void OctahedronBall::lagTriangel(const QVector3D &v1, const QVector3D &v2, const QVector3D &v3)
{
//    QVector3D color{20, 20, 200};
    mVertices.push_back(Vertex(v1));
    mVertices.push_back(Vertex(v2));
    mVertices.push_back(Vertex(v3));

    int m = mVertices.size();
    mIndices.push_back(m-3);
    mIndices.push_back(m-2);
    mIndices.push_back(m-1);
}

void OctahedronBall::init()
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

    // 1st attribute buffer : vertices
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

void OctahedronBall::draw()
{
    glUseProgram(m_shader->getProgram());
    m_shader->setUniformMatrix("mMatrix", mMatrix);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glBindVertexArray( mVAO );
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void OctahedronBall::draw(QMatrix4x4 &projectionMatrix, QMatrix4x4 &viewMatrix)
{
    glUseProgram(m_shader->getProgram());
    m_shader->setUniformMatrix("mMatrix", mMatrix);
    m_shader->setUniformMatrix("pMatrix", projectionMatrix);
    m_shader->setUniformMatrix("vMatrix", viewMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glBindVertexArray( mVAO );
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    /* Draw B-Spline */
    if (bFirstGroundHit && bShowSplineCurve)
    {
        LocationSpline->draw(projectionMatrix, viewMatrix);
    }
}

void OctahedronBall::ShowSplinePoints(bool b)
{
    LocationSpline->ShowPoints(b);
}

void OctahedronBall::Update(float DeltaTime)
{
    if (bScheduleNewSplinePoint)
    {
        MakeSplinePoint(ScheduleSplinePoint);

        bScheduleNewSplinePoint = false;
    }

    if (bFirstGroundHit)
    {
        SplineTimer += DeltaTime;

        if (SplineTimer > SplinePointTimeInterval)
        {
            MakeSplinePoint(Position);
            SplineTimer = 0.f;
        }
    }

}

void OctahedronBall::MakeSplinePoint(const QVector3D &Location)
{
    LocationSpline->NewPoint(Location);
}

void OctahedronBall::InitSpline()
{
    LocationSpline->init();
}


