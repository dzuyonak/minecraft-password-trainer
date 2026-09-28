<p align="center">
  <a href="README.md"><strong>🇷🇺 Русский</strong></a> &nbsp;|&nbsp;
  <strong>🇬🇧 English</strong> &nbsp;|&nbsp;
  <a href="README.pl.md"><strong>🇵🇱 Polski</strong></a>
</p>

<p align="center">
  <img src="assets/characters/superprize.png" width="420" alt="Login & Password Trainer in Minecraft Style Banner" />
</p>

<h1 align="center">🎮 Login & Password Trainer in Minecraft Style</h1>

<p align="center">
  <strong>An interactive educational game in authentic Minecraft style, designed to help children easily and joyfully memorize their personal account login and password.</strong>
</p>

<p align="center">
  <a href="https://github.com/dzuyonak/password-trainer-minecraft/actions/workflows/build.yml">
    <img src="https://github.com/dzuyonak/password-trainer-minecraft/actions/workflows/build.yml/badge.svg" alt="CI Build Status" />
  </a>
  <img src="https://img.shields.io/badge/Platform-Windows%207%2B%20%7C%20Web%20%7C%20macOS-2ea44f?style=flat&logo=windows" alt="Platform" />
  <img src="https://img.shields.io/badge/Languages-EN%20%7C%20RU%20%7C%20PL-brightgreen?style=flat" alt="Languages" />
  <img src="https://img.shields.io/badge/Size-<1%20MB%20(Standalone)-blue?style=flat" alt="Size" />
  <img src="https://img.shields.io/badge/License-MIT-orange?style=flat" alt="License" />
  <img src="https://img.shields.io/badge/Theme-Minecraft%20GUI-5c8a32?style=flat" alt="Theme" />
</p>

---

## 📑 Table of Contents

