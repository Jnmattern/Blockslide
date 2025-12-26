#include <pebble.h>

// Watchapp version retrieval
#include "pebble_process_info.h"
extern const LegacyPebbleAppInfo __pbl_app_info;

#include "Blockslide.h"
#include "Blockslide-Color-Themes.h"

/*
 * 2014.06.18: Round Corners option implementation thanks to Ron64
 */

// Languages
enum {
  LANG_DUTCH =  0,
  LANG_ENGLISH,
  LANG_FRENCH,
  LANG_GERMAN,
  LANG_SPANISH,
  LANG_ITALIAN,
  LANG_RUSSIAN,
  LANG_SWEDISH,
  LANG_MAX
};

enum {
  CONFIG_KEY_DATEORDER = 10,
  CONFIG_KEY_WEEKDAY = 11,
  CONFIG_KEY_LANG = 12,
  CONFIG_KEY_STRIPES = 13,
  CONFIG_KEY_ROUNDCORNERS = 14,
  CONFIG_KEY_FULLDIGITS = 15,
  CONFIG_KEY_BATTERY = 16,
  CONFIG_KEY_BLUETOOTH = 17,
  CONFIG_KEY_COLORTHEME = 18,
  CONFIG_KEY_THEMECODE = 21,
  CONFIG_KEY_INVERT = 22,
  CONFIG_KEY_MIRROR = 23,
  CONFIG_KEY_SPLASH = 24
};

#define DIGIT_CHANGE_ANIM_DURATION 800
#define STARTDELAY 700
#define BATTERYDELAY 5000

#if defined(PBL_ROUND)

#define HSPACE 6
#define DHSPACE 4
#define VSPACE 6
#define TILEW 20
#define TILEH 11
#define DTILEW 4
#define DTILEH 3

#define SCREENW 180
#define SCREENH 180
#define CX 90
#define CY 90

#else

#define HSPACE 8
#define DHSPACE 4
#define VSPACE 8
#define TILEW 22
#define TILEH 13
#define DTILEW 5
#define DTILEH 4

#define SCREENW 144
#define SCREENH 168
#define CX 72
#define CY 84

#endif

#define NUMSLOTS 12
#define TILECORNERRADIUS 4
#define DTILECORNERRADIUS 1


char weekDay[LANG_MAX][7][4] = {
  { "ZO", "MA", "DI", "WO", "DO", "VR", "ZA" },  // Dutch
  { "SU", "MO", "TU", "WE", "TH", "FR", "SA" },  // English
  { "DI", "LU", "MA", "ME", "JE", "VE", "SA" },  // French
  { "SO", "MO", "DI", "MI", "DO", "FR", "SA" },  // German
  { "DO", "LU", "MA", "MI", "JU", "VI", "SA" },  // Spanish
  { "DO", "LU", "MA", "ME", "GI", "VE", "SA" },  // Italian
  { "BC", "\x62H", "BT", "CP", "\x63T", "\x62T", "C\x61" },  // Russian
  { "SO", "MA", "TI", "ON", "TO", "FR", "LO" }  // Swedish
};

int curLang = LANG_ENGLISH;
int showWeekday = 0;
int USDate = 1;
int stripedDigits = 1;
int roundCorners = 1;
int fullDigits = 0;
int batteryStatus = 1;
int bluetoothStatus = 1;
int invertStatus = 0;
int colorTheme = 1;
static char themeCodeText[20] = "ffffffffffc0";
int mirror = 0;
int splash = 1;

bool digitShapesChanged = false;

bool doanim = true;

typedef struct {
  Layer *layer;
  int num;
  int prevDigit;
  int curDigit;
  int tileWidth;
  int tileHeight;
  uint32_t normTime;
  int cornerRadius;
} digitSlot;

Window *window;
Layer *mainLayer;
Layer *rootLayer;

digitSlot slot[NUMSLOTS]; // 4 big digits for the hour, 6 small for the date
int startDigit[NUMSLOTS] = {
  SPACE_L,
  SPACE_R,
  SPACE_L,
  SPACE_R,
  SPACE_D,
  SPACE_D,
  SPACE_D,
  SPACE_D,
  SPACE_D,
  SPACE_D,
  SPACE_D,
  SPACE_D
};

