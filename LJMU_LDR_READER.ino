const int NUM_SENSORS = 5;
const int NUM_SAMPLES = 256;

const int DEBUG = 0;

const float DIFF_PCT_THRESHOLD = 0.1;

int sensorPins[NUM_SENSORS] = {A0, A1, A2, A3, A4};
long baseVals[NUM_SENSORS] = {0, 0, 0, 0, 0};

void setup() {
  Serial.begin(57600);

  for (int i = 0; i < NUM_SAMPLES; i++) {
    for (int s = 0; s < NUM_SENSORS; s++) {
      baseVals[s] += analogRead(sensorPins[s]);
      delay(1);
    }
  }
  Serial.print("\nB: ");
  for (int s = 0; s < NUM_SENSORS; s++) {
    baseVals[s] /= NUM_SAMPLES;
    Serial.print(baseVals[s]);
    Serial.print(" ");
  }
  Serial.print("\n");
  delay(1000);
}

void loop() {
  float maxDiffPct = 0;
  int maxDiffSensor = NUM_SENSORS;

  for (int s = 0; s < NUM_SENSORS; s++) {
    float vdp = float(abs(analogRead(sensorPins[s]) - baseVals[s])) / float(baseVals[s]);
    if (vdp > maxDiffPct) {
      maxDiffPct = vdp;
      maxDiffSensor = s;
    }
    delay(1);
  }

  if (maxDiffPct > DIFF_PCT_THRESHOLD) {
    Serial.write(0xFF & 0xDE);
    Serial.write(0xFF & 0xAD);
    Serial.write(maxDiffSensor & 0xFF);

    if (DEBUG) {
      Serial.print("T: ");
      Serial.print(maxDiffSensor);
      Serial.print(" ");
      Serial.print(maxDiffPct);
      Serial.print("\n");
      delay(2000);
    }
  }
  else if (DEBUG) {
    Serial.print("M: ");
    Serial.print(maxDiffSensor);
    Serial.print(" ");
    Serial.print(maxDiffPct);
    Serial.print("\n");
    delay(1);
  }
}

