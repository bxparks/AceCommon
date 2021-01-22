# AutoBenchmark

Determine the speed of various URL encoding and decoding routines.

* `formUrlEncode(size)` and `formUrlDecode(size)`
    * The routines implemented by the `<UrlEncoding.h>` utility of this library.
* `urlencode_yield()` and `urldecode_yield()`
    * The routines implemented by
      https://github.com/TwilioDevEd/twilio_esp8266_arduino_example which calls
      the `yield()` method on each iteration in an attempt to avoid triggering
      the Watch Dog Timer reset after 20-40 ms.
* `urlencode_no_yield()` and `urldecode_no_yield()`
    * The versions of `urlencode_yield()` and `urldecode_yield()`
      *without* calling the `yield()` method at every iteration.

The `size` parameter in the `Description` column refers to the length of the
string being encoded or decoded. For example, `formUrlEncode(1000)` means
that a string of length 1000 was encoded using the `formUrlEncode()` method.

The `micros` column shows the amount of time taken by the routine in
microseconds.

The `us/1000` column shows the normalized amount of microseconds taken per
string length of 1000 characters.

The `min` and `max` columns show the min and max of `micros` over the number of
iterations (usually 20 iterations). These are recorded to show the variability
of the routine on a particular platform.

See the [url_encoding/README.md](../../../src/url_encoding/) for an explanation
of why the `yield()` call was removed from `<UrlEncoding.h>` routines.

## Arduino Nano

* Arduino IDE 1.8.13
* AVR Boards 1.8.3
* 16 MHz ATmega328P

```
sizeof(PrintStrBase): 10
sizeof(PrintStrN): 10
sizeof(PrintStr<8>): 18
--------------------------+--------+--------+--------+--------+
Description               | micros | us/1000|    min |    max |
--------------------------+--------+--------+--------+--------+
formUrlEncode(  25)       |    234 |   9360 |    216 |    300 |
formUrlEncode(  50)       |    390 |   7800 |    380 |    480 |
formUrlEncode( 100)       |    766 |   7660 |    752 |    932 |
--------------------------+--------+--------+--------+--------+
formUrlEncode(  25)       |    127 |   5080 |    112 |    152 |
formUrlEncode(  50)       |    213 |   4260 |    208 |    260 |
formUrlEncode( 100)       |    449 |   4490 |    436 |    544 |
--------------------------+--------+--------+--------+--------+
urlencode_no_yield(  25)  |   1102 |  44080 |   1088 |   1200 |
urlencode_no_yield(  50)  |   2343 |  46860 |   2340 |   2348 |
urlencode_no_yield( 100)  |   4613 |  46130 |   4608 |   4620 |
--------------------------+--------+--------+--------+--------+
urldecode_no_yield(  25)  |    742 |  29680 |    724 |    812 |
urldecode_no_yield(  50)  |   1421 |  28420 |   1416 |   1428 |
urldecode_no_yield( 100)  |   2832 |  28320 |   2828 |   2836 |
--------------------------+--------+--------+--------+--------+
urlencode_yield(  25)     |   1008 |  40320 |    992 |   1096 |
urlencode_yield(  50)     |   2441 |  48820 |   2436 |   2448 |
urlencode_yield( 100)     |   4187 |  41870 |   4184 |   4192 |
--------------------------+--------+--------+--------+--------+
urldecode_yield(  25)     |    751 |  30040 |    736 |    820 |
urldecode_yield(  50)     |   1407 |  28140 |   1404 |   1412 |
urldecode_yield( 100)     |   2899 |  28990 |   2892 |   2904 |
--------------------------+--------+--------+--------+--------+
Num iterations: 20
```

## ESP8266

* Arduino IDE 1.8.13
* ESP8266 Board 2.7.4
* 80 MHz ESP8266

