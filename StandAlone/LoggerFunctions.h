#ifndef LOGGERFUNCTIONS_H
#define LOGGERFUNCTIONS_H

#include "logger.h"
#include <QVector3D>

/* Logging functions */
/* Log single text */
inline void Log(const std::string log)
{
    Logger::getInstance()->logText(log);
}
inline void LogError(const std::string log)
{
    Logger::getInstance()->logText(log, LogType::REALERROR);
}

/* Able to log different values */
inline void LogValue(const std::string Name, const float value)
{
    Logger::getInstance()->logText(Name + ": " + std::to_string(value));
}
inline void LogValue(const std::string Name, const int value)
{
    Logger::getInstance()->logText(Name + ": " + std::to_string(value));
}
inline void LogValue(const std::string Name, const unsigned int value)
{
    Logger::getInstance()->logText(Name + ": " + std::to_string(value));
}
inline void LogValue(const std::string Name, const double value)
{
    Logger::getInstance()->logText(Name + ": " + std::to_string(value));
}

/* Log a vector */
inline void LogVector(const std::string Name, const QVector3D Vector)
{
    std::string sVector = "( " + std::to_string(Vector.x()) + ", " +std::to_string(Vector.y()) + ", " + std::to_string(Vector.z()) + " )";
    Logger::getInstance()->logText(Name + ": " + sVector);
}

//inline void LogFloat(const std::string Name, const float value)
//{
//    Logger::getInstance()->logText(Name + ": " + std::to_string(value));
//}


#endif // LOGGERFUNCTIONS_H
