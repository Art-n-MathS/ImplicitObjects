#include "MarchingCubes.h"
#include "MCTables.h"
#include <gmtl/gmtl.h>
#include <gmtl/VecOps.h>

//-----------------------------------------------------------------------------
MarchingCubes::MarchingCubes(
        Object *i_obj,
        unsigned int i_x
        ):
    m_obj(i_obj),
    m_numOfCubsX(i_x),

    m_maxLimits(i_obj->getMaxLimits()[0],i_obj->getMaxLimits()[1],i_obj->getMaxLimits()[2]),
    m_minLimits(i_obj->getMinLimits()[0],i_obj->getMinLimits()[1],i_obj->getMinLimits()[2])
{
       // set the limits of the area
   gmtl::Vec3f minLimits = m_obj->getMinLimits();
   gmtl::Vec3f maxLimits = m_obj->getMaxLimits();

//    calculate how many voxels corresponds to the y and z axis
   gmtl::Vec3f dis;
   dis[0] = maxLimits[0] - minLimits[0];
   dis[1] = maxLimits[1] - minLimits[1];
   dis[2] = maxLimits[2] - minLimits[2];

   m_numOfCubsY = ceil(((double)m_numOfCubsX)*dis[1]/dis[0]);
   m_numOfCubsZ = ceil(((double)m_numOfCubsX)*dis[2]/dis[0]);
   std::cout << "Num of Cubes: " << m_numOfCubsX << " " << m_numOfCubsY << " " << m_numOfCubsZ << "\n";

}

//-----------------------------------------------------------------------------
unsigned int MarchingCubes::getIndex(
        GLData *i_glData,
        const gmtl::Vec3f &i_vertex
        )
{
   for(unsigned int i=0; i<i_glData->m_vertices.size(); i+=3)
   {
       gmtl::Vec3f v(i_glData->m_vertices[i  ],
                   i_glData->m_vertices[i+1],
                   i_glData->m_vertices[i+2]);
       if(v[0]<i_vertex[0]+0.000001 && v[0]>i_vertex[0]-0.000001 &&
          v[1]<i_vertex[1]+0.000001 && v[1]>i_vertex[1]-0.000001 &&
          v[2]<i_vertex[2]+0.000001 && v[2]>i_vertex[2]-0.000001 )
       {
           return i/3;
       }
   }
   return i_glData->addVertex(gmtl::Vec3f(i_vertex[0],i_vertex[1],i_vertex[2]));
}

