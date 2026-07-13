//HOTEL MANAGEMENT SYSTEM

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

#define GST 0.18

class Food 
{
protected:
    double price;
public:
    virtual void setPrice() = 0;
    virtual string getName() = 0;
    double getPrice()
     {
         return price; 
     }
};

class Breakfast : public Food 
{
public:
    void setPrice() 
    {
         price = 200; 
    }
    
    string getName() 
    {
        return "Breakfast";
    }
};

class Lunch : public Food 
{
public:
    void setPrice() 
    { 
        price = 400;
    }
    string getName()
     {
         return "Lunch";
     }
};

class Dinner : public Food 
{
public:
    void setPrice() 
    { 
        price = 500;
    }
    string getName() 
    { 
        return "Dinner"; 
    }
};

class ServiceItem 
{
    string name;
    double price;
    int count;

public:
    ServiceItem(string n, double p, int c)
    {
        name=n;
        price=p;
        count=c;
    }

    double getTotal() const 
    {
         return price * count; 
    }

    void display() const 
    {
        cout << left << setw(25) << name << " x" << setw(3) << count << " Rs." << getTotal() << endl;
    }
};

class Room 
{
protected:
    int roomNumber;
    string customerName;
    int days;
    bool occupied;

public:
    Room() : occupied(false) {}

    virtual float getRate() const = 0;
    virtual string getType() const = 0;

    bool isOccupied() const 
    { 
        return occupied;
    }
    string getCustomer() const 
    { 
        return customerName; 
    }

    void checkIn(int num, string name, int d) 
    {
        roomNumber = num;
        customerName = name;
        days = d;
        occupied = true;
    }

    double getBill() const 
    {
        return days * getRate();
    }

    void checkOut() 
    {
        occupied = false;
    }
};

class Deluxe : public Room 
{
public:
    float getRate() const 
    { 
        return 1000; 
    }
    string getType() const 
    { 
        return "Deluxe"; 
    }
};

class SuperDeluxe : public Room 
{
public:
    float getRate() const 
    {
         return 2000; 
    }
    string getType() const 
    { 
        return "Super Deluxe"; 
    }
};

class Suite : public Room 
{
public:
    float getRate() const 
    { 
        return 3000; 
    }
    string getType() const 
    { 
        return "Suite"; 
    }
};

class Hotel 
{
    Room* rooms[20];

    struct GuestData 
    {
        string name;
        vector<ServiceItem> services;
        vector<int> roomNumbers;
        double foodBill = 0;
    };

    vector<GuestData> guests;

public:
    Hotel() 
    {
        for (int i = 0; i < 20; i++) {
            if (i < 7) rooms[i] = new Deluxe();
            else if (i < 14) rooms[i] = new SuperDeluxe();
            else rooms[i] = new Suite();
        }
    }

    ~Hotel() 
    {
        for (int i = 0; i < 20; i++)
            delete rooms[i];
    }

    GuestData* findGuest(string name) 
    {
        for (auto &g : guests)
            if (g.name == name) return &g;
        return nullptr;
    }

    void checkIn() 
    {
        string name;
        int days, people;

        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);

        cout << "Days: "; cin >> days;
        cout << "People: "; cin >> people;

        cout << CYAN << "\n----- ROOM TYPES -----\n" << RESET;
        cout << "1. Deluxe        (Rs.1000/day, Max 3 people)\n";
        cout << "2. Super Deluxe  (Rs.2000/day, Max 3 people)\n";
        cout << "3. Suite         (Rs.3000/day, Max 5 people)\n";

        int Deluxe, SuperDeluxe, Suite;
        cout << "\nNumber of Deluxe rooms: "; cin >> Deluxe;
        cout << "Number of Super Deluxe rooms: "; cin >> SuperDeluxe;
        cout << "Number of Suite rooms: "; cin >> Suite;

        int capacity = Deluxe*3 + SuperDeluxe*3 + Suite*5;

        if (capacity == 0) {
            cout << RED << "No rooms selected!\n" << RESET;
            return;
        }

        if (people > capacity) {
            cout << RED << "Rooms not sufficient!\n" << RESET;
            cout << "Max Capacity = " << capacity << endl;
            return;
        }

        GuestData g;
        g.name = name;

int assignedDeluxe = 0;
int assignedSuperDeluxe = 0;
int assignedSuite = 0;


