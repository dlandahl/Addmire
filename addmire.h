
#pragma once

enum WaveType
{
    Sine,
    Tri,
    Square,
    Saw
}

struct Partial
{
    float frequency;
    float phase;
    float amplitude;
};

Partial make_partial();
void    init_partial(Partial* partial_to_init);

template <unsigned size>
struct Cluster
{
    unsigned partial_count = size;
    Partial partials[size];
};

void init_cluster(Cluster*, cluster_to_init);
void init_cluster_to_wave(Cluster* cluster, float fundamental, WaveType wave);
void samples_from_cluster(Cluster* cluster, float* buffer, int sample_count);