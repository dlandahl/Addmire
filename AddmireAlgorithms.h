
#include <vector>

#include "AddmireCore.h"

namespace add
{

namespace WaveTransforms
{ extern PartialIndexTransform Sine, Tri, Square, Saw; }

namespace AdditiveProcesses {
class Repitch : public AdditiveProcessor
{
    TrackedValue value = TrackedValue(TrackedValue::Quality::additive);

public:
    Repitch(Cluster* c) : AdditiveProcessor(c) { }

    void set_pitch(float new_value) { value.set_value(new_value); }
    void proc() override;
};

class RepitchRatio : public AdditiveProcessor
{
    TrackedValue value = TrackedValue(TrackedValue::Quality::multiplicative);

public:
    RepitchRatio(Cluster* c) : AdditiveProcessor(c) { }

    void set_pitch(float new_value) { value.set_value(new_value); }
    void proc() override;
};
}

}