```
sizeof(PrintStrBase): 16
sizeof(PrintStrN): 16
sizeof(PrintStr<8>): 24
--------------------------+--------+--------+--------+--------+
Description               | micros | us/1000|    min |    max |
--------------------------+--------+--------+--------+--------+
formUrlEncode(1000)       |   1591 |   1591 |   1579 |   1761 |
formUrlEncode(2000)       |   3148 |   1574 |   3131 |   3384 |
formUrlEncode(4000)       |   6333 |   1583 |   6301 |   6805 |
formUrlEncode(8000)       |  12653 |   1581 |  12588 |  13596 |
--------------------------+--------+--------+--------+--------+
formUrlDecode(1000)       |    932 |    932 |    923 |   1014 |
formUrlDecode(2000)       |   1837 |    918 |   1824 |   1974 |
formUrlDecode(4000)       |   3677 |    919 |   3649 |   3949 |
formUrlDecode(8000)       |   7360 |    920 |   7307 |   7925 |
--------------------------+--------+--------+--------+--------+
urlencode_no_yield( 100)  |    773 |   7730 |    769 |    844 |
urlencode_no_yield( 200)  |   1446 |   7230 |   1446 |   1450 |
urlencode_no_yield( 400)  |   2904 |   7260 |   2903 |   2908 |
urlencode_no_yield( 800)  |   5688 |   7110 |   5686 |   5702 |
--------------------------+--------+--------+--------+--------+
urldecode_no_yield( 100)  |    534 |   5340 |    532 |    575 |
urldecode_no_yield( 200)  |   1058 |   5290 |   1058 |   1061 |
urldecode_no_yield( 400)  |   2097 |   5242 |   2097 |   2101 |
urldecode_no_yield( 800)  |   4183 |   5228 |   4182 |   4187 |
--------------------------+--------+--------+--------+--------+
urlencode_yield( 100)     |   1374 |  13740 |   1371 |   1401 |
urlencode_yield( 200)     |   2703 |  13515 |   2698 |   2762 |
urlencode_yield( 400)     |   5410 |  13525 |   5287 |   6319 |
urlencode_yield( 800)     |  10752 |  13440 |  10735 |  10830 |
--------------------------+--------+--------+--------+--------+
urldecode_yield( 100)     |   1192 |  11920 |   1189 |   1225 |
urldecode_yield( 200)     |   2330 |  11650 |   2326 |   2394 |
urldecode_yield( 400)     |   4716 |  11790 |   4631 |   5613 |
urldecode_yield( 800)     |   9314 |  11642 |   9257 |   9947 |
--------------------------+--------+--------+--------+--------+
Num iterations: 20
```

## ESP32

* Arduino IDE 1.8.13
* ESP32 Board 1.0.4
* 240 MHz ESP32

```
sizeof(PrintStrBase): 16
sizeof(PrintStrN): 16
sizeof(PrintStr<8>): 24
--------------------------+--------+--------+--------+--------+
Description               | micros | us/1000|    min |    max |
--------------------------+--------+--------+--------+--------+
formUrlEncode(1000)       |    453 |    453 |    447 |    504 |
formUrlEncode(2000)       |    903 |    451 |    890 |    997 |
formUrlEncode(4000)       |   1779 |    444 |   1761 |   1963 |
formUrlEncode(8000)       |   3571 |    446 |   3543 |   3942 |
--------------------------+--------+--------+--------+--------+
formUrlDecode(1000)       |    299 |    299 |    294 |    330 |
formUrlDecode(2000)       |    612 |    306 |    602 |    669 |
formUrlDecode(4000)       |   1206 |    301 |   1195 |   1312 |
formUrlDecode(8000)       |   2414 |    301 |   2392 |   2633 |
--------------------------+--------+--------+--------+--------+
urlencode_no_yield( 100)  |    358 |   3580 |    353 |    383 |
urlencode_no_yield( 200)  |    739 |   3695 |    733 |    743 |
urlencode_no_yield( 400)  |   1779 |   4447 |   1773 |   1783 |
urlencode_no_yield( 800)  |   4662 |   5827 |   4656 |   4667 |
--------------------------+--------+--------+--------+--------+
urldecode_no_yield( 100)  |    252 |   2520 |    250 |    259 |
urldecode_no_yield( 200)  |    501 |   2505 |    497 |    507 |
urldecode_no_yield( 400)  |   1076 |   2690 |   1075 |   1082 |
urldecode_no_yield( 800)  |   2495 |   3118 |   2490 |   2501 |
--------------------------+--------+--------+--------+--------+
urlencode_yield( 100)     |    860 |   8600 |    853 |    868 |
urlencode_yield( 200)     |   1797 |   8985 |   1792 |   1801 |
urlencode_yield( 400)     |   3835 |   9587 |   3827 |   3840 |
urlencode_yield( 800)     |   8560 |  10700 |   8554 |   8567 |
--------------------------+--------+--------+--------+--------+
urldecode_yield( 100)     |    759 |   7590 |    751 |    769 |
urldecode_yield( 200)     |   1525 |   7625 |   1518 |   1532 |
urldecode_yield( 400)     |   3119 |   7797 |   3115 |   3126 |
urldecode_yield( 800)     |   6605 |   8256 |   6596 |   6616 |
--------------------------+--------+--------+--------+--------+
Num iterations: 20
```

