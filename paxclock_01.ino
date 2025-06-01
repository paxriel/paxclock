#define TONE_USE_INT
#define TONE_PITCH 440

#include "esp_bt.h"
#include <ezButton.h>
#include <RtcDS3231.h>
#include <Wire.h> 
#include <Preferences.h>
#include <TM1637Display.h>
#include <TonePitch.h>
#include <WiFi.h>

#define I2C_SDA 8
#define I2C_SCL 9
#define RTC_SQW 5

#define R_CLK 2
#define R_DIO 3
#define G_CLK 0
#define G_DIO 1

#define BUTTON_NUM 3
#define BUTTON_PIN_MODE 10
#define BUTTON_PIN_INC 20
#define BUTTON_PIN_DEC 21
#define BUTTON_MODE 0
#define BUTTON_INC 1
#define BUTTON_DEC 2

#define MODE_COUNT 4
#define MODE_CLOCK 0
#define MODE_STOPWATCH 1
#define MODE_TIMER 2
#define MODE_INTERVAL_TIMER 3
#define MODE_ALARM_SET -2
#define MODE_SOUND_SET -1
#define MODE_BRIGHTNESS_SET 4

#define HOLD_PRE_INC 1
#define HOLD_PRE_DEC 2
#define HOLD_INC 3
#define HOLD_DEC 4

#define LONG_PRESS_DURATION 500
#define LONG_PRESS_INC_INTERVAL 200 // Increase target value by 1 every 200ms
#define INTERNAL_PRESCALER 80000 // Converts prescaler to 1 millis

#define BUZZER_0 6

#define countof(a) (sizeof(a) / sizeof(a[0]))

ezButton buttonArray[] = {
  ezButton(BUTTON_PIN_MODE),
  ezButton(BUTTON_PIN_INC),
  ezButton(BUTTON_PIN_DEC)
};

RtcDS3231<TwoWire> Rtc(Wire);
TM1637Display rd = TM1637Display(R_CLK, R_DIO);
TM1637Display gd = TM1637Display(G_CLK, G_DIO);
Preferences preferences;
bool pinChanged = false;
unsigned long debounceDelay = 50;

int modeState = 0; // 0 - Clock, 1 - Stopwatch, 2 - Timer, 3 - Interval timer, 4 - Alarm set, 5 - Sound Setting, 6 - Brightness Setting
bool modeChangeBlocked = false; // Used when adjusting things like timers, interval timers and alarms
unsigned long modePressedStart = 0;
unsigned long modePressedStop = 0;
unsigned long holdStart = 0;
int holdState = 0; // 0 - No hold, 1 - Pre-inc hold, 2 - Pre-dec hold, 3 - Inc hold, 4 - Dec hold

// Clock variables
int shownMth = 0;
int shownDay = 0;
int shownHr = 0;
int shownMin = 0;
RtcDateTime currentTime;
int clockState = 0; // 0 - Clock, 1 - Hours change, 2 - Minutes change, 3 - Month change, 4 - Day change, 5 - Year change, 6 - Alarm triggered

// Brightness variables (1-7)
int brightness = 1;
bool brightnessChanged = false;

// Stopwatch variables
RtcDateTime stStartRTC;
RtcDateTime stPausedRTC;
unsigned long stStartTime = 0; // Start time
unsigned long stPausedTime = 0; // Paused time
bool stPaused = true;

// Timer variables
unsigned long timerStartTime = 0; // Start time
unsigned long timerPausedTime = 0; // Paused time
unsigned long timerDuration = 1000*60*90; // Duration in millis
int timerState = 0; // 0 - Stopped, 1 - Running, 2 - Paused, 3 - Hrs set, 4 - Mins set, 5 - Triggered
bool timerDurationChanged = false;

// Interval timer variables
int activeSecs = 40;
int inactiveSecs = 20;
unsigned long intervalStartTime = 0;
unsigned long intervalPauseTime = 0;
int itState = 0; // 0 - Stopped, 1 - Running, 2 - Paused, 3 - Active set, 4 - Inactive set, 5 - Pre-start countdown
bool itDurationChanged = false;
bool itStateActive = false;

// Alarm variables
int alarmHr = 0;
int alarmMin = 0;
bool alarmEnabled = false;

void setup() {
  preferences.begin("paxclock", true);
  brightness = preferences.getInt("brightness", 2);
  activeSecs = preferences.getInt("activeSecs", 40);
  inactiveSecs = preferences.getInt("inactiveSecs", 20);
  timerDuration = preferences.getULong("timerDuration", 1000*60*90);
  alarmHr = preferences.getInt("alarmHr", 0);
  alarmMin = preferences.getInt("alarmMin", 0);
  alarmEnabled = preferences.getBool("alarmEnabled", false);
  preferences.end();

  init7Segment();
  //Serial.begin(115200);
  //while(!Serial); // Wait till serial initialized
  //Serial.println("Starting system...");
  Wire.begin(I2C_SDA, I2C_SCL);
  for (byte i = 0; i < BUTTON_NUM; i++) {
    buttonArray[i].setDebounceTime(100);
  }

  //lowerCPUFreq();
  offBtWifi();
  initRTC();
  if (buttonArray[1].getStateRaw() == 0 && buttonArray[2].getStateRaw() == 0) showTimeReset();
  else tone(BUZZER_0, NOTE_A4, 200);
  // else showCpuFreq();
  setMode();
}

