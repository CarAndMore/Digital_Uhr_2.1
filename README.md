# Uhr Version 2022


[uhr_2022.ino](/uhr_2022.ino) ->Rechtsklick "Ziel speichern unter..."<-

- ESP8366   WiFi Modul 
- DS3231    I²c -> Real Time Clock (Echtzeituhr) Module
- HT16k33   I²c -> 0.56" 4-Digit 7-Segment Display 


![ht16k33](/img/ht16k33_clock_O.svg)

<table>
  <tr><td></td> <th>ESP D1</th><th>DS3231</th> <th>HT16k33</th></tr>
  <tr><td>+5V</td> <td>5V</td>  <td>VCC</td> <td>+ (1)</td></tr>
  <tr><td>GND</td> <td>G</td>   <td>GND</td> <td>- (2)</td></tr>
  <tr><td>SDA</td> <td>D2</td>  <td>SDA</td> <td>D (3)</td></tr>
  <tr><td>SCL</td> <td>D1</td>  <td>SCL</td> <td>C (4)</td></tr>
</table>

## DS3231 Adresse
Hex 0x68 (FIXE Adresse)
## HT16k33 Adresse
Hex 0x75
<table>
  <tr><th>A2</th><th>A1</th> <th>A0</th></tr>
  <tr><td>H</td> <td>L</td>  <td>H</td></tr>
</table>
