# smart-door-lock-system
# 🔐 Smart Door Lock System (Arduino Mega 2560 + ESP32-CAM)

## 🇮🇩 Deskripsi

Smart Door Lock System adalah sistem keamanan pintu berbasis embedded system yang menggabungkan **Arduino Mega 2560** dan **ESP32-CAM**. Sistem ini menggunakan multi autentikasi seperti fingerprint dan password keypad, serta dilengkapi dengan kamera untuk monitoring jarak jauh melalui Telegram.

Sistem ini dirancang untuk meningkatkan keamanan dengan fitur akses real-time dan notifikasi berbasis IoT.

---

## 🇬🇧 Description

Smart Door Lock System is an embedded-based door security system that combines **Arduino Mega 2560** and **ESP32-CAM**. It uses multi-factor authentication such as fingerprint and keypad password, and includes a camera module for remote monitoring via Telegram.

This system is designed to enhance security with real-time access control and IoT-based notifications.

---

## 🚀 Fitur / Features

* 🔑 Fingerprint authentication
* ⌨️ Password input via keypad
* 📷 Photo capture using ESP32-CAM
* 📲 Send photo to Telegram
* 🔔 Buzzer as alert system
* 🖥️ LCD display (I2C)
* 🔍 IR sensor (deteksi keberadaan orang)
* 🔄 Stepper motor sebagai pengunci pintu
* ⚡ Relay control

---

## 🧠 Cara Kerja Sistem / System Workflow

### 🇮🇩

1. Sistem standby dan menunggu input
2. IR sensor mendeteksi keberadaan orang
3. User dapat:

   * Scan fingerprint
   * atau memasukkan password
4. Jika valid → pintu terbuka (motor aktif)
5. ESP32-CAM dapat mengambil foto dan mengirim ke Telegram
6. Jika tidak valid → buzzer aktif dan akses ditolak

### 🇬🇧

1. System stays in standby mode
2. IR sensor detects human presence
3. User can:

   * Scan fingerprint
   * or enter password
4. If valid → door unlocks (motor activates)
5. ESP32-CAM captures and sends photo to Telegram
6. If invalid → buzzer alerts and access denied

---

## 🧩 Komponen / Components

### 🇮🇩

* Arduino Mega 2560
* ESP32-CAM
* Fingerprint Sensor
* Keypad 4x4
* LCD I2C 16x2
* Stepper Motor + Driver
* Relay Module
* Buzzer
* IR Sensor

### 🇬🇧

* Arduino Mega 2560
* ESP32-CAM
* Fingerprint Sensor
* 4x4 Keypad
* I2C LCD 16x2
* Stepper Motor + Driver
* Relay Module
* Buzzer
* IR Sensor

---

## ⚙️ Arsitektur Sistem / System Architecture

* Arduino Mega:

  * Mengontrol sensor dan aktuator (fingerprint, keypad, motor, relay)
* ESP32-CAM:

  * Mengambil gambar dan mengirim ke Telegram melalui WiFi

---

## 📡 Integrasi IoT / IoT Integration

Sistem menggunakan Telegram Bot sebagai media komunikasi:

* Mengambil foto jarak jauh
* Monitoring sistem
* Kontrol sederhana melalui command

---

## 🛠️ Cara Menjalankan / How to Run

### 🇮🇩

1. Upload kode ke Arduino Mega
2. Upload kode ESP32-CAM
3. Hubungkan semua komponen sesuai rangkaian
4. Konfigurasi WiFi dan Telegram Bot
5. Jalankan sistem

### 🇬🇧

1. Upload code to Arduino Mega
2. Upload ESP32-CAM code
3. Connect all components according to the wiring diagram
4. Configure WiFi and Telegram Bot
5. Run the system

---

## 🔐 Catatan Keamanan / Security Notes

### 🇮🇩

* Jangan menyimpan SSID, password, dan BOT token di repository publik
* Gunakan file konfigurasi terpisah jika diperlukan

### 🇬🇧

* Do not store WiFi credentials and BOT token in public repositories
* Use separate configuration files if needed

---

## 📁 Struktur Project / Project Structure

```
smart-door-lock-system/
 ├── arduino-mega/
 │    └── smart_door_lock.ino
 ├── esp32-cam/
 │    └── telegram_camera.ino
 └── README.md
```

---

## 👨‍💻 Author

**Fatih Muhammad Khalid**

---

## ⭐ Penutup / Closing

Project ini merupakan implementasi nyata dari sistem keamanan berbasis IoT dan embedded system yang dapat dikembangkan lebih lanjut menjadi:

* Smart Home System
* Smart Security System
* Industrial Automation System

---