bool splashEnded = false;
bool animRunning = false;
bool lastBluetoothStatus = true;
static AppTimer *timer;

AnimationImplementation animImpl;
Animation *anim = NULL;

char buffer[256] = "";

GRect slotFrame(int i) {
  int x, y;
  int w = slot[i].tileWidth*3;
  int h = slot[i].tileHeight*5;
  static int offsetY = (SCREENH-2*(VSPACE+5*TILEH)-5*DTILEH)/2;

  if (i<4) {
    // Hour slot -> big digits
    if (i%2) {
      x = CX + HSPACE/2; // i = 1 or 3
    } else {
      x = CX - HSPACE/2 - w; // i = 0 or 2
    }

    if (i<2) {
      y =  offsetY;
    } else {
      y = offsetY + VSPACE + h;
    }
  } else {
    // Date slot -> small digits
    x = (SCREENW-(14+8*w))/2 + (i-4)*2 + (i-4)*w;
    y = offsetY + VSPACE + DHSPACE + 10*TILEH;
  }

  return GRect(x, y, w, h);
}

digitSlot *findSlot(Layer *layer) {
  int i;
  for (i=0; i<NUMSLOTS; i++) {
    if (slot[i].layer == layer) {
      return &slot[i];
    }
  }

  return NULL;
}

void updateMainLayer(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "mainLayer (%d,%d)/(%d,%d)", bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h);
  
#ifdef PBL_COLOR
  graphics_context_set_fill_color(ctx, color[colorTheme][5]);
#else
  if (invertStatus) {
    graphics_context_set_fill_color(ctx, GColorWhite);
  } else {
    graphics_context_set_fill_color(ctx, GColorBlack);
  }
#endif
  graphics_fill_rect(ctx, GRect(0, 0, bounds.size.w, bounds.size.h), 0, GCornerNone);
}

void updateSlot(Layer *layer, GContext *ctx) {
  digitSlot *slot = findSlot(layer);
  if (!slot) return;

  // Cache commonly used values to avoid repeated pointer dereferencing
  const int tileW = slot->tileWidth;
  const int tileH = slot->tileHeight;
  const int cornerR = slot->cornerRadius;
  const uint32_t normTime = slot->normTime;
  const int curDigit = slot->curDigit;
  const int prevDigit = slot->prevDigit;
  
  // Pre-calculate normalized progress once
  bool isFinished = (normTime == ANIMATION_NORMALIZED_MAX);
  unsigned int animMiddle = ANIMATION_NORMALIZED_MAX / 2;

  for (int t = 0; t < 13; t++) {
    int ox, oy;
    
    // 1. Efficient Position Calculation
    if (isFinished || (digits[curDigit][t][0] == digits[prevDigit][t][0] && 
                       digits[curDigit][t][1] == digits[prevDigit][t][1])) {
      ox = digits[curDigit][t][0] * tileW;
      oy = digits[curDigit][t][1] * tileH;
    } else {
      int tx1 = digits[prevDigit][t][0] * tileW;
      int tx2 = digits[curDigit][t][0] * tileW;
      int ty1 = digits[prevDigit][t][1] * tileH;
      int ty2 = digits[curDigit][t][1] * tileH;

      ox = (int)(normTime * (tx2 - tx1) / ANIMATION_NORMALIZED_MAX) + tx1;
      oy = (int)(normTime * (ty2 - ty1) / ANIMATION_NORMALIZED_MAX) + ty1;
    }

    // 2. Optimized Corner Logic
    GCornerMask cornerMask = GCornerNone;
    int currentR = 0;
    
    if (roundCorners) {
      if (digitCorners[curDigit][t] == digitCorners[prevDigit][t]) {
        currentR = cornerR;
        cornerMask = digitCorners[curDigit][t];
      } else {
        // Animation corner scaling
        int rawR = (2 * cornerR * (int)normTime / (int)ANIMATION_NORMALIZED_MAX) - cornerR;
        currentR = (rawR < 0) ? -rawR : rawR;
        cornerMask = (normTime < animMiddle) ? digitCorners[prevDigit][t] : digitCorners[curDigit][t];
      }
    }

    // 3. Fast Color Setting
#ifdef PBL_COLOR
    int numcol = digits[curDigit][t][1] % 5;
    if (mirror && (slot->num == 2 || slot->num == 3)) numcol = 4 - numcol;
    graphics_context_set_fill_color(ctx, color[colorTheme][numcol]);
#else
    graphics_context_set_fill_color(ctx, invertStatus ? GColorBlack : GColorWhite);
#endif

    // 4. Draw with stripe-awareness
    // Subtracting stripedDigits here prevents drawing pixels that will just be covered/cleared
    graphics_fill_rect(ctx, GRect(ox, oy, tileW, tileH - stripedDigits), currentR, cornerMask);
  }
}

