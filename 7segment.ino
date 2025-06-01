const uint8_t loadS[] = {
  SEG_D | SEG_E | SEG_F,                 // L
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, // O
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G, // A
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G        // d
};
const uint8_t testS0[] = { 0xff, 0xff, 0xff, 0xff};
const uint8_t testS1[] = { SEG_A, SEG_B, SEG_C, SEG_D };
const uint8_t testS2[] = { SEG_E, SEG_F, SEG_G, 0xff };
const uint8_t s0S[] = {
  SEG_A | SEG_C | SEG_D | SEG_F | SEG_G, // S
  SEG_D | SEG_E | SEG_F | SEG_G, // t
  0x00, 0x00
};
const uint8_t itS[] = {
  SEG_E | SEG_F, // I
  SEG_D | SEG_E | SEG_F | SEG_G, // t
  0x00, 0x00
};
const uint8_t clchS[] = {
  SEG_A | SEG_D | SEG_E | SEG_F, // C
  SEG_D | SEG_E | SEG_F,         // L
  SEG_A | SEG_D | SEG_E | SEG_F, // C
  SEG_C | SEG_E | SEG_F | SEG_G // h
};
const uint8_t itchS[] = {
  SEG_E | SEG_F, // I
  SEG_D | SEG_E | SEG_F | SEG_G, // t
  SEG_A | SEG_D | SEG_E | SEG_F, // C
  SEG_C | SEG_E | SEG_F | SEG_G // h
};
const uint8_t songS[] = {
  SEG_A | SEG_C | SEG_D | SEG_F | SEG_G,  // S
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, // O
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F, // N
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G // g
};
const uint8_t brS[] = {
  SEG_C | SEG_D | SEG_E | SEG_F | SEG_G, // b
  SEG_E | SEG_G, // r
  0x00, 0x00
};
const uint8_t numbersS[] = {
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, // 0
  SEG_B | SEG_C, // 1
  SEG_A | SEG_B | SEG_D | SEG_E | SEG_G, // 2
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_G, // 3
  SEG_B | SEG_C | SEG_F | SEG_G, // 4
  SEG_A | SEG_C | SEG_D | SEG_F | SEG_G,  // 5
  SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G, // 6
  SEG_A | SEG_B | SEG_C, // 7
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G, // 8
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G // 9
};
const uint8_t taS[] = {
  SEG_D | SEG_E | SEG_F | SEG_G, // t
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G, // A
  0x00, 0x00
};
const uint8_t tachS[] = {
  SEG_D | SEG_E | SEG_F | SEG_G, // t
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G, // A
  SEG_A | SEG_D | SEG_E | SEG_F, // C
  SEG_C | SEG_E | SEG_F | SEG_G // h
};
const uint8_t alyS[] {
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G, // A
  SEG_D | SEG_E | SEG_F,                 // L
  0x00, SEG_B | SEG_C | SEG_D | SEG_F | SEG_G // y
};
const uint8_t alnS[] {
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G, // A
  SEG_D | SEG_E | SEG_F,                 // L
  0x00, SEG_C | SEG_E | SEG_G // n
};
const uint8_t cpuS[] {
  SEG_A | SEG_D | SEG_E | SEG_F, // C
  SEG_A | SEG_B | SEG_E | SEG_F | SEG_G, // P
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, // U
  0x00
};
const uint8_t battS[] {
  SEG_C | SEG_D | SEG_E | SEG_F | SEG_G, // b
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G, // A
  SEG_D | SEG_E | SEG_F | SEG_G, // t
  SEG_D | SEG_E | SEG_F | SEG_G // t
};
const uint8_t rtcS[] {
  SEG_E | SEG_G, // r
  SEG_D | SEG_E | SEG_F | SEG_G, // t
  SEG_A | SEG_D | SEG_E | SEG_F, // C
  0x00
};
const uint8_t holdS[] {
  SEG_C | SEG_E | SEG_F | SEG_G, // h
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, // O
  SEG_D | SEG_E | SEG_F,                 // L
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G // d
};

// Modifies seq[] to contain the numbers
void overwriteWithNumbers(uint8_t* seq, int number, bool zeroRear = false, int leadingLimit = 3) {
  // RTL
  if (number == 0 && zeroRear) {
    seq[2] = numbersS[0];
    seq[3] = numbersS[0];
    return;
  }

  if (number > 10000) number %= 10000;
  int current = 3;
  while (number > 0) {
    int c = number % 10;
    number /= 10;
    seq[current] = numbersS[c];
    current -= 1;
  }
  while (current >= leadingLimit) {
    seq[current] = numbersS[0];
    current -= 1;
  }
}

// Modifies first 2 segments of seq[] to contain numbers
void overwriteFrontWithNumbers(uint8_t* seq, int number, bool showZero = false) {
  if (number == 0 && showZero) {
    seq[0] = numbersS[0];
    seq[1] = numbersS[0];
  }

  // Always show leading 0
  // RTL
  if (number > 100) number %= 100;
  seq[1] = numbersS[number % 10];
  seq[0] = numbersS[(number / 10) % 10];
}

