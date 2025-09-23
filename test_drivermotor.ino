// --- Encoder pins ---
#define ENCA 2  // Pin encoder A (kuning)
#define ENCB 3  // Pin encoder B (hijau)

// --- Motor driver (L298N) pins untuk Motor B ---
#define PWM 5  // ENB - PWM control
#define in3 6  // IN3 - Arah motor
#define in4 7  // IN4 - Arah motor

// --- Encoder dan kontrol posisi ---
volatile int pos = 0;                  // Posisi saat ini dari motor (pulsa encoder)
const int pos_one_rotation = 11 * 57;  // 1 rotasi poros motor = 11 rotasi encoder = 627 pulsa

// --- PID variables ---
long prevT = 0;       // Waktu sebelumnya
float eprev = 0;      // Error sebelumnya
float eintegral = 0;  // Akumulasi error (untuk integral)

unsigned long prevtime = 0;

// --- Setup function ---
void setup() {
  // Setup pin output untuk motor
  pinMode(PWM, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Setup pin input untuk encoder
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);  // Interrupt saat ENCA naik

  // Mulai Serial Monitor / Plotter
  Serial.begin(9600);
}

// --- Main loop ---
void loop() {
  int target = 25;  // Target = 1 putaran gearbox (627 pulsa)

  // PID gains (bisa diubah untuk tuning)
  float kp = 3.0;
  float ki = 0.0;
  float kd = 10.0;

  // Hitung waktu (dalam detik)
  long currT = micros();
  float deltaT = (float)(currT - prevT) / 1.0e6;  // konversi ke detik
  prevT = currT;

  // Hitung error posisi
  int e = target - pos;

  // Hitung turunan error (kecepatan perubahan error)
  float dedt = (e - eprev) / deltaT;

  // Hitung integral error (untuk akumulasi)
  eintegral += e * deltaT;

  // Hitung sinyal kontrol PID
  float u = kp * e + kd * dedt + ki * eintegral;

  // Tentukan arah motor (1 = CW, -1 = CCW, 0 = stop)
  int dir = u > 0 ? 1 : -1;

  // Konversi sinyal PID ke PWM (0–255)
  float pwr = fabs(u);
  if (pwr > 255) pwr = 255;
  else if (pwr > 0 && pwr < 60) pwr = 60;  // Minimum PWM agar motor bisa bergerak

  // Jalankan motor
  setMotor(dir, pwr);

  // Simpan error sebelumnya untuk perhitungan selanjutnya
  eprev = e;


  // --- Serial Plotter output ---
  if (millis() - prevtime >= 20 && millis()<1000) {
    Serial.print(" ");
    Serial.print(target);
    Serial.print(" ");
    Serial.print(pos);
    Serial.print(" ");
    Serial.print(0);  // referensi bawah
    Serial.print(" ");
    Serial.println(200);
    // Serial.print(" ");
    // Serial.println(pwr);
    prevtime = millis();
  }
}

// --- Fungsi untuk mengatur motor (arah & kecepatan) ---
void setMotor(int dir, int pwmVal) {
  analogWrite(PWM, pwmVal);
  if (dir == 1) {
    // Clockwise
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  } else if (dir == -1) {
    // Counter-clockwise
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  } else {
    // Stop
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
}

// --- Interrupt Service Routine untuk membaca encoder ---
void readEncoder() {
  if (digitalRead(ENCB))
    pos++;
  else
    pos--;
}