void initSlot(int i, Layer *parent) {
  slot[i].num = i;
  slot[i].normTime = ANIMATION_NORMALIZED_MAX;
  slot[i].prevDigit = 0;
  slot[i].curDigit = startDigit[i];
  if (i<4) {
    // Hour slots -> big digits
    slot[i].tileWidth = TILEW;
    slot[i].tileHeight = TILEH;
    slot[i].cornerRadius = TILECORNERRADIUS;
  } else {
    // Date slots -> small digits
    slot[i].tileWidth = DTILEW;
    slot[i].tileHeight = DTILEH;
    slot[i].cornerRadius = DTILECORNERRADIUS;
  }
  slot[i].layer = layer_create(slotFrame(i));
  layer_set_update_proc(slot[i].layer, updateSlot);
  layer_add_child(parent, slot[i].layer);
}

void deinitSlot(int i) {
  layer_destroy(slot[i].layer);
}

void redrawAllSlots() {
  int i;

  layer_mark_dirty(mainLayer);

  for (i=0; i<NUMSLOTS; i++) {
    layer_mark_dirty(slot[i].layer);
  }
}

void destroyAnim(Animation *animation) {
  /*
  if (anim != NULL) {
    animation_destroy(anim);
  }
    */
  anim = NULL;
}

void createAnim() {
  if (anim != NULL) {
    animation_unschedule(anim);
    anim = NULL;
  }

  anim = animation_create();
  if (anim) {
    animation_set_delay(anim, 0);
    animation_set_duration(anim, DIGIT_CHANGE_ANIM_DURATION);
    animation_set_implementation(anim, &animImpl);
    
    // Explicitly casting the handler to prevent the 'incompatible pointer' error
    AnimationHandlers handlers = {
      .stopped = (AnimationStoppedHandler)destroyAnim
    };
    
    animation_set_handlers(anim, handlers, NULL);
  }
}

#ifdef PBL_PLATFORM_APLITE
void animateDigits(struct Animation *anim, const uint32_t normTime)
#else
void animateDigits(Animation *anim, const AnimationProgress normTime)
#endif
{
  int i;

  for (i=0; i<NUMSLOTS; i++) {
    if (slot[i].curDigit != slot[i].prevDigit) {
      slot[i].normTime = normTime;
      layer_mark_dirty(slot[i].layer);
    }
  }

  if (normTime == ANIMATION_NORMALIZED_MAX) {
    animRunning = false;
  }
}

void handle_tick(struct tm *now, TimeUnits units_changed) {
  int h, m;
  int D, M;
  int i;
  int wd = 0;
  int Y = 0;

  //
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "handle_tick: start");
  //

  if (splashEnded && !animation_is_scheduled(anim)) {
    //
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "handle_tick: setting digits");
    //

    h = now->tm_hour;
    m = now->tm_min;
    D = now->tm_mday;
    M = now->tm_mon+1;

    if (showWeekday) {
      wd = now->tm_wday;
    } else {
      Y = now->tm_year%100;
    }

    if (!clock_is_24h_style()) {
      h = h%12;
      if (h == 0) {
        h = 12;
      }
    }

    for (i=0; i<NUMSLOTS; i++) {
      slot[i].prevDigit = slot[i].curDigit;
    }

    // Hour slots
    slot[0].curDigit = h/10;
    slot[1].curDigit = h%10;
    slot[2].curDigit = m/10;
    slot[3].curDigit = m%10;
    slot[6].curDigit = SPACE_D;
    slot[9].curDigit = SPACE_D;


    // Date slots
    if (showWeekday && USDate) {
      slot[4].curDigit = weekDay[curLang][wd][0] - '0';
      slot[5].curDigit = weekDay[curLang][wd][1] - '0';
      slot[7].curDigit = M/10;
      slot[8].curDigit = M%10;
      slot[10].curDigit = D/10;
      slot[11].curDigit = D%10;
    } else if (showWeekday && !USDate) {
      slot[4].curDigit = weekDay[curLang][wd][0] - '0';
      slot[5].curDigit = weekDay[curLang][wd][1] - '0';
      slot[7].curDigit = D/10;
      slot[8].curDigit = D%10;
      slot[10].curDigit = M/10;
      slot[11].curDigit = M%10;
    } else if (!showWeekday && USDate) {
      slot[4].curDigit = M/10;
      slot[5].curDigit = M%10;
      slot[7].curDigit = D/10;
      slot[8].curDigit = D%10;
      slot[10].curDigit = Y/10;
      slot[11].curDigit = Y%10;
    } else {
      slot[4].curDigit = D/10;
      slot[5].curDigit = D%10;
      slot[7].curDigit = M/10;
      slot[8].curDigit = M%10;
      slot[10].curDigit = Y/10;
      slot[11].curDigit = Y%10;
    }

    //
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "handle_tick: rescheduling anim");
    //

    if (doanim) {
      createAnim();
      animation_schedule(anim);
    }
  }

  //
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "handle_tick: end");
  //

}

