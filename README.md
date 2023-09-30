# Chopper

A simple wave slicer for E/S Daisy POD.

Default tempo is set to 120 BPM (2Hz). Tempo's range is between 30 and 240 BPM.

Tempo can be set using tap tempo (button 2) or MIDI clock.

Using buttons 3 through 6 and knobs 3 and 4 requires a [basic pod expander](https://github.com/KnightHill/daisy-basic-expander).

# Patterns

<table>
<thead>
    <tr>
        <th>Pattern</th>
        <th>LED2</th>
        <th colspan="16">Steps</th>
    </tr>
</thead>
<tbody>
<tr>
<td>1</td><td>RED</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td>
</tr>
<tr>
<td>2</td><td>GREEN</td><td>1</td><td>0</td><td>1</td><td>0</td><td>1</td><td>0</td><td>1</td><td>0</td><td>1</td><td>0</td><td>1</td><td>0</td><td>1</td><td>0</td><td>1</td><td>0</td>
</tr>
<tr>
<td>3</td><td>BLUE</td><td colspan="2">1</td><td colspan="2">1</td><td colspan="2">1</td><td colspan="2">1</td><td colspan="2">1</td><td colspan="2">1</td><td colspan="2">1</td><td colspan="2">1</td>
</tr>

<tr>
<td>4</td><td>MAGENTA</td><td>1</td><td>0</td><td>0</td><td>0</td><td>1</td><td>0</td><td>0</td><td>0</td><td>1</td><td>0</td><td>0</td><td>0</td><td>1</td><td>0</td><td>0</td><td>0</td>
</tr>

<tr>
<td>5</td><td>CYAN</td><td colspan="4">1</td><td colspan="4">1</td><td colspan="4">1</td><td colspan="4">1</td>
</tr>

<tr>
<td>6</td><td>BLACK</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td>
</tr>

<tr>
<td>7</td><td>GREEN</td><td>1</td><td>1</td><td>0</td><td>0</td><td>1</td><td>1</td><td>0</td><td>0</td><td>1</td><td>1</td><td>0</td><td>0</td><td>1</td><td>1</td><td>0</td><td>0</td>
</tr>

<tr>
<td>8</td><td>BLUE</td><td>1</td><td>1</td><td>1</td><td>0</td><td>1</td><td>1</td><td>1</td><td>0</td><td>1</td><td>1</td><td>1</td><td>0</td><td>1</td><td>1</td><td>1</td><td>0</td>
</tr>

<tr>
<td>9</td><td>MAGENTA</td>

<td colspan="2">1</td><td>1</td><td>1</td>
<td colspan="2">1</td><td>1</td><td>1</td>
<td colspan="2">1</td><td>1</td><td>1</td>
<td colspan="2">1</td><td>1</td><td>1</td>
</tr>

<tr>
<td>10</td><td>CYAN</td>
<td colspan="4">1</td><td colspan="2">1</td><td colspan="2">1</td>
<td colspan="4">1</td><td colspan="2">1</td><td colspan="2">1</td>
</tr>
</tbody>
</table>

# Controls

| Control | Description | Comment |
| --- | --- | --- |
| Encoder Turn | Change slicer pattern | pod |
| Encoder Press | Set tempo to 120 BPM | pod |
| Button 1 | Slicer On/Off | pod |
| Button 2 | Tap tempo | pod |
| Button 3 | Reset pattern step | basic expansion board |
| Knob 1 | Set slicer PW ratio (10%-90%) | pod |
| Knob 2 | Dry/Wet Mix (20%-100%) | pod |
| Seed LED | Slicer status (On/Off) | seed |
| LED 1 | N/A | pod |
| LED 2 | Current pattern | pod |

# TODO

- EG/ADSR
- Mix Dry/Wet
- ~~test complex patterns (4+8+16)~~
- ~~tap tempo~~
- ~~MIDI Clock~~
