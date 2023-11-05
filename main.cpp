#include <iostream>
#include <csignal>
#include <cstdlib>
using namespace std;


bool running = true; // A flag to control the loop
int list_functions();

void sigintHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << "Received SIGINT (Ctrl+C). Exiting the program." << std::endl;
        running = false; 
        exit(0);
    }
}

int list_functions()
{
    while(running)
    {
        cout << "1. " << endl;
        cout << "2. " << endl;
        cout << "3. " << endl;
        cout << "4. " << endl;
        cout << "5. Exit" << endl;
        int select;
        cin >> select;
        
        switch (select)
        {
        case 1:
            cout << "1" << endl;
            break;
        case 2:
            cout << "2" << endl;
            break;
        case 3:
            cout << "3" << endl;
            break;
        case 4:
            cout << "4" << endl;
            break;
        case 5:
            cout << "5" << endl;
            return 0;
            // break;
        default:
            cout << "invalid selction" << endl;
            break;
        }
    }
    return 1;
}

int main()
{
    std::signal(SIGINT, sigintHandler);

    list_functions();
}
