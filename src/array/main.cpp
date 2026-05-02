#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>

using namespace std;

// ============================================================================
// CONSTANTS AND STRUCTURES
// ============================================================================

const int MAX_RESIDENTS = 500;
const int MAX_AGE_GROUPS = 5;

// Age group definitions with min and max ages
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

// Resident data structure
struct Resident {
    string residentID;
    int age;
    string modeOfTransport;
    int dailyDistance;
    double carbonEmissionFactor;
    int averageDayPerMonth;
    
    // Default constructor
    Resident() : residentID(""), age(0), modeOfTransport(""), 
                 dailyDistance(0), carbonEmissionFactor(0.0), averageDayPerMonth(0) {}
    
    // Parameterized constructor
    Resident(string id, int a, string mode, int dist, double factor, int days)
        : residentID(id), age(a), modeOfTransport(mode), dailyDistance(dist),
          carbonEmissionFactor(factor), averageDayPerMonth(days) {}
};

// ============================================================================
// ARRAY-BASED RESIDENT CONTAINER
// ============================================================================

class ResidentArray {
private:
    Resident* residents;  // Dynamic array
    int size;             // Current number of residents
    int capacity;         // Maximum capacity
    
public:
    // Constructor: creates empty array
    ResidentArray(int cap = MAX_RESIDENTS) {
        capacity = cap;
        residents = new Resident[capacity];
        size = 0;
    }
    
    // Destructor: frees allocated memory
    ~ResidentArray() {
        delete[] residents;
    }
    
    // Getters
    int getSize() const { return size; }
    Resident* getData() const { return residents; }
    
    // Insert at end - O(1)
    void insertAtEnd(string id, int age, string mode, int dist, double factor, int days) {
        if (size < capacity) {
            residents[size] = Resident(id, age, mode, dist, factor, days);
            size++;
        }
    }
    
    // Array access operator
    Resident& operator[](int index) { return residents[index]; }
    const Resident& operator[](int index) const { return residents[index]; }
    
    // Display all residents
    void display() const {
        if (size == 0) {
            cout << "No data available." << endl;
            return;
        }
        
        cout << left << setw(12) << "ID" << setw(6) << "Age" << setw(15) << "Transport" 
             << setw(12) << "Distance" << setw(10) << "Days" << setw(15) << "Emission" << endl;
        cout << string(70, '-') << endl;
        
        for (int i = 0; i < size; i++) {
            double emission = residents[i].averageDayPerMonth * 
                            residents[i].dailyDistance * 
                            residents[i].carbonEmissionFactor;
            cout << left << setw(12) << residents[i].residentID 
                 << setw(6) << residents[i].age
                 << setw(15) << residents[i].modeOfTransport
                 << setw(12) << residents[i].dailyDistance
                 << setw(10) << residents[i].averageDayPerMonth
                 << setw(15) << fixed << setprecision(2) << emission << endl;
        }
    }
};

// ============================================================================
// FILE LOADING
// ============================================================================

class FileManager {
private:
    ResidentArray* arr;
    
public:
    FileManager() : arr(nullptr) {}
    void setArray(ResidentArray* a) { arr = a; }
    ResidentArray* getArray() { return arr; }  // REQUIRED METHOD
    
    // Load CSV data into array
    bool loadFromCSV(string filename) {
        ifstream file(filename);
        if (!file.is_open()) return false;
        
        string line;
        getline(file, line); // Skip header
        
        while (getline(file, line)) {
            stringstream ss(line);
            string id, ageStr, transport, distStr, factorStr, daysStr;
            
            getline(ss, id, ',');
            getline(ss, ageStr, ',');
            getline(ss, transport, ',');
            getline(ss, distStr, ',');
            getline(ss, factorStr, ',');
            getline(ss, daysStr);
            
            int age = stoi(ageStr);
            int dist = stoi(distStr);
            double factor = stod(factorStr);
            int days = stoi(daysStr);
            
            arr->insertAtEnd(id, age, transport, dist, factor, days);
        }
        file.close();
        return arr->getSize() > 0;
    }
};

// ============================================================================
// EMISSION COMPUTATIONS
// ============================================================================

class Computation {
private:
    FileManager* fileMgr;
    
public:
    Computation(FileManager* fm) : fileMgr(fm) {}
    
    // Calculate emission for a single resident
    double computeEmission(const Resident& r) const {
        return r.averageDayPerMonth * r.dailyDistance * r.carbonEmissionFactor;
    }
    
    // Calculate total emissions for all residents
    double computeTotalEmission() const {
        ResidentArray* arr = fileMgr->getArray();
        double total = 0;
        for (int i = 0; i < arr->getSize(); i++) {
            total += computeEmission((*arr)[i]);
        }
        return total;
    }
    
