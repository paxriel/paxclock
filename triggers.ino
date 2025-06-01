// All rising edge triggers
void triggerRisingEdge(int i, unsigned long cMillis) {
  unsigned long diff;
  switch (i) {
    case BUTTON_MODE:
      // Initialise mode hold
      modePressedStart = cMillis;
      break;
    case BUTTON_INC:
      // Inc change
      if (modeState == MODE_CLOCK && clockState > 0) {
        holdState = HOLD_PRE_INC;
        holdStart = cMillis;
      }
      if (modeState == MODE_STOPWATCH) {
        stPaused = !stPaused;
        if (stPaused) {
          tone(BUZZER_0, NOTE_C5, 200);
          stPausedRTC = Rtc.GetDateTime();
          stPausedTime = cMillis;
          //Serial.println(stPausedTime);
          //Serial.println("Stopwatch paused");
        } else if (stStartTime == 0) {
          tone(BUZZER_0, NOTE_E5, 200);
          stStartRTC = Rtc.GetDateTime();
          stStartTime = cMillis;
          //Serial.println(stStartTime);
          //Serial.println("Stopwatch started");
        } else {
          tone(BUZZER_0, NOTE_E5, 200);
          // Overflow error can still occur if millis rolls over whilst it is paused
          diff = cMillis - stPausedTime;
          stStartTime += diff;
          stStartRTC += diff / 1000;
          //Serial.println(stStartTime);
          stPausedTime = 0;
          //Serial.println("Stopwatch started from pause");
        }
      }
      if (modeState == MODE_TIMER) {
        switch (timerState) {
          case 0:
            tone(BUZZER_0, NOTE_E5, 200);
            // Start
            timerStartTime = cMillis;
            timerState = 1;
            break;
          case 1:
            tone(BUZZER_0, NOTE_C5, 200);
            // Pause
            timerPausedTime = cMillis;
            timerState = 2;
            break;
          case 2:
            tone(BUZZER_0, NOTE_E5, 200);
            // Resume
            timerStartTime += (cMillis - timerPausedTime);
            timerPausedTime = 0;
            timerState = 1;
            break;
          case 3:
          case 4:
            holdState = HOLD_PRE_INC;
            holdStart = cMillis;
            break;
          default:
            break;
        }
      }
      if (modeState == MODE_INTERVAL_TIMER) {
        switch (itState) {
          case 0:
            //tone(BUZZER_0, NOTE_E5, 200);
            intervalStartTime = cMillis + 3000;
            itStateActive = false;
            itState = 5;
            //Serial.println(intervalStartTime);
            //Serial.println("Interval timer started");
            break;
          case 1:
            tone(BUZZER_0, NOTE_C5, 200);
            itState = 2;
            intervalPauseTime = cMillis;
            //Serial.println(intervalPauseTime);
            //Serial.println("Interval timer paused");
            break;
          case 2:
            tone(BUZZER_0, NOTE_E5, 200);
            itState = 1;
            diff = cMillis - intervalPauseTime;
            intervalStartTime += diff;
            //Serial.println(intervalStartTime);
            intervalPauseTime = 0;
            //Serial.println("Interval timer started from pause");
            break;
          default:
            break;
        }
      }
      break;
    case BUTTON_DEC:
      // Dec change
      if (modeState == MODE_CLOCK && clockState > 0) {
        holdState = HOLD_PRE_DEC;
        holdStart = cMillis;
      }
      if (modeState == MODE_TIMER) {
        switch (timerState) {
          case 2:
            tone(BUZZER_0, NOTE_C5, 200);
            resetTimer();
            break;
          case 3:
          case 4:
            holdState = HOLD_PRE_DEC;
            holdStart = cMillis;
            break;
        }
      }

      if (modeState == MODE_STOPWATCH && stPaused) {
        tone(BUZZER_0, NOTE_C5, 200);
        stStartTime = 0;
        stPausedTime = 0;
        //Serial.println("Stopwatch reset");
        showStopwatch();
      }
      if (modeState == MODE_INTERVAL_TIMER && itState == 2) {
        tone(BUZZER_0, NOTE_C5, 200);
        intervalStartTime = 0;
        intervalPauseTime = 0;
        itState = 0;
        //Serial.println("Interval timer reset");
        showIntervalTimer();
      }
      break;
    default:
      // Remaining inputs ignored
      break;
  }
}

