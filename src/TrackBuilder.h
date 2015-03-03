#ifndef PTRACKBUILDER_H
#define PTRACKBUILDER_H

#include <vector>

namespace paolina {

  class Track;
  class Voxel;
  
  class TrackBuilder 
  {

  public:
    TrackBuilder() {}
    ~TrackBuilder() {}

    std::vector<paolina::Track*> IdentifyTracks(std::vector<const paolina::Voxel*> myvoxels) const;
    std::vector<paolina::Track*> IdentifyTracks(std::vector<paolina::Voxel*> myvoxels);

    // This is the method where the algorithm is actually implemented
    std::vector<paolina::Track*> BuildTracks(paolina::Track& initrack) const;

    void GraphSearch(paolina::Track& initrack, std::vector<paolina::Track*>& finaltracks) const;
    void MainPathSearch(paolina::Track& currenttrack, int extr1, int extr2) const;
  };
}

#endif