void setMode() {
  checkStoredValueChange();
  rd.clear();
  rd.setBrightness(brightness);
  gd.clear();
  gd.setBrightness(brightness);
  switch (modeState) {
    case MODE_CLOCK:
      if (clockState == 6) showAlarm();
      else showCurrentTime(Rtc.GetDateTime());
      break;
    case MODE_STOPWATCH:
      showStopwatch();
      break;
    case MODE_TIMER:
      showTimer();
      break;
    case MODE_INTERVAL_TIMER:
      showIntervalTimer();
      break;
    case MODE_ALARM_SET:
      break;
    case MODE_SOUND_SET:
      break;
    case MODE_BRIGHTNESS_SET:
      showBrightnessSetting();
      break;
    default:
      break;
  }
}

// Check if any changes need to be stored to the permanent flash
void checkStoredValueChange() {
  if (brightnessChanged) {
    preferences.begin("paxclock", false);
    preferences.putInt("brightness", brightness);
    preferences.end();
  }
}

void checkAlarm() {
  if (!alarmEnabled) return;
  RtcDateTime now = Rtc.GetDateTime();
  if (now.Minute() == alarmMin && now.Hour() == alarmHr) clockState = 6;
  if (modeState != MODE_CLOCK && clockState == 6 && !modeChangeBlocked) {
    modeState = MODE_CLOCK;
    setMode();
  }
}

// Disable BT and Wifi
void offBtWifi() {
  esp_bt_controller_disable();
  esp_bt_controller_deinit();
  esp_bt_mem_release(ESP_BT_MODE_BTDM);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

/*
// Monitors frequency used by CPU; Frequency is set under 'Tools' in IDE (Current: 40 MHz)
void lowerCPUFreq() {
  // Print the XTAL crystal frequency
  Serial.print("XTAL Crystal Frequency: ");
  Serial.print(getXtalFrequencyMhz());
  Serial.println(" MHz");

  // Print the CPU frequency
  Serial.print("CPU Frequency: ");
  Serial.print(getCpuFrequencyMhz());
  Serial.println(" MHz");

  // Print the APB bus frequency
  Serial.print("APB Bus Frequency: ");
  Serial.print(getApbFrequency());
  Serial.println(" Hz");
}
*/

void loop() {
  unsigned long cMillis = millis();
  pinChanged = false;
  for (byte i = 0; i < BUTTON_NUM; i++) buttonArray[i].loop();

  for (byte i = 0; i < BUTTON_NUM; i++) {
    if (buttonArray[i].isPressed()) {
      // Rising edge triggers
      triggerRisingEdge(i, cMillis);
      pinChanged = true;
    }

    if (buttonArray[i].isReleased()) {
      triggerFallingEdge(i);
      pinChanged = true;
    }
  }

  /*
  if (pinChanged) {
    Serial.print("Buttons - Mode: " + String(buttonArray[0].getState()) + " Inc: " + String(buttonArray[1].getState()) + " Dec: " + String(buttonArray[2].getState()) + "\n");
  } */

  unsigned long mCheck = cMillis % 1000;
  // Fires every cycle
  if (modeState == MODE_STOPWATCH && (!stPaused)) {
    showStopwatch();
  }
  if ((holdState == HOLD_PRE_INC || holdState == HOLD_PRE_DEC) && (cMillis - holdStart) > LONG_PRESS_DURATION) {
    holdStart = cMillis;
    if (holdState == HOLD_PRE_INC) holdState = HOLD_INC;
    else holdState = HOLD_DEC;
    tone(BUZZER_0, NOTE_E5, 200);
    onLongPressTrigger(holdState == HOLD_INC);
  } else if (holdState > 2 && (cMillis - holdStart) > LONG_PRESS_INC_INTERVAL) {
    holdStart = cMillis;
    onLongPressTrigger(holdState == HOLD_INC);
  }

  // Fires every 10ms per 500 ms
  if (mCheck < 11 || (mCheck >= 500 & mCheck < 510)) {
    showTimer(); // Checks if timer time has reached, will return if it is not timer currently

    if (modeState == MODE_CLOCK) {
      if (clockState == 6) showAlarm();
      else if (clockState > 0) showEditClock();
    }
    if (modeState == MODE_INTERVAL_TIMER && (itState == 3 || itState == 4)) {
      showEditIntervalTimer();
    }
  }

  // Clock change fires every 10ms per 1s
  if (mCheck < 11) {
    checkAlarm();

    if (modeState == MODE_CLOCK && clockState == 0) {
      RtcDateTime now = Rtc.GetDateTime();
      if (now.Minute() != shownMin) showCurrentTime(now);
    }
    if (modeState == MODE_INTERVAL_TIMER && (itState == 1 || itState == 5)) {
      showIntervalTimer();
    }
  }
}
