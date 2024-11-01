#include <iostream>
#include <string>
#include <cstdlib> 
#include "C:\Users\sheen\OneDrive - Great Valley School District\Junior Year\path\headerFiLES\functions.hpp"
int main(){

    int choice;
    std::string title = "Main Screen";
     do {
        displayScreen(title);
        std::cin >> choice;
        switch (choice) {
            case 1:
                collinearCalc();
                break;
            case 2:
                curveCalc();
                break;
            case 3:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }

        if (choice != 5) {
            std::cout << "Press Enter to return to the main menu...";
            std::cin.ignore();
            std::cin.get();
        }

    } while (choice != 3);


    return 0;
}