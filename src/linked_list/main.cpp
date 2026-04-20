#include <iostream>
using namespace std;
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
class resident{
    public:
    string residentID;
    int age;
    string modeOfTransport;
    int dailyDistance;
    double carbonEmissionFactor;
    int averageDayPerMonth;
    resident* next;

    resident(){};

    resident(string residentID,int age,string modeOfTransport, int dailyDistance, double carbonEmissionFactor,int averageDayPerMonth,resident* next){
    this->residentID= residentID;
    this->age= age;
    this->modeOfTransport= modeOfTransport;
    this->dailyDistance= dailyDistance;
    this->carbonEmissionFactor= carbonEmissionFactor;
    this->averageDayPerMonth= averageDayPerMonth;
    this->next=next;
    }
    };  

    //Logic for the list class
    class ResidentList {
        private:
        resident * head= nullptr;
        int size=0;
 
    public:
        //Overloaded constructors 
        ResidentList(){};

    void traversePrint(){
        resident* current=head;

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
        cout<<"--End of List--";
        };

        int getSize()
        {
           return size;
        };
        void insertAtStart(string ID,int resage,string resmodeOfTransport, int resdailyDistance, double rescarbonEmissionFactor,int resaverageDayPerMonth){
            resident*newResident=new resident{ID,resage,resmodeOfTransport,resdailyDistance,rescarbonEmissionFactor,resaverageDayPerMonth,nullptr};
            newResident->next=head;
            head=newResident;
            size++;
        };

        void insertAtEnd(string ID,int resage,string resmodeOfTransport, int resdailyDistance, double rescarbonEmissionFactor,int resaverageDayPerMonth)
        {
            resident*newResident=new resident{ID,resage,resmodeOfTransport,resdailyDistance,rescarbonEmissionFactor,resaverageDayPerMonth,nullptr};
            if (head==nullptr){
            head=newResident;
            size++;
            return;
        };
            resident*currentResident=head;
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
                resident* newResident =new resident{ID,resage,resmodeOfTransport,resdailyDistance,rescarbonEmissionFactor,resaverageDayPerMonth,nullptr};
                newResident->next=head;
                head=newResident;
                size++;
                return;
            }
            //IF INDEX IS WITHIN BOUNDS 
            resident* current =head;
            for (int i =0; i < index -1 && current!=nullptr;i++ )
            {
                current=current->next;
            }
            if (current==nullptr){
                cout<<"Index is out of bounds\n";
                return;
            }
            resident* newResident =new resident{ID,resage,resmodeOfTransport,resdailyDistance,rescarbonEmissionFactor,resaverageDayPerMonth,nullptr};
            newResident->next=current->next;
            current->next=newResident;
            size++;
        }
        void clear ()
        {
            while (head!=nullptr){
                resident* current=head;
                head=head->next;
                delete current;
            }
            size=0;
        }
        
        void loadFromCSV(string filename){
            ifstream file(filename);
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

            //convert the data types
            int age = stoi(ageStr);
            int distance = stoi(distanceStr);
            double factor = stod(factorStr);
            int days = stoi(daysStr);
            insertAtEnd(ID,age,transport,distance,factor, days);
        }
            file.close();
        }

        };

       
int main(){

    cout<<"Linked Lists version for demo\n";
    ResidentList resList; //stack 
 
    resList.loadFromCSV("../../data/dataset1-cityA.csv");
    resList.loadFromCSV("../../data/dataset2-cityB.csv");
    resList.loadFromCSV("../../data/dataset3-cityC.csv");
    resList.traversePrint();
    cout << filesystem::current_path() << endl;
    return 0;
}