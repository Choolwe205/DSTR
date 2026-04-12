#include <iostream>
using namespace std;

struct resident{
    public:
    string residentID;
    int age;
    string modeOfTransport;
    int dailyDistance;
    double carbonEmissions;
    int averageDayPerMonth;
    resident* next;
    
};
void traversePrint(resident *head){
    resident* current=head;
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
    
}
int main(){

    cout<<"Linked Lists version for demo\n";
    resident *head=new resident{"A001",34,"car",15,0.21,25,nullptr};
    resident *second=new resident{"A002",42,"bus",12,0.05,26,nullptr};
    resident *third=new resident{"A003",28,"bicycle",8,0,26,nullptr};
    head->next=second;
    second->next=third;
    traversePrint(head);

    return 0;
    
}