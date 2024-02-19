#include <Adafruit_TinyUSB.h>
#include <FlowIO.h>

// ROUTINES
#define OPEN 1
#define CLOSE 0

#define STOP 0
#define INFLATE 1
#define VACUUM 2
#define RELEASE 3

#define ON 0
#define OFF 1

enum pixelPattern
{
  NONEE,
  WHITE,
  GREEN,
  BLUE,
  RED,
  YELLOW,
  PURPLE,
  AWHITE,
  AGREEN,
  ABLUE,
  ARED,
  AYELLOW,
  APURPLE,
  WG,
  WB,
  WR,
  WY,
  WP,
  GW,
  GB,
  GR,
  GY,
  GP,
  BW,
  BG,
  BR,
  BY,
  BP,
  RW,
  RG,
  RB,
  RY,
  RP,
  YW,
  YG,
  YB,
  YR,
  YP,
  PW,
  PG,
  PB,
  PR,
  PY,
  RAINBOW,
  SCAN,
};

struct Routine
{
  int flow1[25];
  int flow2[25];
  int flow3[25];
  int flow4[25];
  int flow5[25];

  double minP[25];
  double maxP[25];
  int mode[25];
  uint8_t pwm[25];

  int fan1[25];
  int fan2[25];
  int fan3[25];
  int fan4[25];

  pixelPattern pixel[25];
};

// 1 - Alive
Routine alive = {
  { OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   }, 
  { CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  },
  { OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   }, 
  { CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  },
  { OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   }, 

  { 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   ,  12.2  , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   ,  12.2  , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   ,  12.2  , 12.2   , 12.2   , 12.2   , 12.2   },
  { 18.3   , 18.3   , 18.3   , 18.3   , 18.3   , 18.3   , 18.3   , 18.3   , 18.3   , 18.3   , 18.3   , 18.3   , 18.3   , 18.3   , 18.3   , 18.3   , 18.3   , 18.3   , 18.3   , 18.3   , 18.3   , 18.3   , 18.3   , 18.3   , 18.3   },

  { INFLATE, INFLATE, VACUUM , VACUUM , INFLATE, INFLATE, VACUUM , VACUUM , INFLATE, INFLATE, VACUUM , VACUUM , INFLATE, INFLATE, VACUUM , VACUUM , INFLATE, INFLATE, VACUUM , VACUUM , INFLATE, INFLATE, VACUUM , VACUUM , INFLATE},
  { 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    },

  { ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },

  { RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW},
  
};

// 2 - Sad
Routine sad = {
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   ,  OPEN  , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN    , OPEN  , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   ,  OPEN  , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN    , OPEN  , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   }, 

  
  { 11.5   , 11.5   , 11.5   , 11.5   , 11.5   , 11.5   , 11.5   , 11.5   , 11.5   , 11.5   , 11.5   , 11.5   , 11.5   , 11.5   , 11.5   , 11.5   , 11.5   , 11.5   , 11.5   , 11.5   , 11.5   , 11.5   , 11.5   , 11.5   , 11.5     },
  { 18.9   , 18.9   , 18.9   , 18.9   , 18.9   , 18.9   , 18.9   , 18.9   , 18.9   , 18.9   , 18.9   , 18.9   , 18.9   , 18.9   , 18.9   , 18.9   , 18.9   , 18.9   , 18.9   , 18.9   , 18.9   , 18.9   , 18.9   , 18.9   , 18.9   },
  { VACUUM , VACUUM , VACUUM , VACUUM , INFLATE, INFLATE, VACUUM , VACUUM , VACUUM , VACUUM , INFLATE, INFLATE, VACUUM , VACUUM , VACUUM , VACUUM , INFLATE, INFLATE, VACUUM , VACUUM , VACUUM , VACUUM , INFLATE, INFLATE, INFLATE},
  { 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    },

  { ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
 
  { BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   },
  
};


