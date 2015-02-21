#include "BlobBuilder.h"
#include "Blob.h"
#include "Track.h"

#include <math.h>

using namespace paolina;

BlobBuilder::BlobBuilder() : fBlobRadius(0.)
{
}

BlobBuilder::BlobBuilder(double radius) : fBlobRadius(radius)
{
}

std::pair<paolina::Blob*, paolina::Blob*> BlobBuilder::MakeBlobs(paolina::Track* mytrack)
{

  std::pair<int, int> extremes = mytrack->GetExtremes();
  int extr1 = extremes.first;
  int extr2 = extremes.second;

  const paolina::Voxel* vxl = mytrack->GetVoxel(extr1);
  double extr1x = vxl->GetPosition().x();
  double extr1y = vxl->GetPosition().y();
  double extr1z = vxl->GetPosition().z();
  vxl = mytrack->GetVoxel(extr2);
  double extr2x = vxl->GetPosition().x();
  double extr2y = vxl->GetPosition().y();
  double extr2z = vxl->GetPosition().z();  

  // To be aware if the two blobs overlap
  bool problem = false;

  // Create two clobs in the extremes
  paolina::Blob* blob1 = new paolina::Blob(fBlobRadius);  
  paolina::Blob* blob2 = new paolina::Blob(fBlobRadius);
  
  double energy1 = 0.;
  double energy2 = 0.;
  //double tmp1;
  //double tmp2;

  for (unsigned int i=0;i<mytrack->NVoxels();i++){
    vxl = mytrack->GetVoxel(i);
    bool insphere1 = false;
    bool insphere2 = false;
    if (sqrt(pow(vxl->GetPosition().x()-extr1x,2)+pow(vxl->GetPosition().y()-extr1y,2)+pow(vxl->GetPosition().z()-extr1z,2)) 
	< fBlobRadius){
      energy1 = energy1 + vxl->GetEDep();
      //   paolina::Voxel* newvxl = new paolina::Voxel(*vxl);
      blob1->AddVoxel(vxl);
      insphere1 = true;
    }     
    if (sqrt(pow(vxl->GetPosition().x()-extr2x,2)+pow(vxl->GetPosition().y()-extr2y,2)+pow(vxl->GetPosition().z()-extr2z,2)) 
	< fBlobRadius){
      energy2 = energy2 + vxl->GetEDep();
      //      paolina::Voxel* newvxl = new paolina::Voxel(*vxl);
      blob2->AddVoxel(vxl);
      insphere2 = true;
    }
    if (insphere1 == true && insphere2 == true) {
      // std::cout << "One voxel belongs to both extremes. 
      // This means that the length of the track is too small compared with the chosen fRadius. 
      // Try to choose a different fBlobRadius. 
      // This event should be dealt with in some way." << std::endl;
      problem = true;
    }
  }

  blob1->SetEnergy(energy1);
  blob2->SetEnergy(energy2);
  
  std::pair<paolina::Blob*, paolina::Blob*> myblobs = std::make_pair(blob1, blob2);
  
  return myblobs;
  
}

std::pair<paolina::Blob*, paolina::Blob*> BlobBuilder::MakeBlobs(const paolina::Track* mytrack)
{

  std::pair<int, int> extremes = mytrack->GetExtremes();
  int extr1 = extremes.first;
  int extr2 = extremes.second;

  const paolina::Voxel* vxl = mytrack->GetVoxel(extr1);
  double extr1x = vxl->GetPosition().x();
  double extr1y = vxl->GetPosition().y();
  double extr1z = vxl->GetPosition().z();
  vxl = mytrack->GetVoxel(extr2);
  double extr2x = vxl->GetPosition().x();
  double extr2y = vxl->GetPosition().y();
  double extr2z = vxl->GetPosition().z();   

  // To be aware if the two blobs overlap
  bool problem = false;

  // Create two blobs in the extremes
  paolina::Blob* blob1 = new paolina::Blob(fBlobRadius);  
  paolina::Blob* blob2 = new paolina::Blob(fBlobRadius);
  
  double energy1 = 0.;
  double energy2 = 0.;
  //double tmp1;
  //double tmp2;

  for (unsigned int i=0;i<mytrack->NVoxels();i++){
    vxl = mytrack->GetVoxel(i);
    bool insphere1 = false;
    bool insphere2 = false;
    if (sqrt(pow(vxl->GetPosition().x()-extr1x,2)+pow(vxl->GetPosition().y()-extr1y,2)+pow(vxl->GetPosition().z()-extr1z,2))
	< fBlobRadius){
      energy1 = energy1 + vxl->GetEDep();
      //    paolina::Voxel* newvxl = new paolina::Voxel(*vxl);
      blob1->AddVoxel(vxl);
      insphere1 = true;
    }     
    if (sqrt(pow(vxl->GetPosition().x()-extr2x,2)+pow(vxl->GetPosition().y()-extr2y,2)+pow(vxl->GetPosition().z()-extr2z,2))
	< fBlobRadius){
      energy2 = energy2 + vxl->GetEDep();
      //     paolina::Voxel* newvxl = new paolina::Voxel(*vxl);
      blob2->AddVoxel(vxl);
      insphere2 = true;
    }
    if (insphere1 == true && insphere2 == true) {
      // std::cout << "One voxel belongs to both extremes. 
      // This means that the length of the track is too small compared with the chosen fRadius. 
      // Try to choose a different fBlobRadius. 
      // This event should be dealt with in some way." << std::endl;
      problem = true;
    }
  }

  blob1->SetEnergy(energy1);
  blob2->SetEnergy(energy2);
  
  std::pair<paolina::Blob*, paolina::Blob*> myblobs = std::make_pair(blob1, blob2);
  
  return myblobs;
  
}
