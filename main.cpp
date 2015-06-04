#include <iostream>

#include "GLData.h"
#include "Object.h"
#include "MarchingCubes.h"

int main(void)
{
   gmtl::Vec3f lowerLimits(-5.0f,-5.0f,-5.0f);
   gmtl::Vec3f higherLimits(5.0f,5.0f,5.0f);
   Object *obj = new Object(0.0, lowerLimits, higherLimits);
   MarchingCubes mc(obj,20);
   GLData *glData = mc.createPolygonisedObject();
   glData->exportToObj("object.obj");
   delete glData;
   delete obj;
   return 0;
}
