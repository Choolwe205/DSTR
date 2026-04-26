#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <cstring>
#include <algorithm>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

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
    
    // Copy constructor
    ResidentArray(const ResidentArray& other) {
        capacity = other.capacity;
        size = other.size;
        residents = new Resident[capacity];
        for (int i = 0; i < size; i++) {
            residents[i] = other.residents[i];
        }
    }
    
    // Assignment operator
    ResidentArray& operator=(const ResidentArray& other) {
        if (this != &other) {
            delete[] residents;
            capacity = other.capacity;
            size = other.size;
            residents = new Resident[capacity];
            for (int i = 0; i < size; i++) {
                residents[i] = other.residents[i];
            }
        }
        return *this;
    }
    
    int getSize() const { return size; }
    int getCapacity() const { return capacity; }
    Resident* getData() const { return residents; }
    
    void insertAtEnd(string id, int age, string mode, int distance, double factor, int days) {
        if (size < capacity) {
            residents[size] = Resident(id, age, mode, distance, factor, days);
            size++;
        } else {
            cout << "Error: Array capacity exceeded!" << endl;
        }
    }
    
    void insertAtIndex(string id, int age, string mode, int distance, double factor, int days, int index) {
        if (index < 0 || index > size) {
            cout << "Index out of bounds!" << endl;
            return;
        }
        if (size >= capacity) {
            cout << "Array capacity exceeded!" << endl;
            return;
        }
        
        for (int i = size; i > index; i--) {
            residents[i] = residents[i - 1];
        }
        
        residents[index] = Resident(id, age, mode, distance, factor, days);
        size++;
    }
    
    void deleteAtIndex(int index) {
        if (index < 0 || index >= size) {
            cout << "Index out of bounds!" << endl;
            return;
        }
        
        for (int i = index; i < size - 1; i++) {
            residents[i] = residents[i + 1];
        }
        size--;
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
             << setw(12) << "Distance" << setw(18) << "Emission Factor" << setw(10) << "Days" << endl;
        cout << string(73, '-') << endl;
        
        for (int i = 0; i < size; i++) {
            cout << left << setw(12) << residents[i].residentID 
                 << setw(6) << residents[i].age
                 << setw(15) << residents[i].modeOfTransport
                 << setw(12) << residents[i].dailyDistance
                 << setw(18) << residents[i].carbonEmissionFactor
                 << setw(10) << residents[i].averageDayPerMonth << endl;
        }
        cout << "--End of Array--" << endl;
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
        ifstream file(datafile);
        if (!file.is_open()) {
            cout << "Error opening file: " << datafile << endl;
            cout << "Current working directory: " << fs::current_path() << endl;
            return false;
        }
        
        string line;
        getline(file, line); // Skip header
        
        int lineCount = 0;
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
                lineCount++;
            } catch (const exception& e) {
                cout << "Error parsing line: " << line << endl;
            }
        }
        file.close();
        cout << "Successfully loaded " << lineCount << " records from " << datafile << endl;
        return true;
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
            cout << "The array is empty!" << endl;
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
        
        bool hasData = false;
        for (int g = 0; g < MAX_AGE_GROUPS; g++) {
            if (residentCount[g] == 0) continue;
            hasData = true;
            
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
            
            cout << "\n" << left << setw(18) << "Mode of Transport" 
                 << setw(10) << "Count" << setw(22) << "Total Emission (kg CO2)" 
                 << setw(18) << "Avg per Resident" << endl;
            cout << string(68, '-') << endl;
            
            for (int m = 0; m < 6; m++) {
                if (modeCount[g][m] > 0) {
                    double modeEmission = 0;
                    for (int i = 0; i < arr->getSize(); i++) {
                        Resident& res = (*arr)[i];
                        if (getAgeGroupIndex(res.age) == g && res.modeOfTransport == transportModes[m]) {
                            modeEmission += computeEmission(res);
                        }
                    }
                    cout << left << setw(18) << transportModes[m] 
                         << setw(10) << modeCount[g][m]
                         << setw(22) << fixed << setprecision(2) << modeEmission
                         << setw(18) << (modeEmission / modeCount[g][m]) << endl;
                }
            }
            cout << string(68, '-') << endl;
            cout << "Total Emission for Age Group: " << totalEmission[g] << " kg CO2" << endl;
        }
        
        if (!hasData) {
            cout << "No age group data available." << endl;
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
        cout << left << setw(18) << "Mode of Transport" 
             << setw(10) << "Count" << setw(22) << "Total Emission (kg CO2)" 
             << setw(18) << "Avg per Resident" << endl;
        cout << string(68, '-') << endl;
        
        for (int m = 0; m < 6; m++) {
            if (modeCount[m] > 0) {
                cout << left << setw(18) << transportModes[m] 
                     << setw(10) << modeCount[m]
                     << setw(22) << fixed << setprecision(2) << modeEmission[m]
                     << setw(18) << (modeEmission[m] / modeCount[m]) << endl;
            }
        }
        cout << string(68, '-') << endl;
    }
};

