
#pragma once

namespace add {
namespace var {
    inline float const tau = 6.28318530;
    float get_nyquist();
    float get_sample_rate();
    void set_sample_rate(float new_sample_rate);
    void set_partial_count(int new_partial_count);
}

void addmire_init(float sample_rate=44100.0f, int partial_count=512);

namespace wavetable
{
    inline int const table_size = 512;
    float get_value();
}

struct VisualData
{
    VisualData(unsigned resolution) : res(resolution)
    {
        data = (uint8_t*) malloc(resolution);
    }

    ~VisualData() { free(data); }

    uint8_t* data = nullptr;
    unsigned resolution() { return res; }
private:
    unsigned res = 0;
};

struct Partial
{
    float frequency;
    float offset_phase;
    float amplitude;
    float current_phase;

    Partial();
};

using WaveTransform = void (*)(unsigned partial_index,
    float fundamental, Partial &partial);

using PartialTransform = void (*) (Partial partial, int index, void* data);

struct Cluster
{
    static int const max_size = 1024;
    static int partials_used;

    Partial partials[max_size];

    Cluster(float fundamental, WaveTransform transform);
    Cluster() = default;

    static Cluster from_dft(float* data, unsigned size);
    void get_samples(float* buffer, int sample_count);
    VisualData get_visual_data(unsigned resolution);
    void draw();
    void apply_to_all_partials(PartialTransform, void* data=nullptr);
};

}
