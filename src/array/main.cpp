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
        ifstream file(datafile.c_str());
        if (!file.is_open()) {
            cout << "Error opening file: " << datafile << endl;
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

    void printAgeGroupSummary(const string& cityName) const {
        ResidentArray* arr = load->getResidentArray();
        if (arr == nullptr || arr->getSize() == 0) return;

        double totalEmission[MAX_AGE_GROUPS] = {0};
        int residentCount[MAX_AGE_GROUPS] = {0};

        for (int i = 0; i < arr->getSize(); i++) {
            int g = getAgeGroupIndex((*arr)[i].age);
            if (g == -1) continue;
            residentCount[g]++;
            totalEmission[g] += computeEmission((*arr)[i]);
        }

        cout << "\n" << cityName << ":\n";
        cout << left << setw(38) << "Age Group"
            << setw(12) << "Residents"
            << setw(24) << "Total Emission (kg CO2)"
            << setw(20) << "Avg per Resident" << endl;
        cout << string(94, '-') << endl;

        for (int g = 0; g < MAX_AGE_GROUPS; g++) {
            if (residentCount[g] == 0) continue;
            string label = to_string(AGE_GROUPS[g].minAge) + "-" + to_string(AGE_GROUPS[g].maxAge)
                        + " (" + AGE_GROUPS[g].name + ")";
            cout << left << setw(38) << label
                << setw(12) << residentCount[g]
                << setw(24) << fixed << setprecision(2) << totalEmission[g]
                << setw(20) << (totalEmission[g] / residentCount[g]) << endl;
        }
        cout << string(94, '-') << endl;
    }


    void printModeSummary(const string& cityName) const {
        ResidentArray* arr = load->getResidentArray();
        if (arr == nullptr || arr->getSize() == 0) return;

        string transportModes[] = {"Car", "Bus", "Bicycle", "Walking", "School Bus", "Carpool"};
        int modeCount[6] = {0};
        double modeEmission[6] = {0};

        for (int i = 0; i < arr->getSize(); i++) {
            for (int m = 0; m < 6; m++) {
                if ((*arr)[i].modeOfTransport == transportModes[m]) {
                    modeCount[m]++;
                    modeEmission[m] += computeEmission((*arr)[i]);
                    break;
                }
            }
        }

        cout << "\n" << cityName << ":\n";
        cout << left << setw(14) << "Mode"
             << setw(12) << "Residents"
             << setw(24) << "Total Emission (kg CO2)"
             << setw(20) << "Avg per Resident" << endl;
        cout << string(70, '-') << endl;

        for (int m = 0; m < 6; m++) {
            if (modeCount[m] > 0) {
                cout << left << setw(14) << transportModes[m]
                     << setw(12) << modeCount[m]
                     << setw(24) << fixed << setprecision(2) << modeEmission[m]
                     << setw(20) << (modeEmission[m] / modeCount[m]) << endl;
            }
        }
        cout << string(70, '-') << endl;
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
    
    void printSortedTable(const string& label) const {
    cout << "\nSorted Results - " << label << ":\n";
    cout << left << setw(12) << "ID" << setw(6) << "Age" << setw(15) << "Transport"
         << setw(12) << "Distance" << setw(18) << "Emission Factor"
         << setw(10) << "Days" << setw(12) << "Emission" << endl;
    cout << string(85, '-') << endl;

    for (int i = 0; i < arr->getSize(); i++) {
        Resident& res = (*arr)[i];
        cout << left << setw(12) << res.residentID
             << setw(6) << res.age
             << setw(15) << res.modeOfTransport
             << setw(12) << res.dailyDistance
             << setw(18) << res.carbonEmissionFactor
             << setw(10) << res.averageDayPerMonth
             << setw(12) << fixed << setprecision(2) << comp->computeEmission(res) << endl;
    }
    cout << string(85, '-') << endl;
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
                    for (size_t c = 0; c < a.length(); c++) a[c] = tolower(a[c]);
                    for (size_t c = 0; c < b.length(); c++) b[c] = tolower(b[c]);
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
            for (size_t c = 0; c < val.length(); c++) val[c] = tolower(val[c]);
            
            int left = 0, right = arr->getSize() - 1;
            int foundIndex = -1;
            
            while (left <= right) {
                int mid = left + (right - left) / 2;
                string midMode = (*arr)[mid].modeOfTransport;
                for (size_t c = 0; c < midMode.length(); c++) midMode[c] = tolower(midMode[c]);
                
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
};

int main() {
    cout << "============================================================================================\n";
    cout << "-------The following is an Implementation of Arrays for DSTR PART 1-------\n";
    cout << "============================================================================================\n\n";
    
    // Create arrays for each city
    ResidentArray arrayA, arrayB, arrayC;
    
    // Create file managers
    FileManagerArray fmA, fmB, fmC;
    fmA.setResidentArray(&arrayA);
    fmB.setResidentArray(&arrayB);
    fmC.setResidentArray(&arrayC);
    
    string dataPath = "../../data/";

    string pathsToTry[] = {
        "../../data/",           // From src/array to DSTR/data
        "../data/",              // From array to src/data (if data was in src)
        "./data/",               // Data folder in same directory
        "../../src/array/data/", // Data inside src/array/data
    };

    bool loadA = false, loadB = false, loadC = false;

    for (int attempt = 0; attempt < 4; attempt++) {
        if (loadA && loadB && loadC) break; // All files loaded successfully
        
        string fileA = pathsToTry[attempt] + "dataset1-cityA.csv";
        string fileB = pathsToTry[attempt] + "dataset2-cityB.csv";
        string fileC = pathsToTry[attempt] + "dataset3-cityC.csv";
        
        cout << "Trying path: " << pathsToTry[attempt] << endl;
        
        if (!loadA) {
            cout << "  Attempting to load City A from: " << fileA << endl;
            loadA = fmA.loadFromCSV(fileA);
        }
        if (!loadB) {
            cout << "  Attempting to load City B from: " << fileB << endl;
            loadB = fmB.loadFromCSV(fileB);
        }
        if (!loadC) {
            cout << "  Attempting to load City C from: " << fileC << endl;
            loadC = fmC.loadFromCSV(fileC);
        }
    }

    if (!loadA && !loadB && !loadC) {
    cout << "\nERROR: Could not find any CSV files!" << endl;
    return 1;
    }

    // Display loaded data
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
    
    // Calculate total emissions
    cout << "\n============================================================================================\n";
    cout << "Total Carbon Emissions by City\n";
    cout << "============================================================================================\n";
    if (loadA) cout << "Total emission for City A: " << fixed << setprecision(2) << compA.computeTotalEmission() << " kg CO2" << endl;
    if (loadB) cout << "Total emission for City B: " << fixed << setprecision(2) << compB.computeTotalEmission() << " kg CO2" << endl;
    if (loadC) cout << "Total emission for City C: " << fixed << setprecision(2) << compC.computeTotalEmission() << " kg CO2" << endl;
    
    // Calculate emissions by mode
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
    
    // Calculate emissions by age group
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
    
    cout << "\n============================================================================================\n";
    cout << "CROSS-CITY COMPARISON: Emissions by Age Group\n";
    cout << "============================================================================================\n";
    if (loadA) compA.printAgeGroupSummary("City A (Metropolitan)");
    if (loadB) compB.printAgeGroupSummary("City B (University Town)");
    if (loadC) compC.printAgeGroupSummary("City C (Suburban/Rural)");

    cout << "\n============================================================================================\n";
    cout << "CROSS-CITY COMPARISON: Emissions by Mode of Transport\n";
    cout << "============================================================================================\n";
    if (loadA) compA.printModeSummary("City A (Metropolitan)");
    if (loadB) compB.printModeSummary("City B (University Town)");
    if (loadC) compC.printModeSummary("City C (Suburban/Rural)");

    // Combined totals across all cities
    cout << "\n============================================================================================\n";
    cout << "OVERALL TOTAL EMISSIONS ACROSS ALL CITIES\n";
    cout << "============================================================================================\n";
    double grandTotal = 0;
    if (loadA) grandTotal += compA.computeTotalEmission();
    if (loadB) grandTotal += compB.computeTotalEmission();
    if (loadC) grandTotal += compC.computeTotalEmission();
    cout << left << setw(30) << "City A Total:"
        << (loadA ? to_string((int)compA.computeTotalEmission()) : "N/A") << " kg CO2\n";
    cout << left << setw(30) << "City B Total:"
        << (loadB ? to_string((int)compB.computeTotalEmission()) : "N/A") << " kg CO2\n";
    cout << left << setw(30) << "City C Total:"
        << (loadC ? to_string((int)compC.computeTotalEmission()) : "N/A") << " kg CO2\n";
    cout << string(50, '-') << "\n";
    cout << left << setw(30) << "Grand Total:" << fixed << setprecision(2) << grandTotal << " kg CO2\n";

    // Sorting and searching experiments
    if (loadA || loadB || loadC) {
        cout << "\n============================================================================================\n";
        cout << "SORTING EXPERIMENTS\n";
        cout << "============================================================================================\n";
        
        if (loadA) {
            SortSearchArray ssaA(&arrayA, &compA);
            ssaA.bubbleSort("age", "asc");
            ssaA.printSortedTable("City A | Bubble Sort | Age Ascending");
        }
        if (loadB) {
            SortSearchArray ssaB(&arrayB, &compB);
            ssaB.bubbleSort("distance", "desc");
            ssaB.printSortedTable("City B | Bubble Sort | Distance Descending");
        }
        if (loadC) {
            SortSearchArray ssaC(&arrayC, &compC);
            ssaC.bubbleSort("emission", "asc");
            ssaC.printSortedTable("City C | Bubble Sort | Emission Ascending");
        }
        
        cout << "\n--- Additional Sorting Algorithm for Comparison ---\n";
        
        // Reload data for fair comparison
        if (loadA) {
            arrayA.clear();
            fmA.loadFromCSV("dataset1-cityA.csv");
            SortSearchArray ssaA(&arrayA, &compA);
            ssaA.selectionSort("age", "asc");
            ssaA.printSortedTable("City A | Selection Sort | Age Ascending");
        }
        if (loadB) {
            arrayB.clear();
            fmB.loadFromCSV("dataset2-cityB.csv");
            SortSearchArray ssaB(&arrayB, &compB);
            ssaB.selectionSort("distance", "desc");
            ssaB.printSortedTable("City B | Selection Sort | Distance Descending");
        }
        if (loadC) {
            arrayC.clear();
            fmC.loadFromCSV("dataset3-cityC.csv");
            SortSearchArray ssaC(&arrayC, &compC);
            ssaC.selectionSort("emission", "asc");
            ssaC.printSortedTable("City C | Selection Sort | Emission Ascending");
        }
        
        // Searching experiments
        cout << "\n============================================================================================\n";
        cout << "SEARCHING EXPERIMENTS\n";
        cout << "============================================================================================\n";
        
        if (loadA) {
            SortSearchArray ssaA(&arrayA, &compA);
            ssaA.linearSearch("transport", "Car");
            ssaA.linearSearch("distance_below", "10");
        }
        if (loadB) {
            SortSearchArray ssaB(&arrayB, &compB);
            ssaB.linearSearch("age_group", "26-45");
        }
        if (loadC) {
            SortSearchArray ssaC(&arrayC, &compC);
            ssaC.linearSearch("distance_above", "15");
        }
        
        cout << "\n--- Binary Search Results (Data will be sorted first) ---\n";
        
        if (loadA) {
            SortSearchArray ssaA(&arrayA, &compA);
            ssaA.binarySearch("transport", "Bicycle");
        }
        if (loadB) {
            SortSearchArray ssaB(&arrayB, &compB);
            ssaB.binarySearch("age_group", "18-25");
        }
        if (loadC) {
            SortSearchArray ssaC(&arrayC, &compC);
            ssaC.binarySearch("distance_above", "15");
            ssaC.binarySearch("distance_below", "10");
        }
    }
    
    //cout << "\nSpace Complexity:\n";
    //cout << "  - Memory per resident: " << sizeof(Resident) << " bytes\n";
    
    return 0;
}