#ifndef PVOXELBUILDER_H
#define PVOXELBUILDER_H

#include "Voxel.h"

#include <TVector3.h>
#include <TLorentzVector.h>
#include <THnSparse.h>

#include <iostream>

namespace paolina {

 class VoxelBuilder
  {
  public:
    VoxelBuilder();
    VoxelBuilder(std::vector<double> voxel_size, std::vector<std::pair<double,double> > vol_range);
    VoxelBuilder(std::vector<double> voxel_size, std::vector<double> left_range, 
                 std::vector<double> right_range);
        
    ~VoxelBuilder(){}

    std::vector<paolina::Voxel*> FillVoxels(std::vector< std::pair< TVector3, double> >);
    std::vector<paolina::Voxel*> FillVoxels(std::vector< std::pair< TLorentzVector, double> >);

  private:
    THnSparseD* eDepHisto_; 
    std::vector<double> voxel_size_;
    std::vector<std::pair<double,double> > vol_range_;

    double xmin_[3];
    double xmax_[3];

    inline std::vector<double> GetVoxelSize(){return voxel_size_;}

  };
}
#endif
