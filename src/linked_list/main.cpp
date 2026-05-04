#include <iostream>
using namespace std;
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iomanip>
#include <chrono>


class Resident{
    //This class holds the values that a resident must have
    public:
    string residentID;
    int age;
    string modeOfTransport;
    int dailyDistance;
    double carbonEmissionFactor;
    int averageDayPerMonth;
    Resident* next; 

    Resident(){};
    Resident(string residentID,int age,string modeOfTransport, int dailyDistance, double carbonEmissionFactor,int averageDayPerMonth,Resident* next){
    this->residentID= residentID;
    this->age= age;
    this->modeOfTransport= modeOfTransport;
    this->dailyDistance= dailyDistance;
    this->carbonEmissionFactor= carbonEmissionFactor;
    this->averageDayPerMonth= averageDayPerMonth;
    this->next=next;
    }
    };  
    class ResidentList {
          //Class to handle the linked list and the operations of the list including insertion and clearing and deletion
        private:
        Resident * head= nullptr;
        int size=0;
    public:
    //Accessors and Mutators for head and accesor for size, only accesor is needed
    int getSize()
        {
           return size;
        };

    void SetHead(Resident*head){
        this->head=head;
    }
    Resident *GetHead(){
        return head;
    }
        //Overloaded constructors 
        ResidentList(){};

    void traversePrint(){
        Resident* current=head;

        if (head==nullptr){
            cout<<"This List is empty";
            return;
        }
        while (current!=nullptr)
        {
            cout<<current->residentID<<", ";
            cout<<current->age<<", ";
            cout<<current->modeOfTransport<<", ";
            cout<<current->dailyDistance<<", ";
            cout<<current->carbonEmissionFactor<<", ";
            cout<<current->averageDayPerMonth<<endl;
            current=current->next;
        }
        cout<<"--End of List--"<<endl;
        };

        void insertAtStart(string ID,int resage,string resmodeOfTransport, int resdailyDistance, double rescarbonEmissionFactor,int resaverageDayPerMonth){
            Resident*newResident=new Resident{ID,resage,resmodeOfTransport,resdailyDistance,rescarbonEmissionFactor,resaverageDayPerMonth,nullptr};
            newResident->next=head;
            head=newResident;
            size++;
        };

        void insertAtEnd(string ID,int resage,string resmodeOfTransport, int resdailyDistance, double rescarbonEmissionFactor,int resaverageDayPerMonth)
        {
            Resident*newResident=new Resident{ID,resage,resmodeOfTransport,resdailyDistance,rescarbonEmissionFactor,resaverageDayPerMonth,nullptr};
            if (head==nullptr){
            head=newResident;
            size++;
            return;
        };
            Resident*currentResident=head;
            while(currentResident->next!=nullptr)
            {
                currentResident=currentResident->next;
            };
            currentResident->next=newResident;
            size++;
        };
        void insertAtindex(string ID,int resage,string resmodeOfTransport, int resdailyDistance, double rescarbonEmissionFactor,int resaverageDayPerMonth, int index)
        {
            if (index<0)
            {
                cout<<"Index cannot be less than 0"<<endl;
                return;
            }

            //IF INDEX IS 0
            if (index==0){
                Resident* newResident =new Resident{ID,resage,resmodeOfTransport,resdailyDistance,rescarbonEmissionFactor,resaverageDayPerMonth,nullptr};
                newResident->next=head;
                head=newResident;
                size++;
                return;
            }
            //IF INDEX IS WITHIN BOUNDS 
            Resident* current =head;
            for (int i =0; i < index -1 && current!=nullptr;i++ )
            {
                current=current->next;
            }
            if (current==nullptr){
                cout<<"Index is out of bounds\n";
                return;
            }
            Resident* newResident =new Resident{ID,resage,resmodeOfTransport,resdailyDistance,rescarbonEmissionFactor,resaverageDayPerMonth,nullptr};
            newResident->next=current->next;
            current->next=newResident;
            size++;
        }
        //Clear method
        void clear ()
        {
            while (head!=nullptr){
                Resident* current=head;
                head=head->next;
                delete current;
            }
            size=0;
        }
    //Deletion Methods
        void deleteAtBeginning()
        {
            if (head==nullptr){
                cout<<"The list is already empty";
                return;
            }
            Resident *temp=head;
            head=head->next;
            delete temp;
            size--;
        }

        void deleteAtEnd (){
            Resident* current=head;
            Resident*temp=head;
            if (head==nullptr)
            {
                cout<<"This list is already empty\n";
                return;
            }
            
            if (head->next==nullptr)
            {  
                head=head->next;
                delete temp;
                cout<<"Single element was deleted\n";
                size--;
                return;
            }
            
            while(current->next->next!=nullptr)
            {
                current=current->next; 
            }
            temp=current->next;
            delete temp;
            current->next=nullptr;
            size--;
        }
        };
        class FileManager{
            //Class to handle all the file logic
            private:
            ResidentList* resList;
            public:
            void SetResidentList(ResidentList*resList){
                this->resList=resList;
            }
            ResidentList *GetResidentList(){
                return resList;
            }

            //Overloaded constructor 
            FileManager(){}
            
            //Method to load from CSV
            void loadFromCSV(string datafile){
            ifstream file(datafile);
            if (!file.is_open()){
                cout<<"Error opening file\n";
                return;
            }
            //Make a line and skip header
            string line;
            getline(file,line);
            //Loop through the file
            while(getline(file,line)){
            //split the line
            stringstream ss(line);
            string ID, ageStr,transport, distanceStr,factorStr,daysStr;

            // Split using commas
            getline(ss, ID, ',');
            getline(ss, ageStr, ',');
            getline(ss, transport, ',');
            getline(ss, distanceStr, ',');
            getline(ss, factorStr, ',');
            getline(ss, daysStr);

            //convert the data types using the string libraty st function 
            int age = stoi(ageStr);
            int distance = stoi(distanceStr);
            double factor = stod(factorStr);
            int days = stoi(daysStr);
            resList->insertAtEnd(ID,age,transport,distance,factor, days);
        }
            file.close();
        }
        };

        class computation {
            //Class to handle all computations 
            public:
            Resident *link=nullptr;
            FileManager* load;

            //Constructor Overload
            computation (FileManager* fm){
                this->load =  fm;
            }// making sure it takes the list from file manager

            double computeEmission(Resident * res){ //Function to calculate emissions for a specified node
                if(res==nullptr){
                    cout<<"The list is empty no value"<<endl;
                    return 0;
                }
                    double emission=res->averageDayPerMonth*
                                    res->dailyDistance*
                                    res->carbonEmissionFactor;
                return emission;
            }

            //Method below calculates the total emisiion, 
            double computeTotalEmission(){
                double totalEmission=0;;
                double product=0;

                ResidentList* list=load->GetResidentList(); //get the list 
                link=list->GetHead();   //assign link to the head of the list
                if(link==nullptr){
                    cout<<"The list is empty no value"<<endl;
                    return 0;
                }
                while(link!=nullptr)    //Traverse the list and accumulate the values 
                {   
                    product=computeEmission(link); //calls the emmission function 
                    totalEmission+=product;
                    link=link->next; 
                }
                return totalEmission;
            }

            //Categorisation for age groups (idk where to place for now)
            string getAgeGroup(int age) {
                if (age >= 6 && age <= 17) return "6-17 (Children & Teens)";
                if (age >= 18 && age <= 25) return "18-25 (University Students & Young Adults)";
                if (age >= 26 && age <= 45) return "26-45 (Working Adults Early Career)";
                if (age >= 46 && age <= 60) return "46-60 (Working Adults Late Career)";
                if (age >= 61 && age <= 100) return "61-100 (Senior Citizens & Retirees)";
                return "Error.";
            }

            // Method to analyze by age group
            void analyzeByAgeGroup(){

                string groups[] = { //age group labels in order
                    "6-17 (Children & Teens)",
                    "18-25 (University Students & Young Adults)",
                    "26-45 (Working Adults Early Career)",
                    "46-60 (Working Adults Late Career)",
                    "61-100 (Senior Citizens & Retirees)"
                };
                int numGroups = 5;

                // transport methods
                string modes[] = {"Car", "Bus", "Bicycle", "Walking", "School Bus", "Carpool"};
                int numModes = 6;

                ResidentList* list = load->GetResidentList();
                Resident* cur = list->GetHead();

                // accumulators
                double totalEmission[5] = {0};
                int count[5] = {0};
                int modeCounts[5][6] = {}; // [group][mode]

                while (cur != nullptr) {
                    string g = getAgeGroup(cur->age);
                    int gi = -1;
                    for (int i = 0; i < numGroups; i++)
                        if (groups[i] == g) { gi = i; break; }
                
                    if (gi != -1) {
                        double em = computeEmission(cur);
                        totalEmission[gi] += em;
                        count[gi]++;

                        for (int m = 0; m < numModes; m++)
                            if (cur->modeOfTransport == modes[m]) { modeCounts[gi][m]++; break; }
                    }
                    cur = cur->next;
                }

                cout << "\n" << string(65, '=') << "\n";
                cout << "  Age Group Categorization Analysis\n";
                cout << string(65, '=') << "\n";

                for (int i = 0; i < numGroups; i++) {
                    if (count[i] == 0) continue;

                    // analysis for most preferred transport
                    int maxIdx = 0;
                    for (int m = 1; m < numModes; m++)
                        if (modeCounts[i][m] > modeCounts[i][maxIdx]) maxIdx = m;

                    double avg = totalEmission[i] / count[i];

                    cout << "\nAge Group: " << groups[i] << "\n";
                    cout << string(65, '-') << "\n";
                    cout << left << setw(20) << "Mode of Transportation"
                         << setw(10) << "Count"
                         << setw(22) << "Total mission"
                         << "Average per Resident\n";
                    cout << string(65, '-') << "\n";

                    for (int m = 0; m < numModes; m++) {
                        if (modeCounts[i][m] == 0) continue;
                        double modeEmission = (double)modeCounts[i][m] / count[i] * totalEmission[i];
                        double modeAvg = modeEmission / modeCounts[i][m];
                        cout << left << setw(20) << modes[m]
                             << setw(10) << modeCounts[i][m]
                             << setw(22) << fixed << setprecision(2) << modeEmission
                             << fixed << setprecision(2) << modeAvg
                             << "\n";
                    }

                    cout << string(65, '-') << "\n";
                    cout << "Most preferred method of transport : " << modes[maxIdx] << "\n";
                    cout << "Total emissions                    : " << fixed << setprecision(2) << totalEmission[i] << " kg CO2\n";
                    cout << "Average emission                   : " << fixed << setprecision(2) << avg << " kg CO2/resident\n";
                    cout << "Residents in Group                 : " << count[i] << "\n";
                }
            }

                // carbon emission analysis
            void analyseEmissions() {
                string modes[] = {"Car", "Bus", "Bicycle", "Walking", "School Bus", "Carpool"};
                int numModes = 6;

                double modeEmission[6] = {0};
                int modeCount[6] = {0};
                double totalEmission = 0;

                ResidentList* list = load->GetResidentList();
                Resident* cur = list->GetHead();

                while (cur != nullptr) {
                    double em = computeEmission(cur);
                    totalEmission += em;
                    for (int m = 0; m < numModes; m++) {
                        if (cur->modeOfTransport == modes[m]) {
                            modeEmission[m] += em;
                            modeCount[m]++;
                            break;
                        }
                    }
                    cur = cur->next;
                }

                cout << "\n" << string(65, '=') << "\n";
                cout << "  Emission by mode of transportation\n";
                cout << string(65, '=') << "\n";
                cout << left << setw(14) << "Transportation"
                    << setw(10) << "Count"
                    << setw(24) << "Total emission"
                    << "% of Total\n";
                cout << string(65, '-') << "\n";

                for (int m = 0; m < numModes; m++) {
                    if (modeCount[m] == 0) continue;
                    double pct = (totalEmission > 0) ? (modeEmission[m] / totalEmission * 100.0) : 0;
                    cout << left << setw(14) << modes[m]
                        << setw(10) << modeCount[m]
                        << setw(24) << fixed << setprecision(2) << modeEmission[m]
                        << fixed << setprecision(1) << pct << "%\n";
                }

                cout << string(65, '-') << "\n";
                cout << "Total emissions: " << fixed << setprecision(2) << totalEmission << " kg CO2\n";
                cout << string(65, '=') << "\n";

            }

    };

        
 
