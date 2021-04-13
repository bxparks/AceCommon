/*
MIT License

Copyright (c) 2018 Brian T. Park

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef ACE_COMMON_TIMING_STATS_H
#define ACE_COMMON_TIMING_STATS_H

#include <stdint.h>

namespace ace_common {

/**
 * Helper class to collect timing statistics such as min, max, average, and
 * exponential-decay average.
 */
class TimingStats {
  public:
    /** Constructor. Calls reset(). */
    TimingStats(): mCounter(0) {
      reset();
    }

    // Use default copy-constructor and assignment operator. Doxygen docs added
    // to keep it quiet.

    /** Default copy constructor. */
    TimingStats(const TimingStats&) = default;

    /** Default assignment operator. */
    TimingStats& operator=(const TimingStats&) = default;

    /**
     * Reset the object to its initial state, except mCounter which is
     * never reset.
     */
    void reset() {
      mExpDecayAvg = 0;
      mMin = UINT16_MAX;
      mMax = 0;
      mSum = 0;
      mCount = 0;
    }

    /** Return the maximum since the last reset(). */
    uint16_t getMax() const { return mMax; }

    /** Return the minium since the last reset(). */
    uint16_t getMin() const { return mMin; }

    /**
     * Return the average since the last reset(). Returns 0 if update()
     * has never been called.
     */
    uint16_t getAvg() const { return (mCount > 0) ? mSum / mCount : 0; }

    /** An exponential decay average since the last reset(). */
    uint16_t getExpDecayAvg() const { return mExpDecayAvg; }

    /** Number of times update() was called since last reset(). */
    uint16_t getCount() const { return mCount; }

    /**
     * Number of times update() was called from the beginning of time. Never
     * reset. This is useful to determining how many times update() was called
     * since it was last checked from the client code.
     */
    uint16_t getCounter() const { return mCounter; }

    /** Add the given duration (often in milliseconds) to the statistics. */
    void update(uint16_t duration) {
      mSum += duration;
      if (duration < mMin) {
        mMin = duration;
      }
      if (duration > mMax) {
        mMax = duration;
      }

      if (mCount == 0) {
        mExpDecayAvg = duration;
      } else {
        // Use a slightly convoluted averaging algorithm to prevent uint16_t
        // overflow and to preserve accuracy in the least significant bit.
        mExpDecayAvg = (mExpDecayAvg/2) + (duration/2)
          + ((mExpDecayAvg & 0x1) & (duration & 0x1));
      }

      mCount++;
      mCounter++;
    }

  private:
    uint16_t mExpDecayAvg;
    uint16_t mMin;
    uint16_t mMax;
    uint32_t mSum;
    uint16_t mCount;
    uint16_t mCounter;
};

}

#endif
