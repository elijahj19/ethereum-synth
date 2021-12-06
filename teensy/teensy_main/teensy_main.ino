#include <AsyncTimer.h> // https://github.com/Aasim-A/AsyncTimer


/* Auto-generated code for Teensy Sound */
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine8;          //xy=79,461
AudioSynthWaveformSine   sine2;          //xy=83.75,93.75000095367432
AudioSynthWaveformSine   sine3;          //xy=84,141.25000190734863
AudioSynthWaveformSine   sine7;          //xy=83,412
AudioSynthWaveformSine   sine1;          //xy=84.5,38.25
AudioSynthWaveformSine   sine4;          //xy=84.5,184.25000286102295
AudioSynthWaveformSine   sine6;          //xy=85,368
AudioSynthWaveformSine   sine5;          //xy=86,327
AudioAmplifier           amp3;           //xy=205.50000381469727,143.75000190734863
AudioAmplifier           amp4;           //xy=205.50000381469727,185.50000286102295
AudioAmplifier           amp1;           //xy=206.50000381469727,35
AudioAmplifier           amp2;           //xy=212.75000381469727,93.50000190734863
AudioAmplifier           amp8;           //xy=220,459
AudioAmplifier           amp5;           //xy=226,327
AudioAmplifier           amp7;           //xy=226,416
AudioAmplifier           amp6;           //xy=227,374
AudioEffectFade          fade1;          //xy=334.5000057220459,34
AudioEffectFade          fade4;          //xy=340.7500057220459,184.50000286102295
AudioEffectFade          fade2;          //xy=341.75000762939453,92.50000095367432
AudioEffectFade          fade3;          //xy=342.0000057220459,139.25000190734863
AudioEffectFade          fade5;          //xy=348.75000381469727,318.7500057220459
AudioEffectFade          fade6;          //xy=363.75000381469727,368.0000057220459
AudioEffectFade          fade7;          //xy=370.25000762939453,416.0000057220459
AudioEffectFade          fade8;          //xy=375,458
AudioMixer4              mixer1;         //xy=519.0000076293945,142.50000190734863
AudioMixer4              mixer2;         //xy=545.7500076293945,274.75000381469727
AudioMixer4              mixer3;         //xy=643.7500076293945,203.75000381469727
AudioAmplifier           amp9;           //xy=656.5000076293945,510.0000057220459
AudioEffectFreeverb      freeverb1;      //xy=762.5000114440918,428.5000057220459
AudioFilterStateVariable filter1;        //xy=765.5000114440918,211.25000286102295
AudioFilterStateVariable filter2;        //xy=875.2500114440918,347.50000381469727
AudioOutputI2S           i2s1;           //xy=933.0000114440918,203.25000286102295
AudioConnection          patchCord1(sine8, amp8);
AudioConnection          patchCord2(sine2, amp2);
AudioConnection          patchCord3(sine3, amp3);
AudioConnection          patchCord4(sine7, amp7);
AudioConnection          patchCord5(sine1, amp1);
AudioConnection          patchCord6(sine4, amp4);
AudioConnection          patchCord7(sine6, amp6);
AudioConnection          patchCord8(sine5, amp5);
AudioConnection          patchCord9(amp3, fade3);
AudioConnection          patchCord10(amp4, fade4);
AudioConnection          patchCord11(amp1, fade1);
AudioConnection          patchCord12(amp2, fade2);
AudioConnection          patchCord13(amp8, fade8);
AudioConnection          patchCord14(amp5, fade5);
AudioConnection          patchCord15(amp7, fade7);
AudioConnection          patchCord16(amp6, fade6);
AudioConnection          patchCord17(fade1, 0, mixer1, 0);
AudioConnection          patchCord18(fade4, 0, mixer1, 3);
AudioConnection          patchCord19(fade2, 0, mixer1, 1);
AudioConnection          patchCord20(fade3, 0, mixer1, 2);
AudioConnection          patchCord21(fade5, 0, mixer2, 0);
AudioConnection          patchCord22(fade6, 0, mixer2, 1);
AudioConnection          patchCord23(fade7, 0, mixer2, 2);
AudioConnection          patchCord24(fade8, 0, mixer2, 3);
AudioConnection          patchCord25(mixer1, 0, mixer3, 0);
AudioConnection          patchCord26(mixer2, 0, mixer3, 1);
AudioConnection          patchCord27(mixer3, 0, filter1, 0);
AudioConnection          patchCord28(amp9, freeverb1);
AudioConnection          patchCord29(freeverb1, 0, filter2, 0);
AudioConnection          patchCord30(filter1, 0, amp9, 0);
AudioConnection          patchCord31(filter2, 0, i2s1, 0);
AudioConnection          patchCord32(filter2, 0, i2s1, 1);
// GUItool: end automatically generated code

