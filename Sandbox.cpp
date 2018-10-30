
#include <iostream>
#include <fstream>

#include "Addmire.h"

int main()
{
    using namespace add;

    std::fstream file("test_data.raw", std::fstream::out | std::fstream::binary);

    const unsigned num_samples = 22050;
    float data[num_samples];

    for (unsigned n = 0; n < num_samples; n++)
        data[n] = 0.f;

    addmire_init();

    Cluster c;
    init_cluster_to_wave(&c, 55.f, WaveType::Square);

    for (unsigned n = 0; n < num_samples; n++)
        samples_from_cluster(&c, data + n, 1);

	for (unsigned i = 0; i < num_samples; i++)
	{
		file.write((const char*)& data[i], sizeof(float));
	}

    file.close();
}
