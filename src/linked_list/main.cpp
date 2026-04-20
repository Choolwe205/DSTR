#include <iostream>
using namespace std;

class resident{
    public:
    string residentID;
    int age;
    string modeOfTransport;
    int dailyDistance;
    double carbonEmissions;
    int averageDayPerMonth;
    resident* next;

    resident(){};

    resident(string residentID,int age,string modeOfTransport, int dailyDistance, double carbonEmissions,int averageDayPerMonth,resident* next){
    this->residentID= residentID;
    this->age= age;
    this->modeOfTransport= modeOfTransport;
    this->dailyDistance= dailyDistance;
    this->carbonEmissions= carbonEmissions;
    this->averageDayPerMonth= averageDayPerMonth;
    this->next=next;
    }
    void traversePrint(resident*head){
    resident* current=this;
    while (current!=nullptr)
    {
        cout<<current->residentID<<", ";
        cout<<current->age<<", ";
        cout<<current->modeOfTransport<<", ";
        cout<<current->dailyDistance<<", ";
        cout<<current->carbonEmissions<<", ";
        cout<<current->averageDayPerMonth<<endl;
        current=current->next;
    }
    cout<<"End";
    };
    int getSize(resident*head)
    {
        int count=0;
        resident *current=head;
        while( current !=nullptr)
        {
            count++;
            current=current->next;
        }
        return count;
    };
    void insertAtStart(string ID,int resage,string resmodeOfTransport, int resdailyDistance, double rescarbonEmissions,int resaverageDayPerMonth,resident* &head){
        resident*newResident=new resident{ID,resage,resmodeOfTransport,resdailyDistance,rescarbonEmissions,resaverageDayPerMonth,nullptr};
        newResident->next=head;
        head=newResident;
    };

    void insertAtEnd(string ID,int resage,string resmodeOfTransport, int resdailyDistance, double rescarbonEmissions,int resaverageDayPerMonth,resident* &head)
    {
        resident*newResident=new resident{ID,resage,resmodeOfTransport,resdailyDistance,rescarbonEmissions,resaverageDayPerMonth,nullptr};
        if (head==nullptr){
        head=newResident;
        return;
    };
        resident*currentResident=head;
        while(currentResident->next!=nullptr)
        {
            currentResident=currentResident->next;
        };
        currentResident->next=newResident;
    };
    void insertAtindex(string ID,int resage,string resmodeOfTransport, int resdailyDistance, double rescarbonEmissions,int resaverageDayPerMonth,resident* &head, int index)
    {

        if (index<0)
        {
            cout<<"Index cannot be less than 0"<<endl;
            return;
        }

        //IF INDEX IS 0
        if (index==0){
            resident* newResident =new resident{ID,resage,resmodeOfTransport,resdailyDistance,rescarbonEmissions,resaverageDayPerMonth,nullptr};
            newResident->next=head;
            head=newResident;
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
        resident* newResident =new resident{ID,resage,resmodeOfTransport,resdailyDistance,rescarbonEmissions,resaverageDayPerMonth,nullptr};
        newResident->next=current->next;
        current->next=newResident;
    }
    void clear (resident* &head)
    {
        resident* current=head;
        while (current!=nullptr){
            resident* temp =current->next;
            delete head;
            current=temp;
        }
        head=nullptr;
    }

    };  
int main(){

    cout<<"Linked Lists version for demo\n";
    resident *head=new resident("A001",34,"car",15,0.21,25,nullptr);
    resident *second=new resident("A002",42,"bus",12,0.05,26,nullptr);
    resident *third=new resident("A003",28,"bicycle",8,0,26,nullptr);
    head->next=second;
    second->next=third;
    // head->traversePrint();
    cout<< "The size is: "<<head->getSize(head)<<endl;
    head->insertAtStart("A004",26,"car",8,0,26,head);
    head->insertAtindex("A006",26,"car",8,0,26,head,2);
    head->traversePrint(head);
    head->clear();

    return 0;
    
}