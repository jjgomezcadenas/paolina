#ifndef VOXEL_H
#define VOXEL_H

#include <TObject.h>
#include <TVector3.h>


namespace paolina {

  template <class T>
    struct vector3 {
    private:
      T xcoord;
      T ycoord;
      T zcoord;
      
    public:
      T x() const {return xcoord;}
      T y() const {return ycoord;}
      T z() const {return zcoord;}

      void setx(T x1) {xcoord = x1;}
      void sety(T y1) {ycoord = y1;}
      void setz(T z1) {zcoord = z1;}
      void setxyz(T x1, T y1, T z1) {xcoord = x1; ycoord = y1; zcoord = z1;}
  };

  /// A Voxel represents an energy deposition within a cube in the space of the chamber, 
  /// with full 3D spatial information and an energy deposition.
  class Voxel : public TObject
    {

    public:
      /// Default constructor
      Voxel();

      /// Construct a Voxel with position and energy information, 
      Voxel(int i, int j, int k, double edep);
      Voxel(int i, int j, int k, double x, double y, double z, double edep);

      ~Voxel();

      void DisplayInfo(std::ostream& s) const;

    private:
      paolina::vector3<int> binPos_;  ///< (x,y,z) spatial location of deposition (# of the bin)    
      paolina::vector3<double> pos_; ///< (x,y,z) spatial location of deposition (in absolute coordinates)

      double eDep_; ///< energy deposition of this voxel
      paolina::vector3<double> size_; ///< size of voxel

    public:

      // Getters
      const paolina::vector3<int> GetBinPosition() const {return binPos_;}
      const paolina::vector3<double> GetPosition() const {return pos_;}

      double GetEDep() const { return eDep_; }
      const paolina::vector3<double> GetSize() const {return size_;}

      // Setters
      void SetBinPosition(int i, int j, int k) 
      { binPos_.setx(i); binPos_.sety(j); binPos_.setz(k); } 
      void SetPosition(double x, double y, double z) 
      { pos_.setx(x); pos_.sety(y); pos_.setz(z); } 

      void SetEDep(double edep){ eDep_ = edep;} 
      void SetSize(double size_x, double size_y, double size_z) 
      {	size_.setx(size_x); size_.sety(size_y); size_.setz(size_z); }
      
    

      ClassDef(Voxel, 1) // Voxel
    };
}

std::ostream& operator << (std::ostream& s, const paolina::Voxel& vx);

#endif // VOXEL_H

////////////////////////////////////////////////////////////////////////