//Contains Bubble Sort, Linear search and Binary search on Linked Listss.
class SortSearch {
    private:
        ResidentList* list;
        computation* comp;
    
        //Swapping data between nodes.
        void swapData(Resident* a, Resident* b) {
            swap(a->residentID, b->residentID);
            swap(a->age, b->age);
            swap(a->modeOfTransport, b->modeOfTransport);
            swap(a->dailyDistance, b->dailyDistance);
            swap(a->carbonEmissionFactor, b->carbonEmissionFactor);
            swap(a->averageDayPerMonth, b->averageDayPerMonth);
        }
        //Getting Memory Size for each algorithm.
        size_t getMemoryUsage() {
            return list->getSize() * sizeof(Resident);
        }

    public:

        //Takes pointer to the linked list and computational object.
        SortSearch(ResidentList* resList, computation* comp) {
            this->list = resList;
            this->comp = comp;
        }

        //Implementation of Bubble Sort.
        void bubbleSort(string sortBy = "age", string order = "asc") {      //Default args set to age and asc order.
            auto start = chrono::high_resolution_clock::now();      //Starts timer.
            Resident* last = nullptr;           //Marks boundary for unsorted data.
            bool swapped;
            do {
                swapped = false;
                Resident* cur = list->GetHead();
                while (cur->next != last) {
                    Resident* nxt = cur->next;
                    bool doSwap = false;
                    
                    //Compares nodes next to each other based on the fields.
                    if (sortBy == "age")
                        doSwap = (order == "asc") ? cur->age > nxt->age : cur->age < nxt->age;
                    else if (sortBy == "distance")
                        doSwap = (order == "asc") ? cur->dailyDistance > nxt->dailyDistance : cur->dailyDistance < nxt->dailyDistance;
                    else if (sortBy == "emission")
                        doSwap = (order == "asc") ? comp->computeEmission(cur) > comp->computeEmission(nxt) : comp->computeEmission(cur) < comp->computeEmission(nxt);
                    if (doSwap) { swapData(cur, nxt); swapped = true; }     //Swaps the nodes if out of order.
                    cur = cur->next;
                }

                last = cur;     //Last sorted node goes back one step after each pass.
            }
            while (swapped);        //Stops the comparision if no swaps occur.
            auto end = chrono::high_resolution_clock::now();        //Stops the timer.
            cout << "\n[Bubble Sort] By: " << sortBy << " | " << order << " | Time: " << chrono::duration<double, milli>(end - start).count() << " ms\n";
            cout << "Memory Usage: " << getMemoryUsage() << " bytes\n";
        }
        