    // Determine age group index for a given age
    int getAgeGroupIndex(int age) const {
        for (int i = 0; i < MAX_AGE_GROUPS; i++) {
            if (age >= AGE_GROUPS[i].minAge && age <= AGE_GROUPS[i].maxAge)
                return i;
        }
        return -1;
    }
    
    // Calculate and display emissions by age group
    void displayEmissionsByAgeGroup() const {
        ResidentArray* arr = fileMgr->getArray();
        int count[MAX_AGE_GROUPS] = {0};
        double totalEmission[MAX_AGE_GROUPS] = {0};
        int modeCount[MAX_AGE_GROUPS][6] = {0};
        string modes[] = {"Car", "Bus", "Bicycle", "Walking", "School Bus", "Carpool"};
        
        for (int i = 0; i < arr->getSize(); i++) {
            Resident& r = (*arr)[i];
            int g = getAgeGroupIndex(r.age);
            if (g == -1) continue;
            
            count[g]++;
            totalEmission[g] += computeEmission(r);
            
            for (int m = 0; m < 6; m++) {
                if (r.modeOfTransport == modes[m]) {
                    modeCount[g][m]++;
                    break;
                }
            }
        }
        
        for (int g = 0; g < MAX_AGE_GROUPS; g++) {
            if (count[g] == 0) continue;
            
            cout << "\nAge Group: " << AGE_GROUPS[g].minAge << "-" << AGE_GROUPS[g].maxAge 
                 << " (" << AGE_GROUPS[g].name << ")" << endl;
            cout << string(60, '-') << endl;
            
            // Find most preferred mode
            int maxMode = 0;
            for (int m = 1; m < 6; m++) {
                if (modeCount[g][m] > modeCount[g][maxMode]) maxMode = m;
            }
            
            cout << "Most preferred mode: " << modes[maxMode] << endl;
            cout << "Total emissions: " << fixed << setprecision(2) << totalEmission[g] << " kg CO2" << endl;
            cout << "Average per resident: " << totalEmission[g] / count[g] << " kg CO2" << endl;
        }
    }
    
    // Calculate and display emissions by transport mode
    void displayEmissionsByMode() const {
        ResidentArray* arr = fileMgr->getArray();
        int count[6] = {0};
        double total[6] = {0};
        string modes[] = {"Car", "Bus", "Bicycle", "Walking", "School Bus", "Carpool"};
        
        for (int i = 0; i < arr->getSize(); i++) {
            Resident& r = (*arr)[i];
            for (int m = 0; m < 6; m++) {
                if (r.modeOfTransport == modes[m]) {
                    count[m]++;
                    total[m] += computeEmission(r);
                    break;
                }
            }
        }
        
        cout << "\nEmissions by Transport Mode:" << endl;
        cout << string(50, '-') << endl;
        for (int m = 0; m < 6; m++) {
            if (count[m] > 0) {
                cout << left << setw(15) << modes[m] 
                     << "Count: " << setw(6) << count[m]
                     << "Total: " << fixed << setprecision(2) << total[m] << " kg CO2" << endl;
            }
        }
    }
};

// ============================================================================
// SORTING AND SEARCHING ALGORITHMS
// ============================================================================

class SortSearch {
private:
    ResidentArray* arr;
    Computation* comp;
    
public:
    SortSearch(ResidentArray* a, Computation* c) : arr(a), comp(c) {}
    
