
#include <iostream>
#include <fstream>
#include <array>

#include "AddmireCore.h"
#include "AddmireAlgorithms.h"

using namespace add;

AdditiveProcess squarify = [](Cluster* cluster, float* args, unsigned argc)
{
    for (unsigned n = 1; n < Cluster::partials_used - 1; n += 2)
        cluster->partials[n].amplitude = 0;
};

PartialIndexTransform my_transform
    = [](unsigned n, float fundamental, float &frequency, float &amplitude)
{
    amplitude = 0.f;
    frequency = fundamental * (n+1);
    if (!(n % 3)) amplitude = 1.f / (n + 1);
};

int main()
{
    const unsigned num_samples = 22050U;
    float data[num_samples] = { 0 };

    addmire_init();

    Cluster c;
    init_cluster_to_wave(&c, 300.f, WaveTransforms::Tri);

    draw_cluster(&c);

    samples_from_cluster(&c, data, num_samples);

    std::fstream file("test_data.raw", std::fstream::out | std::fstream::binary);
	for (unsigned n = 0U; n < num_samples; n++)
		file.write((const char*)& data[n], sizeof(float));
}