        // Implementation of Linear Search.
        void linearSearch(string searchBy, string value) {
            auto start = chrono::high_resolution_clock::now();      //Starts the timer
            Resident* cur = list->GetHead();
            int found = 0;
            cout << "\n[Linear Search] " << searchBy << " = " << value << "\n";
            cout << left << setw(12)<<"ID" << setw(6)<<"Age" << setw(12)<<"Transport" << setw(10)<<"Distance" << setw(8)<<"Days" << setw(12)<<"Emission\n";
            cout << string(60, '-') << "\n";
            while (cur != nullptr) {
                bool match = false;
                if (searchBy == "transport") {
                    //Converts both to lowercase to prevent any capitalization issue.
                    string a = cur->modeOfTransport, b = value;
                    for (char& c : a) c = tolower(c);
                    for (char& c : b) c = tolower(c);
                    match = (a == b);
                }
                //This parses the min and max age from the age range.
                else if (searchBy == "age_group") {
                    int dash = value.find('-');
                    match = cur->age >= stoi(value.substr(0, dash)) && cur->age <= stoi(value.substr(dash + 1));
                }
                //This checks if the distance exceeds the given threshold.
                else if (searchBy == "distance_above") {
                    match = cur->dailyDistance > stoi(value);
                }
                //This checks if the distance given is below the given threshold.
                else if (searchBy == "distance_below") {
                    match = cur->dailyDistance < stoi(value);
                }
                if (match) {
                    cout << left << setw(12)<<cur->residentID << setw(6)<<cur->age << setw(12)<<cur->modeOfTransport << setw(10)<<cur->dailyDistance << setw(8)<<cur->averageDayPerMonth << setw(12)<<comp->computeEmission(cur) << "\n";
                    found++;
                }
                cur = cur->next;        //This moves the current to the next node.
            }
            auto end = chrono::high_resolution_clock::now();        //Stop timer.
            cout << string(60, '-') << "\n";
            cout << "Found: " << found << " | Time: " << chrono::duration<double, milli>(end - start).count() << " ms\n";
            cout << "Memory Usage: " << getMemoryUsage() << " bytes\n";
        }

