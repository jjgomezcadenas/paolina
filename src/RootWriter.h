#ifndef ROOT_WRITER_H
#define ROOT_WRITER_H

#include <TFile.h>
#include <TTree.h>
#include "TopoEvent.h"

namespace paolina {

  class RootWriter {
  public:
    /// Constructor
    RootWriter();
    /// Destructor
    ~RootWriter() {}
    
  private:
    
    TFile* fFile; ///< ROOT file where the output is written
    TTree* fEvtTree; ///< ROOT tree where the events are stored
    TopoEvent* fEvent; ///< event to be stored
    bool fIsOpen; ///< true is the file is already open

  public: 

    bool Open(const std::string filename, std::string option);
    void Close();
    void Write(TopoEvent& event);
    bool IsOpen();
    

  };
}

#endif
