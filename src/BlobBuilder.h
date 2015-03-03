#ifndef PBLOB_BUILDER_H
#define PBLOB_BUILDER_H

#include <utility>

namespace paolina {

  class Blob;
  class Track;
  
  class BlobBuilder
  {
  public:
    BlobBuilder();
    BlobBuilder(double radius);
    ~ BlobBuilder() {}
    
    std::pair<paolina::Blob*, paolina::Blob*> MakeBlobs(paolina::Track* mytrack);
    std::pair<paolina::Blob*, paolina::Blob*> MakeBlobs(const paolina::Track* mytrack);
    
  private:
    double fBlobRadius;
    
  };
  
}

#endif