// 3 - Fat
Routine fat = {
  { OPEN   , OPEN   , OPEN   , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { OPEN   , OPEN   , OPEN   , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  },
  { OPEN   , OPEN   , OPEN   , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { OPEN   , OPEN   , OPEN   , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 

  
  { 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   },
  { 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   },
  { INFLATE, INFLATE, INFLATE, STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   },
  { 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     },

  { ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
 
  { BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   },
  
};




// 4 - Tense
Routine tense = {
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { OPEN   , OPEN   , CLOSE  , OPEN   , OPEN   , OPEN   , OPEN   , CLOSE  , OPEN   , OPEN   ,  OPEN  , CLOSE  , CLOSE  , OPEN   , OPEN   , OPEN   , CLOSE   , OPEN  , OPEN   , OPEN   , CLOSE  , CLOSE  , OPEN   , OPEN   , OPEN   },
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 

  
  { 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   },
  { 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   },
  { VACUUM , VACUUM , STOP   , STOP   , INFLATE, INFLATE, VACUUM , STOP   , INFLATE, VACUUM , VACUUM , STOP   , STOP   , INFLATE, INFLATE, VACUUM , STOP   , INFLATE, VACUUM , VACUUM , STOP   , STOP   , INFLATE, INFLATE, INFLATE },
   { 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     },

  { ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
 
  { PURPLE  , PURPLE, PURPLE , PURPLE , PURPLE , PURPLE  , PURPLE, PURPLE , PURPLE , PURPLE , PURPLE  , PURPLE, PURPLE , PURPLE , PURPLE , PURPLE  , PURPLE, PURPLE , PURPLE , PURPLE , PURPLE  , PURPLE, PURPLE , PURPLE , PURPLE },
  
};

// 5 - Strange 18.1?
Routine strange = {
  { OPEN   , CLOSE  , CLOSE  , CLOSE  , OPEN   , OPEN   , CLOSE  , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , CLOSE  , CLOSE  , OPEN   , OPEN   , CLOSE  , CLOSE  , OPEN   , CLOSE  , OPEN   , OPEN   , CLOSE  , CLOSE  , OPEN  }, 
  { CLOSE  , OPEN   , OPEN   , OPEN   , CLOSE  , CLOSE  , OPEN   , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , OPEN   , OPEN   , CLOSE  , CLOSE  , OPEN   , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , OPEN   , OPEN   , OPEN  },
  { OPEN   , CLOSE  , CLOSE  , CLOSE  , OPEN   , OPEN   , CLOSE  , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , CLOSE  , CLOSE  , OPEN   , OPEN   , CLOSE  , CLOSE  , OPEN   , CLOSE  , OPEN   , OPEN   , CLOSE  , CLOSE  , OPEN  }, 
  { CLOSE  , OPEN   , OPEN   , OPEN   , CLOSE  , CLOSE  , OPEN   , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , OPEN   , OPEN   , CLOSE  , CLOSE  , OPEN   , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , OPEN   , OPEN   , OPEN  },
  { OPEN   , CLOSE  , CLOSE  , CLOSE  , OPEN   , OPEN   , CLOSE  , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , CLOSE  , CLOSE  , OPEN   , OPEN   , CLOSE  , CLOSE  , OPEN   , CLOSE  , OPEN   , OPEN   , CLOSE  , CLOSE  , OPEN  }, 

  
  { 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   },
  { 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   },
  { INFLATE, INFLATE, VACUUM , VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, INFLATE, VACUUM , VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, INFLATE, VACUUM , INFLATE , INFLATE},
  { 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    },

  { ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
 
  { SCAN   , SCAN   , SCAN   , SCAN   , SCAN   , SCAN   , SCAN   , SCAN   , SCAN   , SCAN   , SCAN   , SCAN   , SCAN   , SCAN   , SCAN   , SCAN   , SCAN   , SCAN   , SCAN   , SCAN   , SCAN   , SCAN   , SCAN   , SCAN   , SCAN   },
  
};




// 6 - Relaxed
Routine relaxed = {
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   },
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   },
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   },

  { 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   },
  { 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   },
  { INFLATE, INFLATE, INFLATE, VACUUM , VACUUM , VACUUM , INFLATE, INFLATE, INFLATE, VACUUM , VACUUM , VACUUM , INFLATE, INFLATE, INFLATE, VACUUM , VACUUM , VACUUM , INFLATE, INFLATE, INFLATE, VACUUM , VACUUM , VACUUM , INFLATE},
  { 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     },
  
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
 
  { GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  },
  
};

// 7 - Anger
Routine anger = {
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   ,  OPEN  , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN    , OPEN  , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   },
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 

  
  { 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   },
  { 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   },
  { INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE},
  { 200     , 200     , 200     , 200     , 200     , 200     , 200     , 200     , 200     , 200     , 200     , 200     , 200     , 200     , 200     , 200     , 200     , 200     , 200     , 200     , 200     , 200     , 200     , 200     , 200     },

  { ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
 
  { ARED   , ARED   , ARED   , ARED   , ARED   , ARED   , ARED   , ARED   , ARED   , ARED   , ARED   , ARED   , ARED   , ARED   , ARED   , ARED   , ARED   , ARED   , ARED   , ARED   , ARED   , ARED   , ARED   , ARED   , ARED  },
  
};



// 8 - Assertive
Routine assertive = {
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 

  
  { 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   },
  { 18.7   , 18.7  , 18.7   , 18.7   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   },
  { INFLATE, INFLATE, INFLATE, STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , VACUUM   , INFLATE   },
  { 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    },

  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
 
  { WHITE  , WHITE  , WHITE  , WHITE  ,  WHITE , WHITE  , WHITE  , WHITE  , WHITE  ,  WHITE , WHITE  , WHITE  , WHITE  , WHITE  ,  WHITE , WHITE  , WHITE  , WHITE  , WHITE  ,  WHITE , WHITE  , WHITE  , WHITE  , WHITE  ,  WHITE },
  
};

// 9 - Directive
Routine directive = {
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   }, 
  
  
  { 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   },
  { 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   },
  { INFLATE, INFLATE, INFLATE, VACUUM , VACUUM , VACUUM , INFLATE, INFLATE, INFLATE, VACUUM , VACUUM , VACUUM , INFLATE, INFLATE, INFLATE, VACUUM , VACUUM , VACUUM , INFLATE, INFLATE, INFLATE, VACUUM , VACUUM , VACUUM , INFLATE},
  { 160    , 160    , 160    , 160    , 160    , 160    , 160    , 160    , 160    , 160    , 160    , 160    , 160    , 160    , 160    , 160    , 160    , 160    , 160    , 160    , 160    , 160    , 160    , 160    , 160    },

  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
 
  { WHITE  , WHITE  , WHITE  , WHITE  ,  WHITE , WHITE  , WHITE  , WHITE  , WHITE  ,  WHITE , WHITE  , WHITE  , WHITE  , WHITE  ,  WHITE , WHITE  , WHITE  , WHITE  , WHITE  ,  WHITE , WHITE  , WHITE  , WHITE  , WHITE  ,  WHITE },
  
};

// 10 - Breathing a
Routine breathing = {
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   }, 
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   }, 
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   }, 
  
  
  { 11.8   , 11.8   , 11.8   , 11.8   , 11.8   , 11.8   , 11.8   , 11.8   , 11.8 ,  11.8   , 11.8   , 11.8   , 11.8   , 11.8   , 11.8   , 11.8   , 11.8   , 11.8   , 11.8   , 11.8   , 11.8   , 11.8   , 11.8   , 11.8   , 11.8   },
  { 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   },
  { INFLATE, INFLATE, INFLATE, VACUUM , VACUUM , VACUUM , INFLATE, INFLATE, INFLATE, VACUUM , VACUUM , VACUUM , INFLATE, INFLATE, INFLATE, VACUUM , VACUUM , VACUUM , INFLATE, INFLATE, INFLATE, VACUUM , VACUUM , VACUUM , INFLATE},
  { 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     },

  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
 
  { RED    , RED    , RED    , RED    , RED    , RED    , RED    , RED    , RG     , RG     , RG     , RG     , RG     , RG     , RG     , RG     , RG     ,GREEN   , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  },
};