void init7Segment() {
  rd.clear();
  rd.setBrightness(brightness);
  gd.clear();
  gd.setBrightness((brightness < 7) ? brightness + 1 : brightness);
  rd.setSegments(testS0);
  gd.setSegments(loadS);
}

void showTimeReset() {
  tone(BUZZER_0, NOTE_E5, 200);
  gd.clear();
  rd.clear();
  gd.setBrightness(brightness);
  rd.setBrightness(brightness);
  gd.setSegments(clchS);
  rd.setSegments(holdS);
  delay(2000);
  // Make sure that press is intentional and not cause of bounce
  if (buttonArray[1].getStateRaw() == 0 && buttonArray[2].getStateRaw() == 0) {
    Rtc.SetDateTime(RtcDateTime(__DATE__, __TIME__));
    tone(BUZZER_0, NOTE_A5, 200);
  } else {
    tone(BUZZER_0, NOTE_C5, 200);
  }
}

void showCurrentTime(RtcDateTime now) {
  shownMth = now.Month();
  shownDay = now.Day();
  shownHr = now.Hour();
  shownMin = now.Minute();
  currentTime = now;
  gd.showNumberDecEx(shownMth * 100 + shownDay, 0b00000000, true, 4, 0);
  rd.showNumberDecEx(shownHr * 100 + shownMin, 0b01000000, true, 4, 0);
}

void showAlarm() {
  
}

void showEditClock() {
  unsigned long cMillis = millis() % 1000;
  if (currentTime.Second() > 0) currentTime -= currentTime.Second();
  uint8_t gSegments[] = {0x00, 0x00, 0x00, 0x00};
  uint8_t rSegments[] = {0x00, 0x00, 0x00, 0x00};

  if (clockState != 1 || cMillis < 500 || holdState > 2) overwriteFrontWithNumbers(rSegments, currentTime.Hour(), true);
  if (clockState != 2 || cMillis < 500 || holdState > 2) overwriteWithNumbers(rSegments, currentTime.Minute(), true, 2);

  if (clockState < 5) {
    if (clockState != 3 || cMillis < 500 || holdState > 2) overwriteFrontWithNumbers(gSegments, currentTime.Month(), true);
    if (clockState != 4 || cMillis < 500 || holdState > 2) overwriteWithNumbers(gSegments, currentTime.Day(), true, 2);
    gd.setSegments(gSegments);
  } else if (cMillis < 500 || holdState > 2) {
    gd.showNumberDec(currentTime.Year(), true, 4, 0);
  } else {
    gd.clear();
  }
  rd.setSegments(rSegments);
}

void showLowBatt() {
  gd.clear();
  rd.clear();
  gd.setBrightness(brightness);
  rd.setBrightness(brightness);
  gd.setSegments(battS);
  rd.setSegments(rtcS);
  delay(5000);
}

void showCpuFreq() {
  gd.clear();
  rd.clear();
  gd.setBrightness(brightness);
  rd.setBrightness(brightness);
  gd.setSegments(cpuS);
  rd.showNumberDec(getCpuFrequencyMhz(), false, 4, 0);
  delay(1000);
}

void showStopwatch() {
  // RTC date time used past 60 seconds to shield effects from millis() resetting every 49 days
  unsigned long diff;
  if (stStartTime == 0) {
    diff = 0;
    gd.setSegments(s0S);
    rd.showNumberDecEx(0, 0b01000000, true, 4, 0);
  } else {
    diff = millis() - stStartTime;
    int mil = (diff / 10) % 100;
    unsigned long secsL = diff / 1000;
    int secs = secsL % 60;
    
    if (diff < 60*1000) {
      // Sub 1m, use sec:millis method
      gd.setSegments(s0S);
      rd.showNumberDecEx(secs * 100 + mil, 0b01000000, true, 4, 0);
    } else {
      if (mil > 2) return; // Reduce power draw by updating only 2 milliseconds / 1 sec
      // Substiute for theoretically more accurate figure from RTC
      uint64_t diffRtc =  Rtc.GetDateTime().TotalSeconds64() - stStartRTC.TotalSeconds64();
      secs = diffRtc % 60;
      unsigned long minsL = diffRtc / 60;
      unsigned long hrsL = minsL / 60;
      int mins = minsL % 60;
      int hrs = hrsL % 10000;

      rd.showNumberDecEx(mins * 100 + secs, 0b01000000, true, 4, 0);
      if (diff < 1000*60*60) {
        // Sub 1h, use min:sec method
        gd.setSegments(s0S);
      } else {
        // Use hr:min:sec method
        uint8_t gSegments[4];
        memcpy(gSegments, s0S, sizeof(s0S));
        overwriteWithNumbers(gSegments, hrs);
        gd.setSegments(gSegments);
      }
    }
  }
}

