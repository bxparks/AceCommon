# AutoBenchmark

Determine the speed of various AceCommon functions and algorithms.

**Version**: AceCommon v1.6.0

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

**v1.4.4**

* Created the AutoBenchmark, replacing the `examples/Udiv1000` program that
    measured only the `udiv1000()` function. It seems like `examples/Udiv1000`
    underestimated the duration of the `udiv1000()` function for AVR
    processors, probably because it did not sufficiently disable compiler
    optimizations. That program found that that `udiv1000()` took about 5-6
    microseconds on the AVR. The AutoBenchmark program finds that it actually
    takes 16-16 microseconds.

**v1.4.5**

* Upgrade to ESP32 Core v1.0.6. No significant change.

**v1.4.6**

* Upgrade STM32duino Core from 1.9.0 to 2.0.0.
* Upgrade SparkFun SAMD Core from 1.8.1 to 1.8.3.
* No significant change in CPU times.

**v1.4.7**

* Upgrade Arduino IDE from 1.8.13 to 1.8.16.
* Upgrade Arduino CLI from 0.14.0 to 0.19.2
* Upgrade SparkFun SAMD Core from 1.8.3 to 1.8.5.
* Upgrade ESP8266 Core from 2.7.4 to 3.0.2.
* Upgrade Teensyduino from 1.53 to 1.55.
* No significant change in CPU times.

**v1.5.0**

* Remove SAMD21 board.
* Upgrade Arduino IDE from 1.8.16 to 1.8.19.
* Upgrade Arduino CLI from 0.19.2 to 0.20.2.
* Upgrade Arduino AVR Core from 1.8.3 to 1.8.4.
* Upgrade STM32 Core from 2.0.0 to 2.2.0.
* Upgrade ESP32 Core from 1.0.6 to 2.0.2.
* Upgrade Teensyduino from 1.55 to 1.56.

**v1.5.2**

* Upgrade tool chain
    * Upgrade Arduino CLI from 0.20.2 to 0.27.1.
    * Upgrade Arduino AVR Core from 1.8.4 to 1.8.5.
    * Upgrade STM32 Core from 2.2.0 to 2.3.0.
    * Upgrade ESP32 Core from 2.0.2 to 2.0.5.
    * Upgrade Teensyduino from 1.56 to 1.57.
* No significant changes to CPU times.

**v1.6.0**

* Upgrade tool chain
    * Upgrade Arduino CLI to 0.31.0.
    * Upgrade Arduino AVR Core to 1.8.6.
    * Add Seeeduino SAMD 1.8.4
    * Upgrade STM32 Core to 2.5.0.
    * Add Adafruit SAMD 1.7.11
    * Upgrade ESP32 Core to 2.0.9.
    * Remove Teensy 3.2.
* Add more PROGMEM support in `KString`, `copyReplaceChar()` and
  `copyReplaceString()`.
* No significant changes to memory sizes.

## Results

The following results are given in unit of microseconds for the single function
being tested, in other words, the `loopCnt` has already been divided. As well,
each benchmark result removes the time taken by the for-loop, so the results
should be pretty close to the time taken by the function if they were isolated.

### Arduino Nano

* 16MHz ATmega328P
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* Arduino AVR Boards 1.8.5
* `micros()` has a resolution of 4 microseconds

```
Sizes of Objects:
sizeof(TimingStats): 14

CPU:
+---------------------------+-------------------------+---------+
| AceCommon function        |     min/    avg/    max | loopCnt |
|---------------------------+-------------------------+---------|
| /1000                     |  37.400/ 37.462/ 37.712 |    1000 |
| udiv1000()                |  15.524/ 15.555/ 15.652 |    1000 |
| decToBcdDivOnly()         |   5.096/  5.127/  5.244 |    1000 |
| decToBcdDivMod()          |   4.964/  5.008/  5.156 |    1000 |
| decToBcd()                |   4.964/  5.008/  5.172 |    1000 |
| bcdToDec()                |   0.180/  0.216/  0.344 |    1000 |
+---------------------------+-------------------------+---------+

```

### SparkFun Pro Micro

* 16 MHz ATmega32U4
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* SparkFun AVR Boards 1.1.13
* `micros()` has a resolution of 4 microseconds

```
Sizes of Objects:
sizeof(TimingStats): 14

CPU:
+---------------------------+-------------------------+---------+
| AceCommon function        |     min/    avg/    max | loopCnt |
|---------------------------+-------------------------+---------|
| /1000                     |  37.592/ 37.599/ 37.604 |    1000 |
| udiv1000()                |  15.608/ 15.620/ 15.628 |    1000 |
| decToBcdDivOnly()         |   5.120/  5.127/  5.132 |    1000 |
| decToBcdDivMod()          |   4.984/  4.992/  4.996 |    1000 |
| decToBcd()                |   4.988/  4.998/  5.008 |    1000 |
| bcdToDec()                |   0.176/  0.188/  0.200 |    1000 |
+---------------------------+-------------------------+---------+

```

