
#include <iostream>
#include <fstream>

#include "Addmire.h"

int main()
{
    using namespace add;

    const unsigned num_samples = 22050;
    float data[num_samples];

    for (unsigned n = 0; n < num_samples; n++)
        data[n] = 0.f;

    addmire_init();

    Cluster c;
    init_cluster_to_wave(&c, 159.f, WaveTransforms::Tri);

    samples_from_cluster(&c, data, 22050);

    std::fstream file("test_data.raw", std::fstream::out | std::fstream::binary);
	for (unsigned n = 0; n < num_samples; n++)
		file.write((const char*)& data[n], sizeof(float));

    file.close();
}
