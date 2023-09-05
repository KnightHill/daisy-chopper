# Chopper

A simple wave slicer for E/S Daisy POD.

Default tempo is set to 120 BPM (2Hz). Tempo's range is between 60 and 180 BPM.

Using buttons 3 and 4 requires a [basic pod expander](https://github.com/KnightHill/daisy-basic-expander).

# Patterns

| Pattern | LED2 | S1 | S2 | S3 | S4 | S5 | S6 | S7 | S8 | S9 | S10 | S11 | S12 | S13 | S14 | S15 | S16 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |--- | --- | --- | --- |--- | --- | --- | --- |
| 1 | RED | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 |
| 2 | GREEN | 1 | 0 | 1 | 0 | 1 | 0 | 1 | 0 | 1 | 0 | 1 | 0 | 1 | 0 | 1 | 0 |
| 3 | BLUE | 1 | 0 | 0 | 0 | 1 | 0 | 0 | 0 | 1 | 0 | 0 | 0 | 1 | 0 | 0 | 0 |
| 4 | BLACK | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| 5 | MAGENTA | 1 | 1 | 0 | 0 | 1 | 1 | 0 | 0 | 1 | 1 | 0 | 0 | 1 | 1 | 0 | 0 |
| 6 | CYAN | 1 | 1 | 1 | 0 | 1 | 1 | 1 | 0 | 1 | 1 | 1 | 0 | 1 | 1 | 1 | 0 |
| 7 | GOLD | 1 | 0 | 0 | 1 | 1 | 0 | 0 | 1 | 1 | 0 | 0 | 1 | 1 | 0 | 0 | 1 |
| 8 | WHITE | 0 | 1 | 1 | 0 | 0 | 1 | 1 | 0 | 0 | 1 | 1 | 0 | 0 | 1 | 1 | 0 |


# Controls

| Control | Description | Comment |
| --- | --- | --- |
| Encoder Turn | Change slicer pattern | pod |
| Encoder Press | Set tempo to 120 BPM | pod |
| Button 1 | Slicer On/Off | pod |
| Button 2 | Increase tempo | pod |
| Button 3 | Decrease tempo | basic expansion board |
| Button 4 | Reset pattern step | basic expansion board |
| Knob 1 | Set slicer PW ratio (10%-90%) | pod |
| Knob 2 | N/A | pod |
| Seed LED | Slicer On/Off | seed |
| LED 1 | N/A | pod |
| LED 2 | Current pattern | pod |