## Linux x86-64

* Ubuntu Linux 20.04
* [EpoxyDuino 0.3](https://github.com/bxparks/EpoxyDuino)
* g++ (Ubuntu 9.3.0-10ubuntu2) 9.3.0
* Intel Core i7-3840QM CPU @ 2.80GHz

```
sizeof(PrintStrBase): 24
sizeof(PrintStrN): 24
sizeof(PrintStr<8>): 32
--------------------------+--------+--------+--------+--------+
Description               | micros | us/1000|    min |    max |
--------------------------+--------+--------+--------+--------+
formUrlEncode(1000)       |     62 |     62 |     59 |     98 |
formUrlEncode(2000)       |    123 |     61 |    120 |    149 |
formUrlEncode(4000)       |    246 |     61 |    240 |    296 |
formUrlEncode(8000)       |    418 |     52 |    337 |    580 |
--------------------------+--------+--------+--------+--------+
formUrlDecode(1000)       |     37 |     37 |     25 |     57 |
formUrlDecode(2000)       |     80 |     40 |     75 |     85 |
formUrlDecode(4000)       |    173 |     43 |    168 |    206 |
formUrlDecode(8000)       |    309 |     38 |    267 |    405 |
--------------------------+--------+--------+--------+--------+
urlencode_no_yield(1000)  |    209 |    209 |    203 |    307 |
urlencode_no_yield(2000)  |    375 |    187 |    329 |    458 |
urlencode_no_yield(4000)  |    600 |    150 |    446 |    819 |
urlencode_no_yield(8000)  |    920 |    115 |    770 |   1154 |
--------------------------+--------+--------+--------+--------+
urldecode_no_yield(1000)  |    130 |    130 |     94 |    163 |
urldecode_no_yield(2000)  |    311 |    155 |    293 |    345 |
urldecode_no_yield(4000)  |    506 |    126 |    444 |    662 |
urldecode_no_yield(8000)  |    809 |    101 |    622 |   1107 |
--------------------------+--------+--------+--------+--------+
urlencode_yield(   5)     |   4473 |  42632 |   4446 |   4505 |
urlencode_yield(  10)     |  10061 |  23060 |   9986 |  10125 |
urlencode_yield(  20)     |  21189 |  10874 |  20581 |  21278 |
urlencode_yield(  40)     |  43484 |  38524 |  42150 |  44576 |
--------------------------+--------+--------+--------+--------+
urldecode_yield(   5)     |   4461 |  40232 |   4369 |   4511 |
urldecode_yield(  10)     |  10042 |  21160 |   9940 |  10102 |
urldecode_yield(  20)     |  21164 |   9624 |  20625 |  21298 |
urldecode_yield(  40)     |  43383 |  35999 |  42238 |  43626 |
--------------------------+--------+--------+--------+--------+
Num iterations: 20
```

**Notes**

* The `yield()` function emulated by EpoxyDuino yields control for
  1 whole millisecond. That produces the long execution time of the
  `urlencode_yield()` and `url_decode_yield()` functions.