// 11 - Happy to Anger b
Routine happy2anger = {
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  
  
  { 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   },
  { 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   },
  { INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE},
  { 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    },

  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
 
  { YELLOW , YELLOW , YELLOW , YELLOW , YELLOW , YELLOW , YELLOW , YELLOW , YR     , YR     , YR     , YR     , YR     , YR     , YR     , YR     , YR     , RED    , RED    , RED    , RED    , RED    , RED    , RED    , RED    },
};


// 12 - Happy to Sad c
Routine happy2sad = {
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  },
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   }, 
  
  
  { 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   },
  { 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   , 18.0   },
  { INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM, INFLATE, VACUUM , VACUUM  , VACUUM , INFLATE, INFLATE, INFLATE, VACUUM , VACUUM , VACUUM , INFLATE, INFLATE, INFLATE, VACUUM , INFLATE},
  { 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     },

  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
 
  { YELLOW , YELLOW , YELLOW , YELLOW , YELLOW , YELLOW , YELLOW , YELLOW , YB     , YB     , YB     , YB     , YB     , YB     , YB     , YB     , YB     , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   , BLUE   },
};



// 13 - fear d
Routine fear = {
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   },
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 

  
  { 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   },
  { 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   },
  { INFLATE, VACUUM , STOP   , INFLATE, VACUUM , STOP   , INFLATE, VACUUM , STOP   , INFLATE, VACUUM , STOP   , INFLATE, VACUUM , STOP   , INFLATE, VACUUM , STOP   , INFLATE, VACUUM , STOP   , INFLATE, VACUUM , STOP   , INFLATE},
  { 255    , 255    , 255    , 80     , 80     , 80     , 255    , 255    , 255    , 80     , 80     , 80     , 255    , 255    , 255    , 80     , 80     , 80     , 255    , 255    , 255    , 80     , 80     , 80     , 255    },

  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
 
  { RED    , RED    , RED    , RED    , RED    , RED    , RED    , RED    , RED    , RED    , RED    , RED    , RED    , RED    , RED    , RED    , RED    , RED    , RED    , RED    , RED    , RED    , RED    , RED    , RED    },
  
};


