#ifndef OBJECT_H
#define OBJECT_H

#include <gmtl/Vec.h>
#include "GLData.h"

//-----------------------------------------------------------------------------
/// @file GLData.h
/// @author Milto Miltiadou
/// @version 1.0
/// @date 04/05/15
/// @class Object
/// @brief A sphere represented a function for illustrating that the Marching
/// cubes Algorithm works
//-----------------------------------------------------------------------------


class Object
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    /// @param[in] i_isolevel is the threshold that defines the surface of the
    /// object
    /// @param[in] i_lowerLimits the lower boundaries of the space
    /// @param[in] i_higherLimits the high boundaries of the space
    //-------------------------------------------------------------------------
    Object(
            double i_isolevel,
            const gmtl::Vec3f &i_lowerLimits,
            const gmtl::Vec3f &i_higherLimits
            );
    //-------------------------------------------------------------------------
    /// @brief gets the function value of the object given a point
    /// @param[in] i_point the given point
    //-------------------------------------------------------------------------
    double functionValue(const gmtl::Vec3f &i_point);
    //-------------------------------------------------------------------------
    /// @brief method that returns the min Limits of the object
    //-------------------------------------------------------------------------
    const gmtl::Vec3f getMinLimits()const;
    //-------------------------------------------------------------------------
    /// @brief method that returns the max Limits of the object
    //-------------------------------------------------------------------------
    const gmtl::Vec3f getMaxLimits()const;
    //-------------------------------------------------------------------------
    /// @brief method that returns the isolevel of the object
    //-------------------------------------------------------------------------
    float getIsolevel()const{return m_isolevel;}
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~Object();


private:
    //-------------------------------------------------------------------------
    /// @brief the min limits of the object
    //-------------------------------------------------------------------------
    gmtl::Vec3f m_lowerLimits;
    //-------------------------------------------------------------------------
    /// @brief the max limits of the object
    //-------------------------------------------------------------------------
    gmtl::Vec3f m_higherLimits;
    //-------------------------------------------------------------------------
    /// @brief the isolevel of the object
    //------------------------------------------------------------------------
    double m_isolevel;

};

#endif // OBJECT_H
