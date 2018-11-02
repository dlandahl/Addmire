
#pragma once

namespace add {
namespace var {
    extern const float tau;
    float get_nyquist();
    float get_sample_rate();
    void set_sample_rate(float new_sample_rate);
    void set_partial_count(int new_partial_count);
};

void addmire_init(float sample_rate=44100.0f, int partial_count=512);

struct Cluster;

using PartialIndexTransform = void (*)(unsigned partial_index, float fundamental,
    float &frequency, float &amplitude);
using AdditiveProcess = void (*)(Cluster* cluster_to_process, float* arguments, unsigned argument_count);

struct Partial
{
    float frequency;
    float offset_phase;
    float amplitude;
    float current_phase;
};

Partial make_partial(float frequency=100.f, float phase=0.f, float amplitude=1.f);
void    init_partial(Partial* partial_to_init);

struct Cluster
{
    const static int max_size = 1024;
    static int partials_used;

    Partial partials[max_size];
};

void init_cluster(Cluster* cluster_to_init, int cluster_size);
void init_cluster_to_wave(Cluster* cluster, float fundamental, PartialIndexTransform transform);
void samples_from_cluster(Cluster* cluster, float* buffer, int sample_count);

namespace wavetable
{
    extern const int table_size;
    float get_value();
};

}
