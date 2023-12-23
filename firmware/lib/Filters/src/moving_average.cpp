#include <moving_average.h>

MovingAverage::MovingAverage(uint16_t samples)
{
    this->samples = samples;
    this->buffer = new uint16_t[samples]{};
}

bool MovingAverage::is_initialised()
{
    return this->initialised;
}

uint16_t MovingAverage::current_average()
{
    return this->avg;
}

uint16_t MovingAverage::push(uint16_t value)
{
    // Calculate the new sum of values, then update buffer + oldest index
    this->sum = this->sum - this->buffer[this->index] + value;
    this->buffer[this->index] = value;
    this->index = (this->index + 1) % this->samples;

    // Circular buffer is fully initialised if we've reached the beginning
    this->initialised |= this->index == 0;

    // Calculate and return the new average value
    this->avg = this->sum / this->samples;
    return this->avg;
}
