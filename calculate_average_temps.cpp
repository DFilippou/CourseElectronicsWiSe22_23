// mean.cpp : Calculate mean temperature from .log ifile

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>

const char DELIMITER = ',';

int main()
{
    // define input and output files
    std::ifstream ifile;
    std::ofstream ofile;
    ifile.open("TSENSOR_DF.LOG");
    ofile.open("mean_temp.txt", std::ofstream::out | std::ofstream::trunc);

    // if file does not exist exit with error
    if (!ifile.good())
    {
        throw std::runtime_error("File does not exist!");
        return 1;
    }

    // initialize a buffer to load lines from ifile
    std::string buffer;

    double sum = 0;
    long n = 0;
    while (std::getline(ifile, buffer))
    {
        // convert the line into stream:
        std::istringstream ss(buffer);

        // declare vector of string (instead of fixed array)
        std::vector<std::string> vec;

        // read the line, word by word
        while (std::getline(ss, buffer, DELIMITER))
        {
            vec.push_back(buffer);
        }

        double value = std::stod(vec.back());
        sum += value;
        n++;

        if (n % 40 == 0)
        {
            ofile << sum / n << std::endl;
            sum = 0;
            n = 0;
        }
    }

    // Close files
    ifile.close();
    ofile.close();
    return 0;
}
