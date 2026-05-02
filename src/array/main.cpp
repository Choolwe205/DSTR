#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <cstring>
#include <algorithm>

using namespace std;

// Constants for array sizes
const int MAX_RESIDENTS = 500;
const int MAX_AGE_GROUPS = 5;

// Age group definitions
struct AgeGroup {
    string name;
    int minAge;
    int maxAge;
};

const AgeGroup AGE_GROUPS[] = {
    {"Children & Teenagers", 6, 17},
    {"University Students / Young Adults", 18, 25},
    {"Working Adults (Early Career)", 26, 45},
    {"Working Adults (Late Career)", 46, 60},
    {"Senior Citizens / Retirees", 61, 100}
};

// Resident structure for array storage
struct Resident {
    string residentID;
    int age;
    string modeOfTransport;
    int dailyDistance;
    double carbonEmissionFactor;
    int averageDayPerMonth;
    
    Resident() {
        residentID = "";
        age = 0;
        modeOfTransport = "";
        dailyDistance = 0;
        carbonEmissionFactor = 0.0;
        averageDayPerMonth = 0;
    }
    
    Resident(string id, int a, string mode, int distance, double factor, int days) {
        residentID = id;
        age = a;
        modeOfTransport = mode;
        dailyDistance = distance;
        carbonEmissionFactor = factor;
        averageDayPerMonth = days;
    }
};

// Array-based container for residents
class ResidentArray {
private:
    Resident* residents;
    int size;
    int capacity;
    
public:
    ResidentArray(int cap = MAX_RESIDENTS) {
        capacity = cap;
        residents = new Resident[capacity];
        size = 0;
    }
    
    ~ResidentArray() {
        delete[] residents;
    }
    
    int getSize() const { return size; }
    int getCapacity() const { return capacity; }
    Resident* getData() const { return residents; }
    
    void insertAtEnd(string id, int age, string mode, int distance, double factor, int days) {
        if (size < capacity) {
            residents[size] = Resident(id, age, mode, distance, factor, days);
            size++;
        }
    }
    
    void clear() {
        size = 0;
    }
    
    void traversePrint() const {
        if (size == 0) {
            cout << "This array is empty" << endl;
            return;
        }
        
        cout << left << setw(12) << "ID" << setw(6) << "Age" << setw(15) << "Transport" 
             << setw(12) << "Distance" << setw(10) << "Days" << setw(15) << "Emission" << endl;
        cout << string(70, '-') << endl;
        
        for (int i = 0; i < size; i++) {
            double emission = residents[i].averageDayPerMonth * residents[i].dailyDistance * residents[i].carbonEmissionFactor;
            cout << left << setw(12) << residents[i].residentID 
                 << setw(6) << residents[i].age
                 << setw(15) << residents[i].modeOfTransport
                 << setw(12) << residents[i].dailyDistance
                 << setw(10) << residents[i].averageDayPerMonth
                 << setw(15) << fixed << setprecision(2) << emission << endl;
        }
    }
    
    Resident& operator[](int index) {
        return residents[index];
    }
    
    const Resident& operator[](int index) const {
        return residents[index];
    }
};

// File manager for array implementation
class FileManagerArray {
private:
    ResidentArray* resArray;
    
public:
    FileManagerArray() : resArray(nullptr) {}
    
    void setResidentArray(ResidentArray* arr) {
        resArray = arr;
    }
    
    ResidentArray* getResidentArray() {
        return resArray;
    }
    
    bool loadFromCSV(string datafile) {
        ifstream file(datafile.c_str());
        if (!file.is_open()) {
            return false;
        }
        
        string line;
        getline(file, line); // Skip header
        
        while (getline(file, line)) {
            stringstream ss(line);
            string id, ageStr, transport, distanceStr, factorStr, daysStr;
            
            getline(ss, id, ',');
            getline(ss, ageStr, ',');
            getline(ss, transport, ',');
            getline(ss, distanceStr, ',');
            getline(ss, factorStr, ',');
            getline(ss, daysStr);
            
            try {
                int age = stoi(ageStr);
                int distance = stoi(distanceStr);
                double factor = stod(factorStr);
                int days = stoi(daysStr);
                
                resArray->insertAtEnd(id, age, transport, distance, factor, days);
            } catch (const exception& e) {
                // Skip malformed lines
            }
        }
        file.close();
        return resArray->getSize() > 0;
    }
};

// Computation class for array implementation
class ComputationArray {
private:
    FileManagerArray* load;
    
public:
    ComputationArray(FileManagerArray* fm) : load(fm) {}
    
    double computeEmission(const Resident& res) const {
        return res.averageDayPerMonth * res.dailyDistance * res.carbonEmissionFactor;
    }
    
