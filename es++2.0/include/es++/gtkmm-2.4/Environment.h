#pragma once

#include <stdlib.h>

namespace Es {

class Environment {

public:
  Environment(int argc=0, char** argv=NULL )
  {
    //For future extension
    setenv("UBUNTU_MENUPROXY", "0", 1);
  }
};

}

