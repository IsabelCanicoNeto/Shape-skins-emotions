#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>

// Pattern types supported:
enum pattern
{
  NONE,
  RAINBOW_CYCLE,
  THEATER_CHASE,
  COLOR_WIPE,
  SCANNER,
  FADE
};
// Patern directions supported:
enum direction
{
  FORWARD,
  REVERSE
};

// NeoPixel Class - derived from the Adafruit_NeoPixel class
class NeoPixel : public Adafruit_NeoPixel
{
public:
  // Member Variables:
  pattern ActivePattern; // which pattern is running
  direction Direction;   // direction to run the pattern

  unsigned long Interval;   // milliseconds between updates
  unsigned long lastUpdate; // last update of position

  uint32_t Color1, Color2; // What colors are in use
  uint16_t TotalSteps;     // total number of steps in the pattern
  uint16_t Index;          // current step within the pattern

  void (*OnComplete)(); // Callback on completion of pattern

  // Constructor - calls base-class constructor to initialize strip
  NeoPixel(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)())
      : Adafruit_NeoPixel(pixels, pin, type)
  {
    OnComplete = callback;
  }

  // Update the pattern
  void Update()
  {
    if ((millis() - lastUpdate) > Interval) // time to update
    {
      lastUpdate = millis();
      switch (ActivePattern)
      {
      case RAINBOW_CYCLE:
        RainbowCycleUpdate();
        break;
      case THEATER_CHASE:
        TheaterChaseUpdate();
        break;
      case COLOR_WIPE:
        ColorWipeUpdate();
        break;
      case SCANNER:
        ScannerUpdate();
        break;
      case FADE:
        FadeUpdate();
        break;
      default:
        break;
      }
    }
  }

  // Increment the Index and reset at the end
  void Increment()
  {
    if (Direction == FORWARD)
    {
      Index++;
      if (Index >= TotalSteps)
      {
        Index = 0;
        if (OnComplete != NULL)
        {
          OnComplete(); // call the comlpetion callback
        }
      }
    }
    else // Direction == REVERSE
    {
      --Index;
      if (Index <= 0)
      {
        Index = TotalSteps - 1;
        if (OnComplete != NULL)
        {
          OnComplete(); // call the comlpetion callback
        }
      }
    }
  }

  // Initialize for a RainbowCycle
  void RainbowCycle(uint8_t interval, direction dir = FORWARD)
  {
    ActivePattern = RAINBOW_CYCLE;
    Interval = interval;
    TotalSteps = 255;
    Index = 0;
    Direction = dir;
  }

  // Update the Rainbow Cycle Pattern
  void RainbowCycleUpdate()
  {
    for (int i = 0; i < numPixels(); i++)
    {
      setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
    }
    show();
    Increment();
  }

  // Initialize for a ColorWipe
  void ColorWipe(uint32_t color, uint8_t interval, direction dir = FORWARD)
  {
    ActivePattern = COLOR_WIPE;
    Interval = interval;
    TotalSteps = numPixels();
    Color1 = color;
    Index = 0;
    Direction = dir;
  }

  // Update the Color Wipe Pattern
  void ColorWipeUpdate()
  {
    setPixelColor(Index, Color1);
    show();
    Increment();
  }

  // Initialize for a Theater Chase
  void TheaterChase(uint32_t color1, uint32_t color2, uint8_t interval, direction dir = FORWARD)
  {
    ActivePattern = THEATER_CHASE;
    Interval = interval;
    TotalSteps = numPixels();
    Color1 = color1;
    Color2 = color2;
    Index = 0;
    Direction = dir;
  }

  // Update the Theater Chase Pattern
  void TheaterChaseUpdate()
  {
    for (int i = 0; i < numPixels(); i++)
    {
      if ((i + Index) % 3 == 0)
      {
        setPixelColor(i, Color1);
      }
      else
      {
        setPixelColor(i, Color2);
      }
    }
    show();
    Increment();
  }

  // Initialize for a SCANNNER
  void Scanner(uint32_t color1, uint8_t interval)
  {
    ActivePattern = SCANNER;
    Interval = interval;
    TotalSteps = (numPixels() - 1) * 2;
    Color1 = color1;
    Index = 0;
  }

  // Update the Scanner Pattern
  void ScannerUpdate()
  {
    for (int i = 0; i < numPixels(); i++)
    {
      if (i == Index) // first half of the scan
      {
        setPixelColor(i, Color1);
      }
      else if (i == TotalSteps - Index) // The return trip.
      {
        setPixelColor(i, Color1);
      }
      else // fade to black
      {
        setPixelColor(i, DimColor(getPixelColor(i)));
      }
    }
    show();
    Increment();
  }

  // Initialize for a Fade
  void Fade(uint32_t color1, uint32_t color2, uint16_t steps, uint8_t interval, direction dir = FORWARD)
  {
    ActivePattern = FADE;
    Interval = interval;
    TotalSteps = steps;
    Color1 = color1;
    Color2 = color2;
    Index = 0;
    Direction = dir;
  }

  // Update the Fade Pattern
  void FadeUpdate()
  {
    uint8_t red = ((Red(Color1) * (TotalSteps - Index)) + (Red(Color2) * Index)) / TotalSteps;
    uint8_t green = ((Green(Color1) * (TotalSteps - Index)) + (Green(Color2) * Index)) / TotalSteps;
    uint8_t blue = ((Blue(Color1) * (TotalSteps - Index)) + (Blue(Color2) * Index)) / TotalSteps;
    ColorSet(Color(red, green, blue));
    show();
    Increment();
  }

  // Returns the Red component of a 32-bit color
  uint8_t Red(uint32_t color)
  {
    return (color >> 16) & 0xFF;
  }

  // Returns the Green component of a 32-bit color
  uint8_t Green(uint32_t color)
  {
    return (color >> 8) & 0xFF;
  }

  // Returns the Blue component of a 32-bit color
  uint8_t Blue(uint32_t color)
  {
    return color & 0xFF;
  }

  // Return color, dimmed by 75% (used by scanner)
  uint32_t DimColor(uint32_t color)
  {
    uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
    return dimColor;
  }

  // Input a value 0 to 255 to get a color value.
  // The colours are a transition r - g - b - back to r.
  uint32_t Wheel(byte WheelPos)
  {
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85)
    {
      return Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else if (WheelPos < 170)
    {
      WheelPos -= 85;
      return Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    else
    {
      WheelPos -= 170;
      return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
  }

  // Reverse direction of the pattern
  void Reverse()
  {
    if (Direction == FORWARD)
    {
      Direction = REVERSE;
      Index = TotalSteps - 1;
    }
    else
    {
      Direction = FORWARD;
      Index = 0;
    }
  }

  // Set all pixels to a color (synchronously)
  void ColorSet(uint32_t color)
  {
    for (int i = 0; i < numPixels(); i++)
    {
      setPixelColor(i, color);
    }
    show();
  }
};

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

  { 12.3   , 12.3   ,12.3  ,12.3  ,12.3  ,12.3  ,12.3  ,12.3  ,12.3  ,12.3  ,12.3  ,12.3  ,12.3  ,12.3  ,12.3  ,12.3  ,12.3  ,12.3  ,12.3  ,12.3  ,12.3  ,12.3  ,12.3  ,12.3  ,12.3  },
  { 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2    },

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

  
  { 12.0   , 12.0   , 12.0   , 12.0   , 12.0   , 12.0   , 12.0   , 12.0   , 12.0   , 12.0   , 12.0   , 12.0   , 12.0   , 12.0   , 12.0   , 12.0   , 12.0   , 12.0   , 12.0   , 12.0   , 12.0   , 12.0   , 12.0   , 12.0   , 12.0   },
  { 18.7   , 18.7   , 18.7   , 18.7   , 18.7   , 18.7   , 18.7   , 18.7   , 18.7   , 18.7   , 18.7   , 18.7   , 18.7   , 18.7   , 18.7   , 18.7   , 18.7   , 18.7   , 18.7   , 18.7   , 18.7   , 18.7   , 18.7   , 18.7   , 18.7    },
  { VACUUM , VACUUM , INFLATE, INFLATE, VACUUM , VACUUM , INFLATE, INFLATE, VACUUM , VACUUM , INFLATE, INFLATE,  VACUUM, VACUUM , INFLATE, INFLATE, VACUUM , VACUUM , INFLATE, INFLATE, VACUUM , VACUUM , INFLATE, INFLATE, VACUUM },
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
  { VACUUM , VACUUM , STOP   , STOP   , INFLATE, INFLATE, VACUUM , STOP   , INFLATE, VACUUM , VACUUM , STOP   , STOP   , INFLATE, INFLATE, VACUUM , STOP   , INFLATE, VACUUM , VACUUM , STOP   , STOP   , INFLATE, INFLATE, VACUUM },
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
  { OPEN   , OPEN   , OPEN   , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 
  { OPEN   , OPEN   , OPEN   , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  ,  CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE   , CLOSE , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  , CLOSE  }, 

  
  { 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   , 12.2   },
  { 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   , 18.2   },
  { INFLATE, INFLATE, INFLATE, STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   , STOP   },
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

// PINS
#define FAN1PIN 12
#define FAN2PIN 13
#define FAN3PIN 14
#define FAN4PIN 15
#define FANBIGPIN 16
#define PIXELPIN 0
#define PIXELNUM 24

// NEOPIXEL
void test() {}
NeoPixel neoPixel = NeoPixel(PIXELNUM, PIXELPIN, NEO_GRB + NEO_KHZ800, test);

// SERVER
const char *ssid = "TextureChanging";
const char *password = "texturechanging";
WiFiServer server(80);
IPAddress apIP(41, 41, 41, 41);
IPAddress subnet(255, 255, 255, 0);

// STATE
Routine routine = alive;
bool routineRunning = false;
int startTime = millis();
int fan1 = OFF;
int fan2 = OFF;
int fan3 = OFF;
int fan4 = OFF;
int fan5 = OFF;
pixelPattern pixel = NONEE;

void setup()
{
  Serial.begin(115200);

  pinMode(FAN1PIN, OUTPUT);
  pinMode(FAN2PIN, OUTPUT);
  pinMode(FAN3PIN, OUTPUT);
  pinMode(FAN4PIN, OUTPUT);
  digitalWrite(FAN1PIN, OFF);
  digitalWrite(FAN2PIN, OFF);
  digitalWrite(FAN3PIN, OFF);
  digitalWrite(FAN4PIN, OFF);

  neoPixel.begin();
  neoPixel.setBrightness(255);
  neoPixel.show();
}

void reset()
{
  digitalWrite(FAN1PIN, OFF);
  fan1 = OFF;
  digitalWrite(FAN2PIN, OFF);
  fan2 = OFF;
  digitalWrite(FAN3PIN, OFF);
  fan3 = OFF;
  digitalWrite(FAN4PIN, OFF);
  fan4 = OFF;

  neoPixel.ColorWipe(neoPixel.Color(0, 0, 0), 0);
  pixel = NONEE;

  routineRunning = false;
}

void loop()
{
  if (Serial.available() > 0)
  {
    char cmd = Serial.read();

    if (cmd == '0' && routineRunning)
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
      Serial.println("START fear");
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
    if (delta >= 25)
    {
      reset();
      Serial.println("END");
    }
    else
    {
      // SMELL FANS
      updateFans(delta);
      // NEOPIXEL
      updatePixel(delta);
    }
  }
  neoPixel.Update();
}

void updateFans(int delta)
{
  int newFan1 = routine.fan1[delta];
  if (newFan1 != fan1)
  {
    digitalWrite(FAN1PIN, newFan1);
    fan1 = newFan1;
  }
  int newFan2 = routine.fan2[delta];
  if (newFan2 != fan2)
  {
    digitalWrite(FAN2PIN, newFan2);
    fan2 = newFan2;
  }
  int newFan3 = routine.fan3[delta];
  if (newFan3 != fan3)
  {
    digitalWrite(FAN3PIN, newFan3);
    fan3 = newFan3;
  }
  int newFan4 = routine.fan1[delta];
  if (newFan4 != fan4)
  {
    digitalWrite(FAN4PIN, newFan4);
    fan1 = newFan4;
  }
}

void updatePixel(int delta)
{
  pixelPattern newPixel = routine.pixel[delta];
  if (newPixel != pixel)
  {
    switch (newPixel)
    {
    case NONE:
      neoPixel.ColorWipe(neoPixel.Color(0, 0, 0), 50);
      break;
    case WHITE:
      neoPixel.ColorWipe(neoPixel.Color(100, 100, 100), 50);
      break;
    case GREEN:
      neoPixel.ColorWipe(neoPixel.Color(0, 100, 0), 50);
      break;
    case BLUE:
      neoPixel.ColorWipe(neoPixel.Color(0, 0, 100), 50);
      break;
    case RED:
      neoPixel.ColorWipe(neoPixel.Color(100, 0, 0), 50);
      break;
    case YELLOW:
      neoPixel.ColorWipe(neoPixel.Color(100, 100, 0), 50);
      break;
    case PURPLE:
      neoPixel.ColorWipe(neoPixel.Color(100, 0, 100), 50);
      break;
    case AWHITE:
      neoPixel.TheaterChase(neoPixel.Color(100, 100, 100), neoPixel.Color(0, 0, 0), 50);
      break;
    case AGREEN:
      neoPixel.TheaterChase(neoPixel.Color(0, 100, 0), neoPixel.Color(0, 0, 0), 50);
      break;
    case ABLUE:
      neoPixel.TheaterChase(neoPixel.Color(0, 0, 100), neoPixel.Color(0, 0, 0), 50);
      break;
    case ARED:
      neoPixel.TheaterChase(neoPixel.Color(100, 0, 0), neoPixel.Color(0, 0, 0), 50);
      break;
    case AYELLOW:
      neoPixel.TheaterChase(neoPixel.Color(100, 100, 0), neoPixel.Color(0, 0, 0), 50);
      break;
    case APURPLE:
      neoPixel.TheaterChase(neoPixel.Color(100, 0, 100), neoPixel.Color(0, 0, 0), 50);
      break;
    case RG:
      neoPixel.Fade(neoPixel.Color(100, 0, 0), neoPixel.Color(0, 100, 0), 20, 50);
      break;
    case YR:
      neoPixel.Fade(neoPixel.Color(100, 100, 0), neoPixel.Color(100, 0, 0), 20, 50);
      break;
    case YB:
      neoPixel.Fade(neoPixel.Color(100, 100, 0), neoPixel.Color(0, 0, 100), 20, 50);
      break;
    case GY:
      neoPixel.Fade(neoPixel.Color(0, 100, 0), neoPixel.Color(100, 100, 0), 20, 50);
      break;
    case RAINBOW:
      neoPixel.RainbowCycle(10);
      break;
    case SCAN:
      neoPixel.Scanner(neoPixel.Color(100, 100, 100), 50);
      break;
    }
    pixel = newPixel;
  }
}
