#include "TrackBuilder.h"
#include "Track.h"
#include "Voxel.h"

#include <iostream>
#include <TStopwatch.h>
#include <algorithm>
#include <deque>

using namespace paolina;

const int kMaxIndex = 1000000000;
const double kMaxDistance = 1.E9;

const bool debug = false; // if one wants to follow the steps of the algorithms, just set this to true.

std::vector<paolina::Track*> TrackBuilder::IdentifyTracks(std::vector<const paolina::Voxel*> myvoxels) const
{

  paolina::Track initrack(myvoxels);
  return BuildTracks(initrack);       

}


std::vector<paolina::Track*> TrackBuilder::IdentifyTracks(std::vector<paolina::Voxel*> myvoxels)
{
  
  paolina::Track initrack(myvoxels);
  return BuildTracks(initrack);

}

std::vector<paolina::Track*> TrackBuilder::BuildTracks(paolina::Track& initrack) const
{
  
  std::vector<paolina::Track*> finaltracks;
 
  if (initrack.NVoxels() == 0) {
    std::cerr << 
      "[TrackBuilder::BuildTracks()]: ERROR: this event has 0 voxels."
	      << " An empty std::vector of paolina::Track* is being returned."<< std::endl; 
    return finaltracks;
  } 
  
  if (initrack.NVoxels() == 1){
    initrack.SetLength(0.);
    initrack.SetExtremes(0,0);
    initrack.AddMainPathVoxel(initrack.GetVoxel(0));
    paolina::Track* onevoxeltrack = new paolina::Track(initrack);
    finaltracks.push_back(onevoxeltrack);
    return finaltracks;
  } 

  // Identify how many tracks are hidden in the whole set of voxels 
  // and calculate some interesting properties such as length, extremes...
  
  GraphSearch(initrack, finaltracks);
  
  return finaltracks;
}

