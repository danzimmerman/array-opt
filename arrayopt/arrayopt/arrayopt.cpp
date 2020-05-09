// arrayopt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#define FMT_HEADER_ONLY
#define CXXOPTIONS_NO_EXCEPTIONS
#include <iostream>
#include <filesystem>
#include <complex>
#include <fmt/format.h> //while waiting for official C++20 support
#include <Eigen/Dense>
#include <boost/math/quadrature/gauss.hpp>
#include <cxxopts.hpp>

namespace bmq = boost::math::quadrature;

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

double f_dipole(double theta, double phi)
{
    return std::sin(theta);
}

std::complex<double> blm_integrand(double k, double theta, double phi, double xl, double xm, double yl, double ym)
{
    double amp = std::pow(f_dipole(theta, phi), 2);
    double tmx = k * (xl - xm) * std::sin(theta) * std::cos(phi);
    double tmy = k * (yl - ym) * std::sin(theta) * std::sin(phi);
    std::complex<double> arg(0, -(tmx+tmy)); //-i(tmx+tmy)
    
    return amp * std::exp(arg);
}

std::complex<double> blm(double k, double xl, double xm, double yl, double ym)
{
    // what are our options to do the double integral here??
    // https://royalsocietypublishing.org/doi/pdf/10.1098/rspa.2016.0401
    return std::complex(nan(""), nan(""));
}

std::string efstr = "\n**ERROR! {}\n";
std::string msg;

int main(int argc, char** argv)
{
    cxxopts::Options options("arrayopt", "Uses M.T. Ma's approach to compute array currents for optimal directivity.");
    options.add_options()
        ("help", "Print this screen")
        ("freq", "Frequency in MHz", cxxopts::value<double>()->default_value("1.8"))
        ("az", "Target azimuth angle, degrees", cxxopts::value<double>()->default_value("0.0"))
        ("el", "Target elevation angle, degrees", cxxopts::value<double>()->default_value("0.0"))
        ("file", "Wire description file, EZNEC export format, units in meters/mm", cxxopts::value<std::string>())
        ;
    
    double az, el, azrad, elrad, freq;
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
        freq = opts["freq"].as<double>();
        azrad = pi * az / 180;
        elrad = pi * az / 180;
    }
    catch (cxxopts::OptionParseException& e)
    {
        std::cout << fmt::format(efstr, e.what()) << std::endl;
        return EXIT_UNRECOGNIZED_OPTION;
    }

    msg = "N3OX / M.T. Ma optimal directivity array calculator";
    std::cout << header(80, msg) << std::endl;
    
    double k0 = 2 * pi / (freq * 1e6); //wavenumber

    msg = "azimuth: {0:.3f} degrees, elevation: {1:.3f} degrees, freq: {2:.3f} MHz, wavenumber: {3:.3e}/m";
    std::cout << fmt::format(msg, az, el, freq, k0) << std::endl;

    // ==== hardcode some element positions for now ====
    std::vector<double> elx; 
    std::vector<double> ely;
    for (int i = 0; i < 3; i++)
    {
        elx.push_back(20.0 * i);
        ely.push_back(0.0);
    }
    if (!(elx.size() == ely.size()))
    {
        std::cout << fmt::format(efstr, "Element x, y coordinates not the same size!") << std::endl;
    }
    size_t Nel = elx.size();

    // ==== B matrix ====
    Eigen::MatrixXcd B;
    B.resize(Nel, Nel);

    for (size_t l = 0; l < Nel; l++)
    {
        for (size_t m = 0; m < Nel; m++)
        {
            B(l, m) = std::complex(l / 10.0, m / 10.0);
        }
    }
    
    std::cout << B << std::endl;

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
