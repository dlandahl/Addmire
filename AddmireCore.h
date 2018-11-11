
#pragma once

namespace add {
namespace var {
    extern const float tau;
    float get_nyquist();
    float get_sample_rate();
    void set_sample_rate(float new_sample_rate);
    void set_partial_count(int new_partial_count);
}

void addmire_init(float sample_rate=44100.0f, int partial_count=512);

namespace wavetable
{
    extern const int table_size;
    float get_value();
}

struct Partial
{
    float frequency;
    float offset_phase;
    float amplitude;
    float current_phase;

    Partial();
};

using PartialIndexTransform = void (*)(unsigned partial_index,
    float fundamental, Partial &partial);

class Cluster
{
public:
    const static int max_size = 1024;
    static int partials_used;

    Partial partials[max_size];

    Cluster() = default;
    Cluster(float fundamental, PartialIndexTransform transform);
    void get_samples(float* buffer, int sample_count);
    void draw();

};

class AdditiveProcessor
{
public:
    Cluster* target_cluster;
    virtual void proc() = 0;
};

class TrackedValue
{
public:
    enum Quality { additive = 0, multiplicative = 1 };
    
    TrackedValue(Quality qual) : quality(qual), current_value(qual) { }
    TrackedValue() = delete;

    void set_value(float new_value);
    float get_value();

private:
    Quality quality;

    float current_value;
    float value_delta = 0;
};

}
