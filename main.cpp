#include <iostream>
#include <chrono>
#include <random>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/stat.h>

std::vector<double> normalDistribution(double mean, double standardDeviation, int num) {
    std::mt19937 gen((std::random_device()) ());
    std::normal_distribution<double> normal(mean, standardDeviation);
    std::vector<double> res;
    res.reserve(num);
    while (num-- > 0) {
        res.push_back(normal(gen));
    }
    return res;
}


template<typename T>
std::string join(const T &list, const std::string &deliminator) {
    std::ostringstream s;
    for (const auto &i: list) {
        if (&i != &list[0]) {
            s << deliminator;
        }
        s << i;
    }
    return s.str();
}

std::vector<std::string> split(std::string val, const std::string &delimiter) {
    size_t pos = 0;
    std::string token;
    std::vector<std::string> res;
    while ((pos = val.find(delimiter)) != std::string::npos) {
        token = val.substr(0, pos);
        res.push_back(token);
        val.erase(0, pos + delimiter.length());
    }
    res.push_back(val);
    return res;
}

class Microcapsule {
public:
    double radius;
    double initialRadius;
    double thickness;
    double initialThickness;
    bool burst;

    Microcapsule(double radius_, double thickness_, bool burst_) : radius(radius_), thickness(thickness_), burst(burst_) {
        this->initialRadius = radius;
        this->initialThickness = thickness;
    }
};

class Substance {
public:
    double radius;
    double rUncertainty;
    double thickness;
    double tUncertainty;
    int num;
    std::vector<Microcapsule> microcapsules;
    std::vector<double> volReleased;

    Substance(float radius_, float rUncertainty_, float thickness_, float tUncertainty_, int num_, int length) : radius(
            radius_), rUncertainty(rUncertainty_), thickness(thickness_), tUncertainty(tUncertainty_), num(num_) {
        std::vector<double> radii = normalDistribution(radius, rUncertainty, num);
        std::vector<double> thicknesses = normalDistribution(thickness, tUncertainty, num);

        for (int i = 0; i < num; ++i) {
            Microcapsule p = Microcapsule(radii[i], thicknesses[i], false);
            microcapsules.push_back(p);
        }
        volReleased.reserve(length);
        volReleased.push_back(0);
    }
};

