#include <iostream>
using namespace std;
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
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

            //convert the data types
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
        };
 
int main(){

    cout<<"============================================================================================\n";
    cout<<"-------The following is an Implementation of linked lists for DSTR PART 1-------\n";
    cout<<"============================================================================================\n";
    
    ResidentList listA, listB, listC; 
    FileManager fmcityA, fmcityB, fmcityC;  //both are on stack
    fmcityA.SetResidentList(&listA); 
    fmcityB.SetResidentList(&listB);
    fmcityC.SetResidentList(&listC);
    
    fmcityA.loadFromCSV("../../data/dataset1-cityA.csv");
    fmcityB.loadFromCSV("../../data/dataset2-cityB.csv");
    fmcityC.loadFromCSV("../../data/dataset3-cityC.csv"); 
    
    cout<<"============================================================================================\n";
    cout<<"City A data\n";
    cout<<"============================================================================================\n";
    listA.traversePrint();

    cout<<"============================================================================================\n";
    cout<<"City B data\n";
    cout<<"============================================================================================\n";
    listB.traversePrint();

    cout<<"============================================================================================\n";
    cout<<"City C data\n";
    cout<<"============================================================================================\n";
    listC.traversePrint();
    
    computation computeA(&fmcityA);
    computation computeB(&fmcityB);
    computation computeC(&fmcityC);

    cout<<"Total emission for City A: "<<computeA.computeTotalEmission()<<endl;;
    cout<<"Total emission City B: "<<computeB.computeTotalEmission()<<endl;;
    cout<<"Total emission City C: "<<computeC.computeTotalEmission()<<endl;;

    return 0;
}