AudioControlSGTL5000 codec;

AsyncTimer t(20);

const int idPinsAmount = 4;
int idPins[] = { 2, 3, 4, 5 };

struct Player {
  AudioSynthWaveformSine *sine;
  AudioEffectFade *fade;
  void (*fadeOutFunc)();
  void (*fadeInFunc)();
};

Player players[8];

double f0 = 440.0; // A4 = 440hz
const double a = pow(2, 0.08333333333);
// fn = f0 * (a)^(n)


// just intonation ratios for chords. -1 means that there is no extra note
int chordTunings[8][4] = {
  { 4, 5, 6, -1 },          // Major Chord
  { 10, 12, 15, -1 },       // Minor Chord
  { 6, 8, 9, -1 },          // Sus Fourth Chord
  { 8, 9, 12, -1 },         // Sus Second Chord
  { 8, 10, 12, 15 },        // Major seventh chord
  { 10, 12, 15, 18 },       // Minor seventh chord
  { 4, 5, 6, 7 },           // Dominant seventh chord
  { 125, 150, 180, 216 }    // Dimished seventh chord
};

int chordIndex = 0;
int chordNoteIndex = 0;
bool chordFinished = true;

int noteFadeInTime = 3000;
int noteSustainTime = 2500;
int noteFadeOutTime = 2000;

int stepRange[2] = { -20, 20 }; // basically from C1 to C6

int curNoteStep = 20 - random(0, 40);

// 1 = randomly picked notes to start new chord
// 2 = next half step (wrap around stepRange) to start new chord
// 3 = next full step (wrap around stepRange) to start new chord
// 4 = next note in current chord (wrap around stepRange) to start new chord 
// 5 = go up an octave (wrap around stepRange) to start new chord
int chordProgressionStyle = 1;

