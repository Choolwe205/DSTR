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
    
    // Default constructor
    Resident() {
        residentID = "";
        age = 0;
        modeOfTransport = "";
        dailyDistance = 0;
        carbonEmissionFactor = 0.0;
        averageDayPerMonth = 0;
    }
    
    // Parameterized constructor
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
    // Constructor
    ResidentArray(int cap = MAX_RESIDENTS) {
        capacity = cap;
        residents = new Resident[capacity];
        size = 0;
    }
    
    // Destructor
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
    
    // Accessors
    int getSize() const { return size; }
    int getCapacity() const { return capacity; }
    Resident* getData() const { return residents; }
    
    // Insert at end
    void insertAtEnd(string id, int age, string mode, int distance, double factor, int days) {
        if (size < capacity) {
            residents[size] = Resident(id, age, mode, distance, factor, days);
            size++;
        } else {
            cout << "Error: Array capacity exceeded!" << endl;
        }
    }
    
    // Insert at index
    void insertAtIndex(string id, int age, string mode, int distance, double factor, int days, int index) {
        if (index < 0 || index > size) {
            cout << "Index out of bounds!" << endl;
            return;
        }
        if (size >= capacity) {
            cout << "Array capacity exceeded!" << endl;
            return;
        }
        
        // Shift elements to the right
        for (int i = size; i > index; i--) {
            residents[i] = residents[i - 1];
        }
        
        residents[index] = Resident(id, age, mode, distance, factor, days);
        size++;
    }
    
    // Delete at index
    void deleteAtIndex(int index) {
        if (index < 0 || index >= size) {
            cout << "Index out of bounds!" << endl;
            return;
        }
        
        // Shift elements to the left
        for (int i = index; i < size - 1; i++) {
            residents[i] = residents[i + 1];
        }
        size--;
    }
    
    // Clear all residents
    void clear() {
        size = 0;
    }
    
    // Traverse and print
    void traversePrint() const {
        if (size == 0) {
            cout << "This array is empty" << endl;
            return;
        }
        
        cout << left << setw(12) << "ID" << setw(6) << "Age" << setw(15) << "Transport" 
             << setw(12) << "Distance" << setw(12) << "Emission Factor" << setw(10) << "Days" << endl;
        cout << string(67, '-') << endl;
        
        for (int i = 0; i < size; i++) {
            cout << left << setw(12) << residents[i].residentID 
                 << setw(6) << residents[i].age
                 << setw(15) << residents[i].modeOfTransport
                 << setw(12) << residents[i].dailyDistance
                 << setw(12) << residents[i].carbonEmissionFactor
                 << setw(10) << residents[i].averageDayPerMonth << endl;
        }
        cout << "--End of Array--" << endl;
    }
    
    // Access by index
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
    
    void loadFromCSV(string datafile) {
        ifstream file(datafile);
        if (!file.is_open()) {
            cout << "Error opening file: " << datafile << endl;
            return;
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
            
            int age = stoi(ageStr);
            int distance = stoi(distanceStr);
            double factor = stod(factorStr);
            int days = stoi(daysStr);
            
            resArray->insertAtEnd(id, age, transport, distance, factor, days);
        }
        file.close();
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
    
    // Get age group index for a given age
    int getAgeGroupIndex(int age) const {
        for (int i = 0; i < MAX_AGE_GROUPS; i++) {
            if (age >= AGE_GROUPS[i].minAge && age <= AGE_GROUPS[i].maxAge) {
                return i;
            }
        }
        return -1;
    }
    
    // Calculate emissions by age group
    void calculateEmissionsByAgeGroup() const {
        ResidentArray* arr = load->getResidentArray();
        if (arr == nullptr || arr->getSize() == 0) return;
        
        // Arrays to store statistics per age group
        int residentCount[MAX_AGE_GROUPS] = {0};
        int modeCount[MAX_AGE_GROUPS][6] = {0}; // 6 transport modes
        double totalEmission[MAX_AGE_GROUPS] = {0};
        
        string transportModes[] = {"Car", "Bus", "Bicycle", "Walking", "School Bus", "Carpool"};
        
        // Process each resident
        for (int i = 0; i < arr->getSize(); i++) {
            Resident& res = (*arr)[i];
            int ageGroup = getAgeGroupIndex(res.age);
            if (ageGroup == -1) continue;
            
            residentCount[ageGroup]++;
            totalEmission[ageGroup] += computeEmission(res);
            
            // Count mode of transport
            for (int m = 0; m < 6; m++) {
                if (res.modeOfTransport == transportModes[m]) {
                    modeCount[ageGroup][m]++;
                    break;
                }
            }
        }
        
        // Display results
        for (int g = 0; g < MAX_AGE_GROUPS; g++) {
            if (residentCount[g] == 0) continue;
            
            cout << "\n" << string(80, '=') << endl;
            cout << "Age Group: " << AGE_GROUPS[g].minAge << "-" << AGE_GROUPS[g].maxAge 
                 << " (" << AGE_GROUPS[g].name << ")" << endl;
            cout << string(80, '=') << endl;
            
            // Find most preferred mode
            int maxModeIndex = 0;
            for (int m = 1; m < 6; m++) {
                if (modeCount[g][m] > modeCount[g][maxModeIndex]) {
                    maxModeIndex = m;
                }
            }
            
            cout << "Most preferred mode of transport: " << transportModes[maxModeIndex] 
                 << " (" << modeCount[g][maxModeIndex] << " residents)" << endl;
            cout << "Total carbon emissions: " << totalEmission[g] << " kg CO2" << endl;
            cout << "Average emission per resident: " << totalEmission[g] / residentCount[g] << " kg CO2" << endl;
            
            // Display per-mode breakdown
            cout << "\n" << left << setw(15) << "Mode of Transport" 
                 << setw(10) << "Count" << setw(20) << "Total Emission (kg CO2)" 
                 << setw(15) << "Average per Resident" << endl;
            cout << string(60, '-') << endl;
            
            for (int m = 0; m < 6; m++) {
                if (modeCount[g][m] > 0) {
                    double modeEmission = 0;
                    for (int i = 0; i < arr->getSize(); i++) {
                        Resident& res = (*arr)[i];
                        if (getAgeGroupIndex(res.age) == g && res.modeOfTransport == transportModes[m]) {
                            modeEmission += computeEmission(res);
                        }
                    }
                    cout << left << setw(15) << transportModes[m] 
                         << setw(10) << modeCount[g][m]
                         << setw(20) << modeEmission
                         << setw(15) << (modeEmission / modeCount[g][m]) << endl;
                }
            }
            cout << string(60, '-') << endl;
            cout << "Total Emission for Age Group: " << totalEmission[g] << " kg CO2" << endl;
        }
    }
    
    // Calculate emissions by mode of transport
    void calculateEmissionsByMode() const {
        ResidentArray* arr = load->getResidentArray();
        if (arr == nullptr || arr->getSize() == 0) return;
        
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
        cout << left << setw(15) << "Mode of Transport" 
             << setw(10) << "Count" << setw(20) << "Total Emission (kg CO2)" 
             << setw(15) << "Average per Resident" << endl;
        cout << string(60, '-') << endl;
        
        for (int m = 0; m < 6; m++) {
            if (modeCount[m] > 0) {
                cout << left << setw(15) << transportModes[m] 
                     << setw(10) << modeCount[m]
                     << setw(20) << modeEmission[m]
                     << setw(15) << (modeEmission[m] / modeCount[m]) << endl;
            }
        }
        cout << string(60, '-') << endl;
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
    
    // Bubble Sort implementation
    void bubbleSort(string sortBy = "age", string order = "asc") {
        auto start = chrono::high_resolution_clock::now();
        
        int n = arr->getSize();
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
    
    // Selection Sort implementation (additional algorithm for comparison)
    void selectionSort(string sortBy = "age", string order = "asc") {
        auto start = chrono::high_resolution_clock::now();
        
        int n = arr->getSize();
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
    
    // Linear Search
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
                     << setw(12) << comp->computeEmission(res) << "\n";
                found++;
            }
        }
        
        auto end = chrono::high_resolution_clock::now();
        cout << string(63, '-') << "\n";
        cout << "Found: " << found << " | Time: " 
             << chrono::duration<double, milli>(end - start).count() << " ms\n";
        cout << "Memory Usage: " << getMemoryUsage() << " bytes\n";
    }
    
    // Binary Search (requires sorted data)
    void binarySearch(string searchBy, string value) {
        // First, sort the array by the appropriate field
        if (searchBy == "age_group") {
            bubbleSort("age", "asc");
        } else if (searchBy == "transport") {
            // Simple bubble sort by transport mode
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
        
        if (searchBy == "age_group") {
            int dash = value.find('-');
            int minAge = stoi(value.substr(0, dash));
            int maxAge = stoi(value.substr(dash + 1));
            
            // Find first index with age >= minAge
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
            
            // Print all residents within age range
            if (first != -1) {
                for (int i = first; i < arr->getSize() && (*arr)[i].age <= maxAge; i++) {
                    Resident& res = (*arr)[i];
                    cout << left << setw(12) << res.residentID << setw(6) << res.age 
                         << setw(15) << res.modeOfTransport << setw(10) << res.dailyDistance 
                         << setw(8) << res.averageDayPerMonth 
                         << setw(12) << comp->computeEmission(res) << "\n";
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
                // Expand left and right to get all matches
                int l = foundIndex, r = foundIndex;
                auto toLower = [](string s) { for (char& c : s) c = tolower(c); return s; };
                while (l > 0 && toLower((*arr)[l - 1].modeOfTransport) == val) l--;
                while (r < arr->getSize() - 1 && toLower((*arr)[r + 1].modeOfTransport) == val) r++;
                
                for (int i = l; i <= r; i++) {
                    Resident& res = (*arr)[i];
                    cout << left << setw(12) << res.residentID << setw(6) << res.age 
                         << setw(15) << res.modeOfTransport << setw(10) << res.dailyDistance 
                         << setw(8) << res.averageDayPerMonth 
                         << setw(12) << comp->computeEmission(res) << "\n";
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
                     << setw(12) << comp->computeEmission(res) << "\n";
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
                     << setw(12) << comp->computeEmission(res) << "\n";
                found++;
            }
        }
        
        auto end = chrono::high_resolution_clock::now();
        cout << string(63, '-') << "\n";
        cout << "Found: " << found << " | Time: " 
             << chrono::duration<double, milli>(end - start).count() << " ms\n";
        cout << "Memory Usage (array): " << getMemoryUsage() << " bytes\n";
    }
    
    // Display sorted data
    void displaySorted() {
        arr->traversePrint();
    }
};

// Main function for array implementation
int main() {
    cout << "============================================================================================\n";
    cout << "-------The following is an Implementation of Arrays for DSTR PART 1-------\n";
    cout << "============================================================================================\n";
    
    // Create arrays for each city
    ResidentArray arrayA, arrayB, arrayC;
    
    // Create file managers
    FileManagerArray fmA, fmB, fmC;
    fmA.setResidentArray(&arrayA);
    fmB.setResidentArray(&arrayB);
    fmC.setResidentArray(&arrayC);
    
    // Load data from CSV files
    // Update file paths as needed
    fmA.loadFromCSV("dataset1-cityA.csv");
    fmB.loadFromCSV("dataset2-cityB.csv");
    fmC.loadFromCSV("dataset3-cityC.csv");
    
    // Display loaded data
    cout << "\n============================================================================================\n";
    cout << "City A Data (" << arrayA.getSize() << " residents)\n";
    cout << "============================================================================================\n";
    arrayA.traversePrint();
    
    cout << "\n============================================================================================\n";
    cout << "City B Data (" << arrayB.getSize() << " residents)\n";
    cout << "============================================================================================\n";
    arrayB.traversePrint();
    
    cout << "\n============================================================================================\n";
    cout << "City C Data (" << arrayC.getSize() << " residents)\n";
    cout << "============================================================================================\n";
    arrayC.traversePrint();
    
    // Create computation objects
    ComputationArray compA(&fmA);
    ComputationArray compB(&fmB);
    ComputationArray compC(&fmC);
    
    // Calculate total emissions
    cout << "\n============================================================================================\n";
    cout << "Total Carbon Emissions by City\n";
    cout << "============================================================================================\n";
    cout << "Total emission for City A: " << compA.computeTotalEmission() << " kg CO2" << endl;
    cout << "Total emission for City B: " << compB.computeTotalEmission() << " kg CO2" << endl;
    cout << "Total emission for City C: " << compC.computeTotalEmission() << " kg CO2" << endl;
    
    // Calculate emissions by mode for each city
    cout << "\n============================================================================================\n";
    cout << "City A - Emissions by Mode\n";
    cout << "============================================================================================\n";
    compA.calculateEmissionsByMode();
    
    cout << "\n============================================================================================\n";
    cout << "City B - Emissions by Mode\n";
    cout << "============================================================================================\n";
    compB.calculateEmissionsByMode();
    
    cout << "\n============================================================================================\n";
    cout << "City C - Emissions by Mode\n";
    cout << "============================================================================================\n";
    compC.calculateEmissionsByMode();
    
    // Calculate emissions by age group for each city
    cout << "\n============================================================================================\n";
    cout << "City A - Emissions by Age Group\n";
    cout << "============================================================================================\n";
    compA.calculateEmissionsByAgeGroup();
    
    cout << "\n============================================================================================\n";
    cout << "City B - Emissions by Age Group\n";
    cout << "============================================================================================\n";
    compB.calculateEmissionsByAgeGroup();
    
    cout << "\n============================================================================================\n";
    cout << "City C - Emissions by Age Group\n";
    cout << "============================================================================================\n";
    compC.calculateEmissionsByAgeGroup();
    
    // Create sort/search objects
    SortSearchArray ssaA(&arrayA, &compA);
    SortSearchArray ssaB(&arrayB, &compB);
    SortSearchArray ssaC(&arrayC, &compC);
    
    // Sorting experiments
    cout << "\n============================================================================================\n";
    cout << "SORTING EXPERIMENTS\n";
    cout << "============================================================================================\n";
    
    ssaA.bubbleSort("age", "asc");
    ssaB.bubbleSort("distance", "desc");
    ssaC.bubbleSort("emission", "asc");
    
    // Additional sorting algorithm for comparison
    cout << "\n--- Additional Sorting Algorithm for Comparison ---\n";
    // Reload data for fair comparison
    arrayA.clear();
    arrayB.clear();
    arrayC.clear();
    fmA.loadFromCSV("dataset1-cityA.csv");
    fmB.loadFromCSV("dataset2-cityB.csv");
    fmC.loadFromCSV("dataset3-cityC.csv");
    
    ssaA.selectionSort("age", "asc");
    ssaB.selectionSort("distance", "desc");
    ssaC.selectionSort("emission", "asc");
    
    // Searching experiments
    cout << "\n============================================================================================\n";
    cout << "SEARCHING EXPERIMENTS\n";
    cout << "============================================================================================\n";
    
    // Linear searches
    ssaA.linearSearch("transport", "Car");
    ssaB.linearSearch("age_group", "26-45");
    ssaC.linearSearch("distance_above", "15");
    ssaA.linearSearch("distance_below", "10");
    
    // Binary searches (will sort data first)
    cout << "\n--- Binary Search Results (Data will be sorted first) ---\n";
    ssaA.binarySearch("transport", "Bicycle");
    ssaB.binarySearch("age_group", "18-25");
    ssaC.binarySearch("distance_above", "15");
    ssaC.binarySearch("distance_below", "10");
    
    // Performance summary
    cout << "\n============================================================================================\n";
    cout << "PERFORMANCE ANALYSIS SUMMARY - ARRAY IMPLEMENTATION\n";
    cout << "============================================================================================\n";
    cout << "Array Characteristics:\n";
    cout << "  - Contiguous memory allocation\n";
    cout << "  - O(1) random access via index\n";
    cout << "  - Fixed capacity (can be resized with reallocation)\n";
    cout << "  - Cache-friendly due to data locality\n";
    cout << "\nTime Complexities:\n";
    cout << "  - Access: O(1)\n";
    cout << "  - Search (unsorted): O(n)\n";
    cout << "  - Search (sorted/binary): O(log n)\n";
    cout << "  - Insertion (end): O(1)\n";
    cout << "  - Insertion (middle): O(n)\n";
    cout << "  - Deletion: O(n)\n";
    cout << "  - Bubble Sort: O(n²)\n";
    cout << "  - Selection Sort: O(n²)\n";
    cout << "\nSpace Complexity:\n";
    cout << "  - Memory per resident: " << sizeof(Resident) << " bytes\n";
    
    return 0;
}