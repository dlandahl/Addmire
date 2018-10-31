
#include <iostream>
#include <fstream>

#include "Addmire.h"

using namespace add;

AdditiveProcess squarify = [](Cluster* cluster, float* args, unsigned argc)
{
    for (unsigned n = 1; n < cluster->partials_used - 1; n += 2)
        cluster->partials[n].amplitude = 0;
};

int main()
{
    const unsigned num_samples = 22050U;
    float data[num_samples];
    float rand_strength = 0.5f;

    for (unsigned n = 0U; n < num_samples; n++)
        data[n] = 0.f;

    addmire_init();

    Cluster c;
    init_cluster_to_wave(&c, 159.f, WaveTransforms::Saw);
    random_phase(&c, &rand_strength, 1U);
    squarify(&c, nullptr, 0);

    samples_from_cluster(&c, data, num_samples);

    std::fstream file("test_data.raw", std::fstream::out | std::fstream::binary);
	for (unsigned n = 0U; n < num_samples; n++)
		file.write((const char*)& data[n], sizeof(float));

    file.close();
}