void do_update() {
  time_t curTime;
  struct tm *now;

  splashEnded = true;
  curTime = time(NULL);
  now = localtime(&curTime);
  handle_tick(now, 0);
}

void handle_timer(void *data) {
  timer=NULL;
  animRunning = false; // Release the lock
  do_update();
}

void safe_register_timer(int delay) {
  //If a timer exists, cancel it before making a new one
  if (timer != NULL) {
    app_timer_cancel(timer);
  }
  timer = app_timer_register(delay, handle_timer, NULL);
}

void handle_tap(AccelAxisType axis, int32_t direction) {
  static BatteryChargeState chargeState;
  int i, s;

  // Added a check: Don't trigger if already showing battery/BT info
  if (batteryStatus && splashEnded && !animRunning) {
    animRunning = true; 

    chargeState = battery_state_service_peek();
    s = chargeState.charge_percent;

    for (i=0; i<NUMSLOTS; i++) {
      slot[i].prevDigit = slot[i].curDigit;
    }

    slot[4].curDigit = 'B' - '0';
    slot[5].curDigit = 'A' - '0';
    slot[6].curDigit = 'T' - '0';
    slot[7].curDigit = SPACE_D;
    slot[8].curDigit = (s==100)?1:SPACE_D;
    slot[9].curDigit = (s<100)?s/10:0;
    slot[10].curDigit = (s < 100) ?s % 100:0;
    slot[11].curDigit = PERCENT;

    createAnim();
    animation_schedule(anim);
    safe_register_timer(BATTERYDELAY);
  }
}

void handle_bluetooth(bool connected) {
  int i;

  if (lastBluetoothStatus == connected) {
    return;
  } else {
    lastBluetoothStatus = connected;

    if (bluetoothStatus && splashEnded && !animRunning) {
      if (animation_is_scheduled(anim)) {
        animation_unschedule(anim);
      }

      animRunning = true;

      for (i=0; i<NUMSLOTS; i++) {
        slot[i].prevDigit = slot[i].curDigit;
      }

      slot[0].curDigit = 'B' - '0';
      slot[1].curDigit = 'T' - '0';

      if (connected) {
        slot[2].curDigit = 'O' - '0';
        slot[3].curDigit = 'K' - '0';

        slot[4].curDigit  = SPACE_D;
        slot[5].curDigit  = SPACE_D;
        slot[6].curDigit  = SPACE_D;
        slot[7].curDigit  = SPACE_D;
        slot[8].curDigit  = SPACE_D;
        slot[9].curDigit  = SPACE_D;
        slot[10].curDigit = SPACE_D;
        slot[11].curDigit = SPACE_D;

        vibes_double_pulse();
      } else {
        static const uint32_t  segments[] = {80, 30, 80, 30, 80};
        VibePattern pat = {
          .durations = segments,
          .num_segments = ARRAY_LENGTH(segments),
        };
        slot[2].curDigit = SPACE_L;
        slot[3].curDigit = SPACE_R;

        slot[4].curDigit  = SPACE_D;
        slot[5].curDigit  = 'F' - '0';
        slot[6].curDigit  = 'A' - '0';
        slot[7].curDigit  = 'I' - '0';
        slot[8].curDigit  = 'L' - '0';
        slot[9].curDigit  = 'E' - '0';
        slot[10].curDigit = 'D' - '0';
        slot[11].curDigit = SPACE_D;

        vibes_enqueue_custom_pattern(pat);
      }

      createAnim();
      animation_schedule(anim);
      if (timer==NULL)
        timer=app_timer_register(BATTERYDELAY, handle_timer, NULL);
    }
  }
}

