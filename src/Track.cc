#include "Track.h"
#include "Blob.h"

#include <algorithm>
#include <math.h>
#include <iostream>

using namespace paolina;

ClassImp(paolina::Track);

Track::Track(): length_(-1.)		
{
  voxels_ = 0;
  blobs_ = 0;
  mainPathVoxels_ = 0;
  extremes_.first = -1;
  extremes_.second = -1;
} 


Track::Track(std::vector<const paolina::Voxel*>& voxvec): 
  length_(-1.)
{
  for (unsigned int i=0; i<voxvec.size(); ++i) 
    voxels_.Add((paolina::Voxel*)voxvec[i]);

  extremes_.first = -1;
  extremes_.second = -1;

  mainPathVoxels_ = 0;
} 

Track::Track(std::vector<paolina::Voxel*>& voxvec): 
  length_(-1.)
{
  for (unsigned int i=0; i<voxvec.size(); ++i) 
    voxels_.Add((paolina::Voxel*)voxvec[i]);

  extremes_.first = -1;
  extremes_.second = -1;

  mainPathVoxels_ = 0;
}

Track::~Track()
{
}

int Track::NVoxels() const
{
  return voxels_.GetLast()+1;
}

int Track::NMainPathVoxels() const
{
  return mainPathVoxels_.GetLast()+1;
}

const Voxel* Track::GetVoxel(int i) const
{

  if (i > voxels_.GetLast()) {
    std::cerr << "Track::GetVoxel(): out of bound exception" << std::endl;
  }
  
  paolina::Voxel* vox = 
    dynamic_cast<paolina::Voxel*>(voxels_.At(i));
  return vox;
}

const Voxel* Track::GetMainPathVoxel(int i) const
{

  if (i > mainPathVoxels_.GetLast()) {
    std::cerr << "Track::GetMainPathVoxel(): out of bound exception" << std::endl;
  }
  
  paolina::Voxel* vox = 
    dynamic_cast<paolina::Voxel*>(mainPathVoxels_.At(i));
  return vox;
}

bool Track::AddVoxel(const paolina::Voxel* vox)
{
  voxels_.Add((paolina::Voxel*)vox);
  return true;
}

bool Track::AddMainPathVoxel(const paolina::Voxel* vox)
{
  mainPathVoxels_.Add((paolina::Voxel*)vox);
  return true;
}

int Track::NBlobs() const
{
  return blobs_.GetLast()+1;
}

const Blob* Track::GetBlob(int i) const
{

  if (i > blobs_.GetLast()) {
    std::cerr << "Track::GetBlob(): out of bound exception" << std::endl;
  }
  
  paolina::Blob* blob = 
    dynamic_cast<paolina::Blob*>(blobs_.At(i));
  return blob;
}

bool Track::AddBlob(const paolina::Blob* blob)
{
  blobs_.Add((paolina::Blob*)blob);
  return true;
}

double Track::GetEDep() const
{
  double energy = 0.;
  for (int ihit=0; ihit<this->NVoxels(); ++ihit) {
    const paolina::Voxel* hit = this->GetVoxel(ihit);
    energy += hit->GetEDep();
  }

  return energy;
}

