/*
 * William Varnier
 * 26 February 2023
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include "parseTemperatures.h"

/*
 * Get the timestamps when each CPU core temperature reading took place and all of the recorded core temperatures
 * @param inCoreTemperatureFile File to read the CPU core temperature data from
 * @param outCoreReadingTimestamps Vector of numbers representing the amount of seconds that had passed when each core temperature reading took place
 * @param outCore0Temperatures Vector of all recorded temperatures for CPU core 0
 * @param outCore1Temperatures Vector of all recorded temperatures for CPU core 1
 * @param outCore2Temperatures Vector of all recorded temperatures for CPU core 2
 * @param outCore3Temperatures Vector of all recorded temperatures for CPU core 3
 */
void getCoreTemperatureData(
    std::ifstream inCoreTemperatureFile,
    std::vector<int>& outCoreReadingTimestamps,
    std::vector<double>& outCore0Temperatures,
    std::vector<double>& outCore1Temperatures,
    std::vector<double>& outCore2Temperatures,
    std::vector<double>& outCore3Temperatures
);

/*
 * Print the core temperature results of all four CPU cores
 * @param inCoreReadingTimestamps Vector of numbers representing the amount of seconds that had passed when each core temperature reading took place
 * @param inCore0Temperatures Vector of all recorded temperatures for CPU core 0
 * @param inCore1Temperatures Vector of all recorded temperatures for CPU core 1
 * @param inCore2Temperatures Vector of all recorded temperatures for CPU core 2
 * @param inCore3Temperatures Vector of all recorded temperatures for CPU core 3
 */
void printAllCoreTemperatureResults(
    std::vector<int> const& inCoreReadingTimestamps,
    std::vector<double> const& inCore0Temperatures,
    std::vector<double> const& inCore1Temperatures,
    std::vector<double> const& inCore2Temperatures,
    std::vector<double> const& inCore3Temperatures
);

/*
 * Print the y-intercepts and slopes of the inCoreTemperatures to outCoreTemperatureFile
 * @param inCoreReadingTimestamps Vector of numbers representing the amount of seconds that had passed when each core temperature reading took place
 * @param inCoreTemperatures Vector of CPU core temperatures
 * @param outCoreTemperatureFile Output file that the CPU core temperature results will be printed to
 */
void printCoreTemperatureResults(
    std::vector<int> const& inCoreReadingTimestamps,
    std::vector<double> const& inCoreTemperatures,
    std::ofstream& outCoreTemperatureFile
);

/*
 * Calculate lines between core temperature reading data points
 * @param inCoreTemperatures Vector of CPU core temperatures
 * @param inCoreReadingTimestamps Vector of numbers representing the amount of seconds that had passed when each core temperature reading took place
 * @param outCoreTemperatureFile Output file that the piecewise linear interpolation results will be printed to
 * @param outXYProductSum Sum of all of the core temperature reading timestamps and the distance of the lines connecting the core temperature reading data points
 * @param outXSum Sum of all the core temperature reading timestamps
 * @param outYSum Sum of all the distances between lines connecting core temperature reading data points
 * @param outXSquaredSum Sum of all squared core temperature reading timestamps
 */
void calculatePiecewiseLinearInterpolation(
    std::vector<double> const& inCoreTemperatures,
    std::vector<int> const& inCoreReadingTimestamps,
    std::ofstream& outCoreTemperatureFile,
    double& outXYProductSum,
    int& outXSum,
    double& outYSum,
    int& outXSquaredSum
);

/*
 * Calculate the line of best fit among the core temperature reading data points
 * @param inCoreReadingTimestamps Vector of numbers representing the amount of seconds that had passed when each core temperature reading took place
 * @param inXYProductSum Sum of all of the core temperature reading timestamps and the distance of the lines connecting the core temperature reading data points
 * @param inXSum Sum of all the core temperature reading timestamps
 * @param inYSum Sum of all the distances between lines connecting core temperature reading data points
 * @param inXSquaredSum Sum of all squared core temperature reading timestamps
 * @param outCoreTemperatureFile Output file that the least squares approximation results will be printed to
 */
void calculateLeastSquaresApproximation(
    std::vector<int> const& inCoreReadingTimestamps,
    double const inXYProductSum,
    int const inXSum,
    double const inYSum,
    int const inXSquaredSum,
    std::ofstream& outCoreTemperatureFile
);

/*
 * Perform piecewise linear interpolation on CPU core temperatures
 * @param argc Number of command line arguments
 * @param argv Array of command line argument values
 * @param argv[1] Filename of the file containing the input CPU core temperatures
 */
int main(int argc, char** argv)
{
    if (argc != 2) 
    {
        std::cout << "Invalid command line arguments. To run the program use the command CoreTemperatures [input file name]\n";
    }
    else 
    {
        std::vector<int> coreReadingTimestamps{};
        std::vector<double> core0Temperatures{};
        std::vector<double> core1Temperatures{};
        std::vector<double> core2Temperatures{};
        std::vector<double> core3Temperatures{};
        getCoreTemperatureData(
            std::ifstream(argv[1]),
            coreReadingTimestamps,
            core0Temperatures,
            core1Temperatures,
            core2Temperatures,
            core3Temperatures
        );
        printAllCoreTemperatureResults(
            coreReadingTimestamps,
            core0Temperatures,
            core1Temperatures,
            core2Temperatures,
            core3Temperatures
        );
    }
}