// 1 = play chord notes from left to right
// 2 = play chord notes from right to left
// 3 = play chord notes in random order
// 4 = oscillate chord notes from left to right and right from left
// 5 = play chord notes all at once
int chordPlayStyle = 1;
void (*playChord[5])(double) = { &playChord1, &playChord2, &playChord3, &playChord4, &playChord5 };

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600); // for receiving values from ESP32
  AudioMemory(24);
  codec.enable();
  codec.volume(0.55);

  Serial.println("Setting up program");

  t.setup(); // async timer setup

  /* ESP32 Communication Setup */
  // setup id pins, to read ID of value to be changed
  for (int i = 0; i < idPinsAmount; i++) {
    pinMode(idPins[i], INPUT);
  }
  
  /* Sound Setup */
  // sine wave setup
  Serial.print("Sine wave setup...");
  players[0].sine = &sine1;
  players[1].sine = &sine2;
  players[2].sine = &sine3;
  players[3].sine = &sine4;
  players[4].sine = &sine5;
  players[5].sine = &sine6;
  players[6].sine = &sine7;
  players[7].sine = &sine8;
  Serial.println("Success!");

  // fade effect setup
  Serial.print("Fade Effect setup...");
  players[0].fade = &fade1;
  players[1].fade = &fade2;
  players[2].fade = &fade3;
  players[3].fade = &fade4;
  players[4].fade = &fade5;
  players[5].fade = &fade6;
  players[6].fade = &fade7;
  players[7].fade = &fade8;
  Serial.println("Success!");

  Serial.print("Fade out and in function setup...");
  players[0].fadeOutFunc = &fadeOutPlayer0;
  players[1].fadeOutFunc = &fadeOutPlayer1;
  players[2].fadeOutFunc = &fadeOutPlayer2;
  players[3].fadeOutFunc = &fadeOutPlayer3;
  players[4].fadeOutFunc = &fadeOutPlayer4;
  players[5].fadeOutFunc = &fadeOutPlayer5;
  players[6].fadeOutFunc = &fadeOutPlayer6;
  players[7].fadeOutFunc = &fadeOutPlayer7;

  players[0].fadeInFunc = &fadeInPlayer0;
  players[1].fadeInFunc = &fadeInPlayer1;
  players[2].fadeInFunc = &fadeInPlayer2;
  players[3].fadeInFunc = &fadeInPlayer3;
  players[4].fadeInFunc = &fadeInPlayer4;
  players[5].fadeInFunc = &fadeInPlayer5;
  players[6].fadeInFunc = &fadeInPlayer6;
  players[7].fadeInFunc = &fadeInPlayer7;
  Serial.println("Success!");

  for (int i = 0; i < 8; i++) {
    (*players[i].sine).frequency(0);
    (*players[i].sine).amplitude(0.5);
    (*players[i].fade).fadeOut(20);
  }

  // amps setup
  amp1.gain(0.6);
  amp2.gain(0.6);
  amp3.gain(0.6);
  amp4.gain(0.6);
  amp5.gain(0.6);
  amp6.gain(0.6);
  amp7.gain(0.6);
  amp8.gain(0.6);

  amp9.gain(0.2);

  // mixer setups
  mixer1.gain(0, 0.25);
  mixer1.gain(1, 0.25);
  mixer1.gain(2, 0.25);
  mixer1.gain(3, 0.25);

  mixer2.gain(0, 0.25);
  mixer2.gain(1, 0.25);
  mixer2.gain(2, 0.25);
  mixer2.gain(3, 0.25);

  mixer3.gain(0, 0.5);
  mixer3.gain(1, 0.5);
  
  // filter setup
  // filter1.frequency(700);
  // filter2.frequency(600);

  // freeverb setup
  freeverb1.roomsize(0.9);
  freeverb1.damping(0.7);

  curNoteStep = 20 - random(0, 40);
  delay(100);
}

void loop() {
  t.handle();
  valueControl();
  soundControl();
}

/* ESP32 Communication Functions */
void valueControl() {
  if (Serial1.available() > 0) {
    updateValue();
  }
}

// get id for data being sent by ESP32
byte getID() {
  byte id = 0;
  for (byte i = 0; i < idPinsAmount; i++) {
    byte nextVal = (digitalRead(idPins[i]) == HIGH) ? 1 : 0;
    id = (id << 1) + nextVal;
  }
  return id;
}

// get value for data being sent by ESP32
void updateValue() {
  // read data being sent by ESP32
  byte id = getID();
  int value = Serial1.read();
  
  Serial.print("ID: ");
  Serial.println(id);
  Serial.print("Value: ");
  Serial.println(value);
  Serial.println(" ");
  
  // actually change values
  // TODO
}

/* Sound Functions */
void soundControl() {
  // if the chord is finished, then start a new chord
  if (chordFinished) {
    chordFinished = false;
    f0 = random(420, 460);
    chordProgressionStyle = random(1, 6);
    chordPlayStyle = random(1, 6);
    chordIndex = random(0, 8);

    Serial.println("--------------------------------------------------");
    Serial.println("Starting a new chord");
    Serial.printf("Chord at [%i][%i] with values { %i, %i, %i, %i }", chordIndex, chordNoteIndex, chordTunings[chordIndex][0], chordTunings[chordIndex][1], chordTunings[chordIndex][2], chordTunings[chordIndex][3]);
    Serial.printf("Chord Progression Style: %i\n", chordProgressionStyle);
    Serial.printf("Chord Play Style: %i\n", chordPlayStyle);

    // this is a soft range limit i guess, but this accomplishes a soft wraparound for notes
    if (curNoteStep < stepRange[0] || curNoteStep > stepRange[1]) {
      curNoteStep = stepRange[0];
    }

    switch (chordProgressionStyle) {
      case 1: { // 1 = randomly picked notes to start new chord
        curNoteStep = 20 - random(0, 40);
        break;
      }
      case 2: { // 2 = next half step (wrap around stepRange) to start new chord
        curNoteStep += 1;
        break;
      }
      case 3: { // 3 = next full step (wrap around stepRange) to start new chord
        curNoteStep += 2;
        break;
      }
      case 4: { // 4 = next note in current chord (wrap around stepRange) to start new chord
        double multFactor = getFreq(curNoteStep) / (chordTunings[chordIndex][0]);
        double nextNoteFreq = chordTunings[chordIndex][1] * multFactor;
        while (getFreq(curNoteStep) < nextNoteFreq) {
          curNoteStep++;
        }
        break;
      }
      case 5: { // 5 = go up an octave (wrap around stepRange) to start new chord
        curNoteStep += 12;
        break;
      }
      default: {
        Serial.println("This should not happen!!!!!");
      }
    }
    Serial.printf("Chord should be this many halfsteps from f0: %i \n", curNoteStep);
    // Serial.println(curNoteStep);
    (*playChord[chordPlayStyle - 1])(getFreq(curNoteStep));
  }
}

