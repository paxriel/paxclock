// Return last error recorded on RTC module
// https://github.com/Makuna/Rtc/blob/master/examples/DS3231_Simple/DS3231_Simple.ino
bool rtcWasError(const char* errorTopic = "")
{
    uint8_t error = Rtc.LastError();
    if (error != 0)
    {
        // we have a communications error
        // see https://www.arduino.cc/reference/en/language/functions/communication/wire/endtransmission/
        // for what the number means
        /*
        Serial.print("[");
        Serial.print(errorTopic);
        Serial.print("] WIRE communications error (");
        Serial.print(error);
        Serial.print(") : ");

        switch (error)
        {
        case Rtc_Wire_Error_None:
            Serial.println("(none?!)");
            break;
        case Rtc_Wire_Error_TxBufferOverflow:
            Serial.println("transmit buffer overflow");
            break;
        case Rtc_Wire_Error_NoAddressableDevice:
            Serial.println("no device responded");
            break;
        case Rtc_Wire_Error_UnsupportedRequest:
            Serial.println("device doesn't support request");
            break;
        case Rtc_Wire_Error_Unspecific:
            Serial.println("unspecified error");
            break;
        case Rtc_Wire_Error_CommunicationTimeout:
            Serial.println("communications timed out");
            break;
        } */
        return true;
    }
    return false;
}


// https://github.com/Makuna/Rtc/blob/master/examples/DS3231_Simple/DS3231_Simple.ino
void printDateTime(const RtcDateTime& dt)
{
    char datestring[26];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
}

// https://github.com/Makuna/Rtc/blob/master/examples/DS3231_Simple/DS3231_Simple.ino
void initRTC() {
  // Extra: Set up alarm interrupt
  //pinMode(RTC_SQW, INPUT);
  //attachInterrupt(RTC_SQW, Ext_INT1_ISR, RISING);

  //Serial.print("RTC initializing...\n");
  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  //printDateTime(compiled);
  //Serial.println();
  if (!Rtc.IsDateTimeValid()) 
    {
        if (!rtcWasError("setup IsDateTimeValid"))
        {
            // Common Causes:
            //    1) first time you ran and the device wasn't running yet
            //    2) the battery on the device is low or even missing

            //Serial.println("RTC lost confidence in the DateTime!");

            // following line sets the RTC to the date & time this sketch was compiled
            // it will also reset the valid flag internally unless the Rtc device is
            // having an issue

            Rtc.SetDateTime(compiled);
            showLowBatt();
        }
    }

    if (!Rtc.GetIsRunning())
    {
        if (!rtcWasError("setup GetIsRunning"))
        {
            //Serial.println("RTC was not actively running, starting now");
            Rtc.SetIsRunning(true);
        }
    }

    /*
    RtcDateTime now = Rtc.GetDateTime();
    if (!rtcWasError("setup GetDateTime"))
    {
        if (now < compiled)
        {
            // Clock time can override compile time
            //Serial.println("RTC is older than compile time, updating DateTime");
            //Rtc.SetDateTime(compiled);
        }
        
        else if (now > compiled)
        {
            Serial.println("RTC is newer than compile time, this is expected");
        }
        else if (now == compiled)
        {
            Serial.println("RTC is the same as compile time, while not expected all is still fine");
        }
    } */

    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    Rtc.Enable32kHzPin(false);
    rtcWasError("setup Enable32kHzPin");
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone); // Alarm pin for RTC only set when needed to reduce power consumption
    //Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmOne); 
    rtcWasError("setup SetSquareWavePin");
}