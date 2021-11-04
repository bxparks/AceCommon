#!/usr/bin/python3
#
# Python script that regenerates the README.md from the embedded template. Uses
# ./generate_table.awk to regenerate the ASCII tables from the various *.txt
# files.

from subprocess import check_output

nano_results = check_output(
    "./generate_table.awk < nano.txt", shell=True, text=True)
micro_results = check_output(
    "./generate_table.awk < micro.txt", shell=True, text=True)
samd_results = check_output(
    "./generate_table.awk < samd.txt", shell=True, text=True)
stm32_results = check_output(
    "./generate_table.awk < stm32.txt", shell=True, text=True)
esp8266_results = check_output(
    "./generate_table.awk < esp8266.txt", shell=True, text=True)
esp32_results = check_output(
    "./generate_table.awk < esp32.txt", shell=True, text=True)
teensy32_results = check_output(
    "./generate_table.awk < teensy32.txt", shell=True, text=True)
#teensy32_results = 'TBD'

print(f"""\
# AutoBenchmark

Determine the speed of various AceCommon functions and algorithms.

**Version**: AceCommon v1.4.7

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

## Results

The following results are given in unit of microseconds for the single function
being tested, in other words, the `loopCnt` has already been divided. As well,
each benchmark result removes the time taken by the for-loop, so the results
should be pretty close to the time taken by the function if they were isolated.

### Arduino Nano

* 16MHz ATmega328P
* Arduino IDE 1.8.16, Arduino CLI 0.19.2
* Arduino AVR Boards 1.8.3
* `micros()` has a resolution of 4 microseconds

```
{nano_results}
```

### SparkFun Pro Micro

* 16 MHz ATmega32U4
* Arduino IDE 1.8.16, Arduino CLI 0.19.2
* SparkFun AVR Boards 1.1.13
* `micros()` has a resolution of 4 microseconds

```
{micro_results}
```

### SAMD21 M0 Mini

* 48 MHz ARM Cortex-M0+
* Arduino IDE 1.8.16, Arduino CLI 0.19.2
* SparkFun SAMD Core 1.8.5

```
{samd_results}
```

### STM32

* STM32 "Blue Pill", STM32F103C8, 72 MHz ARM Cortex-M3
* Arduino IDE 1.8.16, Arduino CLI 0.19.2
* STM32duino 2.0.0

```
{stm32_results}
```

### ESP8266

* NodeMCU 1.0 clone, 80MHz ESP8266
* Arduino IDE 1.8.16, Arduino CLI 0.19.2
* ESP8266 Boards 3.0.2

```
{esp8266_results}
```

### ESP32

* ESP32-01 Dev Board, 240 MHz Tensilica LX6
* Arduino IDE 1.8.16, Arduino CLI 0.19.2
* ESP32 Boards 1.0.6

```
{esp32_results}
```

### Teensy 3.2

* 96 MHz ARM Cortex-M4
* Arduino IDE 1.8.16, Arduino CLI 0.19.2
* Teensyduino 1.55
* Compiler options: "Faster"

```
{teensy32_results}
```
""")