        //Implementation of Binary Search.
        void binarySearch(string searchBy, string value) {
            auto start = chrono::high_resolution_clock::now();      //Starts timer.
            int n = list->getSize();
            Resident** arr = new Resident*[n];      //Makes a temp pointer array.
            Resident* cur = list->GetHead();
            for (int i = 0; i < n; i++) { arr[i] = cur; cur = cur->next; }      //This copies the nodes into the temp array.

            // Sort temp array by relevant field.
            for (int i = 0; i < n - 1; i++) {
                for (int j = 0; j < n - i - 1; j++) {
                    bool doSwap = false;
                    if (searchBy == "age_group")
                        doSwap = arr[j]->age > arr[j+1]->age;
                    else if (searchBy == "transport") {
                        string a = arr[j]->modeOfTransport, b = arr[j+1]->modeOfTransport;
                        for (char& c : a) c = tolower(c);
                        for (char& c : b) c = tolower(c);
                        doSwap = a > b;     //Comparing Alphabets.
                    }
                    else
                        doSwap = arr[j]->dailyDistance > arr[j+1]->dailyDistance;
                    if (doSwap) swap(arr[j], arr[j+1]);
                }
            }

            int found = 0, lo = 0, hi = n - 1, mid, startIdx = -1;
            cout << "\n[Binary Search] " << searchBy << " = " << value << "\n";
            cout << left << setw(12)<<"ID" << setw(6)<<"Age" << setw(12)<<"Transport" << setw(10)<<"Distance" << setw(8)<<"Days" << setw(12)<<"Emission\n";
            cout << string(60, '-') << "\n";

            //This parses the min and max age from the range.
            if (searchBy == "age_group") {
                int dash = value.find('-'), minAge = stoi(value.substr(0, dash)), maxAge = stoi(value.substr(dash + 1));
                //This is the binary search for the 1st index.
                while (lo <= hi) { mid = lo + (hi-lo)/2; if (arr[mid]->age >= minAge) { startIdx = mid; hi = mid-1; } else lo = mid+1; }
                if (startIdx != -1)
                    //Prints all the nodes within the age range.
                    for (int i = startIdx; i < n && arr[i]->age <= maxAge; i++, found++)
                        cout << left << setw(12)<<arr[i]->residentID << setw(6)<<arr[i]->age << setw(12)<<arr[i]->modeOfTransport << setw(10)<<arr[i]->dailyDistance << setw(8)<<arr[i]->averageDayPerMonth << setw(12)<<comp->computeEmission(arr[i]) << "\n";
            }

            //This parses the mode of transportation.
            else if (searchBy == "transport") {
                string val = value; for (char& c : val) c = tolower(c);
                //Performs binary search for the matching transport mode.
                while (lo <= hi) { mid = lo+(hi-lo)/2; string m = arr[mid]->modeOfTransport; for (char& c : m) c = tolower(c); if (m == val) { startIdx = mid; break; } else if (m < val) lo = mid+1; else hi = mid-1; }
                if (startIdx != -1) {
                    int l = startIdx, r = startIdx;
                    auto toLow = [](string s) { for (char& c : s) c = tolower(c); return s; };
                    //This expands in both ways to get all the matching transport modes.
                    while (l > 0   && toLow(arr[l-1]->modeOfTransport) == val) l--;
                    while (r < n-1 && toLow(arr[r+1]->modeOfTransport) == val) r++;
                    for (int i = l; i <= r; i++, found++)
                        cout << left << setw(12)<<arr[i]->residentID << setw(6)<<arr[i]->age << setw(12)<<arr[i]->modeOfTransport << setw(10)<<arr[i]->dailyDistance << setw(8)<<arr[i]->averageDayPerMonth << setw(12)<<comp->computeEmission(arr[i]) << "\n";
                }
            }

            //This parses the distance threshold above a given number.
            else if (searchBy == "distance_above") {
                int threshold = stoi(value); startIdx = n;
                //This does binary search for index where distance is more then the threshold.
                while (lo <= hi) { mid = lo+(hi-lo)/2; if (arr[mid]->dailyDistance > threshold) { startIdx = mid; hi = mid-1; } else lo = mid+1; }
                for (int i = startIdx; i < n; i++, found++)
                    cout << left << setw(12)<<arr[i]->residentID << setw(6)<<arr[i]->age << setw(12)<<arr[i]->modeOfTransport << setw(10)<<arr[i]->dailyDistance << setw(8)<<arr[i]->averageDayPerMonth << setw(12)<<comp->computeEmission(arr[i]) << "\n";

            }

            //This parses the distance threshold below a given number.
            else if (searchBy == "distance_below") {
                int threshold = stoi(value); startIdx = -1;
                //This does binary search for index where distance is less then the threshold.
                while (lo <= hi) { mid = lo+(hi-lo)/2; if (arr[mid]->dailyDistance < threshold) { startIdx = mid; lo = mid+1; } else hi = mid-1; }
                for (int i = 0; i <= startIdx; i++, found++)
                    cout << left << setw(12)<<arr[i]->residentID << setw(6)<<arr[i]->age << setw(12)<<arr[i]->modeOfTransport << setw(10)<<arr[i]->dailyDistance << setw(8)<<arr[i]->averageDayPerMonth << setw(12)<<comp->computeEmission(arr[i]) << "\n";
            }

            auto end = chrono::high_resolution_clock::now();        //Stops the timer.
            cout << string(60, '-') << "\n";
            cout << "Found: " << found << " | Time: " << chrono::duration<double, milli>(end - start).count() << " ms\n";
            cout << "Memory Usage (list): " << getMemoryUsage() << " bytes\n";
            cout << "Memory Usage (temp array): " << n * sizeof(Resident*) << " bytes\n";
            delete[] arr;       //Deletes the temp array to prevent memory leak
        }
};

