#include "Blob.h"
#include <iostream>

using namespace paolina;

ClassImp(paolina::Blob);

Blob::Blob(double radius): radius_(radius), energy_(0.)
{
}

void Blob::DisplayInfo(std::ostream& s) const
{

  s << "Number of voxels in the blob = " << NVoxels() << std::endl;
  s << std::endl;
  // s << "---------- List of voxels in the blob"
  //   << " -----------" << std::endl;
  s << "List of voxels in the blob" << std::endl;
  s << std::endl;
  for (int i=0; i<NVoxels(); ++i) {
    s  << *(Voxel*)voxels_.At(i) << std::endl;
  }
  
}

std::ostream& operator << (std::ostream& s, const paolina::Blob& tr) {
  tr.DisplayInfo(s);
  return s; 
}
