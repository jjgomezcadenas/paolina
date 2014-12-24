#include "Voxel.h"
#include <string>
#include <iostream>
#include <iomanip>

using namespace paolina;

ClassImp(paolina::Voxel);

//----------------------------------------------------------------------

Voxel::Voxel() 
  : eDep_(-1.)
{
  SetBinPosition(-1, -1, -1);
  SetPosition(-1000., -1000., -1000.);

  size_.setx(-1.);
  size_.sety(-1.);
  size_.setz(-1.);
  
}

Voxel::Voxel(int i, int j, int k, double edep)
  : eDep_(edep)
{
  SetBinPosition(i, j, k);
  SetPosition(-10000., -10000., -10000.);
  size_.setx(-1.);
  size_.sety(-1.);
  size_.setz(-1.);
}


Voxel::Voxel(int i, int j, int k, double x, double y, double z, double edep)
  : eDep_(edep)
{
  SetBinPosition(i, j, k);
  SetPosition(x, y, z);
  size_.setx(-1.);
  size_.sety(-1.);
  size_.setz(-1.);
}


Voxel::~Voxel()
{
}

void Voxel::DisplayInfo(std::ostream& s) const
{
  s  << " x (mm)    y (mm)    z (mm)    " << std::endl;
  s << std::setw(5) << this->GetPosition().x() <<"     " 
    << std::setw(5) << this->GetPosition().y() <<"     " 
    << std::setw(5) << this->GetPosition().z() << std::endl;
  s << "deposited energy = "<< this->GetEDep() << " MeV"<< std::endl;
  // s << "--------------------------------------" << std::endl;  
}


std::ostream& operator << (std::ostream& s, const paolina::Voxel& vx) {
  vx.DisplayInfo(s);
  return s; 
}



