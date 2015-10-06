#ifdef PBL_COLOR

enum {
  COLOR_THEME_CUSTOM = 0,
  COLOR_THEME_SUNSET,
  COLOR_THEME_AQUAMARINE,
  COLOR_THEME_CANDY,
  COLOR_THEME_EARTH_AND_SKY,
  COLOR_THEME_ICE_BLUE,
  COLOR_THEME_MAX
};

GColor color[COLOR_THEME_MAX][6];

void initColors() {
  // White
  color[COLOR_THEME_CUSTOM][0] = GColorWhite;
  color[COLOR_THEME_CUSTOM][1] = GColorWhite;
  color[COLOR_THEME_CUSTOM][2] = GColorWhite;
  color[COLOR_THEME_CUSTOM][3] = GColorWhite;
  color[COLOR_THEME_CUSTOM][4] = GColorWhite;
  color[COLOR_THEME_CUSTOM][5] = GColorBlack;

  // Sunset
  color[COLOR_THEME_SUNSET][0] = GColorPastelYellow;
  color[COLOR_THEME_SUNSET][1] = GColorYellow;
  color[COLOR_THEME_SUNSET][2] = GColorChromeYellow;
  color[COLOR_THEME_SUNSET][3] = GColorOrange;
  color[COLOR_THEME_SUNSET][4] = GColorRed;
  color[COLOR_THEME_SUNSET][5] = GColorBlack;

  // Aquamarine
  color[COLOR_THEME_AQUAMARINE][0] = GColorCeleste;
  color[COLOR_THEME_AQUAMARINE][1] = GColorElectricBlue;
  color[COLOR_THEME_AQUAMARINE][2] = GColorMediumSpringGreen;
  color[COLOR_THEME_AQUAMARINE][3] = GColorJaegerGreen;
  color[COLOR_THEME_AQUAMARINE][4] = GColorIslamicGreen;
  color[COLOR_THEME_AQUAMARINE][5] = GColorBlack;

  // Candy
  color[COLOR_THEME_CANDY][0] = GColorRichBrilliantLavender;
  color[COLOR_THEME_CANDY][1] = GColorShockingPink;
  color[COLOR_THEME_CANDY][2] = GColorMagenta;
  color[COLOR_THEME_CANDY][3] = GColorFashionMagenta;
  color[COLOR_THEME_CANDY][4] = GColorFolly;
  color[COLOR_THEME_CANDY][5] = GColorBlack;

  // Earth and Sky
  color[COLOR_THEME_EARTH_AND_SKY][0] = GColorVividCerulean;
  color[COLOR_THEME_EARTH_AND_SKY][1] = GColorCyan;
  color[COLOR_THEME_EARTH_AND_SKY][2] = GColorPastelYellow;
  color[COLOR_THEME_EARTH_AND_SKY][3] = GColorChromeYellow;
  color[COLOR_THEME_EARTH_AND_SKY][4] = GColorWindsorTan;
  color[COLOR_THEME_EARTH_AND_SKY][5] = GColorBlack;
  
  // Ice Blue // efdfcfcbc7c0
  color[COLOR_THEME_ICE_BLUE][0] = GColorCeleste;
  color[COLOR_THEME_ICE_BLUE][1] = GColorElectricBlue;
  color[COLOR_THEME_ICE_BLUE][2] = GColorCyan;
  color[COLOR_THEME_ICE_BLUE][3] = GColorVividCerulean;
  color[COLOR_THEME_ICE_BLUE][4] = GColorBlueMoon;
  color[COLOR_THEME_ICE_BLUE][5] = GColorBlack;
}
#else
// Pebble B/W
#define GColorFromHEX(v) GColorBlack

void initColors() {
  // Do nothing!
}

#endif

