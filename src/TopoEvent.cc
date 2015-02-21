#include "TopoEvent.h"

ClassImp(paolina::TopoEvent)

namespace paolina {

  TopoEvent::TopoEvent() : fEventID(0)
  {
    fVoxels = 0;
    fTracks = 0;
    fBlobs = 0;
  }

  TopoEvent::TopoEvent(int id) : fEventID(id)
  {
    fVoxels = new TObjArray();
    fTracks = new TObjArray();
    fBlobs = new TObjArray();
  }

  TopoEvent::TopoEvent(const TopoEvent& right)
  {
    *this = right;
  }

  TopoEvent& TopoEvent::operator=(const TopoEvent& right) 
  {
    if (this != &right) {
      fVoxels = (TObjArray*)right.fVoxels->Clone();
      fTracks = (TObjArray*)right.fTracks->Clone();
      fBlobs = (TObjArray*)right.fBlobs->Clone();
      fEventID = right.fEventID;
    }
   
    return *this;
  }

  TopoEvent::~TopoEvent()
  {
   
    ClearTopoEvent();

    delete fTracks;
    delete fVoxels;
    delete fBlobs;
  }

  void TopoEvent::ClearTopoEvent()
  {
    if (fVoxels) {
      fVoxels->Delete();
    }	
    if (fTracks) {
      fTracks->Delete(); 
    }
    if (fBlobs) {
      fBlobs->Delete(); 
    }
    
    fEventID = 0;
  }
 
  void TopoEvent::AddVoxel(Voxel* vxl)
  {
    fVoxels->AddLast(vxl);
  }
  
  void TopoEvent::AddTrack(Track* trck)
  {
    fTracks->AddLast(trck);
  }
  
  void TopoEvent::AddBlob(Blob* blb)
  {
    fBlobs->AddLast(blb);
  }
  
  void TopoEvent::PInfo(std::ostream& s) const
  {
    
    s << std::endl; 
    s << " ------------------ " << std::endl;  
    s << "| Event number = " << GetID() << " |" << std::endl;
    s << " ------------------ " << std::endl;  
    s << std::endl;
    int nvoxels = fVoxels->GetLast()+1;
    int ntracks = fTracks->GetLast()+1;
  
    s << "Event has " << nvoxels << " voxels" << std::endl;
    s << "Event has " << ntracks << " tracks" << std::endl;
    s << std::endl;
   
    
    s << "---------- Tracks in the event ----------"
      << std::endl;
    s << std::endl;   

    for (unsigned int itrack=0; itrack<fTracks->GetLast()+1; ++itrack) {
      Track* mytrack = (Track*)fTracks->At(itrack);
      s << "Track number " << itrack << std::endl;
      s << std::endl;
      s << *mytrack <<std::endl;
    }
   
  }
  
} // end namespace paolina

std::ostream& operator << (std::ostream& s, const paolina::TopoEvent& ev) {
  ev.PInfo(s);
  return s; 
}
