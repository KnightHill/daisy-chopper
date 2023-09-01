# Chopper

A simple wave slicer for E/S Daisy POD.

Default tempo is set to 120 BPM (2Hz). Tempo settings are:

- 120 BPM - LED 1 at 60% intensity
- 140 BPM - LED 1 at 100% intensity
- 80 BPM - LED 1 at 30% intensity

Using buttons 3 and 4 requires a [basic pod expander](https://github.com/KnightHill/daisy-basic-expander).

# Patterns

| Pattern No | LED 2 Color | Step 1 | Step 2 | Step 3 | Step 4 |
| --- | --- | --- | --- | --- | --- |
| 1 | RED | 1 | 1 | 1 | 1 |
| 2 | GREEN | 1 | 0 | 1 | 0 |
| 3 | BLUE | 1 | 0 | 0 | 0 |
| 4 | DARK | 0 | 0 | 0 | 0 |
| 5 | MAGENTA | 1 | 1 | 0 | 0 |
| 6 | CYAN | 1 | 1 | 1 | 0 |
| 7 | GOLD | 1 | 0 | 0 | 1 |
| 8 | WHITE | 0 | 1 | 1 | 0 |


# Controls


| Control | Description | Comment |
| --- | --- | --- |
| Button 1 | Slicer On/Off | pod |
| Button 2 | Change slicer pattern | pod |
| Button 3 | Reset pattern step | basic expansion board |
| Button 4 | Set tempo (120 -> 140 -> 80 BPM) | basic expansion board |
| Knob 1 | Set slicer PW ratio (10%-90%) | pod |
| Knob 2 | N/A | pod |
| LED 1 | Slicer On/Off, Intensity shows tempo | pod |
| LED 2 | Current pattern | pod |