void showIntervalTimer() {
  if (itState == 0) {
    gd.setSegments(itS);
    rd.showNumberDecEx(activeSecs * 100 + inactiveSecs, 0b01000000, true, 4, 0);
    return;
  }
  unsigned long diff = (itState == 5) ? ((activeSecs + inactiveSecs) * 1000) - (intervalStartTime - millis()) : millis() - intervalStartTime;
  int intervalCount = ((diff / ((activeSecs + inactiveSecs) * 1000)) + 1) % 10000;
  unsigned long currentIntervalEnd = (intervalCount * 1000 * (activeSecs + inactiveSecs));
  unsigned long diff2 = currentIntervalEnd - diff;
  bool isActive = diff2 > (inactiveSecs*1000);
  int cActiveSecs = isActive ? ((diff2 - inactiveSecs*1000) / 1000) % 60 + 1 : 0;
  int cInactiveSecs = isActive ? inactiveSecs : (diff2 / 1000) % 60 + 1;

  if (itState == 1 || itState == 5) {
    if (isActive != itStateActive) {
      if (itState == 5) {
        itState = 1;
        intervalCount = 1;
      }
      itStateActive = !itStateActive;
      if (itStateActive) tone(BUZZER_0, NOTE_G4, 400);
      else tone(BUZZER_0, NOTE_D5, 400);
    }
    if (cActiveSecs > 0 && cActiveSecs < 4) tone(BUZZER_0, NOTE_D4, 200);
    else if (cInactiveSecs > 0 && cInactiveSecs < 4) tone(BUZZER_0, NOTE_G3, 200);
  }

  uint8_t gSegments[4];
  memcpy(gSegments, itS, sizeof(itS));
  overwriteWithNumbers(gSegments, (itState == 5) ? 0 : intervalCount);
  gd.setSegments(gSegments);
  rd.showNumberDecEx(cActiveSecs * 100 + cInactiveSecs, 0b01000000, true, 4, 0);
}

void showEditIntervalTimer() {
  gd.setSegments(itchS);
  unsigned long cMillis = millis() % 1000;
  uint8_t rSegments[] = {0x00, 0x00, 0x00, 0x00};
  if (itState != 3 || cMillis < 500) overwriteFrontWithNumbers(rSegments, activeSecs);
  if (itState != 4 || cMillis < 500) overwriteWithNumbers(rSegments, inactiveSecs);
  rd.setSegments(rSegments);
}

void showTimer() {
  unsigned long cMillis = millis() % 1000;
  unsigned long diffMillis; (timerState == 2) ? timerPausedTime - timerStartTime : millis() - timerStartTime;
  if (diffMillis > timerDuration && timerState == 1) {
    timerState = 5;
    if (modeState == MODE_TIMER) modeChangeBlocked = true;
    else if (!modeChangeBlocked) {
      modeState = MODE_TIMER;
      rd.clear();
      rd.setBrightness(brightness);
      gd.clear();
      gd.setBrightness(brightness);
      modeChangeBlocked = true;
    }
  }
  if (modeState != MODE_TIMER) return; // Timer check code ends here

  unsigned long tMinsL = timerDuration / (1000*60);
  int tHrs = (tMinsL / 60) % 100;
  int tMins = (tMinsL % 60) % 10000;
  unsigned long rSecs = (timerDuration - diffMillis) / 1000;
  unsigned long rMins = rSecs / 60;
  unsigned long rHrs = rMins / 60;
  rSecs %= 60;
  rMins %= 60;
  rHrs %= 10000;
  uint8_t rSegments[] = {0x00, 0x00, 0x00, 0x00};
  uint8_t gSegments[4];

  switch (timerState) {
    case 0:
      gd.setSegments(taS);
      rd.showNumberDecEx(tHrs * 100 + tMins, 0b01000000, true, 4, 0);
      break;
    case 3:
    case 4:
      gd.setSegments(tachS);
      if (timerState != 3 || cMillis < 500 || holdState > 2) overwriteFrontWithNumbers(rSegments, tHrs, true);
      if (timerState != 4 || cMillis < 500 || holdState > 2) overwriteWithNumbers(rSegments, tMins, true, 2);
      rd.setSegments(rSegments);
      break;
    case 1:
    case 2:
      memcpy(gSegments, taS, sizeof(taS));
      if (rHrs > 0) overwriteWithNumbers(gSegments, rHrs);
      gd.setSegments(gSegments);
      rd.showNumberDecEx(rMins * 100 + rSecs, (timerState == 2 || cMillis < 500) ? 0b01000000 : 0x00, true, 4, 0);
      break;
    case 5:
      tone(BUZZER_0, NOTE_A5, 200);
      gd.setSegments(taS);
      if (cMillis < 500) rd.showNumberDecEx(0, 0b01000000, true, 4, 0);
      else rd.clear();
      break;
    default:
      break;
  }
}

void showBrightnessSetting() {
  gd.clear();
  gd.setBrightness(brightness);
  gd.setSegments(brS);
  rd.clear();
  rd.setBrightness(brightness);
  rd.showNumberDec(brightness, false, 4, 0);
}