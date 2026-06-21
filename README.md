# StrikeZone

A console-based arcade shooter built in C++ for Windows. Defend Earth from waves of hostile drones across 4 increasingly difficult missions.

## Story

The year is 2157. Hostile alien drones have entered orbit, and you are humanity's last line of defense. As the planetary defense pilot, you must eliminate all threats before time runs out.

## Gameplay

- Move left/right and shoot down incoming drones before your time runs out (or, in the final level, before you run out of ammo).
- Each level has its own difficulty curve: drone count, ammo limits, time limits, and drone lifespan all change.

| Level | Mission | Target Kills | Time Limit | Ammo |
|-------|---------|---------------|------------|------|
| 1 | First Wave | 15 | 30s | Unlimited |
| 2 | Orbital Disturbance | 20 | 40s | Unlimited |
| 3 | Ammunition Crisis | 25 | 50s | 40 bullets |
| 4 | Last Stand | 30 | None | 40 bullets |

## Controls

| Key | Action |
|-----|--------|
| ← / → | Move |
| SPACE | Shoot |
| ESC | Pause menu |

## Requirements

- Windows OS (uses `windows.h`, `conio.h`, and `winmm.lib` for console graphics and sound)
- A C++ compiler with Windows API support (e.g. MinGW or MSVC)
- Sound files `shoot.wav`, `win.wav`, and `lose.wav` in the same directory as the executable

## Building

Using g++ (MinGW):
```bash
g++ StrikeZone.cpp -o StrikeZone.exe -lwinmm
```

Then run:
```bash
StrikeZone.exe
```

## Author

Built by [aishanaeem-x](https://github.com/aishanaeem-x) as a semester project.
