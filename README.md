# Chopper

A simple wave slicer for E/S Daisy POD.

Default tempo is set to 120 BPM (2Hz). Tempo's range is between 30 and 240 BPM.

Tempo can be set using tap tempo (button 2), MIDI clock, or groove box (Pocket Operator, Volca) sync.

Using buttons 3 through 6 and knobs 3 and 4 requires a [basic pod expander](https://github.com/KnightHill/daisy-basic-expander).

There is a ncurses-based [editor](https://github.com/KnightHill/chopper-pattern-editor) to help generate additional patterns.

Sync modes:
- Tap Tempo (LED1 shows WHITE)
- MIDI Clock (LED1 shows GOLD)
- [TE Pocket Operator](https://teenage.engineering/guides/po-33/en) - sync modes SY1, SY2, and SY5 (LED1 shows CYAN)
- [Korg Volca](https://www.korg-volca.com/en/) - requires 3.5mm TS to TRS splitter [cable](https://www.amazon.com/3-5mm-Stereo-Male-Plug-Cable/dp/B08XHBR8TJ?th=1) (LED1 shows CYAN)

Sync modes are mutually excusive and can only be changed while the effect is off. LED1 will show the color code of the current sync mode for one second after the change. 

# Controls

| Control | Description | Comment |
| --- | --- | --- |
| Encoder Turn | Change slicer pattern | pod |
| Encoder Press | Set tempo to 120 BPM (only when the effect is off)| pod |
| Button 1 | Slicer On/Off | pod |
| Button 2 | Tap tempo | pod |
| Button 3 | Reset pattern step | basic expansion board |
| Button 4 | Switch sync mode (only when the effect is off) | basic expansion board |
| Button 5 | Beat sync | basic expansion board |
| Button 6 | N/A | basic expansion board |

| Knob 1 | Set slicer PW ratio (10%-90%) | pod |
| Knob 2 | Dry/Wet Mix (20%-100%) | pod |
| Knob 3 | Attack (0.02 - 0.2) | basic expansion board |
| Knob 4 | N/A | basic expansion board |
| LED 1 | Pattern indicator | pod |
| LED 2 | Pattern indicator | pod |

# Patterns

<table>
<thead>
    <tr>
        <th>#</th>
        <th>LED1</th>
        <th>LED2</th>
        <th colspan="16">Steps</th>
    </tr>
</thead>
<tbody>

<tr>
<td>1</td>
<td>RED</td>
<td>RED</td>
<td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td>
</tr>

<tr>
<td>2</td>
<td>RED</td>
<td>GREEN</td>
<td>1</td><td>0</td><td>1</td><td>0</td><td>1</td><td>0</td><td>1</td><td>0</td><td>1</td><td>0</td><td>1</td><td>0</td><td>1</td><td>0</td><td>1</td><td>0</td>
</tr>

<tr>
<td>3</td>
<td>RED</td>
<td>BLUE</td>
<td colspan="2">1</td><td colspan="2">1</td><td colspan="2">1</td><td colspan="2">1</td><td colspan="2">1</td><td colspan="2">1</td><td colspan="2">1</td><td colspan="2">1</td>
</tr>

<tr>
<td>4</td>
<td>RED</td>
<td>MAGENTA</td>
<td>1</td><td>0</td><td>0</td><td>0</td><td>1</td><td>0</td><td>0</td><td>0</td><td>1</td><td>0</td><td>0</td><td>0</td><td>1</td><td>0</td><td>0</td><td>0</td>
</tr>

<tr>
<td>5</td>
<td>RED</td>
<td>CYAN</td>
<td colspan="4">1</td><td colspan="4">1</td><td colspan="4">1</td><td colspan="4">1</td>
</tr>

<tr>
<td>6</td>
<td>RED</td>
<td>GOLD</td>
<td>1</td><td>1</td><td>0</td><td>0</td><td>1</td><td>1</td><td>0</td><td>0</td><td>1</td><td>1</td><td>0</td><td>0</td><td>1</td><td>1</td><td>0</td><td>0</td>
</tr>

<tr>
<td>7</td>
<td>RED</td>
<td>WHITE</td>
<td>1</td><td>1</td><td>1</td><td>0</td><td>1</td><td>1</td><td>1</td><td>0</td><td>1</td><td>1</td><td>1</td><td>0</td><td>1</td><td>1</td><td>1</td><td>0</td>
</tr>

<tr>
<td>8</td>
<td>GREEN</td>
<td>RED</td>
<td colspan="2">1</td><td>1</td><td>1</td>
<td colspan="2">1</td><td>1</td><td>1</td>
<td colspan="2">1</td><td>1</td><td>1</td>
<td colspan="2">1</td><td>1</td><td>1</td>
</tr>

<tr>
<td>9</td>
<td>GREEN</td>
<td>GREEN</td>
<td colspan="4">1</td><td colspan="2">1</td><td colspan="2">1</td>
<td colspan="4">1</td><td colspan="2">1</td><td colspan="2">1</td>
</tr>

<tr>
<td>10</td>
<td>GREEN</td>
<td>BLUE</td>
<td>1</td><td>1</td><td colspan="2">1</td>
<td>1</td><td>1</td><td colspan="2">1</td>
<td>1</td><td>1</td><td colspan="2">1</td>
<td>1</td><td>1</td><td colspan="2">1</td>
</tr>

<tr>
<td>11</td>
<td>GREEN</td>
<td>MAGENTA</td>
<td colspan="2">1</td><td colspan="2">1</td><td colspan="4">1</td>
<td colspan="2">1</td><td colspan="2">1</td><td colspan="4">1</td>
</tr>

<tr>
<td>12</td>
<td>GREEN</td>
<td>CYAN</td>
<td>1</td><td>1</td><td>1</td><td>1</td>
<td>1</td><td>1</td><td colspan="2">1</td>
<td>1</td><td>1</td><td>1</td><td>1</td>
<td>1</td><td>1</td><td colspan="2">1</td>
</tr>

<tr>
<td>13</td>
<td>GREEN</td>
<td>GOLD</td>
<td colspan="2">1</td><td>1</td><td>1</td>
<td>1</td><td>1</td><td>1</td><td>1</td>
<td colspan="2">1</td><td>1</td><td>1</td>
<td>1</td><td>1</td><td>1</td><td>1</td>
</tr>

<tr>
<td>14</td>
<td>GREEN</td>
<td>WHITE</td>
<td colspan="2">1</td><td>1</td><td>0</td>
<td>1</td><td>0</td><td>1</td><td>1</td>
<td colspan="2">1</td><td>1</td><td>1</td>
<td>1</td><td>1</td><td>1</td><td>1</td>
</tr>

<tr>
<td>15</td>
<td>BLUE</td>
<td>RED</td>
<td colspan="2">1</td><td>1</td><td>1</td>
<td>0</td><td>1</td><td>1</td><td>1</td>
<td colspan="2">1</td><td>1</td><td>1</td>
<td>0</td><td>1</td><td>0</td><td>1</td>
</tr>

<tr>
<td>16</td>
<td>BLUE</td>
<td>GREEN</td>
<td colspan="2">1</td><td>0</td><td>1</td>
<td>1</td><td>0</td><td>1</td><td>1</td>
<td colspan="2">0</td><td>1</td><td>1</td>
<td>0</td><td>1</td><td>0</td><td>1</td>
</tr>

</tbody>
</table>

# Testing

Sync tested with KeyStep 37, PO-33 and Volca Kick

# TODO

- Sync the oscillator with the selected sync mode clicks
- Improve core engine
- Full MIDI control (program change, CC for PW and mix)
- ~~Add 3 separate sync modes (tap tempo, MIDI Clock, PO Sync)~~
- ~~Change tap tempo/MIDI clock to use uSec resolution~~
- ~~Add PO sync support~~
- ~~Implement pattern editor~~
- ~~Add Attack control~~
- ~~Remove base expander code from libDaisy~~
- ~~Fix clicking~~ 
- ~~EG/ADSR~~
- ~~Dry/Wet mix~~
- ~~test complex patterns (4+8+16)~~
- ~~tap tempo~~
- ~~MIDI Clock~~