void applyConfig() {
  if (splashEnded) {
    do_update();
  }
}

bool checkAndSaveInt(int *var, int val, int key) {
  int ret;

  if (*var != val) {
    *var = val;
    ret = persist_write_int(key, val);
    if (ret < 0) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "checkAndSaveInt() : persist_write_int(%d, %d) returned %d",
              key, val, ret);
    }
    return true;
  } else {
    return false;
  }
}

bool checkAndSaveString(char *var, char *val, int key) {
  int ret;

  if (strcmp(var, val) != 0) {
    strcpy(var, val);
    ret = persist_write_string(key, val);
    if (ret < (int)strlen(val)) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "checkAndSaveString() : persist_write_string(%d, %s) returned %d",
              key, val, ret);
    }
    return true;
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "checkAndSaveString() : value has not changed (was : %s, is : %s)",
            var, val);
    return false;
  }
}

static inline int8_t getDigitTile(int8_t *digit, int x, int y) {
  if ( (x < 0) || (x > 2) || (y < 0) || (y > 4)) {
    return 0;
  } else {
    return digit[x+3*y];
  }
}

uint8_t calcTileCorners(int8_t *digit, int x, int y) {
  int i, j, ox, oy;
  uint8_t mask = GCornerNone;
  int8_t surroundingTile[3][3];

  for (j=0, oy=y-1; j<3; j++, oy++) {
    for (i=0, ox=x-1; i<3; i++, ox++) {
      surroundingTile[i][j] = getDigitTile(digit, ox, oy);
    }
  }

  // Top Left Corner
  if ( (surroundingTile[0][0] == 0) && (surroundingTile[1][0] == 0) && (surroundingTile[0][1] == 0) ) {
    mask |= GCornerTopLeft;
  }

  // Top Right Corner
  if ( (surroundingTile[1][0] == 0) && (surroundingTile[2][0] == 0) && (surroundingTile[2][1] == 0) ) {
    mask |= GCornerTopRight;
  }

  // Bottom Left Corner
  if ( (surroundingTile[0][1] == 0) && (surroundingTile[0][2] == 0) && (surroundingTile[1][2] == 0) ) {
    mask |= GCornerBottomLeft;
  }

  // Bottom Right Corner
  if ( (surroundingTile[1][2] == 0) && (surroundingTile[2][1] == 0) && (surroundingTile[2][2] == 0) ) {
    mask |= GCornerBottomRight;
  }

  return mask;
}

void calcDigitCorners(int i) {
  int t, x, y;
  int8_t digit[15];
  uint8_t cornerMask;

  // Clear digit matrix
  for (t=0; t<15; t++) {
    digit[t] = 0;
  }

  // Compute digit matrix
  for (t=0; t<13; t++) {
    x = digits[i][t][0];
    y = digits[i][t][1];

    if ( (x >= 0) && (x <= 2) && (y >= 0) && (y <= 4)) {
      digit[x + 3*y] = 1;
    }
  }

  // Compute round corners from matrix
  for (y=0; y<5; y++) {
    for (x=0; x<3; x++) {
      cornerMask = GCornerNone;
      if (digit[x+3*y]) {
        cornerMask = calcTileCorners(digit, x, y);
      }
      // Fill the corners array
      for (t=0; t<13; t++) {
        if ( (digits[i][t][0] == x) && (digits[i][t][1] == y) ) {
          digitCorners[i][t] = cornerMask;
        }
      }
    }
  }
}

