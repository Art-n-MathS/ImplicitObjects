#include "GLData.h"
#include <iostream>
#include <fstream>


//-----------------------------------------------------------------------------
GLData::GLData()
{
}


//-----------------------------------------------------------------------------
GLData::GLData(const GLData &i_glData)
{
   setGLData(i_glData);
}

//-----------------------------------------------------------------------------
void GLData::setGLData(const GLData &i_glData)
{
   m_indices.resize(i_glData.m_indices.size());
   for(unsigned int i=0; i<i_glData.m_indices.size(); ++i)
   {
      m_indices[i] = i_glData.m_indices[i];
   }
   m_normals.resize(i_glData.m_normals.size());
   for(unsigned int i=0; i<i_glData.m_normals.size(); ++i)
   {
      m_normals[i] = i_glData.m_normals[i];
   }
   m_vertices.resize(i_glData.m_vertices.size());
   for(unsigned int i=0; i<i_glData.m_vertices.size(); ++i)
   {
      m_vertices[i] = i_glData.m_vertices[i];
   }
}



//-----------------------------------------------------------------------------
unsigned int GLData::addVertex(const gmtl::Vec3f &i_vertex)
{
   m_vertices.push_back(i_vertex[0]);
   m_vertices.push_back(i_vertex[1]);
   m_vertices.push_back(i_vertex[2]);

   return (m_vertices.size()/3-1);
}

//-----------------------------------------------------------------------------
gmtl::Vec3f GLData::getVertex(unsigned int i_index)
{
    return gmtl::Vec3f(m_vertices[i_index/3  ],
                     m_vertices[i_index/3+1],
                     m_vertices[i_index/3+2]);
}


//-----------------------------------------------------------------------------
void GLData::exportToObj(
        std::string i_name)
{

   std::cout << "No of Vertices: " << m_vertices.size()/3 << "\n";
   gmtl::Vec3f offset;
   std::ofstream myfile;
   const std::string extension = ".obj";
   if (i_name.find(extension)!=std::string::npos)
   {
     std::size_t pos = i_name.find_last_of(extension);
     if (pos!=i_name.size()-1)
     {
       i_name+=extension;
     }
   }
   else
   {
     i_name+=extension;
   }
   myfile.open(i_name.c_str());
     if (myfile.is_open())
     {
       int vsize = m_vertices.size();
       int nsize = m_normals.size();
       int isize = m_indices.size();

       for(int i=0; i<vsize; i+=3)
       {
         myfile << "v "<< m_vertices[i  ] - offset[0]
                << " " << m_vertices[i+1] - offset[1]
                << " " << m_vertices[i+2] - offset[2] << "\n";
       }
       myfile <<"\n\n";

       for(int i=0; i<nsize; i+=3)
       {
         myfile <<"vn "<< m_normals[i]<<" "<< m_normals[i+1]
                << " "<< m_normals[i+2] << "\n";
       }
       myfile <<"\n\n";
       for(int i=0; i<isize; i+=3)
       {
         int x = (int)m_indices[i];
         int y = (int)m_indices[i+1];
         int z = (int)m_indices[i+2];
         if(x!=y && y!=z && x!=z)
         {
           x++;y++;z++;
           myfile << "f "<< y <<
                    "//" << y <<
                     " "<< x <<
                   "//" << x <<
                   " "  << z <<
                   "//" << z <<
                   "\n";
         }
       }

       myfile.close();
       std::cout << "File \"" << i_name << "\" saved\n";
     }
}



//-----------------------------------------------------------------------------
GLData::~GLData()
{
}
