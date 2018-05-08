# ws2811
### Arduino

code to run an RGB strip running on the WS2811 chip

it has three pins instead of four (in case you were wondering if you had one)
---
the timings are done in assemby, so whatever you have this running on has to be able to run Arduino code, and run at exactly *16MHz*.


## Patterns
```c++
const byte pulse[3][5][1][3] = {
```
and
```c++
const byte chaser[2][5][5][3] = {
```
are 4D arrays that determine how the lights flash.

This dimension is the version, so you can have different colored versions of each pattern.
```c++
const byte chaser[2][5][5][3] = {
                 ---
```
This dimension is time, running first to last.
```c++
const byte chaser[2][5][5][3] = {
                    ---
```
This dimension is position, this number can be increased or decreased and the pattern will still work.
```c++
const byte chaser[2][5][5][3] = {
                       ---
```
This dimension is color, holding each individual color value
```c++
const byte chaser[2][5][5][3] = {
                          ---
```
