#define ENCA 2 // KUNING
#define ENCB 3 // HIJAU

volatile int pos = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);
}

void loop() {
  // Tampilkan nilai encoder di Serial Monitor
  Serial.print("Posisi: ");
  Serial.println(pos);
  Serial.print("Rotasi Encoder: ");
  Serial.println(pos/11);

  // Tampilkan sinyal ENCA dan ENCB di Serial Plotter
  int encaVal = digitalRead(ENCA);
  int encbVal = digitalRead(ENCB);

  //Serial.print("ENCA:");
  //Serial.print(encaVal);
  //Serial.print("\tENCB:");
  //Serial.println(encbVal);

  delay(50); // Plot stabil di Serial Plotter
}

void readEncoder() {
  // Deteksi arah berdasarkan ENCB
  if (digitalRead(ENCB)) {
    pos++;
  } else {
    pos--;
  }
}
