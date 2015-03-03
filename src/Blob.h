#ifndef PBLOB_H
#define PBLOB_H

#include "TObject.h"
#include "Track.h"

namespace paolina {
  
  class Blob: public paolina::Track {

  public:
    Blob() {}
    Blob(double radius);
    ~Blob() {}

    void DisplayInfo(std::ostream& s) const;

    // Getters
    double GetRadius() const {return radius_;}
    double GetEnergy() const {return energy_;}
    // Setters
    void SetRadius(double value) {radius_ = value;}
    void SetEnergy(double value) {energy_ = value;}

  private:
    double radius_;
    double energy_;

    ClassDef(Blob, 1) // Blob
  };
}

std::ostream& operator << (std::ostream& s, const paolina::Blob& tr);

#endif
