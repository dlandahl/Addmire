
#include <iostream>
#include <fstream>
#include <array>

#include "AddmireCore.h"
#include "AddmireAlgorithms.h"

using namespace add;

int main()
{
    const unsigned num_samples = 44100U;
    float data[num_samples] = { 0 };

    addmire_init();

    Cluster c(55.f, WaveTransforms::Saw);

    AdditiveProcesses::RandomPhase phaserand(&c);

    phaserand.set_strength(var::tau);
    phaserand.proc();
    
    c.draw();
    c.get_samples(data, num_samples);

    std::fstream file("test_data.raw", std::fstream::out | std::fstream::binary);
	for (unsigned n = 0U; n < num_samples; n++)
		file.write((const char*)& data[n], sizeof(float));
}
