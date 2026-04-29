#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

class Resident {
public:
    string residentID;
    int age;
    string modeOfTransport;
    double dailyDistance;
    double carbonEmissionFactor;
    int averageDayPerMonth;

    Resident() {}

    Resident(string id, int a, string mode, double distance, double factor, int days) {
        residentID = id;
        age = a;
        modeOfTransport = mode;
        dailyDistance = distance;
        carbonEmissionFactor = factor;
        averageDayPerMonth = days;
    }

    double calculateEmission() {
        return dailyDistance * carbonEmissionFactor * averageDayPerMonth;
    }
};

class ResidentArray {
private:
    Resident residents[300];
    int size = 0;

public:
    void addResident(Resident r) {
        if (size < 300) { 
            residents[size] = r;
            size++;
        }
    }

    int getSize() {
        return size;
    }

    Resident getResident(int index) {
        return residents[index];
    }
};

class FileManager {
private:
    ResidentArray* resArray;

public:
    void setResidentArray(ResidentArray* array) {
        this->resArray = array;
    }

    void loadFromCSV(string datafile) {
        ifstream file(datafile);

        if (!file.is_open()) {
            cout << "Error opening file: " << datafile << endl;
            return;
        }

        string line;

        getline(file, line);

        while (getline(file, line)) {
            stringstream ss(line);

            string id, ageStr, transport, distanceStr, factorStr, daysStr;

            getline(ss, id, ',');
            getline(ss, ageStr, ',');
            getline(ss, transport, ',');
            getline(ss, distanceStr, ',');
            getline(ss, factorStr, ',');
            getline(ss, daysStr);

            Resident resident(
                id,
                stoi(ageStr),
                transport,
                stod(distanceStr),
                stod(factorStr),
                stoi(daysStr)
            );

            resArray->addResident(resident);
        }

        file.close();
    }
};

void analyzeDataset(ResidentArray& dataset, string cityName) {
    double totalDatasetEmission = 0;

    double carEmission = 0;
    double busEmission = 0;
    double bicycleEmission = 0;
    double walkingEmission = 0;
    double schoolBusEmission = 0;
    double carpoolEmission = 0;

    for (int i = 0; i < dataset.getSize(); i++) {
        Resident r = dataset.getResident(i);

        double emission = r.calculateEmission();

        totalDatasetEmission += emission;

        if (r.modeOfTransport == "Car") {
            carEmission += emission;
        }
        else if (r.modeOfTransport == "Bus") {
            busEmission += emission;
        }
        else if (r.modeOfTransport == "Bicycle") {
            bicycleEmission += emission;
        }
        else if (r.modeOfTransport == "Walking") {
            walkingEmission += emission;
        }
        else if (r.modeOfTransport == "School Bus") {
            schoolBusEmission += emission;
        }
        else if (r.modeOfTransport == "Carpool") {
            carpoolEmission += emission;
        }
    }

    cout << fixed << setprecision(2);

    cout << "\n================ " << cityName << " ================\n";

    cout << "\n----- TOTAL CARBON EMISSION PER DATASET -----\n";
    cout << cityName << " Total Carbon Emission: " << totalDatasetEmission << " kg CO2\n";

    cout << "\n----- CARBON EMISSION PER MODE OF TRANSPORT -----\n";
    cout << left << setw(15) << "Transport Mode" << setw(20) << "Emission (kg CO2)" << endl;
    cout << "------------------------------------------\n";

    cout << left << setw(15) << "Car" << carEmission << endl;
    cout << left << setw(15) << "Bus" << busEmission << endl;
    cout << left << setw(15) << "Bicycle" << bicycleEmission << endl;
    cout << left << setw(15) << "Walking" << walkingEmission << endl;
    cout << left << setw(15) << "School Bus" << schoolBusEmission << endl;
    cout << left << setw(15) << "Carpool" << carpoolEmission << endl;
}

int main() {
    ResidentArray cityA, cityB, cityC;

    FileManager fileA, fileB, fileC;

    fileA.setResidentArray(&cityA);
    fileB.setResidentArray(&cityB);
    fileC.setResidentArray(&cityC);

    fileA.loadFromCSV("../../data/dataset1-cityA.csv");
    fileB.loadFromCSV("../../data/dataset2-cityB.csv");
    fileC.loadFromCSV("../../data/dataset3-cityC.csv");

    analyzeDataset(cityA, "City A");
    analyzeDataset(cityB, "City B");
    analyzeDataset(cityC, "City C");

    return 0;
}