// 14 - Stop e
Routine stop = {
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 

  
  { 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   },
  { 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   },
  { STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   },
  { 255    , 255    , 255    , 80     , 80     , 80     , 255    , 255    , 255    , 80     , 80     , 80     , 255    , 255    , 255    , 80     , 80     , 80     , 255    , 255    , 255    , 80     , 80     , 80     , 255    },

  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
 
  { NONEE  , NONEE  , NONEE  , NONEE  , NONEE  , NONEE  , NONEE  , NONEE  , NONEE  , NONEE  , NONEE  , NONEE  , NONEE  , NONEE  , NONEE  , NONEE  , NONEE  , NONEE  , NONEE  , NONEE  , NONEE  , NONEE  , NONEE  , NONEE  , NONEE  },
};



// 15 - Relaxed 2 f
Routine relaxed2 = {
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   },
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   },
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 

  
  { 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   },
  { 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   },
  { INFLATE, INFLATE, INFLATE, VACUUM , VACUUM , VACUUM , INFLATE, INFLATE, INFLATE, VACUUM , VACUUM , VACUUM , INFLATE, INFLATE, INFLATE, VACUUM , VACUUM , VACUUM , INFLATE, INFLATE, INFLATE, VACUUM , VACUUM , VACUUM , INFLATE},
  { 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     },

  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
 
  { GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  },
  
};


