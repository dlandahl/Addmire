
#include <random>
#include <vector>

#include "AddmireCore.h"

namespace add {

namespace WaveTransforms
{ extern PartialIndexTransform Sine, Tri, Square, Saw; }

namespace AdditiveProcesses {

class Repitch : public AdditiveProcessor
{
    float value;
public:
    Repitch(Cluster* c) : AdditiveProcessor(c) { }

    void set_pitch(float new_value) 
    {
        value = new_value;
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

class FlipFrequencies : public AdditiveProcessor
{
public:
    FlipFrequencies(Cluster* c) : AdditiveProcessor(c) { }
    float pivot;
    void proc() override;
};

}

}