// method to compare emission in cities
void compareCitiesEmission(computation& compA, computation& compB, computation& compC) {
    string modes[] = {"Car", "Bus", "Bicycle", "Walking", "School Bus", "Carpool"};
    int numModes = 6;
    string cities[] = {"City A", "City B", "City C"};
    computation* comps[] = {&compA, &compB, &compC};

    double modeEmission[3][6] = {};
    double cityTotal[3] = {};

    for (int c = 0; c < 3; c++) {
        ResidentList* list = comps[c]->load->GetResidentList();
        Resident* cur = list->GetHead();
        while (cur != nullptr) {
            double em = comps[c]->computeEmission(cur);
            cityTotal[c] += em;
            for (int m = 0; m < numModes; m++) {
                if (cur->modeOfTransport == modes[m]) {
                    modeEmission[c][m] += em;
                    break;
                }
            }
            cur = cur->next;
        }
    }

    cout << "\n" << string(75, '=') << "\n";
    cout << "  Emission Comparison accross the datasets\n";
    cout << string(75, '=') << "\n";
    cout << left << setw(14) << "Transportation"
         << setw(20) << "City A"
         << setw(20) << "City B"
         << setw(20) << "City C" << "\n";
    cout << string(75, '-') << "\n";

    for (int m = 0; m < numModes; m++) {
        bool anyData = false;
        for (int c = 0; c < 3; c++) if (modeEmission[c][m] > 0) anyData = true;
        if (!anyData) continue;
        cout << left << setw(14) << modes[m];
        for (int c = 0; c < 3; c++) //haha c++ reference
            cout << setw(20) << fixed << setprecision(2) << modeEmission[c][m];
        cout << "\n";
    }

    cout << string(75, '-') << "\n";
    cout << left << setw(14) << "Total";
    for (int c = 0; c < 3; c++) //haha another c++ reference good one yeah 
        cout << setw(20) << fixed << setprecision(2) << cityTotal[c];
    cout << "\n" << string(75, '=') << "\n";

    int highestCity = 0;
    for (int c = 1; c < 3; c++)
        if (cityTotal[c] > cityTotal[highestCity]) highestCity = c;
    cout << "Highest emitting city: " << cities[highestCity]
         << " (" << fixed << setprecision(2) << cityTotal[highestCity] << " kg CO2)\n";
}

