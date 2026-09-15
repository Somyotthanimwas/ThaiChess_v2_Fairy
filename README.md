# ThaiChess v2 Fairy ♞

เกมหมากรุกไทย (Thai Chess) สำหรับ Windows พัฒนาด้วย **C++17** และ **SDL2** พร้อมระบบ AI สำหรับเล่นกับคอมพิวเตอร์ และรองรับแนวคิด **Fairy Chess / Fairy Pieces** ผ่าน Fairy Engine ของโปรเจกต์

โปรเจกต์นี้เป็นเวอร์ชัน `ThaiChess_v2_Fairy` และจัดโครงสร้างด้วย **CMake** เพื่อให้สามารถ build ได้อย่างเป็นระบบและนำไปสร้าง executable สำหรับ Windows ได้

## ✨ Features

- 🇹🇭 เล่นหมากรุกไทยบน Windows
- 🧠 AI สำหรับการเล่นกับคอมพิวเตอร์
- ♟️ ระบบตัวหมากและกติกาหมากรุกไทย แยกเป็นคลาส เช่น King, Rook, Knight, Khon, Met และ Pawn
- 🧚 Fairy Engine สำหรับรองรับ/พัฒนาความสามารถของ Fairy Chess
- ♞ รองรับ Chess Engine / Stockfish integration ในโปรเจกต์
- 💾 บันทึกเกมผ่าน `SaveGame`
- 🖥️ GUI สร้างด้วย SDL2
- 🔤 รองรับข้อความภาษาไทยผ่าน SDL2_ttf
- 🖼️ รองรับรูปภาพและ asset ผ่าน SDL2_image
- 📦 รองรับการติดตั้งและสร้าง ZIP package ด้วย CPack
- 🚀 มี launcher สำหรับ Windows เพื่อเปิด `ThaiChessApp.exe` พร้อม runtime DLL
- 🐴 มี Windows application icon จาก `ThaiChess.rc`

## 🛠️ Technology Stack

| Component | Technology |
|---|---|
| Language | C++17 |
| Build System | CMake 3.20+ |
| Graphics / GUI | SDL2 |
| Font Rendering | SDL2_ttf |
| Image Loading | SDL2_image |
| Package Manager | vcpkg |
| AI / Engine | Custom AI + Stockfish components |
| Platform | Windows |

## 📁 Project Structure

```text
ThaiChess_v2_Fairy/
├── assets/                 # รูปภาพ ฟอนต์ และไฟล์ประกอบเกม
├── include/                # Header files
├── src/
│   ├── main.cpp            # จุดเริ่มต้นของเกม
│   ├── Board.cpp           # กระดานและการจัดการเกม
│   ├── Piece.cpp            # Base piece
│   ├── King.cpp             # ขุน
│   ├── Rook.cpp             # เรือ
│   ├── Knight.cpp           # ม้า
│   ├── Khon.cpp             # โคน
│   ├── Met.cpp              # เม็ด
│   ├── Pawn.cpp             # เบี้ย
│   ├── AI.cpp               # ระบบ AI
│   ├── Stockfish.cpp        # ส่วนเชื่อมต่อ Stockfish
│   ├── FairyEngine.cpp      # Fairy Chess Engine
│   ├── ChessGUI.cpp         # GUI ของเกม
│   ├── SaveGame.cpp         # บันทึกเกม
│   ├── TestEngine.cpp       # ทดสอบ Engine
│   ├── launcher.cpp/        # Windows launcher
│   │   └── launcher.cpp
│   └── ThaiChess.rc         # Windows application icon / resource
├── CMakeLists.txt
├── mingw-toolchain.cmake
├── vcpkg.json
└── vcpkg-configuration.json
```

## 📋 Requirements

สำหรับการ build แนะนำให้ใช้:

- Windows 10/11
- C++17 compiler
- CMake 3.20 หรือใหม่กว่า
- MinGW-w64 หรือ compiler ที่รองรับ C++17
- vcpkg
- SDL2
- SDL2_ttf
- SDL2_image

