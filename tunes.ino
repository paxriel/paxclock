// 60 BPM: 1 note - 1000ms, quaver - 500ms, semiquaver - x
// 62.5 BPM: 1 note - 960ms, quaver - 480ms, semiquaver - 240ms
// 68.2 BPM: 1 note - 880ms, quaver - 440ms, semiquaver - x
// 75 BPM: 1 note - 800ms, quaver - 400ms, semiquaver - 200ms
// 83.3 BPM: 1 note - 720ms, quaver - 360ms, semiquaver - x
// 93.8 BPM: 1 note - 640ms,  quaver - 320ms, semiquaver - 160ms
// 107.1 BPM: 1 note - 560ms, quaver - 280ms, semiquaver - x
// 125 BPM: 1 note - 480ms, quaver - 240ms, semiquaver - 120ms
// 150 BPM: 1 note - 400ms, quaver - 200ms, semiquaver - x
// 187.5 BPM: 1 note - 320ms, quaver - 160ms, semiquaver - 80ms
// 250 BPM: 1 note - 240ms, quaver - 120ms, semiquaver - x
// 1 interval BPM - 40ms

// Tunes
// 01. Startup
// 02. Mode switch
// 03. Pause
// 04. Resume
// 05. Reset
// 06. Interval timer (Active start)
// 07. Interval timer (Inactive start)

// Melody selections
// 11. Standard alarm (83.3 BPM quavers - 360ms per note)
// 12. Standard alarm 2 (107.1 BPM quavers - 280ms per note)
// 13. Standard alarm 3 (150 BPM quavers - 200ms per note)
// 14. Fur Elise (125 BPM quavers - 240ms per note)
// 15. Evening at town (107.1 BPM quavers - 280ms per note)
// 16. A Little bit of Evil (125 BPM quavers - 240ms per note)

// Current issue: Default tone library can only play 1 buzzer at a time, alternative library required
// Workaround is to use https://techtutorialsx.com/2017/07/01/esp32-arduino-controlling-a-buzzer-with-pwm/
// with ledcSetup, ledcAttachPin, ledcWrite + ledcWriteTone
// Update: ledc not working, fall back to only 1 timer
// Update 2: Shrink back alarm to standard sound