- [✨ About the Project](#-about-the-project)
- [⚡ Quick Start](#-quick-start)
- [🚀 Key Features](#-key-features)
- [⚙️ Configuration Guide](#️-configuration-guide)
  - [Parameters Reference](#parameters-reference)
  - [For Windows (config.ini)](#for-windows-configini)
  - [For Web (in Browser)](#for-web-in-browser)
- [🔒 Data Security & Windows SmartScreen Notice](#-data-security--windows-smartscreen-notice)
- [🎮 Gameplay Flow](#-gameplay-flow)
- [🦁 Character Collection](#-character-collection)
- [📁 Architecture & Repository Structure](#-architecture--repository-structure)
- [🛠️ Building from Source](#️-building-from-source)
- [☁️ CI/CD & GitHub Pages](#️-cicd--github-pages)
- [🤝 Contributing](#-contributing)
- [📄 License & Legal Disclaimer](#-license--legal-disclaimer)

---

## ✨ About the Project

Many children struggle when logging into their Minecraft accounts: they forget uppercase letters, confuse similar symbols, or panic when passwords are masked with dots.

**Login & Password Trainer in Minecraft Style** transforms the routine practice of memorizing credentials into a rewarding game:
- Children practice typing their login and password in the familiar, beloved Minecraft interface.
- The password is **visible by default**, allowing kids to see what they type and develop reliable finger muscle memory.
- Every correct submission unlocks a brand-new collectible Minecraft mob without duplicates.
- Collecting all 15 mobs triggers a grand **Superprize** celebration featuring the Ender Dragon, victory fanfare, and the Golden Trophy of Mastery!

---

## ⚡ Quick Start

| Platform | Link | Description | Requirements |
| :--- | :--- | :--- | :--- |
| **Windows** | 📥 [**Download MinecraftPasswordTrainer.exe**](https://github.com/dzuyonak/password-trainer-minecraft/raw/main/MinecraftPasswordTrainer.exe) | Standalone executable (~1 MB). Zero setup, double-click to run! | Windows 7, 8, 10, 11 (64-bit) |
| **Online (Web)** | 🌐 [**Open in Browser (GitHub Pages)**](https://dzuyonak.github.io/password-trainer-minecraft/?lang=en) | Play instantly from any smartphone, tablet, iPad, or PC without downloading. | Modern web browser |
| **macOS / Linux** | `open index.html` or `python3 app.py` | Local web runner or lightweight Python server with live `config.ini` sync. | Python 3.8+ (optional) |

---

## 🚀 Key Features

1. **📦 Zero Dependencies (Single Executable)**:
   - Compiled into a single lightweight binary of just **~1 MB**.
   - All 15 character artworks, the Ender Dragon sprite, sound effects, and Diamond icon are embedded inside the executable.
   - Requires no installation of Python, Java, .NET Framework, or third-party DLLs.

2. **🌍 1-Click Multilingual Support (EN / RU / PL)**:
   - Supports three full languages: **English** 🇬🇧, **Russian** 🇷🇺, and **Polish** 🇵🇱.
   - Toggle languages on the fly right in the header bar: `[ EN ] [ RU ] [ PL ]`.
   - Comprehensive localization of all dialogs, labels, error hints, superprize lore, and mob descriptions.

3. **👁️ Plain Text Password by Default**:
   - The password field is readable upon launch, which is essential for early learners building typing confidence.
   - Interactive eye toggle `[ 👁 ]` / `[ 🔒 ]` allows switching between masked bullets and plain text at any moment.

4. **🎯 Kid-Friendly Scaled Interface (Stacked Layout)**:
   - Inputs, buttons, and fonts are enlarged by 1.5x for effortless readability on all screens.
   - Focus is set on the login field automatically at startup.
   - Pressing `Enter` smoothly moves the cursor from login to password and submits.

5. **⭐ 15 Collectible Mobs with Guaranteed Progress**:
   - Smart anti-duplicate algorithm: every correct login unlocks a random uncollected mob from the remaining pool.
   - Live counter: *«⭐ Characters unlocked: 7 of 15 (8 left)»*.

6. **🏆 Grand Victory Superprize**:
   - Unlocking all 15 characters reveals the victory screen:
     - Golden celebratory banner.
     - Artwork of the **Ender Dragon** in deep End magic particles.
     - Achievement: *«[ LEGENDARY MINECRAFT MASTER ]»*.
     - Award: *«🥇 Golden Minecraft Trophy!»*.
     - Triumphant orchestral fanfare sound.
     - Fast reset button to start a new practice adventure anytime.

7. **🎵 Audio Atmosphere**:
   - XP level-up chime on success.
   - Grand triumph fanfare on complete collection.
   - Gentle warning sound on error with highlighted hint box.

---

## ⚙️ Configuration Guide

### Parameters Reference

| Parameter | Values | Default | Description |
| :--- | :--- | :--- | :--- |
| `login` | Any string | `steve` | Correct player login to check against |
| `password` | Any string | `diamond` | Correct player password to check against |
| `language` | `en`, `ru`, `pl` | `en` | Interface language |
| `show_password` | `1` or `0` | `1` | `1` = show password as plain text by default, `0` = mask with dots |
| `show_hint` | `1` or `0` | `1` | `1` = show practice hint box at bottom, `0` = hide |

---

### For Windows (`config.ini`)

Alongside `MinecraftPasswordTrainer.exe`, the application reads `config.ini`:

```ini
[Credentials]
; Set child's login and password here:
login = steve
password = diamond

[Options]
; Interface language: en (English), ru (Russian), pl (Polish)
language = en
; 1 = show password as plain text by default, 0 = mask with dots
show_password = 1
; 1 = display reminder hint at bottom, 0 = hide
show_hint = 1
```

> [!TIP]
> You can edit `config.ini` in Notepad while the application is running — changes are loaded automatically on the next login attempt!

---

### For Web (in Browser)

1. Click the **`[ ⚙️ Settings ]`** button in the top right corner.
2. In the dialog, customize login, password, language, and visibility preferences.
3. Click **`💾 SAVE`**. Settings are persisted in `localStorage`.

> [!NOTE]
> When launched via `python3 app.py`, web changes are synchronized automatically back to `config.ini`.

---

## 🔒 Data Security & Windows SmartScreen Notice

### 1. Plaintext Storage & Password Caution
> [!CAUTION]
> **Do not use real master credentials!**  
> Credentials are saved locally on your device in plain text (in `config.ini` on Windows and in `localStorage` in web browsers) to allow transparent offline practice and easy configuration.  
> **Recommendation:** Use a dedicated practice password or a local launcher password. Never input master credentials for parent email accounts, Microsoft/Xbox accounts linked to payment methods, or sensitive financial services.

### 2. Windows Defender SmartScreen & Antivirus False Positives
`MinecraftPasswordTrainer.exe` is a free, non-commercial open-source project and is not signed with an expensive commercial code-signing certificate (EV Certificate).

Because the binary is built with the Zig compiler and contains interactive login and password input fields, heuristic scanning in **Windows Defender SmartScreen** or web browsers may show an untrusted publisher warning or trigger a false-positive phishing/keylogger heuristic.

**How to run on Windows:**
1. When the blue window appears stating *"Windows protected your PC"* (SmartScreen):
2. Click **"More info"**.
3. Click **"Run anyway"**.
4. *(Alternatively)* You can practice directly in your browser via [GitHub Pages](https://dzuyonak.github.io/password-trainer-minecraft/) or build the binary directly from source code (`./build.sh`).

---

## 🎮 Gameplay Flow

```
   ┌────────────────────────────────────────────────────────┐
   │ 1. LOGIN SCREEN                                        │
   │    • Enter login & password                            │
   │    • Eye button to toggle masking                      │
   │    • Practice hint at the bottom                       │
   └──────────────────────────┬─────────────────────────────┘
                              │
                  (Correct credentials entered)
                              │
   ┌──────────────────────────▼─────────────────────────────┐
   │ 2. CHARACTER UNLOCKED (1 of 15)                        │
   │    • XP level-up sound                                 │
   │    • New mob card (artwork, class badge, lore)         │
   │    • Guaranteed progress counter (no duplicates!)      │
   └──────────────────────────┬─────────────────────────────┘
                              │
                (All 15 characters unlocked)
                              │
   ┌──────────────────────────▼─────────────────────────────┐
   │ 3. SUPERPRIZE: ENDER DRAGON                            │
   │    • Victorious fanfare sound                          │
   │    • Golden Minecraft Trophy presentation              │
   │    • Play Again reset button                           │
   └────────────────────────────────────────────────────────┘
```

---

## 🦁 Character Collection

| Icon | Character | Class | Description |
| :---: | :--- | :--- | :--- |
| 🧑 | **Steve** | Main Hero | Legendary explorer and builder of the Minecraft world! Ready for any adventure. |
| 🏹 | **Alex** | Main Heroine | Brave adventurer with a bow and pickaxe. Survival master in wild biomes! |
| 💥 | **Creeper** | Dangerous Mob | Sss... Boom! The most iconic Minecraft mob. But it is utterly terrified of cats! |
| 👁️ | **Enderman** | End Wanderer | Tall dweller of the End. Teleports instantly and loves picking up and moving blocks! |
| 🧟 | **Zombie** | Night Monster | Dangerous in dark caves and at night, but burns immediately in the morning sun! |
| 💀 | **Skeleton** | Deadeye Archer | Skilled archer with a bone bow. Hides under tree shadows or in water by day. |
| 🦾 | **Iron Golem** | Village Protector | Mighty giant forged from iron blocks. Bravely defends villagers and offers red poppies! |
| 🐺 | **Tamed Wolf** | Loyal Companion | Your most loyal companion! Feed him a bone and he will bravely protect you from any danger. |
| 🐷 | **Pig** | Passive Animal | Adorable little piggy! Put a saddle on and hold a carrot on a stick for a fun ride! |
| 🐮 | **Cow** | Passive Animal | Provides nutritious milk in a bucket that cures any negative potion effects instantly! |
| 🐑 | **Sheep** | Passive Animal | Fluffy sheep! Use its soft wool to craft a cozy bed to safely sleep through the night. |
| 🛡️ | **Warden** | Ancient Guardian | Fearsome blind guardian of the Deep Dark! Senses every footstep and whisper through vibrations. |
| 🐝 | **Bee** | Busy Helper | Cute buzzing bee! Pollinates crops, collects sweet flower nectar, and fills beehives with honey. |
| 🌊 | **Axolotl** | Aquatic Friend | Adorable resident of lush caves. Assists in underwater battles and plays dead to regenerate! |
| 🦊 | **Fox** | Agile Creature | Loves sweet sweetberries, pounces high to catch prey, and curls up cozily to sleep in the snow. |

---

## 📁 Architecture & Repository Structure

```text
├── MinecraftPasswordTrainer.exe   # Standalone Windows executable (~1 MB)
├── index.html                     # Multilingual interactive Web app (HTML5 / Vanilla JS)
├── config.ini                     # Settings for Windows EXE (credentials, language, hints)
├── config.js                      # Settings for Web edition
├── app.py                         # Local Python runner with live config.ini synchronization
├── main.cpp                       # Native Win32 C++ source code (GDI+ rendering, double buffering)
├── generate_assets.py             # Script embedding binary PNGs, WAVs, and multilingual metadata
├── assets_data.h                  # Generated C++ header with embedded assets
├── build.sh                       # Quick build script using Zig compiler
├── assets/                        # Raw assets
│   ├── characters/                # 15 character artworks + superprize (PNG)
│   ├── sounds/                    # Audio: success.wav, fanfare.wav, error.wav
│   └── app_icon.ico               # Diamond icon
├── .github/
│   └── workflows/
│       └── build.yml              # CI/CD: Automated cloud build on GitHub Actions
├── LICENSE                        # MIT License
├── README.md                      # Russian documentation (Русский)
├── README.en.md                   # English documentation (English)
└── README.pl.md                   # Polish documentation (Polski)
```

---

## 🛠️ Building from Source

The project cross-compiles using **Zig** (produces PE32+ Windows executables directly from macOS, Linux, or Windows without MSVC or Wine):

### 1. Build via script:
```bash
./build.sh
```

### 2. Manual build:
```bash
# 1. Generate assets header:
python3 generate_assets.py

# 2. Compile native Windows binary:
zig c++ -target x86_64-windows-gnu \
  -Wl,--subsystem,windows \
  -O2 \
  main.cpp \
  -lgdiplus -lole32 -lwinmm -luser32 -lgdi32 -lcomctl32 -lshlwapi \
  -o MinecraftPasswordTrainer.exe
```

---

## ☁️ CI/CD & GitHub Pages

1. **Automated Cloud Builds (GitHub Actions)**:
   Whenever changes are pushed to `main`, [.github/workflows/build.yml](.github/workflows/build.yml) compiles `MinecraftPasswordTrainer.exe` and uploads it to GitHub Actions Artifacts.

2. **GitHub Pages Web Hosting**:
   - In your repository, go to **Settings** ➔ **Pages**.
   - Under **Branch**, select `main` and folder `/(root)`.
   - Click **Save**.
   - Accessible worldwide at: `https://dzuyonak.github.io/password-trainer-minecraft/`.

---

## 🤝 Contributing

Contributions, issues, and feature requests are welcome!
1. Fork the Project.
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`).
3. Commit your Changes (`git commit -m 'feat: Add some AmazingFeature'`).
4. Push to the Branch (`git push origin feature/AmazingFeature`).
5. Open a **Pull Request**.

---

## 📄 License & Legal Disclaimer

### License
The source code of this project (`.cpp`, `.py`, `.js`, `.html`, and build scripts) is distributed under the **MIT License**. See [LICENSE](LICENSE) for details.

### Third-Party Assets & Trademarks Notice
> [!IMPORTANT]
> **NOT AN OFFICIAL MINECRAFT PRODUCT. NOT APPROVED BY OR ASSOCIATED WITH MOJANG OR MICROSOFT.**  
> Minecraft is a registered trademark of Mojang AB / Microsoft Corporation. All Minecraft brand assets, character names, mob imagery, and sound effects remain the exclusive intellectual property of Mojang AB and Microsoft Corporation, used herein under fair-use educational and fan guidelines. This project is not affiliated with or endorsed by Mojang or Microsoft.
