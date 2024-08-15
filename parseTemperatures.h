#ifndef PARSE_TEMPERATURES_H_INCLUDED
#define PARSE_TEMPERATURES_H_INCLUDED

#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

/*
 * Read the CPU core temperatures that were read every interval seconds from inStream
 * @param inTemperatureStream Input stream to read the CPU core temperatures from
 * @param interval Amount of seconds that pass between each CPU core temperature reading
 * @return Vector containing pairs containing the time at which a CPU core temperature reading took place along with the temperatures of the four cores at that time
 */
std::vector<std::pair<int, std::vector<double>>> parseTemperatures(std::istream& inTemperatureStream, int const interval)
{
    /*
     * Vector of pairs of
     * First: Amount of seconds that passed from when the temperature readings began to when the Second's temperatures were read
     * Second: Vector of temperatures that were read at the First's time
     */
    std::string inCoreTemperaturesLine{};
    int totalSecondsElapsed{ 0 };
    std::vector<std::pair<int, std::vector<double>>> allTemperatureReadings{};
    while (getline(inTemperatureStream, inCoreTemperaturesLine))
    {
        std::istringstream coreTemperaturesLine{ inCoreTemperaturesLine };
        std::vector<double> temperatures;
        std::transform(
            std::istream_iterator<std::string>(coreTemperaturesLine),
            std::istream_iterator<std::string>(),
            std::back_inserter(temperatures),
            [](std::string const& coreTemperatureReading) -> double { return stod(coreTemperatureReading); }
        );
        allTemperatureReadings.emplace_back(totalSecondsElapsed, temperatures);
        totalSecondsElapsed += interval;
    }
    return allTemperatureReadings;
}

#endif