void resetTimer() {
  timerState = 0;
  timerStartTime = 0;
  timerPausedTime = 0;
  modeChangeBlocked = false;
}

// All falling edge triggers
void triggerFallingEdge(int i) {
  if ((i == BUTTON_INC && (holdState == HOLD_PRE_INC || holdState == HOLD_INC)) || (i == BUTTON_DEC && (holdState == HOLD_PRE_DEC || holdState == HOLD_DEC))) {
    int oldHold = holdState;
    holdState = 0;
    // Long hold termination overrides remaining responses
    if (oldHold > 2) return;
  }

  switch (i) {
    case BUTTON_MODE:
      modePressedStop = millis();
      if (modePressedStop - modePressedStart > LONG_PRESS_DURATION) {
        // Long press handler
        if (modeState == MODE_CLOCK && clockState == 0) {
          // Enter editing mode
          tone(BUZZER_0, NOTE_C5, 200);
          clockState = 1;
          modeChangeBlocked = true;
          showEditClock();
          break;
        }
        if (modeState == MODE_TIMER && timerState == 0) {
          // Enter editing mode
          tone(BUZZER_0, NOTE_C5, 200);
          timerState = 3;
          modeChangeBlocked = true;
          showTimer();
          break;
        }
        if (modeState == MODE_INTERVAL_TIMER && itState == 0) {
          // Enter editing mode
          tone(BUZZER_0, NOTE_C5, 200);
          itState = 3;
          modeChangeBlocked = true;
          showEditIntervalTimer();
          break;
        }
      }

      if (!modeChangeBlocked) {
        modeState += 1;
        if (modeState > MODE_COUNT) modeState = 0;
        tone(BUZZER_0, NOTE_A4, 200);
        setMode();
        break;
      }

      if (modeState == MODE_CLOCK && clockState > 0) {
        if (holdState != 0) return;
        tone(BUZZER_0, NOTE_C5, 200);
        clockState += 1;
        if (clockState > 5) {
          clockState = 0;
          modeChangeBlocked = false;
          tone(BUZZER_0, NOTE_A4, 200);
          Rtc.SetDateTime(currentTime);
          showCurrentTime(currentTime);
        }
      }

      if (modeState == MODE_INTERVAL_TIMER && itState > 2) {
        itState += 1;
        if (itState > 4) {
          tone(BUZZER_0, NOTE_A4, 200);
          itState = 0;
          modeChangeBlocked = false;
          // Lock in interval timer changes
          if (itDurationChanged) {
            itDurationChanged = false;
            preferences.begin("paxclock", false);
            preferences.putInt("activeSecs", activeSecs);
            preferences.putInt("inactiveSecs", inactiveSecs);
            preferences.end();
          }
          showIntervalTimer();
        } else tone(BUZZER_0, NOTE_C5, 200);
        break;
      }

      if (modeState == MODE_TIMER) {
        switch (timerState) {
          case 3:            
            if (holdState != 0) return;
            tone(BUZZER_0, NOTE_C5, 200);
            timerState = 4;
            break;
          case 4:
            if (holdState != 0) return;
            tone(BUZZER_0, NOTE_A4, 200);
            timerState = 0;
            modeChangeBlocked = false;
            // Lock in changes
            if (timerDurationChanged) {
              timerDurationChanged = false;
              preferences.begin("paxclock", false);
              preferences.putULong("timerDuration", timerDuration);
              preferences.end();
            }
            break;
          case 5:
            resetTimer();
            break;
          default:
            break;
        }
      }
      break;
    case BUTTON_INC:
      // Inc change
      if (modeState == MODE_BRIGHTNESS_SET && brightness < 7) {
        tone(BUZZER_0, NOTE_C5, 200);
        brightness += 1;
        brightnessChanged = true;
        showBrightnessSetting();
      }
      if (modeState == MODE_CLOCK && clockState > 0) {
        tone(BUZZER_0, NOTE_C5, 200);
        incrementClock();
      }
      if (modeState == MODE_TIMER) {
        tone(BUZZER_0, NOTE_C5, 200);
        if (timerState == 5) resetTimer();
        else incrementTimer();
      }

      if (modeState == MODE_INTERVAL_TIMER) {
        if (itState == 3 && activeSecs < 95) {
          tone(BUZZER_0, NOTE_C5, 200);
          itDurationChanged = true;
          activeSecs += 5;
        } else if (itState == 4 && inactiveSecs < 95) {
          tone(BUZZER_0, NOTE_C5, 200);
          itDurationChanged = true;
          inactiveSecs += 5;
        }
      }
      break;
    case BUTTON_DEC:
      // Dec change
      if (modeState == MODE_CLOCK && clockState > 0) {
        tone(BUZZER_0, NOTE_C5, 200);
        decrementClock();
      }
      if (modeState == MODE_TIMER) {
        tone(BUZZER_0, NOTE_C5, 200);
        if (timerState == 5) resetTimer();
        else decrementTimer();
      }
      if (modeState == MODE_INTERVAL_TIMER) {
        if (itState == 3 && activeSecs > 5) {
          tone(BUZZER_0, NOTE_C5, 200);
          itDurationChanged = true;
          activeSecs -= 5;
        } else if (itState == 4 & activeSecs > 5) {
          tone(BUZZER_0, NOTE_C5, 200);
          itDurationChanged = true;
          inactiveSecs -= 5;
        }
      }
      if (modeState == MODE_BRIGHTNESS_SET && brightness > 1) {
        tone(BUZZER_0, NOTE_C5, 200);
        brightness -= 1;
        brightnessChanged = true;
        showBrightnessSetting();
      }
      break;
    default:
      break;
  }
}

