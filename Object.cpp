#include "Object.h"


//-----------------------------------------------------------------------------
Object::Object(
        double i_isolevel,
        const gmtl::Vec3f &i_lowerLimits,
        const gmtl::Vec3f &i_higherLimits
        ):
    m_lowerLimits(i_lowerLimits),
    m_higherLimits(i_higherLimits),
    m_isolevel(i_isolevel)
{
}


//-----------------------------------------------------------------------------
double Object::functionValue(const gmtl::Vec3f &i_point)
{
   // the equation of a sphere passing through the origin with radius 3
   return i_point[0]*i_point[0]+i_point[1]*i_point[1]+i_point[2]*i_point[2]-9;
}

//-----------------------------------------------------------------------------
const gmtl::Vec3f Object::getMinLimits()const
{
   return m_lowerLimits;
}


//-----------------------------------------------------------------------------
const gmtl::Vec3f Object::getMaxLimits()const
{
    return m_higherLimits;
}

//-----------------------------------------------------------------------------
Object::~Object()
{}
