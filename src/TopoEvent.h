#ifndef TOPO_EVENT_H
#define TOPO_EVENT_H

#include "Voxel.h"
#include "Track.h"
#include "Blob.h"

#include <iostream>
#include <TObject.h>
#include <TObjArray.h>

namespace paolina {

  class TopoEvent: public TObject  {

  public: 
    /// Constructor
    TopoEvent();
    TopoEvent(int id);
    /// Copy Constructor
    TopoEvent(const TopoEvent& right);
    /// Destructor
    ~TopoEvent();
    
  private:

    int fEventID; ///< unique identificative number
    TObjArray* fVoxels; ///< voxels of the event    
    TObjArray* fTracks; ///< separated sets of voxels
    TObjArray* fBlobs; ///< 
   
  public: 

    void Clear();

    TopoEvent& operator=(const TopoEvent& right);
   
    void AddVoxel(Voxel* vxl);
    const TObjArray* GetVoxels() const {return fVoxels;}
    
    void AddTrack(Track* trck);
    const TObjArray* GetTracks() const {return fTracks;}
    
    void AddBlob(Blob* blb);  
    const TObjArray* GetBlobs() const {return fBlobs;}
   
    void SetID(int id) {fEventID = id;}
    int GetID() const {return fEventID;}

    void Info(std::ostream& s) const;

    ClassDef(TopoEvent,1);
  };
}

std::ostream& operator << (std::ostream& s, const paolina::TopoEvent& ev);

#endif