double getFreq(int halfSteps) {
  return (f0 * pow(a, halfSteps));
}

// 1 = play chord notes from left to right
void playChord1(double startFreq) {
  Serial.printf("Playing chord from left to right, starting on frequency %f\n", startFreq);

  double multFactor = startFreq / (chordTunings[chordIndex][0]);
  for (int i = 0; i < 4; i++) {
    playNote(i, i * noteFadeInTime / 2, chordTunings[chordIndex][i] * multFactor);
  }
  t.setTimeout(resetChord, (5 * noteFadeInTime / 2) + noteSustainTime + noteFadeOutTime);
}

// 2 = play chord notes from right to left
void playChord2(double startFreq) {
  Serial.printf("Playing chord notes from right to left, leftmost frequency is %f\n", startFreq);

  double multFactor = startFreq / (chordTunings[chordIndex][0]);
  for (int i = 3; i > 0; i--) {
    playNote(i, i * noteFadeInTime / 2, chordTunings[chordIndex][i] * multFactor);
  }
  t.setTimeout(resetChord, (5 * noteFadeInTime / 2) + noteSustainTime + noteFadeOutTime);
}

// 3 = play chord notes in random order
void playChord3(double startFreq) {
  Serial.printf("Playing chord notes in random order, leftmost frequency is %f\n", startFreq);

  double multFactor = startFreq / (chordTunings[chordIndex][0]);

  bool alreadyPlayed[4] = { false, false, false, false };
  int numPlayed = 0;
  while (numPlayed < 4) {
    int index = random(0, 4);
    if (!alreadyPlayed[index]) {
      alreadyPlayed[index] = true;
      numPlayed++;
      playNote(index, (numPlayed - 1) * noteFadeInTime / 2, chordTunings[chordIndex][index] * multFactor);
    }
  }
  t.setTimeout(resetChord, (5 * noteFadeInTime / 2) + noteSustainTime + noteFadeOutTime);
}

// 4 = oscillate chord notes from left to right and right from left
void playChord4(double startFreq) {
  Serial.printf("Playing chord notes in oscillating fashion, leftmost frequency is %f\n", startFreq);

  double multFactor = startFreq / (chordTunings[chordIndex][0]);
  for (int i = 0; i < 4; i++) {
    playNote(i, i * noteFadeInTime / 2, chordTunings[chordIndex][i] * multFactor);
  }
  for (int i = 3; i > 0; i--) {
    playNote(4 + i, ((7-i) * noteFadeInTime / 2), chordTunings[chordIndex][i] * multFactor);
  }
  t.setTimeout(resetChord, (9 * noteFadeInTime / 2) + noteSustainTime + noteFadeOutTime);
}

// 5 = play chord notes all at once
void playChord5(double startFreq) {
  Serial.printf("Playing chord notes all at once, leftmost frequency is %f\n", startFreq);

  double multFactor = startFreq / (chordTunings[chordIndex][0]);
  for (int i = 0; i < 4; i++) {
    playNote(i, 0, chordTunings[chordIndex][i] * multFactor);
  }
  t.setTimeout(resetChord, 0 + noteFadeInTime + noteSustainTime + noteFadeOutTime);
}

