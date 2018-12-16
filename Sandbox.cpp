
#include <iostream>
#include <fstream>
#include <array>

#include "AddmireCore.h"
#include "AddmireAlgorithms.h"

using namespace add;

int main()
{
    const unsigned num_samples = 4410u;
    float data[num_samples] = { 0 };

    addmire_init();

    Cluster c = Cluster(220.f, WaveTransforms::Tri);
    c.get_samples(data, num_samples);
    c = Cluster::from_dft(data, num_samples);

    for (unsigned n = 0; n < num_samples; n++) data[n] = 0;
    c.get_samples(data, num_samples);

    std::fstream file("test_data.raw", std::fstream::out | std::fstream::binary);
	for (unsigned n = 0u; n < num_samples; n++)
		file.write((const char*)& data[n], sizeof(float));
}