std::vector < std::vector <double> > Track::AdjMatr() const
{
  std::vector < std::vector <double> > a;
  for(int j=0; j<this->NVoxels(); j++) {
    const paolina::Voxel* myvoxel1 = this->GetVoxel(j);
    int voxpos1[3], abspos1[3];
    voxpos1[0]= myvoxel1->GetBinPosition().x();
    voxpos1[1]= myvoxel1->GetBinPosition().y();
    voxpos1[2]= myvoxel1->GetBinPosition().z();
    abspos1[0] = myvoxel1->GetPosition().x();
    abspos1[1] = myvoxel1->GetPosition().y();
    abspos1[2] = myvoxel1->GetPosition().z();
    std::vector <double> tmp;
    for(int k=0; k<this->NVoxels(); k++) { 
      const paolina::Voxel* myvoxel2 = this->GetVoxel(k);
      int voxpos2[3], abspos2[3];
      voxpos2[0]= myvoxel2->GetBinPosition().x();
      voxpos2[1]= myvoxel2->GetBinPosition().y();
      voxpos2[2]= myvoxel2->GetBinPosition().z();
      abspos2[0] = myvoxel2->GetPosition().x();
      abspos2[1] = myvoxel2->GetPosition().y();
      abspos2[2] = myvoxel2->GetPosition().z();
      // First thing: check if the voxels are the same. If so, 
      // the matrix element is a very high number.
      // Otherwise, check if they have one side, one edge or one vertex in common.
      // If they do, the matrix element is the absolute distance between the centers
      // of those voxels. If not, they are not neighbours, thus the matrix element 
      // is zero.
      // std::cout << voxpos1[0] << " and " << voxpos2[0] << std::endl;
      if (voxpos1[0] == voxpos2[0] &&
	  voxpos1[1] == voxpos2[1] &&
	  voxpos1[2] == voxpos2[2]){
	tmp.push_back(1.E9);
      } else if ((voxpos1[0] == voxpos2[0]+1 ||
		  voxpos1[0] == voxpos2[0]-1 ||
		  voxpos1[0] == voxpos2[0]) &&
		 (voxpos1[1] == voxpos2[1]+1 ||
		  voxpos1[1] == voxpos2[1]-1 ||
		  voxpos1[1] == voxpos2[1]) &&
		 (voxpos1[2] == voxpos2[2]+1 ||
		  voxpos1[2] == voxpos2[2]-1 ||
		  voxpos1[2] == voxpos2[2])){
	double dist = sqrt(pow(abspos1[0]-abspos2[0],2)+
			   pow(abspos1[1]-abspos2[1],2)+
			   pow(abspos1[2]-abspos2[2],2));
	tmp.push_back(dist);
      } else {
	tmp.push_back(0.);
      }
    } 
    a.push_back(tmp);
    tmp.clear();
  }
  return a;
}


std::vector <int> Track::closeness (int k, std::vector< std::vector <double> >& a) const {
  std::vector <int> tmp;
  for (unsigned int j=0;j<a.size();j++){
    if (a[k][j] != 0){
      tmp.push_back(j); 
    }
  }
  return tmp;
}

void Track::DisplayInfo(std::ostream& s) const
{

  s << "Length of track = " << length_ << " mm" << std::endl;
  if (extremes_.first<0 || extremes_.second<0) {
    s << "Extremes of the track not calculated yet" << std::endl;
  } else {

    Voxel* extr1 = (Voxel*)(voxels_.At(extremes_.first)); 
    Voxel* extr2 = (Voxel*)(voxels_.At(extremes_.second)); 
   
    s << "Position of the extremes = (" <<  extr1->GetPosition().x() << ", "
    << extr1->GetPosition().y() << ", " <<  extr1->GetPosition().z() << "), ("
    << extr2->GetPosition().x() << ", " <<  extr1->GetPosition().y() << ", " 
    << extr1->GetPosition().z() << ")"  << std::endl;
  }
  s << std::endl;
  s << "List of voxels in the track" << std::endl;
  s << std::endl;
  for (int i=0; i<NVoxels(); ++i) {
    s  << *(Voxel*)voxels_.At(i) << std::endl;
  }
  s << "---------- List of blobs in the track"
    << " -----------" << std::endl;
  s << std::endl;
  for (int i=0; i<NBlobs(); ++i) {
    s << "Blob number " << i << std::endl;
    s << *(Blob*)blobs_.At(i) << std::endl;
    s << "--------------------------------------" << std::endl;
  }
  
}

std::ostream& operator << (std::ostream& s, const paolina::Track& tr) {
  tr.DisplayInfo(s);
  return s; 
}

////////////////////////////////////////////////////////////////////////