        for (int i=0;i<20;i++) 
        {
            if (!rooms[i]->isOccupied())
             {

                if (rooms[i]->getType()=="Deluxe" && assignedDeluxe<Deluxe) {
                    rooms[i]->checkIn(i+1,name,days);
                    g.roomNumbers.push_back(i+1);
                    cout << CYAN << "Room " << i+1 << " (Deluxe) booked\n" << RESET;
                    assignedDeluxe++;
                }
                else if (rooms[i]->getType()=="Super Deluxe" && assignedSuperDeluxe<SuperDeluxe) {
                    rooms[i]->checkIn(i+1,name,days);
                    g.roomNumbers.push_back(i+1);
                    cout << CYAN << "Room " << i+1 << " (Super Deluxe) booked\n" << RESET;
                    assignedSuperDeluxe++;
                }
                else if (rooms[i]->getType()=="Suite" && assignedSuite<Suite) {
                    rooms[i]->checkIn(i+1,name,days);
                    g.roomNumbers.push_back(i+1);
                    cout << CYAN << "Room " << i+1 << " (Suite) booked\n" << RESET;
                    assignedSuite++;
                }
            }
        }

        guests.push_back(g);
        cout << GREEN << "Check-in successful!\n" << RESET;
    }

    void orderFood() 
    {
        string name;
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);

        GuestData* g = findGuest(name);
        if (!g) { cout << RED << "Guest not found!\n" << RESET; return; }

        int ch, qty;

        do {
            cout << CYAN << "\n----- RESTAURANT MENU -----\n" << RESET;
            cout << "1. Breakfast  - Rs.200\n";
            cout << "2. Lunch      - Rs.400\n";
            cout << "3. Dinner     - Rs.500\n";
            cout << "0. Exit\n";

            cin >> ch;
            if (ch==0) break;

            Food* f;
            if (ch==1) f=new Breakfast();
            else if (ch==2) f=new Lunch();
            else f=new Dinner();

            f->setPrice();

            cout << "Quantity: ";
            cin >> qty;

            g->foodBill += f->getPrice()*qty;
            delete f;

        } while(true);
    }

    void addService() 
    {
        string name;
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);

        GuestData* g = findGuest(name);
        if (!g) { cout << RED << "Guest not found!\n" << RESET; return; }

        int ch;

        do {
            cout << CYAN << "\n----- SERVICES MENU -----\n" << RESET;
            cout << "1. Room Cleaning   - Rs.100\n";
            cout << "2. Spa             - Rs.800\n";
            cout << "3. Laundry         - Rs.150\n";
            cout << "4. Transport       - Rs.500\n";
            cout << "0. Exit\n";

            cin >> ch;
            if (ch==0) break;

            int count;
            cout << "Enter count: ";
            cin >> count;

            if (ch==1) g->services.push_back(ServiceItem("Cleaning",100,count));
            else if (ch==2) g->services.push_back(ServiceItem("Spa",800,count));
            else if (ch==3) g->services.push_back(ServiceItem("Laundry",150,count));
            else if (ch==4) g->services.push_back(ServiceItem("Transport",500,count));

        } while(true);
    }

    void checkOut() 
    {
        string name;
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);

        GuestData* g = findGuest(name);
        if (!g) { cout << RED << "Guest not found!\n" << RESET; return; }

        double roomBill=0, serviceBill=0;

        for (int i=0;i<20;i++) {
            if (rooms[i]->isOccupied() && rooms[i]->getCustomer()==name) {
                roomBill += rooms[i]->getBill();
                rooms[i]->checkOut();
            }
        }

        for (auto &s : g->services)
            serviceBill += s.getTotal();

        double subtotal = roomBill + g->foodBill + serviceBill;
        double gst = subtotal * GST;

        cout << GREEN << BOLD << "\n=========== FINAL BILL ===========\n" << RESET;
        cout << CYAN;
        cout << "+--------------------------------+--------------+\n";
        cout << "| Category                       | Amount (Rs.) |\n";
        cout << "+--------------------------------+--------------+\n";
        cout << "| Room Charges                  | " << setw(10) << roomBill << " |\n";
        cout << "| Food Charges                  | " << setw(10) << g->foodBill << " |\n";
        cout << "| Service Charges               | " << setw(10) << serviceBill << " |\n";
        cout << "| GST (18%)                     | " << setw(10) << gst << " |\n";
        cout << "+--------------------------------+--------------+\n";
        cout << "| TOTAL                         | " << setw(10) << subtotal + gst << " |\n";
        cout << "+--------------------------------+--------------+\n";
        cout << RESET;

        ofstream file("invoice.txt", ios::app);
        file << name << " Total: " << subtotal + gst << endl;
    }
};

int main() 
{
    Hotel h;
    int choice;

    do {
        cout << CYAN << BOLD << "\n===== HOTEL MANAGEMENT SYSTEM =====\n" << RESET;
        cout << YELLOW;
        cout << "1.Check In\n2.Restaurant\n3.Services\n4.Check Out\n5.Exit\n";
        cout << RESET;

        cout<<"Enter Your Choice: "<<endl;
        cin >> choice;

        switch(choice) {
            case 1: h.checkIn(); break;
            case 2: h.orderFood(); break;
            case 3: h.addService(); break;
            case 4: h.checkOut(); break;
        }

    } while(choice != 5);

    cout << GREEN << "Thank you for visiting!\n" << RESET;
    return 0;
}