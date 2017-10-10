// DistinctColors.h

#ifndef DISTINCTCOLORS_H

#define DISTINCTCOLORS_H

#include <stdint.h>

class DistinctColors {
public:
  DistinctColors();
  ~DistinctColors();
  uint32_t color(uint16_t n) const {
    return n<N ? data[n] : 0;
  }
private:
  uint32_t *data;
  int N;
};

#endif