void playNote(int sineIndex, int initialDelayTime, double freq) {
  // negative frequency means it's not supposed to be played
  if (freq <= 0.0) {
    return;
  }

  Serial.printf("After %i time, playing freq %f on sinewave %i for %i ms with additional fades %i ms and %i ms \n", initialDelayTime, freq, sineIndex, noteSustainTime, noteFadeInTime, noteFadeOutTime);
  
  // set sinewave frequency
  (*players[sineIndex].sine).frequency(freq);

  // fade in sinewave
  t.setTimeout((*players[sineIndex].fadeInFunc), initialDelayTime);

  // fade out sinewave
  t.setTimeout((*players[sineIndex].fadeOutFunc), initialDelayTime + noteFadeInTime + noteSustainTime);
}



void resetChord() {
  chordFinished = true;
}

void doNothing() {
  return;
}

// hacky way to get over the fact that i can't capture lambda function list
// fade out functions
void fadeOutPlayer0() {
  Serial.printf("Fading out player 0 for %i seconds\n", noteFadeOutTime);
  (*players[0].fade).fadeOut(noteFadeOutTime);
}

void fadeOutPlayer1() {
  Serial.printf("Fading out player 1 for %i seconds\n", noteFadeOutTime);
  (*players[1].fade).fadeOut(noteFadeOutTime);
}

void fadeOutPlayer2() {
  Serial.printf("Fading out player 2 for %i seconds\n", noteFadeOutTime);
  (*players[2].fade).fadeOut(noteFadeOutTime);
}

void fadeOutPlayer3() {
  Serial.printf("Fading out player 3 for %i seconds\n", noteFadeOutTime);
  (*players[3].fade).fadeOut(noteFadeOutTime);
}

void fadeOutPlayer4() {
  Serial.printf("Fading out player 4 for %i seconds\n", noteFadeOutTime);
  (*players[4].fade).fadeOut(noteFadeOutTime);
}

void fadeOutPlayer5() {
  Serial.printf("Fading out player 5 for %i seconds\n", noteFadeOutTime);
  (*players[5].fade).fadeOut(noteFadeOutTime);
}

void fadeOutPlayer6() {
  Serial.printf("Fading out player 6 for %i seconds\n", noteFadeOutTime);
  (*players[6].fade).fadeOut(noteFadeOutTime);
}

void fadeOutPlayer7() {
  Serial.printf("Fading out player 7 for %i seconds\n", noteFadeOutTime);
  (*players[7].fade).fadeOut(noteFadeOutTime);
}

// Fade in functions
void fadeInPlayer0() {
  Serial.printf("Fading in player 0 for %i seconds\n", noteFadeInTime);
  (*players[0].fade).fadeIn(noteFadeInTime);
}

void fadeInPlayer1() {
  Serial.printf("Fading in player 1 for %i seconds\n", noteFadeInTime);
  (*players[1].fade).fadeIn(noteFadeInTime);
}

void fadeInPlayer2() {
  Serial.printf("Fading in player 2 for %i seconds\n", noteFadeInTime);
  (*players[2].fade).fadeIn(noteFadeInTime);
}

void fadeInPlayer3() {
  Serial.printf("Fading in player 3 for %i seconds\n", noteFadeInTime);
  (*players[3].fade).fadeIn(noteFadeInTime);
}

void fadeInPlayer4() {
  Serial.printf("Fading in player 4 for %i seconds\n", noteFadeInTime);
  (*players[4].fade).fadeIn(noteFadeInTime);
}

void fadeInPlayer5() {
  Serial.printf("Fading in player 5 for %i seconds\n", noteFadeInTime);
  (*players[5].fade).fadeIn(noteFadeInTime);
}

void fadeInPlayer6() {
  Serial.printf("Fading in player 6 for %i seconds\n", noteFadeInTime);
  (*players[6].fade).fadeIn(noteFadeInTime);
}

void fadeInPlayer7() {
  Serial.printf("Fading in player 7 for %i seconds\n", noteFadeInTime);
  (*players[7].fade).fadeIn(noteFadeInTime);
}