// 16 - Relaxed 2 Happy g
Routine relaxed2happy = {
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   }, 
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 

  
  { 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   },
  { 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   },
  { INFLATE, INFLATE, INFLATE, VACUUM , VACUUM , VACUUM , INFLATE, INFLATE, INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE},
  { 80     , 80     , 80     , 80     , 80     , 80     , 80     , 80     , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    },

  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
 
  { GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GREEN  , GY     , GY     , GY     , GY     , GY     , GY     , GY     , GY     , YELLOW , YELLOW , YELLOW , YELLOW , YELLOW , YELLOW , YELLOW , YELLOW , YELLOW },
  
};



// 17 - Happy 2 Alive h
Routine happy2alive = {
  { OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  },
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  },
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   , CLOSE  , OPEN   }, 

  
  { 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   },
  { 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   },
  { INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, VACUUM , INFLATE, INFLATE, VACUUM , VACUUM , INFLATE, INFLATE, VACUUM , VACUUM , INFLATE, INFLATE, VACUUM , VACUUM , INFLATE, INFLATE, VACUUM , VACUUM , INFLATE},
  { 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    , 255    },

  { ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
  { ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     , ON     },
  { OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    , OFF    },
 
  { YELLOW , YELLOW , YELLOW , YELLOW , YELLOW , YELLOW , YELLOW , YELLOW , RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW, RAINBOW},
  
};

// FLOWIO
FlowIO flowio;

// STATE
Routine routine = alive;
bool routineRunning = false;
int startTime = millis();
uint8_t portsState = 0b00000000;
int mode = 0;
uint8_t pwm = 0;

void setup()
{
  Serial.begin(115200);

  flowio = FlowIO(GENERAL);
  flowio.blueLED(HIGH);
  flowio.pixel(1, 1, 1);
  while (flowio.activateSensor() == false)
  {
    delay(10);
  }
}

void reset()
{
  flowio.startRelease(0b00010111);
  delay(3000);
  double p = flowio.getPressure(PSI);
  flowio.startVacuum(0b00001000);
  //delay (2000); 
  while (p > 13.5)
  {
    delay(100);
    p = flowio.getPressure(PSI);
    Serial.println(p);
    
  }
  flowio.stopAction(0b00011111);
  routineRunning = false;
}

void loop()
{
  if (Serial.available() > 0)
  {
    char cmd = Serial.read();
    if (cmd == '0')
    {
      reset();
      Serial.println("FORCED END");
    }
    else if (cmd == '1' && !routineRunning)
    {
      routine = alive;
      routineRunning = true;
      startTime = millis();
      Serial.println("START ACTIVE");
    }
    else if (cmd == '2' && !routineRunning)
    {
      routine = sad;
      routineRunning = true;
      startTime = millis();
      Serial.println("START SAD");
    }
    else if (cmd == '3' && !routineRunning)
    {
      routine = fat;
      routineRunning = true;
      startTime = millis();
      Serial.println("START FAT");
    }
    else if (cmd == '4' && !routineRunning)
    {
      routine = tense;
      routineRunning = true;
      startTime = millis();
      Serial.println("START TENSE");
    }
    else if (cmd == '5' && !routineRunning)
    {
      routine = strange;
      routineRunning = true;
      startTime = millis();
      Serial.println("START STRANGE");
    }
    else if (cmd == '6' && !routineRunning)
    {
      routine = relaxed;
      routineRunning = true;
      startTime = millis();
      Serial.println("START RELAXED");
    }
    else if (cmd == '7' && !routineRunning)
    {
      routine = anger;
      routineRunning = true;
      startTime = millis();
      Serial.println("START ANGER");
    }
    else if (cmd == '8' && !routineRunning)
    {
      routine = assertive;
      routineRunning = true;
      startTime = millis();
      Serial.println("START ASSERTIVE");
    }
    else if (cmd == '9' && !routineRunning)
    {
      routine = directive;
      routineRunning = true;
      startTime = millis();
      Serial.println("START DIRECTIVE");
    }
    else if (cmd == 'a' && !routineRunning)
    {
      routine = breathing;
      routineRunning = true;
      startTime = millis();
      Serial.println("START BREATHING");
    }
    else if (cmd == 'b' && !routineRunning)
    {
      routine = happy2anger;
      routineRunning = true;
      startTime = millis();
      Serial.println("START HAPPY TO ANGER");
    }
    else if (cmd == 'c' && !routineRunning)
    {
      routine = happy2sad;
      routineRunning = true;
      startTime = millis();
      Serial.println("START HAPPY TO SAD");
    }
    else if (cmd == 'd' && !routineRunning)
    {
      routine = fear;
      routineRunning = true;
      startTime = millis();
      Serial.println("START FEAR");
    }
    else if (cmd == 'e' && !routineRunning)
    {
      routine = stop;
      routineRunning = true;
      startTime = millis();
      Serial.println("START STOP");
    }
    else if (cmd == 'f' && !routineRunning)
    {
      routine = relaxed2;
      routineRunning = true;
      startTime = millis();
      Serial.println("START RELAXED");
    }
    else if (cmd == 'g' && !routineRunning)
    {
      routine = relaxed2happy;
      routineRunning = true;
      startTime = millis();
      Serial.println("START RELAXED TO HAPPY");
    }
    else if (cmd == 'h' && !routineRunning)
    {
      routine = happy2alive;
      routineRunning = true;
      startTime = millis();
      Serial.println("START HAPPY TO ALIVE");
    }
  }

  if (routineRunning)
  {
    int delta = floor((millis() - startTime) / 1000);
    float deltaF = (millis() - startTime) / 1000.0;
    if (delta >= 25)
    {
      Serial.println("START RESET ");
      reset();
      Serial.println("END");
    }
    else
    {
      Serial.print("Delta: ");
      Serial.println(deltaF, 4);
      // FLOWIO
      updateFLOWIO(delta);
    }
  }
}

