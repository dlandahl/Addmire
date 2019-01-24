
#include <iostream>
#include <fstream>
#include <array>
#include <random>

#include "AddmireCore.h"
#include "AddmireAlgorithms.h"

using namespace add;

int main()
{
    unsigned const num_samples = 4410u * 2;
    float data[num_samples] = { 0 };

    std::default_random_engine engine;
    std::uniform_real_distribution<float> dist(-1.f, 1.f);

    for (unsigned n = 0; n < num_samples; n++)
        data[n] = dist(engine);

    addmire_init();

    //Cluster c = Cluster(220.f, WaveTransforms::Square);
    //c.get_samples(data, num_samples);
    Cluster c = Cluster::from_dft(data, num_samples);

    for (unsigned n = 0; n < num_samples; n++) data[n] = 0;
    c.get_samples(data, num_samples);
    c.draw();

    std::fstream file("test_data.raw", std::fstream::out | std::fstream::binary);
    for (unsigned n = 0u; n < num_samples; n++)
        file.write((char const*)& data[n], sizeof(float));
}