    // Bubble Sort - O(n^2) - sorts by age in ascending order
    void bubbleSortByAge() {
        int n = arr->getSize();
        bool swapped;
        
        for (int i = 0; i < n - 1; i++) {
            swapped = false;
            for (int j = 0; j < n - i - 1; j++) {
                if ((*arr)[j].age > (*arr)[j + 1].age) {
                    swap((*arr)[j], (*arr)[j + 1]);
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }
    
    // Linear Search - O(n) - finds residents matching age range
    void linearSearchByAgeGroup(int minAge, int maxAge) {
        int found = 0;
        cout << "\n[Linear Search] Age Group: " << minAge << "-" << maxAge << endl;
        cout << left << setw(12) << "ID" << setw(6) << "Age" << setw(15) << "Transport" 
             << setw(10) << "Distance" << setw(15) << "Emission" << endl;
        cout << string(58, '-') << endl;
        
        for (int i = 0; i < arr->getSize(); i++) {
            Resident& r = (*arr)[i];
            if (r.age >= minAge && r.age <= maxAge) {
                cout << left << setw(12) << r.residentID << setw(6) << r.age
                     << setw(15) << r.modeOfTransport << setw(10) << r.dailyDistance
                     << setw(15) << fixed << setprecision(2) << comp->computeEmission(r) << endl;
                found++;
            }
        }
        cout << "Found: " << found << " residents" << endl;
    }
    
    // Binary Search - O(log n) - requires sorted array (age must be sorted first)
    void binarySearchByAge(int targetAge) {
        int left = 0, right = arr->getSize() - 1;
        int foundIndex = -1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if ((*arr)[mid].age == targetAge) {
                foundIndex = mid;
                break;
            } else if ((*arr)[mid].age < targetAge) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        if (foundIndex != -1) {
            Resident& r = (*arr)[foundIndex];
            cout << "\n[Binary Search] Found age " << targetAge << ": "
                 << r.residentID << " | Age: " << r.age << " | " << r.modeOfTransport << endl;
        } else {
            cout << "\n[Binary Search] Age " << targetAge << " not found" << endl;
        }
    }
};

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

// Find valid file path for CSV
string findFilePath(const string& filename) {
    string paths[] = {"../../data/", "../data/", "./data/", ""};
    for (string path : paths) {
        string fullPath = path + filename;
        ifstream test(fullPath);
        if (test.is_open()) {
            test.close();
            return fullPath;
        }
    }
    return filename;
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    cout << "====================================================================\n";
    cout << "Array Implementation - Transport Data Analysis\n";
    cout << "====================================================================\n\n";
    
    // File paths
    string fileA = findFilePath("dataset1-cityA.csv");
    string fileB = findFilePath("dataset2-cityB.csv");
    string fileC = findFilePath("dataset3-cityC.csv");
    
    // Create arrays and load data
    ResidentArray cityA, cityB, cityC;
    FileManager fmA, fmB, fmC;
    fmA.setArray(&cityA);
    fmB.setArray(&cityB);
    fmC.setArray(&cityC);
    
    fmA.loadFromCSV(fileA);
    fmB.loadFromCSV(fileB);
    fmC.loadFromCSV(fileC);
    
    // Display data summary
    cout << "City A: " << cityA.getSize() << " residents" << endl;
    cout << "City B: " << cityB.getSize() << " residents" << endl;
    cout << "City C: " << cityC.getSize() << " residents" << endl;
    
    // Create computation objects
    Computation compA(&fmA), compB(&fmB), compC(&fmC);
    
    // Display total emissions
    cout << "\n--- TOTAL CARBON EMISSIONS ---" << endl;
    cout << "City A: " << fixed << setprecision(2) << compA.computeTotalEmission() << " kg CO2" << endl;
    cout << "City B: " << compB.computeTotalEmission() << " kg CO2" << endl;
    cout << "City C: " << compC.computeTotalEmission() << " kg CO2" << endl;
    
    // Display emissions by mode for City A
    cout << "\n--- CITY A: EMISSIONS BY MODE ---" << endl;
    compA.displayEmissionsByMode();
    
    // Display emissions by age group for City A
    cout << "\n--- CITY A: EMISSIONS BY AGE GROUP ---" << endl;
    compA.displayEmissionsByAgeGroup();
    
    // Sorting demonstration (with timer inside main)
    cout << "\n--- SORTING DEMONSTRATION ---" << endl;
    SortSearch sorter(&cityA, &compA);
    
    auto start = chrono::high_resolution_clock::now();
    sorter.bubbleSortByAge();
    auto end = chrono::high_resolution_clock::now();
    cout << "Bubble Sort (by age) time: " 
         << chrono::duration<double, milli>(end - start).count() << " ms" << endl;
    
    // Display first 10 sorted records
    cout << "\nFirst 10 residents sorted by age:" << endl;
    for (int i = 0; i < 10 && i < cityA.getSize(); i++) {
        cout << "  " << cityA[i].residentID << " | Age: " << cityA[i].age 
             << " | " << cityA[i].modeOfTransport << endl;
    }
    
    // Linear search demonstration (with timer inside main)
    cout << "\n--- LINEAR SEARCH DEMONSTRATION ---" << endl;
    start = chrono::high_resolution_clock::now();
    sorter.linearSearchByAgeGroup(26, 45);
    end = chrono::high_resolution_clock::now();
    cout << "Linear Search time: " << chrono::duration<double, milli>(end - start).count() << " ms" << endl;
    
    // Binary search requires sorted array (already sorted from bubble sort)
    cout << "\n--- BINARY SEARCH DEMONSTRATION ---" << endl;
    start = chrono::high_resolution_clock::now();
    sorter.binarySearchByAge(30);
    end = chrono::high_resolution_clock::now();
    cout << "Binary Search time: " << chrono::duration<double, milli>(end - start).count() << " ms" << endl;
    
    // Performance summary
    cout << "\n--- PERFORMANCE SUMMARY ---" << endl;
    cout << "Array Characteristics:" << endl;
    cout << "  - Contiguous memory, O(1) access, cache-friendly" << endl;
    cout << "  - Memory per resident: " << sizeof(Resident) << " bytes" << endl;
    cout << "  - Total memory for City A: " << cityA.getSize() * sizeof(Resident) << " bytes" << endl;
    
    cout << "\nPress Enter to exit...";
    cin.get();
    
    return 0;
}