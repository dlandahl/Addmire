
#include <iostream>
#include <fstream>

#include "Addmire.h"

int main()
{
    using namespace add;
    std::cout << "Welcome to Addmire\n";

    std::fstream file("test_data.raw", std::fstream::out | std::fstream::binary);

    const unsigned num_samples = 22050;
    float data[num_samples];

    for (unsigned n = 0; n < num_samples; n++)
        data[n] = 0.f;

    addmire_init();

    Cluster c;
    init_cluster_to_wave(&c, 440, WaveType::Sine);
    samples_from_cluster(&c, data, num_samples);

	for (unsigned i = 0; i < num_samples; i++)
	{
		file.write((const char*)& data[i], sizeof(float));
	}

    file.close();
}
