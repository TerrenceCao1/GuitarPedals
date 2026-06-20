# Digital Guitar Pedals

A modular digital guitar pedal platform built on the STM32F103C8T6 microcontroller. The goal is a single hardware enclosure that can be reflashed to produce different effects.

---

## Effects Firmware

| Effect | Status |
|---|---|
| Bypass (clean passthrough) | Software Tested |
| Boost | Planned |
| Overdrive / Distortion | Planned |
| Delay | Planned |
| Chorus | Planned |
| Reverb | Planned |
| Tremolo | Planned |
| Tuner | Planned |
| Looper | Planned |

---

## Hardware

### Signal Chain

```
Guitar → 1/4" input jack → input protection → op-amp buffer → ADC (PCM1802)
                                                                      ↓
Amp    ← 1/4" output jack ←   output cap    ← DAC (PCM5102A) ← STM32 via I2S
```

### Power

Powered via USB through the STM32 Blue Pill's onboard regulator. No external PSU needed.

| Rail | Source | Powers |
|---|---|---|
| 3.3V | Blue Pill `3V3` pin | STM32, PCM1802, PCM5102A |
| 5V | Blue Pill `5V` pin | MCP6002 op-amp |

### Components

| Component | Part | Status | Link |
|---|---|---|---|
| Microcontroller | STM32F103C8T6 (Blue Pill) | Have | — |
| ADC | PCM1802 | Ordered | [Amazon](https://www.amazon.com/PCM1808-Single-Ended-Analog-Input-Decoder-Amplifier/dp/B0D9LNGBD1) |
| DAC | PCM5102A breakout | Ordered | [Amazon](https://www.amazon.com/ACEIRMC-Lossless-Digital-Converter-Raspberry/dp/B08YNJGSN4) |
| Op-amp | MCP6002 (rail-to-rail, 3.3V/5V) | Ordered | [Amazon](https://www.amazon.com/MICROCHIP-MCP6002-I-OP-AMP-DIP-8-pieces/dp/B00LPJXQJW) |
| Input/output jacks | GLS 1/4" female panel jacks × 2 | Ordered | [Amazon](https://www.amazon.com/GLS-Audio-Jacks-Female-Panel/dp/B00CMXRLXM) |
| Input protection | 1MΩ resistor + 10nF capacitor | Ordered | [Resistors](https://www.amazon.com/ALLECIN-8W-Resistor-Film-Assortment/dp/B0BTP6WYH1) / [Caps](https://www.amazon.com/Multilayer-Monolithic-Capacitor-Assortment-Electronics/dp/B0D2GVNDHY) |
| Decoupling caps | 100nF ceramic × 8, 10µF electrolytic × 2 | Needed | — |
| Breadboard + jumper wires | Full-size 830-tie | Have | — |

---

## Roadmap

- [ ] Breadboard MVP — get audio passing through STM32 via I2S
- [ ] Implement and test simulations (pure software on computer)
- [ ] Test effects on firmware
- [ ] Wire potentiometers for live parameter control
- [ ] Design schematic and PCB
- [ ] Design and fabricate enclosure