// Sort and Search class for array implementation
class SortSearchArray {
private:
    ResidentArray* arr;
    ComputationArray* comp;
    
    size_t getMemoryUsage() const {
        return arr->getSize() * sizeof(Resident) + sizeof(ResidentArray);
    }
    
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
        cout << "Memory Usage: " << getMemoryUsage() << " bytes\n";
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
        cout << "Memory Usage: " << getMemoryUsage() << " bytes\n";
    }
    
    void linearSearch(string searchBy, string value) {
        auto start = chrono::high_resolution_clock::now();
        
        int found = 0;
        cout << "\n[Linear Search] " << searchBy << " = " << value << "\n";
        cout << left << setw(12) << "ID" << setw(6) << "Age" << setw(15) << "Transport" 
             << setw(10) << "Distance" << setw(8) << "Days" << setw(12) << "Emission\n";
        cout << string(63, '-') << "\n";
        
        for (int i = 0; i < arr->getSize(); i++) {
            Resident& res = (*arr)[i];
            bool match = false;
            
            if (searchBy == "transport") {
                string a = res.modeOfTransport;
                string b = value;
                for (char& c : a) c = tolower(c);
                for (char& c : b) c = tolower(c);
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
                cout << left << setw(12) << res.residentID << setw(6) << res.age 
                     << setw(15) << res.modeOfTransport << setw(10) << res.dailyDistance 
                     << setw(8) << res.averageDayPerMonth 
                     << setw(12) << fixed << setprecision(2) << comp->computeEmission(res) << "\n";
                found++;
            }
        }
        
        auto end = chrono::high_resolution_clock::now();
        cout << string(63, '-') << "\n";
        cout << "Found: " << found << " | Time: " 
             << chrono::duration<double, milli>(end - start).count() << " ms\n";
        cout << "Memory Usage: " << getMemoryUsage() << " bytes\n";
    }
    
    void binarySearch(string searchBy, string value) {
        if (searchBy == "age_group") {
            bubbleSort("age", "asc");
        } else if (searchBy == "transport") {
            int n = arr->getSize();
            for (int i = 0; i < n - 1; i++) {
                for (int j = 0; j < n - i - 1; j++) {
                    string a = (*arr)[j].modeOfTransport;
                    string b = (*arr)[j + 1].modeOfTransport;
                    for (char& c : a) c = tolower(c);
                    for (char& c : b) c = tolower(c);
                    if (a > b) {
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
             << setw(10) << "Distance" << setw(8) << "Days" << setw(12) << "Emission\n";
        cout << string(63, '-') << "\n";
        
        if (arr->getSize() == 0) {
            cout << "No data available for binary search." << endl;
            auto end = chrono::high_resolution_clock::now();
            cout << string(63, '-') << "\n";
            cout << "Found: 0 | Time: " << chrono::duration<double, milli>(end - start).count() << " ms\n";
            return;
        }
        
        if (searchBy == "age_group") {
            int dash = value.find('-');
            int minAge = stoi(value.substr(0, dash));
            int maxAge = stoi(value.substr(dash + 1));
            
            int left = 0, right = arr->getSize() - 1, first = -1;
            while (left <= right) {
                int mid = left + (right - left) / 2;
                if ((*arr)[mid].age >= minAge) {
                    first = mid;
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            }
            
            if (first != -1) {
                for (int i = first; i < arr->getSize() && (*arr)[i].age <= maxAge; i++) {
                    Resident& res = (*arr)[i];
                    cout << left << setw(12) << res.residentID << setw(6) << res.age 
                         << setw(15) << res.modeOfTransport << setw(10) << res.dailyDistance 
                         << setw(8) << res.averageDayPerMonth 
                         << setw(12) << fixed << setprecision(2) << comp->computeEmission(res) << "\n";
                    found++;
                }
            }
        } else if (searchBy == "transport") {
            string val = value;
            for (char& c : val) c = tolower(c);
            
            int left = 0, right = arr->getSize() - 1;
            int foundIndex = -1;
            
            while (left <= right) {
                int mid = left + (right - left) / 2;
                string midMode = (*arr)[mid].modeOfTransport;
                for (char& c : midMode) c = tolower(c);
                
                if (midMode == val) {
                    foundIndex = mid;
                    break;
                } else if (midMode < val) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
            
            if (foundIndex != -1) {
                int l = foundIndex, r = foundIndex;
                auto toLower = [](string s) { for (char& c : s) c = tolower(c); return s; };
                while (l > 0 && toLower((*arr)[l - 1].modeOfTransport) == val) l--;
                while (r < arr->getSize() - 1 && toLower((*arr)[r + 1].modeOfTransport) == val) r++;
                
                for (int i = l; i <= r; i++) {
                    Resident& res = (*arr)[i];
                    cout << left << setw(12) << res.residentID << setw(6) << res.age 
                         << setw(15) << res.modeOfTransport << setw(10) << res.dailyDistance 
                         << setw(8) << res.averageDayPerMonth 
                         << setw(12) << fixed << setprecision(2) << comp->computeEmission(res) << "\n";
                    found++;
                }
            }
        } else if (searchBy == "distance_above") {
            int threshold = stoi(value);
            int left = 0, right = arr->getSize() - 1;
            int firstAbove = arr->getSize();
            
            while (left <= right) {
                int mid = left + (right - left) / 2;
                if ((*arr)[mid].dailyDistance > threshold) {
                    firstAbove = mid;
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            }
            
            for (int i = firstAbove; i < arr->getSize(); i++) {
                Resident& res = (*arr)[i];
                cout << left << setw(12) << res.residentID << setw(6) << res.age 
                     << setw(15) << res.modeOfTransport << setw(10) << res.dailyDistance 
                     << setw(8) << res.averageDayPerMonth 
                     << setw(12) << fixed << setprecision(2) << comp->computeEmission(res) << "\n";
                found++;
            }
        } else if (searchBy == "distance_below") {
            int threshold = stoi(value);
            int left = 0, right = arr->getSize() - 1;
            int lastBelow = -1;
            
            while (left <= right) {
                int mid = left + (right - left) / 2;
                if ((*arr)[mid].dailyDistance < threshold) {
                    lastBelow = mid;
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
            
            for (int i = 0; i <= lastBelow; i++) {
                Resident& res = (*arr)[i];
                cout << left << setw(12) << res.residentID << setw(6) << res.age 
                     << setw(15) << res.modeOfTransport << setw(10) << res.dailyDistance 
                     << setw(8) << res.averageDayPerMonth 
                     << setw(12) << fixed << setprecision(2) << comp->computeEmission(res) << "\n";
                found++;
            }
        }
        
        auto end = chrono::high_resolution_clock::now();
        cout << string(63, '-') << "\n";
        cout << "Found: " << found << " | Time: " 
             << chrono::duration<double, milli>(end - start).count() << " ms\n";
        cout << "Memory Usage (array): " << getMemoryUsage() << " bytes\n";
    }
    
    void displaySorted() {
        arr->traversePrint();
    }
};

// Function to attempt finding CSV files in common locations
string findCSVFile(const string& filename) {
    // List of possible paths to check
    vector<string> paths = {
        filename,                                    // Current directory
        "./" + filename,                            // Current directory explicit
        "../" + filename,                           // Parent directory
        "../../" + filename,                        // Two levels up
        "../../data/" + filename,                   // Data subdirectory
        "../data/" + filename,                      // Data in parent
        "./data/" + filename,                       // Data in current
        "/mnt/c/Users/ASUS/OneDrive/Desktop/Assignment 1/" + filename  // Absolute path from your error
    };
    
    for (const auto& path : paths) {
        ifstream test(path);
        if (test.is_open()) {
            test.close();
            return path;
        }
    }
    return filename; // Return original if not found
}

int main() {
    cout << "============================================================================================\n";
    cout << "-------The following is an Implementation of Arrays for DSTR PART 1-------\n";
    cout << "============================================================================================\n";
    
    // Display current working directory for debugging
    cout << "\nCurrent working directory: " << fs::current_path() << endl;
    cout << "\nAttempting to locate CSV files...\n" << endl;
    
    // Find the correct file paths
    string fileA = findCSVFile("dataset1-cityA.csv");
    string fileB = findCSVFile("dataset2-cityB.csv");
    string fileC = findCSVFile("dataset3-cityC.csv");
    
    cout << "Using file paths:" << endl;
    cout << "  City A: " << fileA << endl;
    cout << "  City B: " << fileB << endl;
    cout << "  City C: " << fileC << endl << endl;
    
    // Create arrays for each city
    ResidentArray arrayA, arrayB, arrayC;
    
    // Create file managers
    FileManagerArray fmA, fmB, fmC;
    fmA.setResidentArray(&arrayA);
    fmB.setResidentArray(&arrayB);
    fmC.setResidentArray(&arrayC);
    
    // Load data from CSV files
    bool loadA = fmA.loadFromCSV(fileA);
    bool loadB = fmB.loadFromCSV(fileB);
    bool loadC = fmC.loadFromCSV(fileC);
    
    if (!loadA && !loadB && !loadC) {
        cout << "\nERROR: Could not find any CSV files!" << endl;
        cout << "Please ensure the CSV files are in the same directory as the executable." << endl;
        cout << "Or update the file paths in the findCSVFile() function." << endl;
        return 1;
    }
    
    // Display loaded data only if files were found
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
    
    // Create computation objects only for loaded data
    ComputationArray compA(&fmA);
    ComputationArray compB(&fmB);
    ComputationArray compC(&fmC);
    
    // Calculate total emissions
    cout << "\n============================================================================================\n";
    cout << "Total Carbon Emissions by City\n";
    cout << "============================================================================================\n";
    if (loadA) cout << "Total emission for City A: " << fixed << setprecision(2) << compA.computeTotalEmission() << " kg CO2" << endl;
    if (loadB) cout << "Total emission for City B: " << fixed << setprecision(2) << compB.computeTotalEmission() << " kg CO2" << endl;
    if (loadC) cout << "Total emission for City C: " << fixed << setprecision(2) << compC.computeTotalEmission() << " kg CO2" << endl;
    
    // Calculate emissions by mode for each city
    if (loadA) {
        cout << "\n============================================================================================\n";
        cout << "City A - Emissions by Mode\n";
        cout << "============================================================================================\n";
        compA.calculateEmissionsByMode();
    }
    
    if (loadB) {
        cout << "\n============================================================================================\n";
        cout << "City B - Emissions by Mode\n";
        cout << "============================================================================================\n";
        compB.calculateEmissionsByMode();
    }
    
    if (loadC) {
        cout << "\n============================================================================================\n";
        cout << "City C - Emissions by Mode\n";
        cout << "============================================================================================\n";
        compC.calculateEmissionsByMode();
    }
    
    // Calculate emissions by age group for each city
    if (loadA) {
        cout << "\n============================================================================================\n";
        cout << "City A - Emissions by Age Group\n";
        cout << "============================================================================================\n";
        compA.calculateEmissionsByAgeGroup();
    }
    
    if (loadB) {
        cout << "\n============================================================================================\n";
        cout << "City B - Emissions by Age Group\n";
        cout << "============================================================================================\n";
        compB.calculateEmissionsByAgeGroup();
    }
    
    if (loadC) {
        cout << "\n============================================================================================\n";
        cout << "City C - Emissions by Age Group\n";
        cout << "============================================================================================\n";
        compC.calculateEmissionsByAgeGroup();
    }
    
    // Create sort/search objects only for loaded data
    if (loadA || loadB || loadC) {
        cout << "\n============================================================================================\n";
        cout << "SORTING EXPERIMENTS\n";
        cout << "============================================================================================\n";
        
        if (loadA) {
            SortSearchArray ssaA(&arrayA, &compA);
            ssaA.bubbleSort("age",