void updatePorts(int delta)
{
  uint8_t newPortsState = routine.flow5[delta];
  newPortsState = (newPortsState << 1) | routine.flow4[delta];
  newPortsState = (newPortsState << 1) | routine.flow3[delta];
  newPortsState = (newPortsState << 1) | routine.flow2[delta];
  newPortsState = (newPortsState << 1) | routine.flow1[delta];
  if (newPortsState != portsState)
  {
    flowio.setPorts(newPortsState);
    portsState = newPortsState;
  }
}

void updateFLOWIO(int delta)
{
  delay(100);
  double p = flowio.getPressure(PSI);
  Serial.print("Pressure: ");
  Serial.println(p);
  double minP = routine.minP[delta];
  Serial.print("Min Pressure: ");
  Serial.println(minP);
  double maxP = routine.maxP[delta];
  Serial.print("Max Pressure: ");
  Serial.println(maxP);
  int newMode = routine.mode[delta];
  int pwm = routine.pwm[delta];
  uint8_t newPortsState = routine.flow5[delta];
  newPortsState = (newPortsState << 1) | routine.flow4[delta];
  newPortsState = (newPortsState << 1) | routine.flow3[delta];
  newPortsState = (newPortsState << 1) | routine.flow2[delta];
  newPortsState = (newPortsState << 1) | routine.flow1[delta];
  if (newMode != mode)
  {
    if (newMode == INFLATE)
    {
      if (p < maxP)
      {
        Serial.println("INFLATE");
        flowio.startInflation(newPortsState, pwm);
      }
      else
      {
        Serial.println("IGNORED INFLATE");
      }
    }
    else if (newMode == VACUUM)
    {
      if (p > minP)
      {
        Serial.println("VACUUM");
        flowio.startVacuum(newPortsState, pwm);
      }
      else
      {
        Serial.println("IGNORED VACUUM");
      }
    }
    else if (newMode == RELEASE)
    {
      flowio.startRelease(newPortsState);
    }
    else if (newMode == STOP)
    {
      flowio.stopAction(0b0011111);
    }
    mode = newMode;
    portsState = newPortsState;
  }
  else
  {
    if (newPortsState != portsState)
    {
      flowio.setPorts(newPortsState);
      portsState = newPortsState;
    }
    if (p >= maxP)
    {
      Serial.println("FORCED STOP");
      flowio.stopAction(portsState);
    }
  }
}
