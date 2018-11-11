
#include <vector>

#include "AddmireCore.h"

namespace add
{

namespace WaveTransforms
{ extern PartialIndexTransform Sine, Tri, Square, Saw; }

class TrackedValue
{
public:
    enum Quality { additive = 0, multiplicative = 1 };
    
    TrackedValue(Quality qual) : quality(qual), current_value(qual) { }
    TrackedValue() = delete;

    void set_value(float new_value);
    float get_value();

private:
    float current_value;
    float value_delta = 0;

    Quality quality;
};

class Repitch : public AdditiveProcessor
{
    TrackedValue value = TrackedValue(TrackedValue::Quality::additive);

public:
    void set_pitch(float new_value) { value.set_value(new_value); }
    void proc() override;
};

class RepitchRatio : public AdditiveProcessor
{
    TrackedValue value = TrackedValue(TrackedValue::Quality::multiplicative);

public:
    void set_pitch(float new_value) { value.set_value(new_value); }
    void proc() override;
};

}