void incrementClock() {
  RtcDateTime ref = RtcDateTime(9999, 12, 31, 23, 59, 00);
  switch (clockState) {
    case 1:
      currentTime += 60*60;
      break;
    case 2:
      currentTime += 60;
      break;
    case 3:
      currentTime += currentTime.DaysInMonth(currentTime.Year(), currentTime.Month())*24*60*60;
      break;
    case 4:
      currentTime += 60*60*24;
      break;
    case 5:
      currentTime = RtcDateTime(currentTime.Year() + 1, currentTime.Month(), currentTime.Day(), currentTime.Hour(), currentTime.Minute(), 0);
      break;
    default:
      break;
  }
  if (currentTime > ref) currentTime = ref;
}

void decrementClock() {
  RtcDateTime ref = RtcDateTime(2000,1,1,0,1,0);
  switch (clockState) {
    case 1:
      currentTime -= 60*60;
      break;
    case 2:
      currentTime -= 60;
      break;
    case 3:
      if (currentTime.Month() == 1) {
        currentTime -= currentTime.DaysInMonth(currentTime.Year() - 1, 12)*24*60*60;
      } else {
        currentTime -= currentTime.DaysInMonth(currentTime.Year(), currentTime.Month() - 1)*24*60*60;
      }
      break;
    case 4:
      currentTime -= 60*60*24;
      break;
    case 5:
      currentTime = RtcDateTime(currentTime.Year() - 1, currentTime.Month(), currentTime.Day(), currentTime.Hour(), currentTime.Minute(), 0);
      break;
    default:
      break;
  }
  if (currentTime < ref) currentTime = ref;
}

// Inrease timer by 1 interval
void incrementTimer() {
  if (timerState == 3 && timerDuration < 100*60*60*1000) {
    timerDurationChanged = true;
    if (timerDuration < 5*60*1000) timerDuration = 5*60*1000;
    timerDuration += 60*60*1000;
  } else if (timerState == 4) {
    timerDurationChanged = true;
    if (timerDuration < 5*60*1000) timerDuration += 1*60*1000;
    else if (timerDuration < 100*60*60*1000) timerDuration += 5*60*1000;
  }
}

// Decrease timer by 1 interval
void decrementTimer() {
  if (timerState == 3 && timerDuration > 60*60*1000) {
    timerDurationChanged = true;
    timerDuration -= 60*60*1000;
  } else if (timerState == 4) {
    timerDurationChanged = true;
    if (timerDuration > 5*60*1000) timerDuration -= 5*60*1000;
    else if (timerDuration > 60*1000) timerDuration -= 1*60*1000;
  }
}

// true - Increment, false, Decrement
void onLongPressTrigger(bool isInc) {
  switch (modeState) {
    case MODE_CLOCK:
      if (isInc) incrementClock();
      else decrementClock();
      showEditClock();
      break;
    case MODE_TIMER:
      if (isInc) incrementTimer();
      else decrementTimer();
      showTimer();
      break;
    default:
      break;
  }
}