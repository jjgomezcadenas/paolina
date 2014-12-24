#include "RootWriter.h"

#include <sys/stat.h>
#include <sstream>

namespace paolina {

  RootWriter::RootWriter()
  {
    fFile = 0;
    fEvtTree = 0;   
    fIsOpen = false;
  }

  bool RootWriter::Open(const std::string filename, std::string option)
  {
    if (fFile)
      delete fFile;

    // Check if the path exists
    std::istringstream iss(filename);
    std::vector<std::string> myline;
    std::string text;
    while (getline(iss, text, '/')) { 
      myline.push_back(text);
    }   

    std::string path;
    if (myline.size() == 1) {
      path = ".";
    }
    for (int i=0; i<myline.size()-1; ++i) {
      path.append(myline[i]);
      if (i != myline.size()-2)
	path.append("/");
    }
    
    struct stat st;
    if (stat(path.c_str(),&st) != 0)
      return false;

    std::cout << "Paolina: opening file" << std::endl;

    fEvent = 0;
    fFile = new TFile(filename.c_str(), option.c_str());
    fEvtTree = new TTree("TOPOEVENT","Paolina topoevent tree");
    TTree::SetBranchStyle(1);
    fEvtTree->Branch("TopoEventBranch","paolina::TopoEvent",&fEvent);
    fIsOpen = true;
  
    return true;
  }

   void RootWriter::Close()
  {
    if (!fFile || !fFile->IsOpen())
      return;
    
    fFile->Write();
    fFile->Close();
    std::cout << "Paolina: closing file" << std::endl;

    fIsOpen = false;
  }


   void RootWriter::Write(TopoEvent& event)
  {
    fEvent = &event;
    if (!fEvtTree)
      std::cout << "TopoEvent tree does not exist!" << std::endl;
  
    fEvtTree->Fill();
   
  }

 
  bool RootWriter::IsOpen()
  {
    return fIsOpen;
  }

}