//-----------------------------------------------------------------------------
/// Modified from:-
/// Paul Bourke (May, 1994) Polygonise a scalar field
/// Available from: paulbourke.net/geometry/polygonise [Accessed 10 Feb 2012]
//-----------------------------------------------------------------------------
// Linearly interpolate the gmtl::Vec3f where an isosurface cuts
// an edge between two vertices, each with their own scalar value
//-----------------------------------------------------------------------------
gmtl::Vec3f MarchingCubes::VertexInterp(
        double isolevel,
        const gmtl::Vec3f &p1,
        const gmtl::Vec3f &p2,
        const double valp1,
        const double valp2
        )
{
   double mu;
   gmtl::Vec3f p;

   mu = (isolevel - valp1) / (valp2 - valp1);
   p[0] = p1[0] + mu * (p2[0] - p1[0]);
   p[1] = p1[1] + mu * (p2[1] - p1[1]);
   p[2] = p1[2] + mu * (p2[2] - p1[2]);

   return(p);
}
//-----------------------------------------------------------------------------
void MarchingCubes::Polygonise(
        const std::vector<gmtl::Vec3f> &points,
        double isolevel,
        GLData *i_glData
        )
{
   std::vector<double> values;
   unsigned int short cubeindex =0;
   unsigned int vertlist[12];

   values.resize(8);
   values[0] = m_obj->functionValue(points[0]);
   values[1] = m_obj->functionValue(points[1]);
   values[2] = m_obj->functionValue(points[2]);
   values[3] = m_obj->functionValue(points[3]);
   values[4] = m_obj->functionValue(points[4]);
   values[5] = m_obj->functionValue(points[5]);
   values[6] = m_obj->functionValue(points[6]);
   values[7] = m_obj->functionValue(points[7]);


   if (values[0] <= isolevel) cubeindex |= 1;
   if (values[1] <= isolevel) cubeindex |= 2;
   if (values[2] <= isolevel) cubeindex |= 4;
   if (values[3] <= isolevel) cubeindex |= 8;
   if (values[4] <= isolevel) cubeindex |= 16;
   if (values[5] <= isolevel) cubeindex |= 32;
   if (values[6] <= isolevel) cubeindex |= 64;
   if (values[7] <= isolevel) cubeindex |= 128;

   /* Cube is entirely in/out of the surface */
   if (edgeTable[cubeindex] == 0)
   {
      return;
   }
   /* Find the vertices where the surface intersects the cube */
   if (edgeTable[cubeindex] & 1)
   {
      vertlist[0] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[0],points[1],values[0],values[1]));
   }
   if (edgeTable[cubeindex] & 2)
   {
      vertlist[1] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[1],points[2],values[1],values[2]));
   }
   if (edgeTable[cubeindex] & 4)
   {
      vertlist[2] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[2],points[3],values[2],values[3]));
   }
   if (edgeTable[cubeindex] & 8)
   {
      vertlist[3] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[3],points[0],values[3],values[0]));
   }
   if (edgeTable[cubeindex] & 16)
   {
      vertlist[4] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[4],points[5],values[4],values[5]));
   }
   if (edgeTable[cubeindex] & 32)
   {
      vertlist[5] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[5],points[6],values[5],values[6]));
   }
   if (edgeTable[cubeindex] & 64)
   {
      vertlist[6] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[6],points[7],values[6],values[7]));
   }
   if (edgeTable[cubeindex] & 128)
   {
      vertlist[7] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[7],points[4],values[7],values[4]));
   }
   if (edgeTable[cubeindex] & 256)
   {
      vertlist[8] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[0],points[4],values[0],values[4]));
   }
   if (edgeTable[cubeindex] & 512)
   {
      vertlist[9] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[1],points[5],values[1],values[5]));
   }
   if (edgeTable[cubeindex] & 1024)
   {
      vertlist[10] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[2],points[6],values[2],values[6]));
   }
   if (edgeTable[cubeindex] & 2048)
   {
      vertlist[11] = m_hashTable.getIndex(
         VertexInterp(isolevel,points[3],points[7],values[3],values[7]));
   }
   for(unsigned int i=0; triTable[cubeindex][i]!=-1;i+=3)
   {
       i_glData->m_indices.push_back(vertlist[triTable[cubeindex][i  ]]);
       i_glData->m_indices.push_back(vertlist[triTable[cubeindex][i+1]]);
       i_glData->m_indices.push_back(vertlist[triTable[cubeindex][i+2]]);
   }
}

//-----------------------------------------------------------------------------
/// end of citation
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
gmtl::Vec3f MarchingCubes::getXYZ(
        unsigned int i_x,
        unsigned int i_y,
        unsigned int i_z
        )
{
   gmtl::Vec3f result;
   double dis = m_maxLimits[0]-m_minLimits[0];
   dis/= (double)(m_numOfCubsX *i_x);
   result[0] = m_minLimits[0] + ( (m_maxLimits[0] - m_minLimits[0])
                                    / (double(m_numOfCubsX)) * (double( i_x)));
   result[1] = m_minLimits[1] + ( (m_maxLimits[1] - m_minLimits[1])
                                    / double(m_numOfCubsY) *(double (i_y)));
   result[2] = m_minLimits[2] + ( (m_maxLimits[2] - m_minLimits[2])
                                    / double(m_numOfCubsZ) *(double (i_z)));
   return result;
}

//-----------------------------------------------------------------------------

void MarchingCubes::polygoniseXYZ(
        const unsigned int i_x,
        const unsigned int i_y,
        const unsigned int i_z,
        const double i_isolevel,
        GLData *i_glData
        )
{
    std::vector<gmtl::Vec3f> points;
    points.resize(8);
    points[0]= getXYZ(i_x  , i_y   ,i_z  );
    points[1]= getXYZ(i_x+1, i_y   ,i_z  );
    points[2]= getXYZ(i_x+1, i_y   ,i_z+1);
    points[3]= getXYZ(i_x  , i_y   ,i_z+1);
    points[4]= getXYZ(i_x  , i_y+1 ,i_z  );
    points[5]= getXYZ(i_x+1, i_y+1 ,i_z  );
    points[6]= getXYZ(i_x+1, i_y+1 ,i_z+1);
    points[7]= getXYZ(i_x  , i_y+1 ,i_z+1);
    Polygonise(points,i_isolevel,i_glData);
}