int main() {


    std::string FILENAME = "data.csv";
    const double CONSTANT = 4.0 / 3.0 * M_PI;

//    int num1 = 360000000;
//    int num1Remover = 80000;
//    int num2 = 99000;
//    int num3 = 61000;
//    int num4 = 42000;
//    int num5 = 15600;
//
//    int length = 25000;
//    double volRemovedPerStep = 1;

    int num1 = 230000000;
    int num1Remover = 15800;
    int num2 = 17000;
    int num3 = 19400;
    int num4 = 30000;
    int num5 = 16000;

    int length = 100000;
    double volRemovedPerStep = 0.5;

    int timeStep[length];
    timeStep[0] = 1;

    std::vector<double> taste1;
    taste1.reserve(length);
    taste1.push_back(num1);

    Substance taste1Remover = Substance(17, 1.5, 2, 0.1, num1Remover, length);
    Substance taste2 = Substance(17, 1.5, 2, 0.1, num2, length);
    Substance taste2Remover = Substance(18.5, 1.5, 3.5, 0.175, num2, length);
    Substance taste3 = Substance(18.5, 1.5, 3.5, 0.175, num3, length);
    Substance taste3Remover = Substance(20, 1.5, 5, 0.25, num3, length);
    Substance taste4 = Substance(20, 1.5, 5, 0.25, num4, length);
    Substance taste4Remover = Substance(21, 1.5, 6, 0.3, num4, length);
    Substance taste5 = Substance(21, 1.5, 6, 0.3, num5, length);

    Substance tastes[4] = {taste2, taste3, taste4, taste5};
    Substance removers[4] = {taste1Remover, taste2Remover, taste3Remover, taste4Remover};

    std::chrono::milliseconds start = duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());

    for (int i = 1; i < length; ++i) {
        timeStep[i] = i;

        double vols[5] = {taste1[i - 1], 0, 0, 0, 0};
        for (int j = 0; j < 4; ++j) {
            for (Microcapsule &removerMicrocapsule: removers[j].microcapsules) {
                if (!removerMicrocapsule.burst) {
                    double volOfShell = CONSTANT * (pow(removerMicrocapsule.radius, 3) -
                                                    pow(removerMicrocapsule.initialRadius -
                                                        removerMicrocapsule.initialThickness, 3));
                    volOfShell -= volRemovedPerStep;
                    if (volOfShell <= 0 && !removerMicrocapsule.burst) {
                        vols[j] -= CONSTANT * pow(removerMicrocapsule.radius, 3);
                        removerMicrocapsule.burst = true;
                    } else {
                        volOfShell += CONSTANT *
                                      pow(removerMicrocapsule.initialRadius - removerMicrocapsule.initialThickness, 3);
                        removerMicrocapsule.radius = pow(volOfShell / CONSTANT, 1.0 / 3.0);
                        removerMicrocapsule.thickness = removerMicrocapsule.initialThickness - removerMicrocapsule.initialRadius +
                                                    removerMicrocapsule.radius;
                    }
                }
            }
        }
        for (int j = 0; j < 4; ++j) {
            for (Microcapsule &microcapsule: tastes[j].microcapsules) {
                if (!microcapsule.burst) {
                    double volOfShell = CONSTANT * (pow(microcapsule.radius, 3) -
                                                    pow(microcapsule.initialRadius - microcapsule.initialThickness, 3));
                    volOfShell -= volRemovedPerStep;
                    if (volOfShell <= 0 && !microcapsule.burst) {
                        vols[j + 1] += CONSTANT * pow(microcapsule.radius, 3);
                        microcapsule.burst = true;
                    } else {
                        volOfShell += CONSTANT * pow(microcapsule.initialRadius - microcapsule.initialThickness, 3);
                        microcapsule.radius = pow(volOfShell / CONSTANT, 1.0 / 3.0);
                        microcapsule.thickness = microcapsule.initialThickness - microcapsule.initialRadius + microcapsule.radius;
                    }
                }
            }

            if (tastes[j].volReleased[i - 1] + vols[j + 1] > 0) {
                tastes[j].volReleased.push_back(tastes[j].volReleased[i - 1] + vols[j + 1]);
            } else {
                tastes[j].volReleased.push_back(0);
            }
        }
        if (vols[0] > 0) {
            taste1.push_back(vols[0]);
        } else {
            taste1.push_back(0);
        }
        if (i % 500 == 0) {
            std::chrono::milliseconds curr = duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch());
            std::cout << "Elapsed time: " << round((curr.count() - start.count()) / 1000) << "s, Elapsed frames: " << i
                      << ", Remaining Time: " << ((curr.count() - start.count()) / i * (length - i)) / 1000
                      << "s, Remaining Frames: " << length - i << std::endl;
        }
    }

    std::cout << "Attempting to open file " << FILENAME << "... ";

    struct stat buffer;
    int count = 1;
    std::vector<std::string> list = split(FILENAME, ".");
    list[list.size() - 2] += " (" +  std::to_string(count) + ")";
    while (stat(FILENAME.c_str(), &buffer) == 0) {
        std::vector<std::string> endingList = split(list[list.size() - 2], " ");
        endingList[endingList.size() - 1] = "(" + std::to_string(count) + ")";
        list[list.size() - 2] = join(endingList, " ");
        FILENAME = join(list, ".");
        std::cout << "File already exists, trying new file: " << FILENAME << std::endl;
        std::cout << "Attempting to open file " << FILENAME << "... ";
        count++;
    }

    std::ofstream file;
    file.open(FILENAME);

    std::cout << "Success!" << std::endl;
    std::cout << "Writing to file..." << std::endl;

    file << join(taste1, ", ") << std::endl;

    for (auto &taste : tastes) {
        file << join(taste.volReleased, ", ") << std::endl;
    }
    file.close();

    std::cout << "Finished writing to file " << FILENAME << "!" << std::endl;

    return 0;
}