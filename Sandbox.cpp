
#include <iostream>
#include <fstream>
#include <array>
#include <random>

#include "AddmireCore.h"
#include "AddmireAlgorithms.h"

int main()
{
    unsigned const num_samples = 4410u * 2;
    float data[num_samples] = { 0 };

	add::addmire_init();
    
    auto engine = std::default_random_engine(5);
    auto distribution = std::uniform_real_distribution<float>(-1., 1.);

    for (unsigned n = 0; n < num_samples; n++)
        data[n] = distribution(engine);
    

//    auto a = add::Cluster(440, add::WaveTransforms::Tri);
//    a.get_samples(data, num_samples);
    auto c = add::Cluster::from_dft(data, num_samples);
    c.draw();

//    std::fstream file("test_data.raw", std::fstream::out | std::fstream::binary);
//    for (unsigned n = 0u; n < num_samples; n++)
//        file.write((char const*)& data[n], sizeof(float));
}
