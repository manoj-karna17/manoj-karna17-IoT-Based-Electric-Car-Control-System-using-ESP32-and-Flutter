#Project Title: IoT-Based Electric Car Control System using ESP32 and Flutter

##Overview:
This project demonstrates a smart robotic car controlled wirelessly via a mobile application. It uses an ESP32 microcontroller and a Flutter-based mobile app for real-time control over Wi-Fi.

##Tools & Technologies
•	ESP32 Microcontroller (Arduino IDE)
•	L298N Motor Driver
•	Wi-Fi Module (built-in ESP32)
•	Flutter & Dart (for Android app)
•	Mobile Phone (control interface)

##How to Run
###Hardware Setup (ESP32 + Motor Driver)
1. Open `main_firmware.ino` from `/Code/ESP32_Firmware/` in Arduino IDE
2. Connect ESP32 and upload the sketch
3. ESP32 connects to Wi-Fi (SSID & Password inside code)
4. L298N driver controls motor via GPIOs
###Mobile App Setup (Flutter)
1. Go to `/Code/Mobile_App/`
2. Open in Android Studio / VS Code with Flutter installed
3. Run `flutter pub get`
4. Connect mobile to the same Wi-Fi as ESP32
5. Replace the ESP32 IP address in `main.dart` if needed
6. Build and run the app on your mobile


##Screenshots
Check the `/Images/` folder for:
•	`Circuit_Diagram.png`: Full wiring diagram
•	`Car_Photo.jpg`: Assembled robotic car
•	`App_Screenshot.jpg`: Control interface of mobile app

##Notes
•	Ensure mobile and ESP32 are on the **same Wi-Fi network**
•	WebSocket or HTTP is used depending on version
•	Range depends on Wi-Fi signal

##Timeline
•	Start Date: June 2025
•	Completion Date: July 2025
•	Status: ✅ Completed

