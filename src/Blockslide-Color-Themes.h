#ifdef PBL_COLOR

enum {
  COLOR_THEME_WHITE = 0,
  COLOR_THEME_SUNSET,
  COLOR_THEME_AQUAMARINE,
  COLOR_THEME_CANDY,
  COLOR_THEME_EARTH_AND_SKY,
  COLOR_THEME_MAX
};

GColor color[COLOR_THEME_MAX][5];

void initColors() {
  // White
  color[COLOR_THEME_WHITE][0] = GColorWhite;
  color[COLOR_THEME_WHITE][1] = GColorWhite;
  color[COLOR_THEME_WHITE][2] = GColorWhite;
  color[COLOR_THEME_WHITE][3] = GColorWhite;
  color[COLOR_THEME_WHITE][4] = GColorWhite;

  // Sunset
  color[COLOR_THEME_SUNSET][0] = GColorPastelYellow;
  color[COLOR_THEME_SUNSET][1] = GColorIcterine;
  color[COLOR_THEME_SUNSET][2] = GColorRajah;
  color[COLOR_THEME_SUNSET][3] = GColorOrange;
  color[COLOR_THEME_SUNSET][4] = GColorRed;

  // Aquamarine
  color[COLOR_THEME_AQUAMARINE][0] = GColorCeleste;
  color[COLOR_THEME_AQUAMARINE][1] = GColorElectricBlue;
  color[COLOR_THEME_AQUAMARINE][2] = GColorMediumSpringGreen;
  color[COLOR_THEME_AQUAMARINE][3] = GColorJaegerGreen;
  color[COLOR_THEME_AQUAMARINE][4] = GColorIslamicGreen;

  // Candy
  color[COLOR_THEME_CANDY][0] = GColorRichBrilliantLavender;
  color[COLOR_THEME_CANDY][1] = GColorShockingPink;
  color[COLOR_THEME_CANDY][2] = GColorMagenta;
  color[COLOR_THEME_CANDY][3] = GColorFashionMagenta;
  color[COLOR_THEME_CANDY][4] = GColorFolly;

  // Earth and Sky
  color[COLOR_THEME_EARTH_AND_SKY][0] = GColorVividCerulean;
  color[COLOR_THEME_EARTH_AND_SKY][1] = GColorCyan;
  color[COLOR_THEME_EARTH_AND_SKY][2] = GColorPastelYellow;
  color[COLOR_THEME_EARTH_AND_SKY][3] = GColorRajah;
  color[COLOR_THEME_EARTH_AND_SKY][4] = GColorWindsorTan;
}
#else

#define GColorFromHEX(v) GColorBlack

void initColors() {
  // Do nothing!
}

#endif

