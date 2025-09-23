#include "NewPing.h"

// --- Encoder pins ---
#define ENCA 2  // Kuning: pin A encoder
#define ENCB 3  // Hijau: pin B encoder

// --- Motor driver pins ---
#define PWM 5  // Hitam: PWM speed control
#define in3 6  // Merah: arah motor
#define in4 7  // Coklat: arah motor

// --- Sensor ultrasonic referensi (target posisi bola) ---
#define TRIG_REF A3
#define ECHO_REF A2
NewPing refsensor(TRIG_REF, ECHO_REF, 30);  // Sensor target (maks. jarak 30cm)

// --- Sensor ultrasonic posisi bola sebenarnya ---
#define TRIG_BALL A1
#define ECHO_BALL A0
NewPing ballsensor(TRIG_BALL, ECHO_BALL, 30);  // Sensor aktual (maks. jarak 30cm)

// --- Variabel encoder ---
volatile int pos = 0;                  // Posisi motor berdasarkan encoder
const int pos_one_rotation = 11 * 57;  // 627 pulse per rotasi

// --- Target posisi motor dari PID luar ---
float motor_target = 0;

// --- Variabel PID dalam (motor) ---
long prevT = 0;       // Waktu sebelumnya (untuk deltaT)
float eprev = 0;      // Error sebelumnya
float eintegral = 0;  // Akumulasi error

// --- Variabel PID luar (bola) ---
float eintegral_ball = 0;  // Akumulasi error posisi bola
float eprev_ball = 0;      // Error posisi bola sebelumnya

// --- Untuk filter pembacaan sensor ---
unsigned long prevtime = 0;
int last_valid_ball_reading = 0;

void setup() {
  // Inisialisasi pin motor
  pinMode(PWM, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Inisialisasi pin encoder
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);

  // Mulai komunikasi serial
  Serial.begin(9600);
}

void loop() {
  pidOuterLoop();
  pidInnerLoop();
  delay(30);  // Delay agar sistem tidak terlalu cepat polling
}

// === PID Outer Loop: kontrol posisi bola ===
void pidOuterLoop() {
  // Baca posisi target dan posisi bola saat ini
  int ball_target = refsensor.ping_cm();  // target = jarak referensi - offset
  int ball_pos = getFilteredBallReading(ballsensor);

  int e_ball = ball_target - ball_pos;  // Error posisi bola

  // --- PID gains (tuning sesuai kebutuhan) ---
  float kp_ball = 5.0;
  float ki_ball = 3.0;
  float kd_ball = 0.5;

  // Hitung waktu antar iterasi
  static unsigned long lastTime = 0;
  unsigned long now = millis();
  float deltaT = (now - lastTime) / 1000.0;
  lastTime = now;

  // Hitung integral (area error)
  eintegral_ball += e_ball * deltaT;

  //Anti-windup: batasi nilai integral
  float integralMax = 100;
  if (eintegral_ball > integralMax) eintegral_ball = integralMax;
  else if (eintegral_ball < -integralMax) eintegral_ball = -integralMax;

  // Hitung derivatif (laju perubahan error)
  float dedt_ball = (e_ball - eprev_ball) / deltaT;
  eprev_ball = e_ball;

  // Hitung output PID luar → target posisi motor
  motor_target = kp_ball * e_ball + ki_ball * eintegral_ball + kd_ball * dedt_ball;

  // Batasi motor target agar tidak berlebihan
  if (motor_target > 150) motor_target = 150;
  else if (motor_target < -150) motor_target = -150;

  // Debug: tampilkan info posisi bola, error, dan output
  if (millis() - prevtime >= 100) {
    // Serial.print("[BallPID] Target: ");
    Serial.print(ball_target);
    Serial.print("\t");
    Serial.print(ball_pos);
    Serial.print("\t");
    Serial.print(0);
    Serial.print("\t");
    Serial.println(24);
    // Serial.print(" | Motor_Target: ");
    // Serial.println(motor_target);
    prevtime = millis();
  }
}

// === PID Inner Loop: kontrol posisi motor (encoder) ===
void pidInnerLoop() {
  // PID gains untuk motor (tuning sesuai karakteristik motor)
  float kp_motor = 3.0;
  float ki_motor = 1.0;
  float kd_motor = 0.0;

  // Hitung waktu antar iterasi
  long currT = micros();
  float deltaT = (float)(currT - prevT) / 1.0e6;
  prevT = currT;

  // Hitung error posisi motor (dari encoder)
  float e_motor = motor_target - pos;

  // Hitung integral
  eintegral += e_motor * deltaT;

  // Anti-windup
  float integralMax = 300;
  if (eintegral > integralMax) eintegral = integralMax;
  if (eintegral < -integralMax) eintegral = -integralMax;

  // Hitung derivatif
  float dedt = (e_motor - eprev) / deltaT;
  eprev = e_motor;

  // Hitung output PID (u) sebagai sinyal kontrol
  float u = kp_motor * e_motor + ki_motor * eintegral + kd_motor * dedt;

  // Tentukan arah dan kecepatan motor berdasarkan u
  int dir = 0;
  if (u > 0) dir = 1;
  else if (u < 0) dir = -1;

  float pwm = fabs(u);  // Konversi u ke nilai PWM positif

  // Batasi nilai PWM agar cukup kuat dan tidak melebihi batas
  if (pwm > 255) pwm = 255;
  else if (pwm > 0 && pwm < 60) pwm = 60;  // Dead zone minimum PWM (Motor DC bekerja pada 3V-12V)

  // Jalankan motor
  setMotor(dir, pwm);
}

// === Fungsi untuk mengatur motor ===
void setMotor(int dir, int pwmVal) {
  analogWrite(PWM, pwmVal);
  if (dir == 1) {
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  } else if (dir == -1) {
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  } else {
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
}

// === Fungsi interrupt encoder ===
void readEncoder() {
  if (digitalRead(ENCB))
    pos++;
  else
    pos--;
}

// === Fungsi pembacaan sensor dengan filtering sederhana ===
int getFilteredBallReading(NewPing &sensor) {
  int reading = sensor.ping_cm();
  if (reading == 0 && last_valid_ball_reading > 5) {
    return last_valid_ball_reading;
  }
  last_valid_ball_reading = reading;
  return reading;
}
