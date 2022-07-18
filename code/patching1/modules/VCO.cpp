#include "VCO.h"

VCO::VCO() {

}

void VCO::update() {
  for(int i=0; i<4; i++) {
    _osc[i].Process();
  }
}