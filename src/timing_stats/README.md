# Timing Stats

This directory provides 2 classes to collect a stream of data (often duration in
milliseconds), then calculate a number of statistical quantities, such as:

* min
* max
* average
* [exponential decay average](https://en.wikipedia.org/wiki/Moving_average#Exponential_moving_average)
* count

The classes are:

* `TimingStats`: the original stats class specialized to `uint16_t`
* `GenericStats`: a template class to support type `T` (e.g. `float`)

## Usage

Use the `update()` function to collect the data. After data collection,
call the various `getMin()`, `getMax()`, `getAvg()`, `getCount()`, and
`getExpDecayAvg()` functions.

Call the `reset()` method to reset the counters.

### TimingStats

```C++
#include <Arduino.h>
#include <AceCommon.h>
using namespace ace_common;

TimingStats stats;

void collectData() {
  stats.update(10);
  stats.update(20);
  stats.update(30);
}

void printStats() {
  Serial.print("Min: ");
  Serial.println(stats.getMin());

  Serial.print("Max: ");
  Serial.println(stats.getMax());

  Serial.print("Avg: ");
  Serial.println(stats.getAvg());

  Serial.print("Exp Decay Avg: ");
  Serial.println(stats.getExpDecayAvg());

  Serial.print("Count: ");
  Serial.println(stats.getCount());
}

void setup() {
  Serial.begin(115200);
  while (! Serial); // Leonardo/ProMicro

  collectData();
  printStats();
}

void loop() {
}
```

Running this should print:
```
Min: 10
Max: 30
Avg: 20
Exp Decay Avg: 21
Count: 3
```

### GenericStats

Very similar to `TimingStats`:

```C++
#include <Arduino.h>
#include <AceCommon.h>
using namespace ace_common;

GenericStats<float> stats;

void collectData() {
  stats.update(1.0);
  stats.update(2.0);
  stats.update(3.0);
}

[...same as above...]
```