void swapDigitShapes() {
  int i;

  for (i = 0; i < 13; i++) {
    // Swap digit 2
    digits[2][i][0] = digit2[fullDigits][i][0];
    digits[2][i][1] = digit2[fullDigits][i][1];
    // Swap digit 4
    digits[4][i][0] = digit4[fullDigits][i][0];
    digits[4][i][1] = digit4[fullDigits][i][1];
    // Swap digit 5
    digits[5][i][0] = digit5[fullDigits][i][0];
    digits[5][i][1] = digit5[fullDigits][i][1];
  }

  calcDigitCorners(2);
  calcDigitCorners(4);
  calcDigitCorners(5);
}

int hexCharToInt(const char digit) {
  if ((digit >= '0') && (digit <= '9')) {
    return (int)(digit - '0');
  } else if ((digit >= 'a') && (digit <= 'f')) {
    return 10 + (int)(digit - 'a');
  } else if ((digit >= 'A') && (digit <= 'F')) {
    return 10 + (int)(digit - 'A');
  } else {
    return -1;
  }
}

int hexStringToByte(const char *hexString) {
  int l = strlen(hexString);
  if (l == 0) return 0;
  if (l == 1) return hexCharToInt(hexString[0]);

  return 16*hexCharToInt(hexString[0]) + hexCharToInt(hexString[1]);
}

void decodeThemeCode(char *code) {
#ifdef PBL_COLOR
  int i;

  for (i=0; i<6; i++) {
    color[COLOR_THEME_CUSTOM][i] = (GColor8){.argb=(uint8_t)hexStringToByte(code + 2*i)};
  }
#else
  // Do nothing on APLITE
#endif
}

void in_dropped_handler(AppMessageResult reason, void *context) {
}

void in_received_handler(DictionaryIterator *received, void *context) {
  bool somethingChanged = false;
  bool digitShapesChanged = false;

  // 1. Time & Date Settings
  Tuple *dateorder = dict_find(received, CONFIG_KEY_DATEORDER);
  if(dateorder) somethingChanged |= checkAndSaveInt(&USDate, dateorder->value->int32, CONFIG_KEY_DATEORDER);

  Tuple *weekday = dict_find(received, CONFIG_KEY_WEEKDAY);
  if(weekday) somethingChanged |= checkAndSaveInt(&showWeekday, weekday->value->int32, CONFIG_KEY_WEEKDAY);

  Tuple *lang = dict_find(received, CONFIG_KEY_LANG);
  if(lang) somethingChanged |= checkAndSaveInt(&curLang, lang->value->int32, CONFIG_KEY_LANG);

  // 2. Status Alerts
  Tuple *battery = dict_find(received, CONFIG_KEY_BATTERY);
  if(battery) checkAndSaveInt(&batteryStatus, battery->value->int32, CONFIG_KEY_BATTERY);

  Tuple *bluetooth = dict_find(received, CONFIG_KEY_BLUETOOTH);
  if(bluetooth) checkAndSaveInt(&bluetoothStatus, bluetooth->value->int32, CONFIG_KEY_BLUETOOTH);

  // 3. Visuals & Themes
  Tuple *invert = dict_find(received, CONFIG_KEY_INVERT);
  if(invert) digitShapesChanged |= checkAndSaveInt(&invertStatus, invert->value->int32, CONFIG_KEY_INVERT);

  Tuple *themeCodeTuple = dict_find(received, CONFIG_KEY_THEMECODE);
  if(themeCodeTuple) {
      if(checkAndSaveString(themeCodeText, themeCodeTuple->value->cstring, CONFIG_KEY_THEMECODE)) {
          decodeThemeCode(themeCodeText);
          digitShapesChanged = true;
      }
  }

  Tuple *colorThemeTuple = dict_find(received, CONFIG_KEY_COLORTHEME);
  if(colorThemeTuple) digitShapesChanged |= checkAndSaveInt(&colorTheme, colorThemeTuple->value->int32, CONFIG_KEY_COLORTHEME);

  // 4. Geometry & Style
  Tuple *stripes = dict_find(received, CONFIG_KEY_STRIPES);
  if(stripes) digitShapesChanged |= checkAndSaveInt(&stripedDigits, stripes->value->int32, CONFIG_KEY_STRIPES);

  Tuple *corners = dict_find(received, CONFIG_KEY_ROUNDCORNERS);
  if(corners) digitShapesChanged |= checkAndSaveInt(&roundCorners, corners->value->int32, CONFIG_KEY_ROUNDCORNERS);

  Tuple *digits = dict_find(received, CONFIG_KEY_FULLDIGITS);
  if(digits) {
      if(checkAndSaveInt(&fullDigits, digits->value->int32, CONFIG_KEY_FULLDIGITS)) {
          swapDigitShapes();
          digitShapesChanged = true;
      }
  }

  Tuple *mirrorTuple = dict_find(received, CONFIG_KEY_MIRROR);
  if(mirrorTuple) digitShapesChanged |= checkAndSaveInt(&mirror, mirrorTuple->value->int32, CONFIG_KEY_MIRROR);

  Tuple *splashTuple = dict_find(received, CONFIG_KEY_SPLASH);
  if(splashTuple) checkAndSaveInt(&splash, splashTuple->value->int32, CONFIG_KEY_SPLASH);

  // Execution
  if (somethingChanged) {
    applyConfig();
  }

  if (digitShapesChanged) {
    redrawAllSlots();
  }
}