//-----------------------------------------------------------------------------
void MarchingCubes::computeVertices(GLData *i_glData)
{
   double isolevel = m_obj->getIsolevel();

   for(unsigned int x=0; x<m_numOfCubsX-1; ++x)
   {
      for(unsigned int y=0; y<m_numOfCubsY-1; ++y)
      {
         for (unsigned int z=0; z<m_numOfCubsZ-1; ++z)
         {
            polygoniseXYZ(x,y,z,isolevel,i_glData);
         }
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
void MarchingCubes::computeNormals(GLData *i_glData
        )
{
  // resize the normals, so that the number of normals is equal to the number of vertices
  // each vertex should have each own normal.
  i_glData->m_normals.resize(i_glData->m_vertices.size());

  const std::vector<float>& vertices = i_glData->m_vertices;
  const std::vector<int>& indices = i_glData->m_indices;
  unsigned int numOfIndices = i_glData->m_indices.size();

  // calculate normals
  gmtl::Vec3f normal;
  for (unsigned int i=0;i<numOfIndices;i+=3) // size
  {
    // calculate normal of next triangle

    int index = indices[i]*3;
    const gmtl::Vec3f& A = gmtl::Vec3f(vertices[index],vertices[index+1],vertices[index+2]);
    index = indices[i+1]*3;
    const gmtl::Vec3f& B = gmtl::Vec3f(vertices[index],vertices[index+1],vertices[index+2]);
    index = indices[i+2]*3;
    const gmtl::Vec3f C = gmtl::Vec3f(vertices[index],vertices[index+1],vertices[index+2]);
    gmtl::Vec3f AB = B-A;
    gmtl::Vec3f AC = C-A;
    gmtl::cross(normal,AB,AC);

    // add the normal to all the vertices of that triangle
    if (gmtl::length(normal)>0.0001f)
    {
       i_glData->m_normals[indices[i]*3  ]-=normal[0];
       i_glData->m_normals[indices[i]*3+1]-=normal[1];
       i_glData->m_normals[indices[i]*3+2]-=normal[2];

       i_glData->m_normals[indices[i+1]*3  ]-=normal[0];
       i_glData->m_normals[indices[i+1]*3+1]-=normal[1];
       i_glData->m_normals[indices[i+1]*3+2]-=normal[2];

       i_glData->m_normals[indices[i+2]*3  ]-=normal[0];
       i_glData->m_normals[indices[i+2]*3+1]-=normal[1];
       i_glData->m_normals[indices[i+2]*3+2]-=normal[2];
    }
  }
  std::cout << "Normals calculated\n";
}

//----------------------------------------------------------------------------------------------------------------------
void MarchingCubes::computeNormalsUsingGradientDecent(GLData *i_glData
        )
{
    int vsize = i_glData->m_vertices.size();
    float gx,gy,gz;
    float offset = 0.0001;
    for(int i=0;i<vsize;i+=3)
    {
      gx = (m_obj->functionValue(gmtl::Vec3f(i_glData->m_vertices[i]+offset,i_glData->m_vertices[i+1],i_glData->m_vertices[i+2])) -
            m_obj->functionValue(gmtl::Vec3f(i_glData->m_vertices[i]-offset,i_glData->m_vertices[i+1],i_glData->m_vertices[i+2]))) / (2.0f*offset);
      gy = (m_obj->functionValue(gmtl::Vec3f(i_glData->m_vertices[i],i_glData->m_vertices[i+1]+offset,i_glData->m_vertices[i+2])) -
            m_obj->functionValue(gmtl::Vec3f(i_glData->m_vertices[i],i_glData->m_vertices[i+1]-offset,i_glData->m_vertices[i+2]))) / (2.0f*offset);
      gz = (m_obj->functionValue(gmtl::Vec3f(i_glData->m_vertices[i],i_glData->m_vertices[i+1],i_glData->m_vertices[i+2]+offset)) -
            m_obj->functionValue(gmtl::Vec3f(i_glData->m_vertices[i],i_glData->m_vertices[i+1],i_glData->m_vertices[i+2]-offset))) / (2.0f*offset);
      i_glData->m_normals.push_back(gx);
      i_glData->m_normals.push_back(gy);
      i_glData->m_normals.push_back(gz);
    }
  std::cout << "Normals calculated\n";
}


//-----------------------------------------------------------------------------
GLData *MarchingCubes::createPolygonisedObject()
{
   clock_t t1,t2;
   t1 =clock();

   GLData *glData = new GLData;
   glData->m_maxLimits = gmtl::Vec3f(m_maxLimits[0],m_maxLimits[1],m_maxLimits[2]);
   glData->m_minLimits = gmtl::Vec3f(m_minLimits[0],m_minLimits[1],m_minLimits[2]);
   m_hashTable.setGLData(glData);
   computeVertices(glData);
   computeNormals(glData);
   t2 = clock();
   float diff= ((float)t2-(float)t1) / CLOCKS_PER_SEC;
   std::cout << "Polygonisation took " << diff << " SECONDS!!!\n";

   return glData;
}


//-----------------------------------------------------------------------------
MarchingCubes::~MarchingCubes()
{
}