## SAMD21 Seeeduino XIAO M0

* SAMD51, 120 MHz ARM Cortex-M4
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* Seeeduino SAMD 1.8.4

```
Sizes of Objects:
sizeof(TimingStats): 16

CPU:
+---------------------------+-------------------------+---------+
| AceCommon function        |     min/    avg/    max | loopCnt |
|---------------------------+-------------------------+---------|
| /1000                     |   2.552/  2.553/  2.554 |    5000 |
| udiv1000()                |   0.083/  0.084/  0.085 |    5000 |
| decToBcdDivOnly()         |   1.382/  1.383/  1.384 |    5000 |
| decToBcdDivMod()          |   2.783/  2.784/  2.785 |    5000 |
| decToBcd()                |   1.382/  1.383/  1.384 |    5000 |
| bcdToDec()                |   0.083/  0.084/  0.085 |    5000 |
+---------------------------+-------------------------+---------+

```

### STM32

* STM32 "Blue Pill", STM32F103C8, 72 MHz ARM Cortex-M3
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* STM32duino 2.3.0

```
Sizes of Objects:
sizeof(TimingStats): 16

CPU:
+---------------------------+-------------------------+---------+
| AceCommon function        |     min/    avg/    max | loopCnt |
|---------------------------+-------------------------+---------|
| /1000                     |   0.081/  0.081/  0.083 |   10000 |
| udiv1000()                |   0.041/  0.042/  0.043 |   10000 |
| decToBcdDivOnly()         |   0.103/  0.104/  0.106 |   10000 |
| decToBcdDivMod()          |   0.117/  0.118/  0.120 |   10000 |
| decToBcd()                |   0.103/  0.104/  0.105 |   10000 |
| bcdToDec()                |   0.097/  0.097/  0.099 |   10000 |
+---------------------------+-------------------------+---------+

```

## SAMD51 Adafruit ItsyBitsy M4

* SAMD51, 120 MHz ARM Cortex-M4
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* Adafruit SAMD 1.7.11

```
Sizes of Objects:
sizeof(TimingStats): 16

CPU:
+---------------------------+-------------------------+---------+
| AceCommon function        |     min/    avg/    max | loopCnt |
|---------------------------+-------------------------+---------|
| /1000                     |   0.063/  0.063/  0.063 |    5000 |
| udiv1000()                |   0.024/  0.025/  0.025 |    5000 |
| decToBcdDivOnly()         |   0.070/  0.070/  0.070 |    5000 |
| decToBcdDivMod()          |   0.070/  0.070/  0.070 |    5000 |
| decToBcd()                |   0.070/  0.070/  0.070 |    5000 |
| bcdToDec()                |   0.041/  0.041/  0.042 |    5000 |
+---------------------------+-------------------------+---------+

```

### ESP8266

* NodeMCU 1.0 clone, 80MHz ESP8266
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* ESP8266 Boards 3.0.2

```
Sizes of Objects:
sizeof(TimingStats): 16

CPU:
+---------------------------+-------------------------+---------+
| AceCommon function        |     min/    avg/    max | loopCnt |
|---------------------------+-------------------------+---------|
| /1000                     |   1.897/  1.897/  1.897 |   10000 |
| udiv1000()                |   0.049/  0.050/  0.050 |   10000 |
| decToBcdDivOnly()         |   0.775/  0.776/  0.776 |   10000 |
| decToBcdDivMod()          |   1.372/  1.372/  1.373 |   10000 |
| decToBcd()                |   0.775/  0.776/  0.776 |   10000 |
| bcdToDec()                |   0.049/  0.049/  0.050 |   10000 |
+---------------------------+-------------------------+---------+

```

### ESP32

* ESP32-01 Dev Board, 240 MHz Tensilica LX6
* Arduino IDE 1.8.19, Arduino CLI 0.31.0
* ESP32 Boards 2.0.5

```
Sizes of Objects:
sizeof(TimingStats): 16

CPU:
+---------------------------+-------------------------+---------+
| AceCommon function        |     min/    avg/    max | loopCnt |
|---------------------------+-------------------------+---------|
| /1000                     |   0.000/  0.013/  0.065 | 1000000 |
| udiv1000()                |   0.025/  0.025/  0.025 | 1000000 |
| decToBcdDivOnly()         |   0.020/  0.020/  0.020 | 1000000 |
| decToBcdDivMod()          |   0.029/  0.029/  0.029 | 1000000 |
| decToBcd()                |   0.020/  0.020/  0.020 | 1000000 |
| bcdToDec()                |   0.020/  0.020/  0.020 | 1000000 |
+---------------------------+-------------------------+---------+

```

