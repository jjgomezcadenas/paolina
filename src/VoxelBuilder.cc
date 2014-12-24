#include "VoxelBuilder.h"

using namespace paolina;

VoxelBuilder::VoxelBuilder()
{
  voxel_size_.push_back(10.);
  voxel_size_.push_back(10.);
  voxel_size_.push_back(1.);
  // fvol_dim.push_back(500.);  // hard core dimensiones NEW
  // fvol_dim.push_back(500.);
  // fvol_dim.push_back(600.);
  
}

VoxelBuilder::VoxelBuilder(std::vector<double> voxel_size, std::vector<double> left_range, 
            std::vector<double> right_range)
{
  std::vector<std::pair<double, double> > vol_range;
  for (size_t i=0; i<3; i++)
  {
    std::pair<double, double> vol;
    vol.first = left_range[i];
    vol.second = right_range[i];
    vol_range.push_back(vol);
  }
  VoxelBuilder(voxel_size, vol_range);

}


VoxelBuilder::VoxelBuilder(std::vector<double> voxel_size, 
    std::vector<std::pair<double,double> > vol_range) :
  voxel_size_(voxel_size), vol_range_(vol_range)
{
  // Build the histogram to be filled with the energy depositions
  // reconstructed or from the true simulation
  int n_bins[3];

  for (unsigned int i=0; i<3; i++){
    n_bins[i] = (int) (vol_range_[i].second - vol_range_[i].first)/voxel_size_[i];
  }

  for (unsigned int i=0; i<3; i++){
    xmin_[i] = vol_range_[i].first;
    xmax_[i] = vol_range_[i].second;
  }

  eDepHisto_ = 
    new THnSparseD("DepSlices", "DepSlices", 3, n_bins, xmin_, xmax_, 1024*16);
  
}

// VoxelBuilder::VoxelBuilder(std::vector<double> voxel_size, std::vector<double> voldim): 
//   voxel_size_(voxel_size), fvol_dim(voldim)
// {
//   // Build the histogram to be filled with the energy depositions
//   // reconstructed or from the true simulation

//   for (unsigned int i=0; i<3; i++){
//   n_bins_[i] = (int) fvol_dim[i]/voxel_size_[i];
//   }

//   double xmin[3];
//   double xmax[3];
//   for (unsigned int i=0; i<2; i++){
//     xmin[i] = -fvol_dim[i]/2;
//     xmax[i] = fvol_dim[i]/2;
//   }

//   xmin[2] = 0;
//   xmax[2] = fvol_dim[2];

//   for (unsigned int i=0; i<3; ++i) {
//     xmin_[i] = xmin[i];
//     xmax_[i] = xmax[i];
//   }

//   eDepHisto_ = 
//     new THnSparseD("DepSlices", "DepSlices", 3, n_bins_, xmin, xmax, 1024*16);
// }

std::vector<paolina::Voxel*> VoxelBuilder::FillVoxels(std::vector< std::pair <TVector3, double> > point)
{
  eDepHisto_->Reset();
  std::vector<paolina::Voxel*> voxelvector;

  // Fill the histograms with the energy deposition points
  for (unsigned int i=0; i<point.size(); i++){
    double pos[3] = {0, 0, 0}; 
    TVector3 position = point[i].first;
    position.GetXYZ(pos);
    eDepHisto_->Fill(pos, point[i].second);  
  }

  double e_dep = 0.;
  // int binarray[3]; 
  double absposition[3];

  
  for (Long64_t indx=0; indx<eDepHisto_->GetNbins(); ++indx) {

    // The coordinates in terms of the indices of the bin are
    // stored into the coord variable
    int* coord = new int[3];
    e_dep = eDepHisto_->GetBinContent(indx, coord);
    
    // binarray[0] = coord[0];
    // binarray[1] = coord[1];
    // binarray[2] = coord[2];

    // Calculate the position in the absolute coordinates of the 
    // detector
    for (unsigned int j=0; j<3; ++j) {
      absposition[j] = xmin_[j]+(coord[j]-1)*voxel_size_[j] 
	+ (voxel_size_[j]/2.);
    }

    paolina::Voxel myvoxel(coord[0], coord[1], coord[2], 
			   absposition[0], absposition[1], absposition[2], 
			   e_dep);
    myvoxel.SetSize(voxel_size_[0], voxel_size_[1], voxel_size_[2]);

    voxelvector.push_back(new paolina::Voxel(myvoxel)); 
 
    delete coord;
  }

  return voxelvector;
}

std::vector<paolina::Voxel*> VoxelBuilder::FillVoxels(std::vector< std::pair< TLorentzVector, 
    double> > point)
{
  eDepHisto_->Reset();
  std::vector<paolina::Voxel*> voxelvector;

  // Fill the histograms with the energy deposition points
  for (unsigned int i=0; i<point.size(); i++){
    double pos[3] = {0, 0, 0}; 
    TLorentzVector xyzt = point[i].first; 
    TVector3 position = xyzt.Vect();  
    position.GetXYZ(pos);
    eDepHisto_->Fill(pos, point[i].second);  
  }

  double e_dep = 0.;
  //  int binarray[3]; 
  double absposition[3];

  for (Long64_t indx=0; indx<eDepHisto_->GetNbins(); ++indx) {

    // The coordinates in terms of the indices of the bin are
    // stored into the coord variable
    int* coord = new int[3];
    e_dep = eDepHisto_->GetBinContent(indx, coord);
    
    // binarray[0] = coord[0];
    // binarray[1] = coord[1];
    // binarray[2] = coord[2];

    // Calculate the position in the absolute coordinates of the 
    // detector
    for (unsigned int j=0; j<3; ++j) {
      absposition[j] = xmin_[j]+(coord[j]-1)*voxel_size_[j] 
	+ (voxel_size_[j]/2.);
    }

    paolina::Voxel myvoxel(coord[0], coord[1], coord[2], 
			   absposition[0], absposition[1], absposition[2], 
			   e_dep);
    
    voxelvector.push_back(new paolina::Voxel(myvoxel)); 
 
    delete coord;
  }

  return voxelvector;
}