Dependencies หลักของโปรเจกต์ถูกระบุไว้ใน `vcpkg.json`

```json
{
  "dependencies": [
    "sdl2",
    "sdl2-ttf",
    "sdl2-image"
  ]
}
```

## 🔧 Build

ตัวอย่างการ build ด้วย CMake + vcpkg:

```bash
cmake -S . -B build \
  -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake

cmake --build build --config Release
```

หากใช้ MinGW toolchain ของโปรเจกต์ สามารถระบุ toolchain file ได้ เช่น:

```bash
cmake -S . -B build \
  -DCMAKE_TOOLCHAIN_FILE=mingw-toolchain.cmake

cmake --build build --config Release
```

> ตำแหน่ง `vcpkg` และ compiler อาจแตกต่างกันตามเครื่องที่ใช้ build

## ▶️ Running

หลังจาก build สำเร็จ โปรเจกต์จะสร้าง executable หลัก:

```text
ThaiChessApp.exe
```

และมี launcher:

```text
ThaiChess.exe
```

Launcher จะเตรียม `runtime` DLL path และเปิด `ThaiChessApp.exe` จากโฟลเดอร์เดียวกัน

โครงสร้างสำหรับแจกโปรแกรมควรมีลักษณะประมาณนี้:

```text
ThaiChess/
├── ThaiChess.exe
├── ThaiChessApp.exe
├── runtime/
└── assets/
```

## 🧩 CMake Targets

โปรเจกต์มี executable หลัก 2 ตัว:

- `ThaiChessApp` — ตัวเกมหลัก
- `ThaiChess` — Windows launcher สำหรับเริ่มเกม

`ThaiChessApp` เชื่อมต่อกับ:

```text
SDL2
SDL2_ttf
SDL2_image
```

และ copy โฟลเดอร์ `assets` ไปยัง directory ของ executable หลัง build โดยอัตโนมัติ

## 🧠 Engine

โปรเจกต์ประกอบด้วยส่วนของ AI และ Engine หลายส่วน ได้แก่:

- `AI.cpp` — ระบบ AI ของเกม
- `Stockfish.cpp` — ส่วนที่เกี่ยวข้องกับ Stockfish
- `FairyEngine.cpp` — Engine สำหรับแนวคิด Fairy Chess
- `TestEngine.cpp` — ส่วนสำหรับทดสอบ Engine

โครงสร้างนี้ช่วยให้สามารถพัฒนาระบบการเล่นและทดลองกติกาหรือรูปแบบตัวหมากเพิ่มเติมได้ในอนาคต

## 💾 Save Game

ระบบบันทึกเกมอยู่ใน:

```text
src/SaveGame.cpp
```

ใช้สำหรับพัฒนาความสามารถในการบันทึกและนำเกมกลับมาเล่นต่อ

## 🎯 Project Goal

เป้าหมายของโปรเจกต์คือการพัฒนาเกมหมากรุกไทยบน Windows ที่มี GUI ใช้งานง่าย พร้อม AI และโครงสร้าง Engine ที่สามารถต่อยอดไปสู่การทดลองรูปแบบการเล่นและตัวหมากแบบ Fairy Chess ได้

## 📌 Project Status

โปรเจกต์อยู่ระหว่างการพัฒนา (**Work in Progress**)

ฟังก์ชันและรายละเอียดของเกมอาจมีการเปลี่ยนแปลงในระหว่างการพัฒนา

## 👤 Author

**Somyot Thanimwas**

GitHub: [Somyotthanimwas](https://github.com/Somyotthanimwas)

## 📄 License

ขณะนี้ repository ยังไม่ได้ระบุ License อย่างเป็นทางการ

หากต้องการนำโค้ดไปใช้งานหรือเผยแพร่ต่อ กรุณาตรวจสอบสิทธิ์การใช้งานกับผู้พัฒนาโครงการก่อน
