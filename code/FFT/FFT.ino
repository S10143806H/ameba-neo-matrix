#include "AudioCodec.h"
#include "FFT.h"

#define SAMPLERATE 16000
#define SAMPLECOUNT 64

int16_t audio_buffer[SAMPLECOUNT] = {0};
float fft_buffer[SAMPLECOUNT / 2] = {0};
uint16_t freq_bins[SAMPLECOUNT / 2] = {0};
int i = 0;

FFT fft;

void setup() {
  Serial.begin(2000000);

  fft.setWindow(HANN, SAMPLECOUNT);
  fft.getFrequencyBins(freq_bins, SAMPLECOUNT, SAMPLERATE);

  for (i = 0; i < (SAMPLECOUNT / 2); i++) {
    Serial.print(freq_bins[i]);
    Serial.print(" Hz | ");
  }
  Serial.println();

  Codec.setSampleRate(SAMPLERATE);
  Codec.setAMicBoost(3, 3);
  Codec.setADCGain(0x7f, 0x7f);
  Codec.begin(TRUE, FALSE);
}

void loop() {
  if (Codec.readAvaliable()) {
    Codec.readDataPage(audio_buffer, SAMPLECOUNT);    // read latest received data from buffer
    fft.calculate(audio_buffer, fft_buffer, SAMPLECOUNT);
    for (i = 0; i < (SAMPLECOUNT / 2); i++) {
      // set y-axis init point
      if (fft_buffer[i] > 0.01) {
        fft_buffer[i] *= 100; // max 16
        if (fft_buffer[i] <= 2) {
          Serial.print("1");
          // set heigth of x-axis
        }
        else if (fft_buffer[i] <= 4) {
          Serial.print("2");
        }
        else if (fft_buffer[i] <= 6) {
          Serial.print("3");
        }
        else if (fft_buffer[i] <= 8) {
          Serial.print("4");
        }
        else if (fft_buffer[i] <= 10) {
          Serial.print("5");
        }
        else if (fft_buffer[i] <= 12) {
          Serial.print("6");
        }        
        else if (fft_buffer[i] <= 14) {
          Serial.print("7");
        }
        else if (fft_buffer[i] <= 16) {
          Serial.print("8");
        }
        else {
          Serial.print("-");
        }
        Serial.print(" | ");
      } 
    }
    Serial.println();
  }
  delay(1);
}
