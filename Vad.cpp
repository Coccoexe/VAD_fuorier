#include "Vad.hpp"
#include <cmath>
#include <fstream>

Vad::Vad(std::string i, std::string o)
{
    input_filename = i;
    output_filename = o;

    process();
}

double Vad::max_freq(std::vector<std::complex<double>> vec) const
{
    double max = 0;

    //get the max in a complex vector
    for(int i = 0; i < vec.size(); i++)
    {
        if(sqrt(pow(vec[i].real(),2) + pow(vec[i].imag(),2)) > max)
            max = sqrt(pow(vec[i].real(),2) + pow(vec[i].imag(),2));
    }

    return max;
}

std::vector<std::complex<double>> Vad::fourier(std::vector<std::complex<double>> p) const
{
    // base case
    if (p.size() == 1)
    {
        return p;
    }
    // even list of complex numbers
    std::vector<std::complex<double>> even_packets;
    for (int i = 0; i < p.size(); i += 2)
    {
        even_packets.push_back(p[i]);
    }
    even_packets = fourier(even_packets);
    // odd list of complex numbers
    std::vector<std::complex<double>> odd_packets;
    for (int i = 1; i < p.size(); i += 2)
    {
        odd_packets.push_back(p[i]);
    }
    odd_packets = fourier(odd_packets);
    // fouriered list of complex numbers
    std::vector<std::complex<double>> ret(p.size());
    for (int i = 0; i < p.size() / 2; i++)
    {
        std::complex<double> w(cos(2 * PI * i / p.size()), sin(2 * PI * i / p.size()));
        ret[i] = even_packets[i] + w * odd_packets[i];
        ret[i + p.size() / 2] = even_packets[i] - w * odd_packets[i];
    }
    return ret;
}

bool Vad::check_voice(std::vector<signed char> vec)
{
    std::vector<std::complex<double>> c;
    for(int i = 0; i < vec.size(); i++)
        c.push_back(std::complex<double>(vec[i]));
    
    //fast fourier transform
    std::vector<std::complex<double>> fft = fourier(c);

    //get the max freq
    double freq = max_freq(fft);

    //check if packet contain voice
    if(freq > MIN_FREQ && freq < MAX_FREQ)
    {
        to_insert = NEXT_PACKET;
        return true;
    }

    return false;
}

bool Vad::add_packet(std::vector<signed char> &vec)
{
    //if prev packet was voice
    if(to_insert >= 1)
    {
        to_insert--;
        check_voice(vec);
        return true;
    }

    return check_voice(vec);
}

void Vad::process()
{
    std::ifstream input(INPUT_FOLDER + input_filename + FILE_EXTENSION, std::ifstream::binary);
    std::ofstream output(OUTPUT_FOLDER + output_filename + FILE_EXTENSION, std::ofstream::binary);
    std::ofstream outxt(OUTPUT_FOLDER + output_filename + ".txt");

    //buffer
    std::vector<signed char> old;

    while(input.good())
    {
        std::vector<signed char> tmp;

        //get packet
        for(int i = 0; i < PACKET_SIZE && input.good(); i++)
        {
            signed char n;
            input.read((char *)&n,sizeof(n));
            tmp.push_back(n);
        }

        //add packet
        if(add_packet(tmp))
        {
            //buffer full
            if(old.empty())
            {
                outxt << '1';
                for(int i = 0; i < tmp.size(); i++)
                    audio.push_back(tmp[i]);
            }
            //buffer empty
            else
            {      
                outxt << '1';  
                for(int i = 0; i < old.size(); i++)
                    audio.push_back(old[i]);
                outxt << '1';
                for(int i = 0; i < tmp.size(); i++)
                    audio.push_back(tmp[i]);

                old.clear();
            }
        }
        //save packet in buffer
        else
        {
            //buffer full
            if(!old.empty())
            {
                outxt << '0';
                for(int i = 0; i < old.size(); i++)
                    audio.push_back(0);

            }
            old = tmp; //save in buffer the packet
        }

        //write the processed packet
        for(int i = 0; i < audio.size(); i++)
            output.write((char *)&audio[i],sizeof(char));

        audio.clear();
    }

    //final check for buffer
    if(!old.empty())
    {
        outxt << '0';
        for(int i = 0; i < old.size(); i++)
            audio.push_back(0);
    }

    for(int i = 0; i < audio.size(); i++)
        output.write((char *)&audio[i],sizeof(char));

    input.close();
    output.close();
    outxt.close();
}
