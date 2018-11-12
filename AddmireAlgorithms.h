
#include <random>
#include <vector>

#include "AddmireCore.h"

namespace add {

namespace WaveTransforms
{ extern PartialIndexTransform Sine, Tri, Square, Saw; }

namespace AdditiveProcesses {

class Repitch : public AdditiveProcessor
{
    TrackedValue absolute = TrackedValue(TrackedValue::Quality::additive);
    TrackedValue ratio    = TrackedValue(TrackedValue::Quality::multiplicative);

    float delta;
    bool is_absolute = false;

public:
    Repitch(Cluster* c) : AdditiveProcessor(c) { }

    void set_pitch(float new_value) 
    {
        if (is_absolute) absolute.set_value(new_value);
        else ratio.set_value(new_value);
    }

    void proc() override;
};

class RandomPhase : public AdditiveProcessor
{
    float max = var::tau;
    std::default_random_engine engine;

public:
    RandomPhase(Cluster* c) : AdditiveProcessor(c) { }
    void set_strength(float strength) { max = strength; }
    void proc() override;
};

}

}