void TrackBuilder::GraphSearch(paolina::Track& initrack, std::vector<paolina::Track*>& finaltracks) const
{
  // Build the matrix that tells you which voxels are neighbours and with which distance
  std::vector < std::vector <double> > adjacency;
  adjacency = initrack.AdjMatr();

  std::vector<const paolina::Voxel*> onetrackVoxels;

  std::vector <int> queue;
  std::vector <int> neighbs;
  int start;
  int enqueued[initrack.NVoxels()];
  int current;
  
  for(unsigned int i=0;i<initrack.NVoxels();i++){
    enqueued[i] = 0;
  }

  bool end = false;
  unsigned int it = 0;

  while (!end){
    if (it == initrack.NVoxels()){
      if (debug) std::cout << "All voxels visited. Tracks formed."<<std::endl;
      break;
    } else {
      if (enqueued[it] != 0){
	it = it +1;
      } else {
	start = it;
	it = it + 1;
	const paolina::Voxel* vxl1 = new const paolina::Voxel;
	vxl1 =  initrack.GetVoxel(start);
	onetrackVoxels.push_back(vxl1);
	
	queue.push_back(start);
	enqueued[start] = 1;
	
	bool finished = false;

	/// Identify connected voxels to form a track
	while (!finished){
	  if (queue.size() == 0){
	    break;
	  } 
	  current = queue.front();
	  queue.erase(queue.begin());
      
	  neighbs.clear();
	  for (unsigned int nb=0;nb<adjacency.size();++nb){
	    if (adjacency[current][nb] != 0){
	      neighbs.push_back(nb); 
	    }
	  }
	  
	  for (unsigned int j=0;j<neighbs.size();j++){
	    if (enqueued[neighbs[j]] == 0){ 
	      const paolina::Voxel* vxl = new const paolina::Voxel;
	      vxl = initrack.GetVoxel(neighbs[j]);
	      onetrackVoxels.push_back(vxl);
	      queue.push_back(neighbs[j]);
	      enqueued[neighbs[j]] = 1;
	    }
	  }
	  
	} // exit from while(!finished)

	/// Now we have a single track: let's characterize it, finding the extremes,
	/// the lengths...
	paolina::Track* tracksingle = new paolina::Track(onetrackVoxels);

	std::vector < std::vector <double> > singleadj = tracksingle->AdjMatr();  

	if (singleadj.size() > 1) {

	  // Breadth First Search algorithm: distance between two points.
	
	  std::vector <int> queuesingle;
	  std::vector <int> neighbssingle;
	  bool foundsingle;
	  int startsingle;
	  int goalsingle;
	  int examined[tracksingle->NVoxels()];
	  double pathsingle[tracksingle->NVoxels()];
	  int currentsingle;


	  double var = 0;
	
	  double length = 0.;
	  int vox1 = kMaxIndex;
	  int vox2 = kMaxIndex;

	  for (unsigned int i=0; i<tracksingle->NVoxels(); i++){
	    for (unsigned int j=0; j<tracksingle->NVoxels(); j++){       
	      if (i < j){ /// we don't want to calculate the distance between the same pair of voxels twice
		startsingle = i;
		goalsingle = j;
		if (debug) std::cout << "Start: " << i << ", goal: " << j << std::endl;
		currentsingle = kMaxIndex;
		for (unsigned int k=0; k<tracksingle->NVoxels(); k++){
		  pathsingle[k] = kMaxDistance; examined[k] = 0;
		}
	        
		neighbssingle.clear();
		queuesingle.clear();
		pathsingle[startsingle] = 0.;
		examined[startsingle] = 1;
	        
		queuesingle.push_back(startsingle);
	        
		foundsingle = false;
	        
		while (!foundsingle){
		  if (queuesingle.size() == 0){
		    if (debug) std::cout << "No voxels in queue" << std::endl;
		    break;
		  } 
		  currentsingle = queuesingle.front();
		
		  if (debug) std::cout << "current = " << currentsingle << std::endl;
		  if (currentsingle == goalsingle){
		    foundsingle = true;
		    if (debug) {
		      std::cout<< "goal found!: "<<"between "<< startsingle << " and " << currentsingle 
			       << " the distance is "<< pathsingle[currentsingle] <<std::endl;
		    }
		    if (pathsingle[currentsingle] > length){
		      length = pathsingle[currentsingle];	       
		      vox1 = i;
		      vox2 = j;
		    }
		 
		    break;
		  } else {
		    if (debug) {
		      std::cout << "**** Current queue ***" << std::endl;
		      for (int ll=0;ll<queuesingle.size(); ++ll) {
		        std::cout << queuesingle[ll] << ", ";
		      }
		      std::cout << "Erasing " << *queuesingle.begin() << " from the queue" << std::endl;
		    }
		    queuesingle.erase(queuesingle.begin());

		    examined[currentsingle] = 1;

		    neighbssingle.clear();
		    for (unsigned int nb=0; nb<singleadj.size(); ++nb){
		      if (singleadj[currentsingle][nb] != 0){
			neighbssingle.push_back(nb); 
		      }
		    }
		    if (debug) {
		      std::cout<<std::endl;
		      std::cout << "neighbs = ";
		      for (int l=0;l<neighbssingle.size();l++) {
			std::cout << neighbssingle[l] << ", ";
		      }
		      std::cout << std::endl;
		    }
		    for (unsigned int l=0; l<neighbssingle.size(); l++){
		      if (debug) {
			std::cout << "examined[" << neighbssingle[l] << "] = " << examined[neighbssingle[l]] << std::endl;
		      }
		      if (examined[neighbssingle[l]] == 0){ 
			var = pathsingle[currentsingle] + singleadj[currentsingle][neighbssingle[l]];
			if (var < pathsingle[neighbssingle[l]]){
			  pathsingle[neighbssingle[l]] = var;
			  if (debug) {
			    std::cout << "Path of " << neighbssingle[l] << " is " << pathsingle[neighbssingle[l]] << std::endl;
			  }
			}
			if (debug) {
			  std::cout << "Path of " << neighbssingle[l] << " is " << pathsingle[neighbssingle[l]] << std::endl;
			}
			if (std::find(queuesingle.begin(), queuesingle.end(), neighbssingle[l]) == queuesingle.end()) {
			  if (debug) std::cout << neighbssingle[l] << " is not in queue" << std::endl;
			  queuesingle.push_back(neighbssingle[l]);  
			}
		      } 
		    } //for
		    if (debug) {
		      std::cout << "queue = ";
		      for (int l=0;l<queuesingle.size();l++) {
			std::cout << queuesingle[l] << ", ";
		      }
		      std::cout << std::endl;
		    }
		  } // else
		} 	      
	      } // end if i<j
   
	    }
	  }
	
	  tracksingle->SetLength(length);
	  tracksingle->SetExtremes(vox1, vox2);

	  MainPathSearch(*tracksingle, vox1, vox2);

	} else {
	  tracksingle->SetLength(0.);
	  tracksingle->SetExtremes(0,0);
	  tracksingle->AddMainPathVoxel(tracksingle->GetVoxel(0));
	}

	finaltracks.push_back(tracksingle);

	onetrackVoxels.clear();	

      } // exit from else 
    } // exit from else (if there are voxels left)
  
  }
}

