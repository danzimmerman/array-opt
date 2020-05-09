// arrayopt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#define FMT_HEADER_ONLY
#define CXXOPTIONS_NO_EXCEPTIONS
#include <iostream>
#include <filesystem>
#include <fmt/format.h> //while waiting for official C++20 support
#include <Eigen/Dense>
#include <cxxopts.hpp>

#define pi EIGEN_PI

constexpr int EXIT_UNRECOGNIZED_OPTION = 2;
constexpr int EXIT_LINALG_FAILURE = 3;

std::string header(const size_t width, const std::string msg)
{
    std::string hdr = "";
    size_t hmn = (width - msg.size()-4);
    if (hmn % 2 == 1)
    {
        hmn += 1;
    }
    hdr += "\n";
    for (int i = 0; i < hmn / 2; i++)
    {
        hdr += "-";
    }
    hdr += " " + msg + " ";
    while (!(hdr.size() == width))
    {
        hdr += "-";
    }
    hdr += "\n";
    return hdr;
}

std::string efstr = "\n**ERROR! {}\n";
std::string msg;

int main(int argc, char** argv)
{
    cxxopts::Options options("arrayopt", "Uses M.T. Ma's approach to compute array currents for optimal directivity.");
    options.add_options()
        ("help", "Print this screen")
        ("az", "Target azimuth angle, degrees", cxxopts::value<double>()->default_value("0.0"))
        ("el", "Target elevation angle, degrees", cxxopts::value<double>()->default_value("0.0"))
        ("file", "Wire description file, EZNEC export format, units in meters/mm", cxxopts::value<std::string>())
        ;
    
    double az, el, azrad, elrad;
    std::string fn;
    try
    {
        auto opts = options.parse(argc, argv);
        if (opts.count("help"))
        {
            std::cout << options.help() << std::endl;
            return EXIT_FAILURE;
        }
        az = opts["az"].as<double>();
        el = opts["el"].as<double>();
        azrad = pi * az / 180;
        elrad = pi * az / 180;
    }
    catch (cxxopts::OptionParseException& e)
    {
        std::cout << fmt::format(efstr, e.what());
        return EXIT_UNRECOGNIZED_OPTION;
    }

    msg = "N3OX / M.T. Ma optimal directivity array calculator";
    std::cout << header(80, msg) << std::endl;
    
    msg = "azimuth: {0:.3f} degrees, elevation: {1:.3f} degrees";
    std::cout << fmt::format(msg, az, el) << std::endl;
    
    std::cout << header(80, "Finished");
    return EXIT_SUCCESS;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
