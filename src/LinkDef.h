// ----------------------------------------------------------------------------
//  $Id: LinkDef.h 9556 2014-01-14 22:24:14Z gomez $
//
//  Author:  <paola.ferrario@ific.uv.es>
//  Created: 14 Feb 2013
//  
//  Copyright (c) 2013 NEXT Collaboration
// ---------------------------------------------------------------------------- 

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ namespace paolina+;
#pragma link C++ class paolina::TopoEvent+;
#pragma link C++ class paolina::Voxel+;
#pragma link C++ class std::vector<paolina::Voxel*>+;
#pragma link C++ class paolina::vector3<int>+;
#pragma link C++ class paolina::vector3<double>+;
#pragma link C++ class paolina::VoxelBuilder+;
#pragma link C++ class paolina::Track+;
#pragma link C++ class std::vector<paolina::Track*>+;
#pragma link C++ class paolina::TrackBuilder+;
#pragma link C++ class paolina::Blob+;
#pragma link C++ class paolina::BlobBuilder+;
#pragma link C++ class std::pair<paolina::Blob*, paolina::Blob*>+;

#endif