void readConfig() {
  if (persist_exists(CONFIG_KEY_DATEORDER)) {
    USDate = persist_read_int(CONFIG_KEY_DATEORDER);
  } else {
    USDate = 1;
    persist_write_int(CONFIG_KEY_DATEORDER, USDate);
  }

  if (persist_exists(CONFIG_KEY_WEEKDAY)) {
    showWeekday = persist_read_int(CONFIG_KEY_WEEKDAY);
  } else {
    showWeekday = 0;
    persist_write_int(CONFIG_KEY_WEEKDAY, showWeekday);
  }

  if (persist_exists(CONFIG_KEY_BATTERY)) {
    batteryStatus = persist_read_int(CONFIG_KEY_BATTERY);
  } else {
    batteryStatus = 1;
    persist_write_int(CONFIG_KEY_BATTERY, batteryStatus);
  }

  if (persist_exists(CONFIG_KEY_BLUETOOTH)) {
    bluetoothStatus = persist_read_int(CONFIG_KEY_BLUETOOTH);
  } else {
    bluetoothStatus = 1;
    persist_write_int(CONFIG_KEY_BLUETOOTH, bluetoothStatus);
  }

  if (persist_exists(CONFIG_KEY_INVERT)) {
    invertStatus = persist_read_int(CONFIG_KEY_INVERT);
  } else {
    invertStatus = 0;
    persist_write_int(CONFIG_KEY_INVERT, invertStatus);
  }

  if (persist_exists(CONFIG_KEY_LANG)) {
    curLang = persist_read_int(CONFIG_KEY_LANG);
  } else {
    curLang = LANG_ENGLISH;
    persist_write_int(CONFIG_KEY_LANG, curLang);
  }

  if (persist_exists(CONFIG_KEY_STRIPES)) {
    stripedDigits = persist_read_int(CONFIG_KEY_STRIPES);
  } else {
    stripedDigits = 1;
    persist_write_int(CONFIG_KEY_STRIPES, stripedDigits);
  }

  if (persist_exists(CONFIG_KEY_ROUNDCORNERS)) {
    roundCorners = persist_read_int(CONFIG_KEY_ROUNDCORNERS);
  } else {
    roundCorners = 1;
    persist_write_int(CONFIG_KEY_ROUNDCORNERS, roundCorners);
  }

  if (persist_exists(CONFIG_KEY_FULLDIGITS)) {
    fullDigits = persist_read_int(CONFIG_KEY_FULLDIGITS);
  } else {
    fullDigits = 0;
    persist_write_int(CONFIG_KEY_FULLDIGITS, fullDigits);
  }

  if (persist_exists(CONFIG_KEY_COLORTHEME)) {
    colorTheme = persist_read_int(CONFIG_KEY_COLORTHEME);
  } else {
    colorTheme = 0;
    persist_write_int(CONFIG_KEY_COLORTHEME, colorTheme);
  }

  if (persist_exists(CONFIG_KEY_THEMECODE)) {
    persist_read_string(CONFIG_KEY_THEMECODE, themeCodeText, sizeof(themeCodeText));
  } else {
    strcpy(themeCodeText, "ffffffffffc0");
    persist_write_string(CONFIG_KEY_THEMECODE, themeCodeText);
  }

  if (persist_exists(CONFIG_KEY_MIRROR)) {
    mirror = persist_read_int(CONFIG_KEY_MIRROR);
  } else {
    mirror = 0;
    persist_write_int(CONFIG_KEY_MIRROR, mirror);
  }


  if (persist_exists(CONFIG_KEY_SPLASH)) {
    splash = persist_read_int(CONFIG_KEY_SPLASH);
  } else {
    splash = 1;
    persist_write_int(CONFIG_KEY_SPLASH, splash);
  }

  decodeThemeCode(themeCodeText);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Stored config :");
  APP_LOG(APP_LOG_LEVEL_DEBUG, "  dateorder=%d, weekday=%d, battery=%d, BT=%d, invert=%d",
          USDate, showWeekday, batteryStatus, bluetoothStatus, invertStatus);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "  lang=%d, stripedDigits=%d, roundCorners=%d, fullDigits=%d",
          curLang, stripedDigits, roundCorners, fullDigits);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "  colorTheme=%d, themecode=%s, mirror=%d, splash=%d", colorTheme, themeCodeText, mirror, splash);
}

