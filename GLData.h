#ifndef GLDATA_H
#define GLDATA_H

//-----------------------------------------------------------------------------
/// @file GLData.h
/// @author Milto Miltiadou
/// @version 2.0
/// @date 14/10/13
/// @class GLData
/// @brief in this class all the vertices, normals and indices are saved
//-----------------------------------------------------------------------------

#include <gmtl/Vec.h>
#include <vector>
#include <string>

class GLData
{
   friend class MarchingCubes;

public:
   //-------------------------------------------------------------------------
   /// @brief default constructor
   //-------------------------------------------------------------------------
   GLData();
   //-------------------------------------------------------------------------
   /// @brief default copy constructor
   /// @param[in] i_glData data to be copied
   //------------------------------------------------------------------------
   GLData(const GLData &i_glData);
   //------------------------------------------------------------------------
   /// @brief method that sets the data
   //------------------------------------------------------------------------
   void setGLData(const GLData &i_glData);
   //------------------------------------------------------------------------
   /// @brief method that exports data to as an .obj file
   /// @param[in] i_name the name of the file that will be saved
   //------------------------------------------------------------------------
   void exportToObj(std::string _name);
   //------------------------------------------------------------------------
   /// @brief method that adds vertex and gets its index in return
   /// @param[in] i_vertex vertex to be added
   /// @return the index of the vertex
   //------------------------------------------------------------------------
   unsigned int addVertex(const gmtl::Vec3f &i_vertex);
   //------------------------------------------------------------------------
   /// @brief method that returns a vertex given its index
   /// @param[in] i_index the index of the vertex
   /// @return the vertex of our interest
   //------------------------------------------------------------------------
   gmtl::Vec3f getVertex(unsigned int i_index);
   //------------------------------------------------------------------------
   /// @brief default destructor
   //------------------------------------------------------------------------
   ~GLData();

private:
   //-------------------------------------------------------------------------
   /// @brief the vertices of an object
   //-------------------------------------------------------------------------
   std::vector<float> m_vertices;
   //-------------------------------------------------------------------------
   /// @brief the indices of an object
   //-------------------------------------------------------------------------
   std::vector<int> m_indices;
   //-------------------------------------------------------------------------
   /// @brief the normals of an object
   //-------------------------------------------------------------------------
   std::vector<float> m_normals;
   //-------------------------------------------------------------------------
   /// @brief lower limits
   //-------------------------------------------------------------------------
   gmtl::Vec3f m_minLimits;
   //-------------------------------------------------------------------------
   /// @brief max Limits
   //-------------------------------------------------------------------------
   gmtl::Vec3f m_maxLimits;






};

#endif // GLDATA_H
