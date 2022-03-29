#pragma once

#include <vector>
#include <string>
#include <complex>

class Vad{
    private:

        //modifiable values
        const double PI = 3.14159265358979323846;
        const int MAX_FREQ = 3400;
        const int MIN_FREQ = 200;
        const int PACKET_SIZE = 160;
        const int NEXT_PACKET = 2;                      //Number of packet u want to include after last voice is detected
        const std::string INPUT_FOLDER = "input/";
        const std::string OUTPUT_FOLDER = "output/";
        const std::string FILE_EXTENSION = ".data";

        //unmodifiable values
        std::vector<signed char> audio;                 //output buffer 
        std::string input_filename; 
        std::string output_filename;
        int to_insert = 0;                              //count for NEXT_PACKET

        /**
         * @brief find the max frequency in a packet
         * 
         * @param vec complex vector
         * @return double 
         */
        double max_freq(std::vector<std::complex<double>> vec) const;

        /**
         * @brief apply fast fourier transform to a complex vector
         * 
         * @param p complex vector
         * @return std::vector<std::complex<double>> 
         */
        std::vector<std::complex<double>> fourier(std::vector<std::complex<double>> p) const;

        /**
         * @brief check if a packet contain voice
         * 
         * @param vec the packet
         * @return true 
         * @return false 
         */
        bool check_voice(std::vector<signed char> vec);

        /**
         * @brief VAD function - decide if a packet is saved or set to 0
         * 
         * @param vec the packet
         * @return true 
         * @return false 
         */
        bool add_packet(std::vector<signed char> &vec);

        /**
         * @brief the "main" function, process all received packets
         * 
         */
        void process();

    public:
        /**
         * @brief Construct a new Vad object
         * 
         * @param i name of input file
         * @param o name of output file
         */
        Vad(std::string i,std::string o);

};
