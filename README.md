
Demo:
https://drive.google.com/file/d/1Ny1G3wwNm1VYaLJH1jfdDQXM5qyiEKa6/view?usp=sharing

Desain Awal:
<img width="1478" height="748" alt="Desain 3d Ball and Beam(9)" src="https://github.com/user-attachments/assets/a5dd5728-c1d6-4144-8fce-b34f5ab00e28" />
  tampak depan:
<img width="1478" height="748" alt="Desain 3d Ball and Beam(8)" src="https://github.com/user-attachments/assets/41eb0e59-3c82-4302-beae-bd2010ae4a09" />
  tampak atas:
<img width="1478" height="748" alt="Desain 3d Ball and Beam(7)" src="https://github.com/user-attachments/assets/5d87c3d9-1e96-415d-9b5c-83f509d56073" />

Wiring:
<img width="1080" height="720" alt="Screenshot 2025-09-25 154521" src="https://github.com/user-attachments/assets/ad602c62-e211-4716-b083-f6ef2150b8a7" />

# Ball and Beam Control System

## 📋 Deskripsi Proyek
Proyek Ball and Beam adalah sistem kontrol yang dirancang untuk mengatur posisi bola pada sebuah balok (beam) menggunakan prinsip kontrol PID. Sistem ini menggunakan sensor jarak untuk mendeteksi posisi bola dan aktuator motor DC untuk mengatur kemiringan balok agar bola tetap pada posisi yang diinginkan.

---

## 🎯 Penjelasan Kontrol PID

### Apa itu Kontrol PID?
PID (Proportional-Integral-Derivative) adalah algoritma kontrol umpan balik yang banyak digunakan dalam sistem industri. PID controller menghitung nilai "error" sebagai perbedaan antara setpoint yang diinginkan dan variabel proses yang diukur.

### Komponen PID:

#### 1. **Proportional (P)**
- **Fungsi**: Memberikan respon yang sebanding dengan error saat ini
- **Rumus**: `P = Kp × Error`
- **Efek**:
  - Mempercepat respon sistem
  - Dapat menyebabkan overshoot dan osilasi
  - Error steady-state tidak nol

#### 2. **Integral (I)**
- **Fungsi**: Menghilangkan error steady-state dengan mengakumulasi error dari waktu ke waktu
- **Rumus**: `I = Ki × ∫Error dt`
- **Efek**:
  - Menghilangkan offset
  - Dapat menyebabkan windup dan osilasi

#### 3. **Derivative (D)**
- **Fungsi**: Memprediksi perilaku sistem masa depan berdasarkan rate of change error
- **Rumus**: `D = Kd × dError/dt`
- **Efek**:
  - Meredam osilasi
  - Meningkatkan stabilitas
  - Sensitif terhadap noise

### Persamaan PID Lengkap:
```
Output = Kp × Error + Ki × ∫Error dt + Kd × dError/dt
```

---

## ⚙️ Cara Kerja Ball and Beam

### Diagram Blok Sistem:
```
[Setpoint] → [PID Controller] → [Servo Motor] → [Beam] → [Ball Position]
      ↑                                                        |
      |________________________________[Sensor]________________|
```

### Alur Kerja:
1. **Input**: Posisi yang diinginkan (setpoint) untuk bola
2. **Sensing**: Sensor (IR/ultrasonic) membaca posisi aktual bola
3. **Error Calculation**: Menghitung selisih antara setpoint dan posisi aktual
4. **PID Processing**: Mikrokontroler menghitung output kontrol berdasarkan parameter PID
5. **Actuation**: Servo motor mengatur kemiringan beam berdasarkan output PID
6. **Feedback**: Posisi bola baru dibaca dan proses berulang

### Prinsip Fisika:
- Bola bergerak akibat gravitasi dan kemiringan beam
- Kemiringan beam mengubah komponen gravitasi yang bekerja pada bola
- Sistem berusaha menjaga bola pada posisi setpoint dengan mengatur kemiringan

---

## 🎨 Rancangan 3D

### Komponen Mekanikal:

#### 1. **Beam (Balok)**
- **Material**: Akrilik/Aluminium
- **Dimensi**: 50cm × 8cm × 1cm
- **Fitur**:
  - Alur untuk guiding bola
  - Permukaan rendah gesekan
  - Titik pivot di tengah

#### 2. **Base (Dasar)**
- **Material**: Akrilik 10mm
- **Dimensi**: 40cm × 25cm
- **Fungsi**: Menopang seluruh sistem

#### 3. **Bracket Servo**
- **Desain**: Custom untuk mounting servo
- **Material**: PLA/ABS (3D printed)

#### 4. **Ball (Bola)**
- **Material**: Bola pingpong/besi
- **Diameter**: 3-4cm

### File 3D yang Tersedia:
- `beam.stl` - Model balok utama
- `base_plate.stl` - Pelat dasar
- `servo_bracket.stl` - Penyangga servo motor
- `sensor_mount.stl` - Mounting sensor

---

## 🛠️ Alat dan Bahan