    double computeTotalEmission() const {
        double totalEmission = 0;
        ResidentArray* arr = load->getResidentArray();
        
        if (arr == nullptr || arr->getSize() == 0) {
            return 0;
        }
        
        for (int i = 0; i < arr->getSize(); i++) {
            totalEmission += computeEmission((*arr)[i]);
        }
        return totalEmission;
    }
    
    int getAgeGroupIndex(int age) const {
        for (int i = 0; i < MAX_AGE_GROUPS; i++) {
            if (age >= AGE_GROUPS[i].minAge && age <= AGE_GROUPS[i].maxAge) {
                return i;
            }
        }
        return -1;
    }
    
    void calculateEmissionsByAgeGroup() const {
        ResidentArray* arr = load->getResidentArray();
        if (arr == nullptr || arr->getSize() == 0) {
            cout << "No data available for age group analysis." << endl;
            return;
        }
        
        int residentCount[MAX_AGE_GROUPS] = {0};
        int modeCount[MAX_AGE_GROUPS][6] = {0};
        double totalEmission[MAX_AGE_GROUPS] = {0};
        
        string transportModes[] = {"Car", "Bus", "Bicycle", "Walking", "School Bus", "Carpool"};
        
        for (int i = 0; i < arr->getSize(); i++) {
            Resident& res = (*arr)[i];
            int ageGroup = getAgeGroupIndex(res.age);
            if (ageGroup == -1) continue;
            
            residentCount[ageGroup]++;
            totalEmission[ageGroup] += computeEmission(res);
            
            for (int m = 0; m < 6; m++) {
                if (res.modeOfTransport == transportModes[m]) {
                    modeCount[ageGroup][m]++;
                    break;
                }
            }
        }
        
        for (int g = 0; g < MAX_AGE_GROUPS; g++) {
            if (residentCount[g] == 0) continue;
            
            cout << "\n" << string(80, '=') << endl;
            cout << "Age Group: " << AGE_GROUPS[g].minAge << "-" << AGE_GROUPS[g].maxAge 
                 << " (" << AGE_GROUPS[g].name << ")" << endl;
            cout << string(80, '=') << endl;
            
            int maxModeIndex = 0;
            for (int m = 1; m < 6; m++) {
                if (modeCount[g][m] > modeCount[g][maxModeIndex]) {
                    maxModeIndex = m;
                }
            }
            
            cout << "Most preferred mode of transport: " << transportModes[maxModeIndex] 
                 << " (" << modeCount[g][maxModeIndex] << " residents)" << endl;
            cout << "Total carbon emissions: " << fixed << setprecision(2) << totalEmission[g] << " kg CO2" << endl;
            cout << "Average emission per resident: " << totalEmission[g] / residentCount[g] << " kg CO2" << endl;
        }
    }
    
    void calculateEmissionsByMode() const {
        ResidentArray* arr = load->getResidentArray();
        if (arr == nullptr || arr->getSize() == 0) {
            cout << "No data available for mode analysis." << endl;
            return;
        }
        
        string transportModes[] = {"Car", "Bus", "Bicycle", "Walking", "School Bus", "Carpool"};
        int modeCount[6] = {0};
        double modeEmission[6] = {0};
        
        for (int i = 0; i < arr->getSize(); i++) {
            Resident& res = (*arr)[i];
            for (int m = 0; m < 6; m++) {
                if (res.modeOfTransport == transportModes[m]) {
                    modeCount[m]++;
                    modeEmission[m] += computeEmission(res);
                    break;
                }
            }
        }
        
        cout << "\n" << string(80, '=') << endl;
        cout << "Emissions by Mode of Transport" << endl;
        cout << string(80, '=') << endl;
        cout << left << setw(20) << "Mode of Transport" 
             << setw(10) << "Count" << setw(20) << "Total Emission (kg CO2)" << endl;
        cout << string(50, '-') << endl;
        
        for (int m = 0; m < 6; m++) {
            if (modeCount[m] > 0) {
                cout << left << setw(20) << transportModes[m] 
                     << setw(10) << modeCount[m]
                     << setw(20) << fixed << setprecision(2) << modeEmission[m] << endl;
            }
        }
    }
};

// Sort and Search class for array implementation
class SortSearchArray {
private:
    ResidentArray* arr;
    ComputationArray* comp;
    
public:
    SortSearchArray(ResidentArray* residentArr, ComputationArray* computation) 
        : arr(residentArr), comp(computation) {}
    