static void app_message_init(void) {
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_open(512, 512);
}

void initDigitCorners() {
  int i;

  for (i=0; i<NUM_DIGITS; i++) {
    calcDigitCorners(i);
  }
}

void initSplash() {
  if (splash) {
    char vers[10];
    int len, s, i;
    
    snprintf(vers, sizeof(vers), "%d.%d", __pbl_app_info.process_version.major, __pbl_app_info.process_version.minor);
    len = strlen(vers);
    s = (8 - len)/2;

    APP_LOG(APP_LOG_LEVEL_DEBUG, "Blockslide %s", vers);

    for (i=0; i<len; i++) {
      if ((vers[i] >= '0') && (vers[i] <= 'Z')) {
        startDigit[4+s+i] = vers[i] - '0';
      } else if (vers[i] == '.') {
        startDigit[4+s+i] = DOT;
      }
    }
  } else {
    int i;
    splashEnded = true;
    time_t t = time(NULL);
    struct tm *tt = localtime(&t);
    doanim = false;
    handle_tick(tt, 0);
    for (i=0; i<NUMSLOTS; i++) {
      startDigit[i] = slot[i].curDigit;
    }
    handle_tick(tt, 0);
    doanim = true;
  }
}

void handle_init() {
  int i;

  srand(time(NULL));
  initColors();

  readConfig();
  swapDigitShapes();
  initSplash();

  app_message_init();

  window = window_create();
  if (invertStatus) {
    window_set_background_color(window, GColorWhite);
  } else {
    window_set_background_color(window, GColorBlack);
  }
  window_stack_push(window, true);

  rootLayer = window_get_root_layer(window);
  mainLayer = layer_create(layer_get_bounds(rootLayer));
  layer_add_child(rootLayer, mainLayer);
  layer_set_update_proc(mainLayer, updateMainLayer);

  for (i=0; i<NUMSLOTS; i++) {
    initSlot(i, mainLayer);
  }

  initDigitCorners();

  animImpl.setup = NULL;
  animImpl.update = (AnimationUpdateImplementation)animateDigits;
#ifdef PBL_PLATFORM_APLITE
  animImpl.teardown = destroyAnim;
#else
  animImpl.teardown = NULL;
#endif
  createAnim();

  timer = app_timer_register(STARTDELAY, handle_timer, NULL);

  tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);

  accel_tap_service_subscribe(handle_tap);

  lastBluetoothStatus = bluetooth_connection_service_peek();
  bluetooth_connection_service_subscribe(handle_bluetooth);
}

void handle_deinit() {
  int i;
  
  // 1. Safety: Stop any running animations immediately
  if (anim != NULL) {
    animation_unschedule(anim);
    anim = NULL;
  }

  // 2. Cancel the return-to-time timer
  if (timer != NULL) {
    app_timer_cancel(timer);
    timer = NULL;
  }

  // 3. Unsubscribe from all services
  bluetooth_connection_service_unsubscribe();
  accel_tap_service_unsubscribe();
  tick_timer_service_unsubscribe();
  
  // 4. Destroy layers from the bottom up
  for (i = 0; i < NUMSLOTS; i++) {
    deinitSlot(i);
  }
  
  if (mainLayer) {
    layer_destroy(mainLayer);
    mainLayer = NULL;
  }

  if (window) {
    window_destroy(window);
    window = NULL;
  }
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}


