#ifndef TRACK_H
#define TRACK_H

#include <TObject.h>
#include <TVector3.h>
#include <TRefArray.h>
#include <vector>
#include <string>
#include <map>

#include "Voxel.h"

namespace paolina {

  class Blob;
 
  /// A Track is formed from a collection of paolina::Voxel

  class Track : public TObject
  {
  public:
    Track();
    Track(std::vector<const paolina::Voxel*>& voxvec);
    Track(std::vector<paolina::Voxel*>& voxvec);
    ~Track();

    void DisplayInfo(std::ostream& s) const;

  protected:
    // data members

    /// vector of pointers to paolina::Voxel forming paolina::Track
    TRefArray voxels_; 

    /// vector of pointers to paolina::Blobs
    TRefArray blobs_;

    /// vector of pointers to paolina::Blobs
    TRefArray mainPathVoxels_;
    
    // Properties of Track
    std::pair<int,int> extremes_;
    double length_;


  public: 
    
    // deal with associated Voxels
    int NVoxels() const;
    int NMainPathVoxels() const;
    const paolina::Voxel* GetVoxel(int i) const;
    const paolina::Voxel* GetMainPathVoxel(int i) const;
    bool AddVoxel(const paolina::Voxel* vox);
    bool AddMainPathVoxel(const paolina::Voxel* vox);
    
    int NBlobs() const;
    const paolina::Blob* GetBlob(int i) const;
    bool AddBlob(const paolina::Blob* blob);

    // Getters
    double GetLength() const {return length_;} ///< Track length
    std::pair<int,int> GetExtremes() const {return extremes_;} ///< end-voxels

    double GetEDep() const; ///< track (voxel-type) deposited energy (MeV)

    std::vector <std::vector <double> > AdjMatr() const; /// adjacency matrix (case of voxels)
    std::vector <int> closeness (int k, std::vector< std::vector <double> >& a) const;

    // Setters
    void SetExtremes(int vox1, int vox2) { extremes_.first = vox1; extremes_.second = vox2; }
    void SetLength(double value) { length_ = value; }
     

    ClassDef(Track, 1)
      
      };
}

std::ostream& operator << (std::ostream& s, const paolina::Track& tr);


#endif // TRACK_H