void getCoreTemperatureData(
    std::ifstream inCoreTemperatureFile,
    std::vector<int>& outCoreReadingTimestamps,
    std::vector<double>& outCore0Temperatures,
    std::vector<double>& outCore1Temperatures,
    std::vector<double>& outCore2Temperatures,
    std::vector<double>& outCore3Temperatures)
{
    std::vector<std::pair<int, std::vector<double>>> coreTemperatureReadings{};
    if (inCoreTemperatureFile)
    {
        coreTemperatureReadings = parseTemperatures(inCoreTemperatureFile, 30);
    }
    for (std::pair<int, std::vector<double>> coretemperatureReading : coreTemperatureReadings)
    {
        outCoreReadingTimestamps.push_back(coretemperatureReading.first);
        std::vector<double> const& coreTemperatures{ coretemperatureReading.second };
        for (int i = 0; i < coreTemperatures.size(); i++)
        {
            double const coreTemperature{ coreTemperatures[i] };
            switch (i)
            {
                case 0:
                    outCore0Temperatures.push_back(coreTemperature);
                break;
                case 1:
                    outCore1Temperatures.push_back(coreTemperature);
                break;
                case 2:
                    outCore2Temperatures.push_back(coreTemperature);
                break;
                case 3:
                    outCore3Temperatures.push_back(coreTemperature);
                break;
            }
        }
    }
}

void printAllCoreTemperatureResults(
    std::vector<int> const& inCoreReadingTimestamps,
    std::vector<double> const& inCore0Temperatures,
    std::vector<double> const& inCore1Temperatures,
    std::vector<double> const& inCore2Temperatures,
    std::vector<double> const& inCore3Temperatures)
{
    {
        std::ofstream outCoreTemperatureFile0{ "outputCore0.txt" };
        outCoreTemperatureFile0 << "Core 0:\n";
        printCoreTemperatureResults(inCoreReadingTimestamps, inCore0Temperatures, outCoreTemperatureFile0);
    }
    {
        std::ofstream outCoreTemperatureFile1{ "outputCore1.txt" };
        outCoreTemperatureFile1 << "Core 1:\n";
        printCoreTemperatureResults(inCoreReadingTimestamps, inCore1Temperatures, outCoreTemperatureFile1);
    }
    {
        std::ofstream outCoreTemperatureFile2{ "outputCore2.txt" };
        outCoreTemperatureFile2 << "Core 2:\n";
        printCoreTemperatureResults(inCoreReadingTimestamps, inCore2Temperatures, outCoreTemperatureFile2);
    }
    {
        std::ofstream outCoreTemperatureFile3{ "outputCore3.txt" };
        outCoreTemperatureFile3 << "Core 3:\n";
        printCoreTemperatureResults(inCoreReadingTimestamps, inCore3Temperatures, outCoreTemperatureFile3);
    }
}

void printCoreTemperatureResults(
    std::vector<int> const& inCoreReadingTimestamps,
    std::vector<double> const& inCoreTemperatures,
    std::ofstream& outCoreTemperatureFile)
{
    /*
     * X = Timestamp when the core temperature reading took place
     * Y = Line connecting two core temperature readings
     */
    double xyProductSum{ 0.0 };
    int xSum{ 0 };
    double ySum{ 0.0 };
    int xSquaredSum{ 0 };
    calculatePiecewiseLinearInterpolation(
        inCoreTemperatures,
        inCoreReadingTimestamps,
        outCoreTemperatureFile,
        xyProductSum,
        xSum,
        ySum,
        xSquaredSum
    );
    calculateLeastSquaresApproximation(
        inCoreReadingTimestamps,
        xyProductSum,
        xSum,
        ySum,
        xSquaredSum,
        outCoreTemperatureFile
    );
}

void calculatePiecewiseLinearInterpolation(
    std::vector<double> const& inCoreTemperatures,
    std::vector<int> const& inCoreReadingTimestamps,
    std::ofstream& outCoreTemperatureFile,
    double& outXYProductSum,
    int& outXSum,
    double& outYSum,
    int& outXSquaredSum)
{
    for (int i = 0; i < inCoreTemperatures.size() - 1; i++)
    {
        int const x{ inCoreReadingTimestamps[i] };
        outCoreTemperatureFile << std::fixed << std::setprecision(4) << x << " <= x < " << inCoreReadingTimestamps[i + 1] << "; y_" << i << " = ";
        double const currentCoreTemperature{ inCoreTemperatures[i] };
        double const slope{ (inCoreTemperatures[i + 1] - currentCoreTemperature) / 30.0 };
        double const slopeTimeStampProduct{ slope * x };
        double const yIntercept{ currentCoreTemperature - slopeTimeStampProduct };
        outCoreTemperatureFile << yIntercept << " + " << slope << "x; interpolation\n";
        double const y{ yIntercept + slopeTimeStampProduct };
        outXYProductSum += x * y;
        outXSum += x;
        outYSum += y;
        outXSquaredSum += x * x;
    }
}

void calculateLeastSquaresApproximation(
    std::vector<int> const& inCoreReadingTimestamps,
    double const inXYProductSum,
    int const inXSum,
    double const inYSum,
    int const inXSquaredSum,
    std::ofstream& outCoreTemperatureFile)
{
    unsigned __int64 const numCoreReadings{ inCoreReadingTimestamps.size() };
    double c1{ (numCoreReadings * inXYProductSum - (inXSum * inYSum)) / (numCoreReadings * inXSquaredSum - (inXSum * inXSum)) };
    double c0{ inYSum / numCoreReadings - (inXSum / numCoreReadings * c1) };
    outCoreTemperatureFile << c0 << " + " << c1 << "x\n";
}