void TrackBuilder::MainPathSearch(paolina::Track& currenttrack, int extr1, int extr2) const
{
  if (debug) {
    std::cout << std::endl;
    std::cout << "*** Looking for the main path between the extremes ***" << std::endl;
    std::cout << std::endl;
  }
  std::vector < std::vector <double> > adj = currenttrack.AdjMatr();  

  // BFS algorithm: distance between two points.
	
  std::vector <int> queue;
  std::vector <int> neighbs;
  bool found = false;
  int start = extr1;
  int goal = extr2;
  int current = kMaxIndex;
  int examined[currenttrack.NVoxels()]; // It's 0 if a voxel has not been examines as current, 1 if it has.
  double path[currenttrack.NVoxels()]; // It's the shortest distance between a voxel and the starting voxel. 
                                       // It gets updated along the algorithm.
  
  for (int k=0; k<currenttrack.NVoxels(); ++k) {
    path[k] = kMaxDistance; examined[k] = 0;
  }
 
  double var = 0;

  // Initialize the set of main paths (one for each voxel), 
  // meaning the voxels in the shortest path from the starting point to the voxel. 
  // We need to keep track of all the shortest paths from the initial voxel to every other voxel 
  // in the track, because the path of each voxel is calculated starting from the path of the previous one.
  // (This is why there is a clear before adding new items to a mainpath).

  std::vector<std::vector<const paolina::Voxel*> > mainpaths;
  for (unsigned int mps=0; mps<currenttrack.NVoxels(); ++mps) {
    std::vector<const paolina::Voxel*> mainvoxels;
    mainvoxels.push_back(currenttrack.GetVoxel(extr1));
    mainpaths.push_back(mainvoxels);
  }
  
  neighbs.clear();
  queue.clear();
  path[start] = 0.;
  examined[start] = 1;
  
  queue.push_back(start);
	        
  while (!found){
    if (queue.size() == 0){
      break;
    } 
    current = queue.front();
    if (current == goal){
      found = true;
      break;
    } else {
      queue.erase(queue.begin());

      examined[current] = 1;

      neighbs.clear();
      for (unsigned int nb=0;nb<adj.size();++nb){
	if (adj[current][nb] != 0){
	  neighbs.push_back(nb); 
	}
      }
      if (debug) {
	std::cout << "Current voxel = "<< current << ", its path is " << path[current] << std::endl;
      }
      for (unsigned int l=0;l<neighbs.size();l++){
	if (examined[neighbs[l]] == 0){ 
	  var = path[current] + adj[current][neighbs[l]];
	  if (debug) {
	    std::cout << "Neighbour " << neighbs[l] << " distant "<< adj[current][neighbs[l]]  << " from current."<< std::endl;
	    std::cout << "Its path is " << path[neighbs[l]] << " and the proposed new one is "<< var << std::endl;
	  }
	  if (var < path[neighbs[l]]){
	    if (debug) std::cout << "Updating path of " << neighbs[l] << " to " << var << std::endl;
	    path[neighbs[l]] = var;
	    if (debug) std::cout<< "Clearing main path of " << neighbs[l] << std::endl;
	    mainpaths[neighbs[l]].clear();
	    for (unsigned int k=0; k<mainpaths[current].size(); ++k) {
	      mainpaths[neighbs[l]].push_back(mainpaths[current][k]);
	      if (debug) {
		std::cout << "Adding voxel (" << mainpaths[current][k]->GetPosition().x() 
			  << ", "<< mainpaths[current][k]->GetPosition().y() << ") to the main path of voxel "
			  << neighbs[l] << std::endl;
	      }
	    }
	    if (debug) std::cout << "Adding itself to the main path of voxel "<< neighbs[l] << std::endl;
	    mainpaths[neighbs[l]].push_back(currenttrack.GetVoxel(neighbs[l]));
	    if (debug) {
	      std::cout << "Main path of " << neighbs[l] << " is composed by ";
	      for (int b=0; b<mainpaths[neighbs[l]].size(); ++b ) {
		std::cout << "(" << mainpaths[neighbs[l]][b]->GetPosition().x() 
			  << ", " << mainpaths[neighbs[l]][b]->GetPosition().y() << ")";
	      }
	      std::cout << std::endl;
	    }
	  }
	  if (std::find(queue.begin(), queue.end(), neighbs[l]) == queue.end()) {
	    queue.push_back(neighbs[l]);  
	  }
	} 
      } //for
		  
    } // else
  } 
  for (int i=0; i<mainpaths[extr2].size(); ++i) {
    currenttrack.AddMainPathVoxel(mainpaths[extr2][i]);
  }
  
}



