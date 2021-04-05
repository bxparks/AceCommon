# AutoBenchmark

Determine the speed of various AceCommon functions and algorithms.

**Version**: AceCommon v1.4.4

**DO NOT EDIT**: This file was auto-generated using `make README.md`.

## Dependencies

This program depends on the following libraries:

* [AceCommon](https://github.com/bxparks/AceCommon)

## How to Generate

This requires the [AUniter](https://github.com/bxparks/AUniter) script
to execute the Arduino IDE programmatically.

The `Makefile` has rules to generate the `*.txt` results file for several
microcontrollers that I usually support, but the `$ make benchmarks` command
does not work very well because the USB port of the microcontroller is a
dynamically changing parameter. I created a semi-automated way of collect the
`*.txt` files:

1. Connect the microcontroller to the serial port. I usually do this through a
USB hub with individually controlled switch.
2. Type `$ auniter ports` to determine its `/dev/ttyXXX` port number (e.g.
`/dev/ttyUSB0` or `/dev/ttyACM0`).
3. If the port is `USB0` or `ACM0`, type `$ make nano.txt`, etc.
4. Switch off the old microontroller.
5. Go to Step 1 and repeat for each microcontroller.

The `generate_table.awk` program reads one of `*.txt` files and prints out an
ASCII table that can be directly embedded into this README.md file. For example
the following command produces the table in the Nano section below:

```
$ ./generate_table.awk < nano.txt
```

Fortunately, we no longer need to run `generate_table.awk` for each `*.txt`
file. The process has been automated using the `generate_readme.py` script which
will be invoked by the following command:
```
$ make README.md
```

The CPU times below are given in microseconds. The "samples" column is the
number of `TimingStats::update()` calls that were made.

## CPU Time Changes

* 1.4.4: Created the AutoBenchmark, replacing the `examples/Udiv1000` program
  that measured only the `udiv1000()` function. It seems like
  `examples/Udiv1000` underestimated the duration of the `udiv1000()` function
  for AVR processors, probably because it did not sufficiently disable compiler
  optimizations. That program found that that `udiv1000()` took about 5-6
  microseconds on the AVR. The AutoBenchmark program finds that it actually
  takes 16-16 microseconds.

## Results

The following results are given in unit of microseconds for the single function
being tested, in other words, the `loopCnt` has already been divided. As well,
each benchmark result removes the time taken by the for-loop, so the results
should be pretty close to the time taken by the function if they were isolated.

### Arduino Nano

* 16MHz ATmega328P
* Arduino IDE 1.8.13
* Arduino AVR Boards 1.8.3
* `micros()` has a resolution of 4 microseconds

```
Sizes of Objects:
sizeof(TimingStats): 14

CPU:
+---------------------------+-------------------------+---------+
| AceCommon function        |     min/    avg/    max | loopCnt |
|---------------------------+-------------------------+---------|
| /1000                     |  37.400/ 37.462/ 37.712 |    1000 |
| udiv1000()                |  15.524/ 15.556/ 15.656 |    1000 |
| decToBcdDivOnly()         |   5.096/  5.128/  5.248 |    1000 |
| decToBcdDivMod()          |   4.960/  5.006/  5.160 |    1000 |
| decToBcd()                |   4.960/  5.006/  5.168 |    1000 |
| bcdToDec()                |   0.180/  0.217/  0.344 |    1000 |
+---------------------------+-------------------------+---------+

```

### Sparkfun Pro Micro

* 16 MHz ATmega32U4
* Arduino IDE 1.8.13
* SparkFun AVR Boards 1.1.13
* `micros()` has a resolution of 4 microseconds

```
Sizes of Objects:
sizeof(TimingStats): 14

CPU:
+---------------------------+-------------------------+---------+
| AceCommon function        |     min/    avg/    max | loopCnt |
|---------------------------+-------------------------+---------|
| /1000                     |  37.588/ 37.601/ 37.612 |    1000 |
| udiv1000()                |  15.612/ 15.618/ 15.624 |    1000 |
| decToBcdDivOnly()         |   5.124/  5.129/  5.132 |    1000 |
| decToBcdDivMod()          |   4.984/  4.992/  5.004 |    1000 |
| decToBcd()                |   4.984/  4.990/  4.996 |    1000 |
| bcdToDec()                |   0.176/  0.188/  0.200 |    1000 |
+---------------------------+-------------------------+---------+

```

### SAMD21 M0 Mini

* 48 MHz ARM Cortex-M0+
* Arduino IDE 1.8.13
* Sparkfun SAMD Core 1.8.1

```
Sizes of Objects:
sizeof(TimingStats): 16

CPU:
+---------------------------+-------------------------+---------+
| AceCommon function        |     min/    avg/    max | loopCnt |
|---------------------------+-------------------------+---------|
| /1000                     |   3.813/  3.813/  3.814 |    5000 |
| udiv1000()                |   0.103/  0.104/  0.105 |    5000 |
| decToBcdDivOnly()         |   2.405/  2.406/  2.407 |    5000 |
| decToBcdDivMod()          |   5.084/  5.084/  5.085 |    5000 |
| decToBcd()                |   2.465/  2.466/  2.467 |    5000 |
| bcdToDec()                |   0.146/  0.147/  0.147 |    5000 |
+---------------------------+-------------------------+---------+

```

### STM32

* STM32 "Blue Pill", STM32F103C8, 72 MHz ARM Cortex-M3
* Arduino IDE 1.8.13
* STM32duino 1.9.0

```
Sizes of Objects:
sizeof(TimingStats): 16

CPU:
+---------------------------+-------------------------+---------+
| AceCommon function        |     min/    avg/    max | loopCnt |
|---------------------------+-------------------------+---------|
| /1000                     |   0.080/  0.081/  0.083 |   10000 |
| udiv1000()                |   0.055/  0.056/  0.056 |   10000 |
| decToBcdDivOnly()         |   0.131/  0.132/  0.134 |   10000 |
| decToBcdDivMod()          |   0.145/  0.146/  0.148 |   10000 |
| decToBcd()                |   0.131/  0.132/  0.133 |   10000 |
| bcdToDec()                |   0.097/  0.098/  0.100 |   10000 |
+---------------------------+-------------------------+---------+

```

### ESP8266

* NodeMCU 1.0 clone, 80MHz ESP8266
* Arduino IDE 1.8.13
* ESP8266 Boards 2.7.4

```
Sizes of Objects:
sizeof(TimingStats): 16

CPU:
+---------------------------+-------------------------+---------+
| AceCommon function        |     min/    avg/    max | loopCnt |
|---------------------------+-------------------------+---------|
| /1000                     |   1.896/  1.898/  1.900 |   10000 |
| udiv1000()                |   0.049/  0.050/  0.050 |   10000 |
| decToBcdDivOnly()         |   0.875/  0.876/  0.877 |   10000 |
| decToBcdDivMod()          |   1.472/  1.473/  1.475 |   10000 |
| decToBcd()                |   0.875/  0.876/  0.877 |   10000 |
| bcdToDec()                |   0.062/  0.062/  0.062 |   10000 |
+---------------------------+-------------------------+---------+

```

### ESP32

* ESP32-01 Dev Board, 240 MHz Tensilica LX6
* Arduino IDE 1.8.13
* ESP32 Boards 1.0.4

```
Sizes of Objects:
sizeof(TimingStats): 16

CPU:
+---------------------------+-------------------------+---------+
| AceCommon function        |     min/    avg/    max | loopCnt |
|---------------------------+-------------------------+---------|
| /1000                     |   0.000/  0.013/  0.065 | 1000000 |
| udiv1000()                |   0.016/  0.016/  0.016 | 1000000 |
| decToBcdDivOnly()         |   0.012/  0.012/  0.012 | 1000000 |
| decToBcdDivMod()          |   0.025/  0.025/  0.025 | 1000000 |
| decToBcd()                |   0.012/  0.012/  0.012 | 1000000 |
| bcdToDec()                |   0.012/  0.012/  0.012 | 1000000 |
+---------------------------+-------------------------+---------+

```

### Teensy 3.2

* 96 MHz ARM Cortex-M4
* Arduino IDE 1.8.13
* Teensyduino 1.53
* Compiler options: "Faster"

```
Sizes of Objects:
sizeof(TimingStats): 16

CPU:
+---------------------------+-------------------------+---------+
| AceCommon function        |     min/    avg/    max | loopCnt |
|---------------------------+-------------------------+---------|
| /1000                     |   0.041/  0.041/  0.041 | 1000000 |
| udiv1000()                |   0.007/  0.007/  0.007 | 1000000 |
| decToBcdDivOnly()         |   0.052/  0.052/  0.052 | 1000000 |
| decToBcdDivMod()          |   0.062/  0.062/  0.062 | 1000000 |
| decToBcd()                |   0.052/  0.052/  0.052 | 1000000 |
| bcdToDec()                |   0.052/  0.052/  0.052 | 1000000 |
+---------------------------+-------------------------+---------+

```