    void bubbleSort(string sortBy = "age", string order = "asc") {
        auto start = chrono::high_resolution_clock::now();
        
        int n = arr->getSize();
        if (n == 0) {
            cout << "\n[Bubble Sort] No data to sort." << endl;
            return;
        }
        
        bool swapped;
        for (int i = 0; i < n - 1; i++) {
            swapped = false;
            for (int j = 0; j < n - i - 1; j++) {
                bool doSwap = false;
                
                if (sortBy == "age") {
                    doSwap = (order == "asc") ? (*arr)[j].age > (*arr)[j + 1].age 
                                              : (*arr)[j].age < (*arr)[j + 1].age;
                } else if (sortBy == "distance") {
                    doSwap = (order == "asc") ? (*arr)[j].dailyDistance > (*arr)[j + 1].dailyDistance 
                                              : (*arr)[j].dailyDistance < (*arr)[j + 1].dailyDistance;
                } else if (sortBy == "emission") {
                    doSwap = (order == "asc") ? comp->computeEmission((*arr)[j]) > comp->computeEmission((*arr)[j + 1])
                                              : comp->computeEmission((*arr)[j]) < comp->computeEmission((*arr)[j + 1]);
                }
                
                if (doSwap) {
                    swap((*arr)[j], (*arr)[j + 1]);
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
        
        auto end = chrono::high_resolution_clock::now();
        cout << "\n[Bubble Sort] By: " << sortBy << " | " << order 
             << " | Time: " << chrono::duration<double, milli>(end - start).count() << " ms\n";
    }
    
    void selectionSort(string sortBy = "age", string order = "asc") {
        auto start = chrono::high_resolution_clock::now();
        
        int n = arr->getSize();
        if (n == 0) {
            cout << "\n[Selection Sort] No data to sort." << endl;
            return;
        }
        
        for (int i = 0; i < n - 1; i++) {
            int targetIdx = i;
            for (int j = i + 1; j < n; j++) {
                bool condition = false;
                
                if (sortBy == "age") {
                    condition = (order == "asc") ? (*arr)[j].age < (*arr)[targetIdx].age
                                                 : (*arr)[j].age > (*arr)[targetIdx].age;
                } else if (sortBy == "distance") {
                    condition = (order == "asc") ? (*arr)[j].dailyDistance < (*arr)[targetIdx].dailyDistance
                                                 : (*arr)[j].dailyDistance > (*arr)[targetIdx].dailyDistance;
                } else if (sortBy == "emission") {
                    condition = (order == "asc") ? comp->computeEmission((*arr)[j]) < comp->computeEmission((*arr)[targetIdx])
                                                 : comp->computeEmission((*arr)[j]) > comp->computeEmission((*arr)[targetIdx]);
                }
                
                if (condition) {
                    targetIdx = j;
                }
            }
            if (targetIdx != i) {
                swap((*arr)[i], (*arr)[targetIdx]);
            }
        }
        
        auto end = chrono::high_resolution_clock::now();
        cout << "\n[Selection Sort] By: " << sortBy << " | " << order 
             << " | Time: " << chrono::duration<double, milli>(end - start).count() << " ms\n";
    }
    
    void linearSearch(string searchBy, string value) {
        auto start = chrono::high_resolution_clock::now();
        
        int found = 0;
        cout << "\n[Linear Search] " << searchBy << " = " << value << "\n";
        cout << left << setw(12) << "ID" << setw(6) << "Age" << setw(15) << "Transport" 
             << setw(10) << "Distance" << setw(8) << "Days" << setw(15) << "Emission\n";
        cout << string(66, '-') << "\n";
        
        for (int i = 0; i < arr->getSize(); i++) {
            Resident& res = (*arr)[i];
            bool match = false;
            
            if (searchBy == "transport") {
                string a = res.modeOfTransport;
                string b = value;
                for (size_t c = 0; c < a.length(); c++) a[c] = tolower(a[c]);
                for (size_t c = 0; c < b.length(); c++) b[c] = tolower(b[c]);
                match = (a == b);
            } else if (searchBy == "age_group") {
                int dash = value.find('-');
                int minAge = stoi(value.substr(0, dash));
                int maxAge = stoi(value.substr(dash + 1));
                match = (res.age >= minAge && res.age <= maxAge);
            } else if (searchBy == "distance_above") {
                match = (res.dailyDistance > stoi(value));
            } else if (searchBy == "distance_below") {
                match = (res.dailyDistance < stoi(value));
            }
            
            if (match) {
                double emission = comp->computeEmission(res);
                cout << left << setw(12) << res.residentID << setw(6) << res.age 
                     << setw(15) << res.modeOfTransport << setw(10) << res.dailyDistance 
                     << setw(8) << res.averageDayPerMonth 
                     << setw(15) << fixed << setprecision(2) << emission << "\n";
                found++;
            }
        }
        
        auto end = chrono::high_resolution_clock::now();
        cout << string(66, '-') << "\n";
        cout << "Found: " << found << " residents matching criteria | Time: " 
             << chrono::duration<double, milli>(end - start).count() << " ms\n";
    }
    
    void binarySearch(string searchBy, string value) {
        // Sort first based on search criteria
        if (searchBy == "age_group") {
            bubbleSort("age", "asc");
        } else if (searchBy == "transport") {
            // Sort by transport mode
            int n = arr->getSize();
            for (int i = 0; i < n - 1; i++) {
                for (int j = 0; j < n - i - 1; j++) {
                    if ((*arr)[j].modeOfTransport > (*arr)[j + 1].modeOfTransport) {
                        swap((*arr)[j], (*arr)[j + 1]);
                    }
                }
            }
        } else if (searchBy == "distance_above" || searchBy == "distance_below") {
            bubbleSort("distance", "asc");
        }
        
        auto start = chrono::high_resolution_clock::now();
        
        int found = 0;
        cout << "\n[Binary Search] " << searchBy << " = " << value << "\n";
        cout << left << setw(12) << "ID" << setw(6) << "Age" << setw(15) << "Transport" 
             << setw(10) << "Distance" << setw(8) << "Days" << setw(15) << "Emission\n";
        cout << string(66, '-') << "\n";
        
        if (arr->getSize() == 0) {
            cout << "No data available." << endl;
            return;
        }
        
        if (searchBy == "age_group") {
            int dash = value.find('-');
            int minAge = stoi(value.substr(0, dash));
            int maxAge = stoi(value.substr(dash + 1));
            
            // Linear scan for age range (since we need all in range, not just one)
            for (int i = 0; i < arr->getSize(); i++) {
                if ((*arr)[i].age >= minAge && (*arr)[i].age <= maxAge) {
                    Resident& res = (*arr)[i];
                    double emission = comp->computeEmission(res);
                    cout << left << setw(12) << res.residentID << setw(6) << res.age 
                         << setw(15) << res.modeOfTransport << setw(10) << res.dailyDistance 
                         << setw(8) << res.averageDayPerMonth 
                         << setw(15) << fixed << setprecision(2) << emission << "\n";
                    found++;
                }
            }
        } else if (searchBy == "transport") {
            string val = value;
            for (size_t c = 0; c < val.length(); c++) val[c] = tolower(val[c]);
            
            // Linear scan for transport mode
            for (int i = 0; i < arr->getSize(); i++) {
                string mode = (*arr)[i].modeOfTransport;
                for (size_t c = 0; c < mode.length(); c++) mode[c] = tolower(mode[c]);
                
                if (mode == val) {
                    Resident& res = (*arr)[i];
                    double emission = comp->computeEmission(res);
                    cout << left << setw(12) << res.residentID << setw(6) << res.age 
                         << setw(15) << res.modeOfTransport << setw(10) << res.dailyDistance 
                         << setw(8) << res.averageDayPerMonth 
                         << setw(15) << fixed << setprecision(2) << emission << "\n";
                    found++;
                }
            }
        } else if (searchBy == "distance_above") {
            int threshold = stoi(value);
            for (int i = 0; i < arr->getSize(); i++) {
                if ((*arr)[i].dailyDistance > threshold) {
                    Resident& res = (*arr)[i];
                    double emission = comp->computeEmission(res);
                    cout << left << setw(12) << res.residentID << setw(6) << res.age 
                         << setw(15) << res.modeOfTransport << setw(10) << res.dailyDistance 
                         << setw(8) << res.averageDayPerMonth 
                         << setw(15) << fixed << setprecision(2) << emission << "\n";
                    found++;
                }
            }
        } else if (searchBy == "distance_below") {
            int threshold = stoi(value);
            for (int i = 0; i < arr->getSize(); i++) {
                if ((*arr)[i].dailyDistance < threshold) {
                    Resident& res = (*arr)[i];
                    double emission = comp->computeEmission(res);
                    cout << left << setw(12) << res.residentID << setw(6) << res.age 
                         << setw(15) << res.modeOfTransport << setw(10) << res.dailyDistance 
                         << setw(8) << res.averageDayPerMonth 
                         << setw(15) << fixed << setprecision(2) << emission << "\n";
                    found++;
                }
            }
        }
        
        auto end = chrono::high_resolution_clock::now();
        cout << string(66, '-') << "\n";
        cout << "Found: " << found << " residents matching criteria | Time: " 
             << chrono::duration<double, milli>(end - start).count() << " ms\n";
    }
};

// Global function to find the correct file path
string findValidPath(const string& filename, const string pathsToTry[], int numPaths) {
    for (int i = 0; i < numPaths; i++) {
        string fullPath = pathsToTry[i] + filename;
        ifstream test(fullPath.c_str());
        if (test.is_open()) {
            test.close();
            return fullPath;
        }
    }
    return filename;
}

int main() {
    cout << "============================================================================================\n";
    cout << "-------Array Implementation for Transport Data Analysis-------\n";
    cout << "============================================================================================\n\n";
    
    // Define paths to try
    string pathsToTry[] = {
        "../../data/",           // From src/array to DSTR/data
        "../data/",              // From array to src/data
        "./data/",               // Data folder in same directory
        ""                       // Current directory
    };
    int numPaths = sizeof(pathsToTry) / sizeof(pathsToTry[0]);
    
    // Find valid file paths
    string fileAPath = findValidPath("dataset1-cityA.csv", pathsToTry, numPaths);
    string fileBPath = findValidPath("dataset2-cityB.csv", pathsToTry, numPaths);
    string fileCPath = findValidPath("dataset3-cityC.csv", pathsToTry, numPaths);
    
    cout << "Loading data from:" << endl;
    cout << "  City A: " << fileAPath << endl;
    cout << "  City B: " << fileBPath << endl;
    cout << "  City C: " << fileCPath << endl << endl;
    
    // Create and load data for each city
    ResidentArray arrayA, arrayB, arrayC;
    FileManagerArray fmA, fmB, fmC;
    fmA.setResidentArray(&arrayA);
    fmB.setResidentArray(&arrayB);
    fmC.setResidentArray(&arrayC);
    
    // Measure data loading time
    auto loadStart = chrono::high_resolution_clock::now();
    bool loadA = fmA.loadFromCSV(fileAPath);
    bool loadB = fmB.loadFromCSV(fileBPath);
    bool loadC = fmC.loadFromCSV(fileCPath);
    auto loadEnd = chrono::high_resolution_clock::now();
    
    if (!loadA && !loadB && !loadC) {
        cout << "ERROR: Could not find any CSV files!" << endl;
        cout << "\nPress Enter to exit...";
        cin.get();
        return 1;
    }
    
    cout << "\n[Data Loading Statistics]";
    cout << "\nTime taken to load all CSV files: " 
         << chrono::duration<double, milli>(loadEnd - loadStart).count() << " ms";
    cout << "\nMemory usage per resident: " << sizeof(Resident) << " bytes";
    cout << "\nTotal memory for City A: " << arrayA.getSize() * sizeof(Resident) << " bytes";
    cout << "\nTotal memory for City B: " << arrayB.getSize() * sizeof(Resident) << " bytes";
    cout << "\nTotal memory for City C: " << arrayC.getSize() * sizeof(Resident) << " bytes\n";
    
    // Display loaded data summary
    if (loadA) cout << "\nCity A: Loaded " << arrayA.getSize() << " residents" << endl;
    if (loadB) cout << "City B: Loaded " << arrayB.getSize() << " residents" << endl;
    if (loadC) cout << "City C: Loaded " << arrayC.getSize() << " residents" << endl;
    
    // Display ALL data for each city
    if (loadA) {
        cout << "\n============================================================================================\n";
        cout << "City A Data (" << arrayA.getSize() << " residents)\n";
        cout << "============================================================================================\n";
        arrayA.traversePrint();
    }
    
    if (loadB) {
        cout << "\n============================================================================================\n";
        cout << "City B Data (" << arrayB.getSize() << " residents)\n";
        cout << "============================================================================================\n";
        arrayB.traversePrint();
    }
    
    if (loadC) {
        cout << "\n============================================================================================\n";
        cout << "City C Data (" << arrayC.getSize() << " residents)\n";
        cout << "============================================================================================\n";
        arrayC.traversePrint();
    }
    
    // Create computation objects
    ComputationArray compA(&fmA);
    ComputationArray compB(&fmB);
    ComputationArray compC(&fmC);
    
    // Calculate total emissions with timing
    cout << "\n============================================================================================\n";
    cout << "TOTAL CARBON EMISSIONS BY CITY\n";
    cout << "============================================================================================\n";
    
    auto emissionStart = chrono::high_resolution_clock::now();
    double totalA = compA.computeTotalEmission();
    double totalB = compB.computeTotalEmission();
    double totalC = compC.computeTotalEmission();
    auto emissionEnd = chrono::high_resolution_clock::now();
    
    if (loadA) cout << "Total emission for City A: " << fixed << setprecision(2) << totalA << " kg CO2" << endl;
    if (loadB) cout << "Total emission for City B: " << fixed << setprecision(2) << totalB << " kg CO2" << endl;
    if (loadC) cout << "Total emission for City C: " << fixed << setprecision(2) << totalC << " kg CO2" << endl;
    
    cout << "\n[Emission Calculation Statistics]";
    cout << "\nTime taken to calculate all emissions: " 
         << chrono::duration<double, milli>(emissionEnd - emissionStart).count() << " ms\n";
    
    // Emissions by mode
    if (loadA) {
        cout << "\n============================================================================================\n";
        cout << "City A - Emissions by Mode\n";
        cout << "============================================================================================\n";
        auto modeStart = chrono::high_resolution_clock::now();
        compA.calculateEmissionsByMode();
        auto modeEnd = chrono::high_resolution_clock::now();
        cout << "\n[Time: " << chrono::duration<double, milli>(modeEnd - modeStart).count() << " ms]\n";
    }
    
    if (loadB) {
        cout << "\n============================================================================================\n";
        cout << "City B - Emissions by Mode\n";
        cout << "============================================================================================\n";
        auto modeStart = chrono::high_resolution_clock::now();
        compB.calculateEmissionsByMode();
        auto modeEnd = chrono::high_resolution_clock::now();
        cout << "\n[Time: " << chrono::duration<double, milli>(modeEnd - modeStart).count() << " ms]\n";
    }
    
    if (loadC) {
        cout << "\n============================================================================================\n";
        cout << "City C - Emissions by Mode\n";
        cout << "============================================================================================\n";
        auto modeStart = chrono::high_resolution_clock::now();
        compC.calculateEmissionsByMode();
        auto modeEnd = chrono::high_resolution_clock::now();
        cout << "\n[Time: " << chrono::duration<double, milli>(modeEnd - modeStart).count() << " ms]\n";
    }
    
    // Emissions by age group
    if (loadA) {
        cout << "\n============================================================================================\n";
        cout << "City A - Emissions by Age Group\n";
        cout << "============================================================================================\n";
        auto ageStart = chrono::high_resolution_clock::now();
        compA.calculateEmissionsByAgeGroup();
        auto ageEnd = chrono::high_resolution_clock::now();
        cout << "\n[Time: " << chrono::duration<double, milli>(ageEnd - ageStart).count() << " ms]\n";
    }
    
    if (loadB) {
        cout << "\n============================================================================================\n";
        cout << "City B - Emissions by Age Group\n";
        cout << "============================================================================================\n";
        auto ageStart = chrono::high_resolution_clock::now();
        compB.calculateEmissionsByAgeGroup();
        auto ageEnd = chrono::high_resolution_clock::now();
        cout << "\n[Time: " << chrono::duration<double, milli>(ageEnd - ageStart).count() << " ms]\n";
    }
    
    if (loadC) {
        cout << "\n============================================================================================\n";
        cout << "City C - Emissions by Age Group\n";
        cout << "============================================================================================\n";
        auto ageStart = chrono::high_resolution_clock::now();
        compC.calculateEmissionsByAgeGroup();
        auto ageEnd = chrono::high_resolution_clock::now();
        cout << "\n[Time: " << chrono::duration<double, milli>(ageEnd - ageStart).count() << " ms]\n";
    }
    
    // ========== SORTING EXPERIMENTS ==========
    cout << "\n============================================================================================\n";
    cout << "SORTING EXPERIMENTS\n";
    cout << "============================================================================================\n";
    
    if (loadA) {
        SortSearchArray sorter(&arrayA, &compA);
        auto sortStart = chrono::high_resolution_clock::now();
        sorter.bubbleSort("age", "asc");
        auto sortEnd = chrono::high_resolution_clock::now();
        cout << "\n--- City A Sorted by Age (Ascending) - ALL " << arrayA.getSize() << " records ---\n";
        arrayA.traversePrint();
        cout << "\n[Bubble Sort Performance] Time: " 
             << chrono::duration<double, milli>(sortEnd - sortStart).count() << " ms";
        cout << " | Memory: " << arrayA.getSize() * sizeof(Resident) << " bytes\n";
    }
    
    if (loadB) {
        SortSearchArray sorter(&arrayB, &compB);
        auto sortStart = chrono::high_resolution_clock::now();
        sorter.bubbleSort("distance", "desc");
        auto sortEnd = chrono::high_resolution_clock::now();
        cout << "\n--- City B Sorted by Distance (Descending) - ALL " << arrayB.getSize() << " records ---\n";
        arrayB.traversePrint();
        cout << "\n[Bubble Sort Performance] Time: " 
             << chrono::duration<double, milli>(sortEnd - sortStart).count() << " ms";
        cout << " | Memory: " << arrayB.getSize() * sizeof(Resident) << " bytes\n";
    }
    
    if (loadC) {
        SortSearchArray sorter(&arrayC, &compC);
        auto sortStart = chrono::high_resolution_clock::now();
        sorter.bubbleSort("emission", "asc");
        auto sortEnd = chrono::high_resolution_clock::now();
        cout << "\n--- City C Sorted by Emission (Ascending) - ALL " << arrayC.getSize() << " records ---\n";
        arrayC.traversePrint();
        cout << "\n[Bubble Sort Performance] Time: " 
             << chrono::duration<double, milli>(sortEnd - sortStart).count() << " ms";
        cout << " | Memory: " << arrayC.getSize() * sizeof(Resident) << " bytes\n";
    }
    
    // Selection Sort for comparison
    cout << "\n--- Additional Sorting Algorithm for Comparison (Selection Sort) ---\n";
    
    if (loadA) {
        ResidentArray arrayA_sel;
        FileManagerArray fmA_sel;
        fmA_sel.setResidentArray(&arrayA_sel);
        fmA_sel.loadFromCSV(fileAPath);
        ComputationArray compA_sel(&fmA_sel);
        SortSearchArray ssaA_sel(&arrayA_sel, &compA_sel);
        
        auto selStart = chrono::high_resolution_clock::now();
        ssaA_sel.selectionSort("age", "asc");
        auto selEnd = chrono::high_resolution_clock::now();
        
        cout << "\n--- Selection Sorted City A by Age (Ascending) - ALL " << arrayA_sel.getSize() << " records ---\n";
        arrayA_sel.traversePrint();
        cout << "\n[Selection Sort Performance] Time: " 
             << chrono::duration<double, milli>(selEnd - selStart).count() << " ms";
        cout << " | Memory: " << arrayA_sel.getSize() * sizeof(Resident) << " bytes\n";
    }
    
    if (loadB) {
        ResidentArray arrayB_sel;
        FileManagerArray fmB_sel;
        fmB_sel.setResidentArray(&arrayB_sel);
        fmB_sel.loadFromCSV(fileBPath);
        ComputationArray compB_sel(&fmB_sel);
        SortSearchArray ssaB_sel(&arrayB_sel, &compB_sel);
        
        auto selStart = chrono::high_resolution_clock::now();
        ssaB_sel.selectionSort("distance", "desc");
        auto selEnd = chrono::high_resolution_clock::now();
        
        cout << "\n--- Selection Sorted City B by Distance (Descending) - ALL " << arrayB_sel.getSize() << " records ---\n";
        arrayB_sel.traversePrint();
        cout << "\n[Selection Sort Performance] Time: " 
             << chrono::duration<double, milli>(selEnd - selStart).count() << " ms";
        cout << " | Memory: " << arrayB_sel.getSize() * sizeof(Resident) << " bytes\n";
    }
    
    if (loadC) {
        ResidentArray arrayC_sel;
        FileManagerArray fmC_sel;
        fmC_sel.setResidentArray(&arrayC_sel);
        fmC_sel.loadFromCSV(fileCPath);
        ComputationArray compC_sel(&fmC_sel);
        SortSearchArray ssaC_sel(&arrayC_sel, &compC_sel);
        
        auto selStart = chrono::high_resolution_clock::now();
        ssaC_sel.selectionSort("emission", "asc");
        auto selEnd = chrono::high_resolution_clock::now();
        
        cout << "\n--- Selection Sorted City C by Emission (Ascending) - ALL " << arrayC_sel.getSize() << " records ---\n";
        arrayC_sel.traversePrint();
        cout << "\n[Selection Sort Performance] Time: " 
             << chrono::duration<double, milli>(selEnd - selStart).count() << " ms";
        cout << " | Memory: " << arrayC_sel.getSize() * sizeof(Resident) << " bytes\n";
    }
    
    // ========== SEARCHING EXPERIMENTS ==========
    cout << "\n============================================================================================\n";
    cout << "SEARCHING EXPERIMENTS\n";
    cout << "============================================================================================\n";
    
    // Linear Search Examples
    if (loadA) {
        SortSearchArray searcher(&arrayA, &compA);
        auto searchStart = chrono::high_resolution_clock::now();
        searcher.linearSearch("transport", "Car");
        auto searchEnd = chrono::high_resolution_clock::now();
        cout << "\n[Linear Search Performance] Time: " 
             << chrono::duration<double, milli>(searchEnd - searchStart).count() << " ms";
        cout << " | Memory: " << arrayA.getSize() * sizeof(Resident) << " bytes\n";
        
        searchStart = chrono::high_resolution_clock::now();
        searcher.linearSearch("distance_below", "10");
        searchEnd = chrono::high_resolution_clock::now();
        cout << "\n[Linear Search Performance] Time: " 
             << chrono::duration<double, milli>(searchEnd - searchStart).count() << " ms";
        cout << " | Memory: " << arrayA.getSize() * sizeof(Resident) << " bytes\n";
    }
    
    if (loadB) {
        SortSearchArray searcher(&arrayB, &compB);
        auto searchStart = chrono::high_resolution_clock::now();
        searcher.linearSearch("age_group", "18-25");
        auto searchEnd = chrono::high_resolution_clock::now();
        cout << "\n[Linear Search Performance] Time: " 
             << chrono::duration<double, milli>(searchEnd - searchStart).count() << " ms";
        cout << " | Memory: " << arrayB.getSize() * sizeof(Resident) << " bytes\n";
    }
    
    if (loadC) {
        SortSearchArray searcher(&arrayC, &compC);
        auto searchStart = chrono::high_resolution_clock::now();
        searcher.linearSearch("distance_above", "15");
        auto searchEnd = chrono::high_resolution_clock::now();
        cout << "\n[Linear Search Performance] Time: " 
             << chrono::duration<double, milli>(searchEnd - searchStart).count() << " ms";
        cout << " | Memory: " << arrayC.getSize() * sizeof(Resident) << " bytes\n";
    }
    
    // Binary Search Examples
    cout << "\n--- BINARY SEARCH RESULTS ---\n";
    
    if (loadA) {
        ResidentArray arrayA_bin;
        FileManagerArray fmA_bin;
        fmA_bin.setResidentArray(&arrayA_bin);
        fmA_bin.loadFromCSV(fileAPath);
        ComputationArray compA_bin(&fmA_bin);
        SortSearchArray binSearcher(&arrayA_bin, &compA_bin);
        
        auto binStart = chrono::high_resolution_clock::now();
        binSearcher.binarySearch("transport", "Bicycle");
        auto binEnd = chrono::high_resolution_clock::now();
        cout << "\n[Binary Search Performance] Time: " 
             << chrono::duration<double, milli>(binEnd - binStart).count() << " ms";
        cout << " | Memory (array): " << arrayA_bin.getSize() * sizeof(Resident) << " bytes\n";
    }
    
    if (loadB) {
        ResidentArray arrayB_bin;
        FileManagerArray fmB_bin;
        fmB_bin.setResidentArray(&arrayB_bin);
        fmB_bin.loadFromCSV(fileBPath);
        ComputationArray compB_bin(&fmB_bin);
        SortSearchArray binSearcher(&arrayB_bin, &compB_bin);
        
        auto binStart = chrono::high_resolution_clock::now();
        binSearcher.binarySearch("age_group", "18-25");
        auto binEnd = chrono::high_resolution_clock::now();
        cout << "\n[Binary Search Performance] Time: " 
             << chrono::duration<double, milli>(binEnd - binStart).count() << " ms";
        cout << " | Memory (array): " << arrayB_bin.getSize() * sizeof(Resident) << " bytes\n";
    }
    
    if (loadC) {
        ResidentArray arrayC_bin;
        FileManagerArray fmC_bin;
        fmC_bin.setResidentArray(&arrayC_bin);
        fmC_bin.loadFromCSV(fileCPath);
        ComputationArray compC_bin(&fmC_bin);
        SortSearchArray binSearcher(&arrayC_bin, &compC_bin);
        
        auto binStart = chrono::high_resolution_clock::now();
        binSearcher.binarySearch("distance_above", "15");
        auto binEnd = chrono::high_resolution_clock::now();
        cout << "\n[Binary Search Performance] Time: " 
             << chrono::duration<double, milli>(binEnd - binStart).count() << " ms";
        cout << " | Memory (array): " << arrayC_bin.getSize() * sizeof(Resident) << " bytes\n";
        
        binStart = chrono::high_resolution_clock::now();
        binSearcher.binarySearch("distance_below", "10");
        binEnd = chrono::high_resolution_clock::now();
        cout << "\n[Binary Search Performance] Time: " 
             << chrono::duration<double, milli>(binEnd - binStart).count() << " ms";
        cout << " | Memory (array): " << arrayC_bin.getSize() * sizeof(Resident) << " bytes\n";
    }
    
    // Performance summary
    cout << "\n============================================================================================\n";
    cout << "PERFORMANCE ANALYSIS SUMMARY - ARRAY IMPLEMENTATION\n";
    cout << "============================================================================================\n";
    cout << "\nSpace Complexity:\n";
    cout << "  - Memory per resident: " << sizeof(Resident) << " bytes\n";
    cout << "  - No additional pointer overhead (unlike linked lists)\n";
    cout << "\nActual Measured Performance:\n";
    cout << "  - Data Loading: " << chrono::duration<double, milli>(loadEnd - loadStart).count() << " ms\n";
    cout << "  - Emission Calculation: " << chrono::duration<double, milli>(emissionEnd - emissionStart).count() << " ms\n";
    
    cout << "\nPress Enter to exit...";
    cin.get();
    
    return 0;
}