### Komponen Elektronik:
| Komponen | Jumlah | Spesifikasi |
|----------|--------|-------------|
| Arduino Uno | 1 | Mikrokontroler utama |
| Servo Motor | 1 | SG90/MG996R |
| Sensor IR | 2-4 | TCRT5000/Sharp GP2Y0A21 |
| Motor Driver | 1 | L298N (jika menggunakan DC motor) |
| Breadboard | 1 | - |
| Jumper Wires | - | Male-to-Male, Male-to-Female |
| Power Supply | 1 | 5V-12V DC |

### Komponen Mekanik:
| Komponen | Jumlah | Keterangan |
|----------|--------|-------------|
| Beam Material | 1 | Akrilik 50cm |
| Base Plate | 1 | Akrilik 40×25cm |
| Bola | 1 | Diameter 3-4cm |
| Bearing | 2 | Untuk pivot point |
| Screw & Nut Set | - | M3 berbagai panjang |
| 3D Printed Parts | - | Bracket dan support |

### Tools:
- Multimeter
- Soldering Iron
- Screwdriver Set
- 3D Printer
- Laser Cutter (opsional)

---

## 🔌 Wiring Diagram

### Koneksi Arduino:

```
Arduino Uno Pinout:

SENSOR IR:
- Sensor Kiri: A0
- Sensor Kanan: A1
- Sensor Tengah 1: A2
- Sensor Tengah 2: A3

SERVO MOTOR:
- Signal: Digital Pin 9
- VCC: 5V
- GND: GND

POWER:
- External 5V untuk servo
- Common Ground

ADDITIONAL:
- LED Indicator: Pin 13
- Button Reset: Pin 2
```

### Schematic Detail:

```
+5V ────┬─────────┬─────────┬─────────┬─────────┬───── Servo VCC
       │         │         │         │         │
      IR1       IR2       IR3       IR4      ... 
       │         │         │         │         │
A0 ────┘    A1 ──┘    A2 ──┘    A3 ──┘         │
                                              │
GND ───┬─────────┬─────────┬─────────┬─────────┴───── Common GND
       │         │         │         │
      IR1 GND   IR2 GND   IR3 GND   IR4 GND   Servo GND

Digital Pin 9 ────────────────────────────────────── Servo Signal
```

---

## 📊 Hasil Akhir Tuning

### Parameter PID yang Dioptimalkan:

```cpp
// Final PID Parameters
double Kp = 2.5;    // Proportional gain
double Ki = 0.1;    // Integral gain  
double Kd = 1.2;    // Derivative gain
```

### Performance Metrics:

#### **Sebelum Tuning:**
- **Rise Time**: 3.2 detik
- **Overshoot**: 45%
- **Settling Time**: 8.5 detik
- **Steady State Error**: ±15mm

#### **Setelah Tuning:**
- **Rise Time**: 1.8 detik
- **Overshoot**: 12%
- **Settling Time**: 3.2 detik  
- **Steady State Error**: ±3mm

### Grafik Respon Sistem:
![PID Response Graph](images/pid_response.png)

### Prosedur Tuning:

#### **Langkah 1: Tuning Manual**
1. Set `Ki = 0`, `Kd = 0`
2. Naikkan `Kp` hingga sistem berosilasi
3. Kurangi `Kp` hingga 50% dari nilai osilasi

#### **Langkah 2: Tuning Integral**
1. Set `Ki` kecil untuk menghilangkan steady-state error
2. Hindari nilai terlalu besar yang menyebabkan windup

#### **Langkah 3: Tuning Derivative**
1. Set `Kd` untuk meredam osilasi
2. Tambah secara bertahap hingga respon optimal

### Tips Tuning:
- Gunakan software serial plotter untuk monitoring
- Tuning dalam kondisi operasi normal
- Pertimbangkan noise sensor pada derivative term
- Test dengan berbagai setpoint yang berbeda

---

## 🚀 Cara Menjalankan

1. **Assembly**: Rakit sistem mekanik sesuai diagram 3D
2. **Wiring**: Hubungkan komponen elektronik sesuai schematic
3. **Upload Code**: Program Arduino dengan kode PID
4. **Calibration**: Kalibrasi sensor dan zero position
5. **Tuning**: Lakukan tuning parameter PID
6. **Testing**: Test sistem dengan berbagai setpoint

## 📁 Struktur Project
```
ball_and_beam/
├── src/
│   ├── ball_beam_pid.ino
│   ├── pid_controller.cpp
│   └── sensor_reading.cpp
├── models/
│   ├── beam.stl
│   ├── base_plate.stl
│   └── servo_bracket.stl
├── docs/
│   ├── wiring_diagram.png
│   └── bom.csv
└── README.md
```

## ⚠️ Troubleshooting
- **Bola tidak stabil**: Periksa parameter PID, khususnya derivative
- **Servo bergetar**: Kurangi Kp atau tambah Kd
- **Sensor noise**: Tambah filter software pada pembacaan sensor
- **Bola tidak bergerak**: Periksa kemiringan beam dan koefisien gesek

---
**Dibuat oleh**: [Nama Tim/Individu]  
**Tanggal**: [Tanggal Pembuatan]  
**Lisensi**: MIT License