int main(){

    cout<<"============================================================================================\n";
    cout<<"-------The following is an Implementation of linked lists for DSTR PART 1-------\n";
    cout<<"============================================================================================\n";
    cout<<"Linked Lists"<<endl;
    
    ResidentList listA, listB, listC; // make lists for each city
    FileManager fmcityA, fmcityB, fmcityC;  //both are on stack
    //Make the Lists from each data set 
    fmcityA.SetResidentList(&listA); 
    fmcityB.SetResidentList(&listB);
    fmcityC.SetResidentList(&listC);
    
    //Load from the data set
    fmcityA.loadFromCSV("../../data/dataset1-cityA.csv");
    fmcityB.loadFromCSV("../../data/dataset2-cityB.csv");   //load using the function for load csv
    fmcityC.loadFromCSV("../../data/dataset3-cityC.csv"); 
    
    cout<<"============================================================================================\n";
    cout<<"City A data\n";
    cout<<"============================================================================================\n";
    listA.traversePrint();  //Print the list with traversal

    cout<<"============================================================================================\n";
    cout<<"City B data\n";
    cout<<"============================================================================================\n";
    listB.traversePrint();  //Print the list with traversal

    cout<<"============================================================================================\n";
    cout<<"City C data\n";
    cout<<"============================================================================================\n";
    listC.traversePrint();  //Print the list with traversal
    
    computation computeA(&fmcityA);
    computation computeB(&fmcityB); //compute objects
    computation computeC(&fmcityC); 

    cout<<"Total emission for City A: "<<computeA.computeTotalEmission()<<endl;;
    cout<<"Total emission City B: "<<computeB.computeTotalEmission()<<endl;;    //Print the total emissions for each city
    cout<<"Total emission City C: "<<computeC.computeTotalEmission()<<endl;;

    
    cout << "\n=== CITY A ===\n"; computeA.analyzeByAgeGroup();
    cout << "\n=== CITY B ===\n"; computeB.analyzeByAgeGroup();
    cout << "\n=== CITY C ===\n"; computeC.analyzeByAgeGroup();

    cout << "\n=== CITY A ===\n"; computeA.analyseEmissions();
    cout << "\n=== CITY B ===\n"; computeB.analyseEmissions();
    cout << "\n=== CITY C ===\n"; computeC.analyseEmissions();

    //Comp emissions across cities
    compareCitiesEmission(computeA, computeB, computeC);

    //Creating objects for each city dataset.
    SortSearch cityA(&listA, &computeA);
    SortSearch cityB(&listB, &computeB);
    SortSearch cityC(&listC, &computeC);

    //Sorting
    cityA.bubbleSort("age", "asc");             //Sorting by age in an assending order
    listA.traversePrint();
    cityB.bubbleSort("distance", "desc");       //Sorting by distance in a descending order
    listB.traversePrint();
    cityC.bubbleSort("emission", "asc");        //Sorting by emissions in an assending order
    listC.traversePrint();

    //Performing linear search on dataset based on criterias
    cityA.linearSearch("transport", "Car");
    cityB.linearSearch("age_group", "18-25");
    cityC.linearSearch("distance_above", "15");
    cityA.linearSearch("distance_below", "10");

    //Performing binary search on databset based on criterias
    cityA.binarySearch("transport", "Bicycle");
    cityB.binarySearch("age_group", "18-25");
    cityC.binarySearch("distance_above", "15");
    cityC.binarySearch("distance_below", "10");

    return 0;
}