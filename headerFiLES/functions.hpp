#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <iostream>
#include <string>
#include <cstdlib> // For system("clear") or system("CLS")
#include "C:\Users\sheen\OneDrive - Great Valley School District\Junior Year\path\globals\globals.hpp"


// Function to clear the screen
void clearScreen() {
    #if defined(_WIN32)
        system("CLS"); // For Windows
    #else
        system("clear"); // For Linux/Unix/MacOS
    #endif
}

// Function to display the screen
void displayScreen(const std::string &title) {
    clearScreen();
    std::cout << "=============================\n";
    std::cout << "       " << title << "       \n";
    std::cout << "=============================\n";
    std::cout << "1. Option One: Start the collinear Calc\n";
    std::cout << "2. Option Two: Start some random calc idk yet\n";
    std::cout << "3. Exit\n";
    std::cout << "=============================\n";
    std::cout << "Select an option: ";
}

double degreesToRadians(double degrees) {
    return degrees * M_PI / 180.0;
}


void collinearCalc(){
    clearScreen();
    double x;
    double y;
    double theta;
    double distance;
    double newX;
    double newY;
    double thetaRadians;
    std::cout << "Please Enter Current X \n";
    std::cin >> x;
    std::cout << "Please Enter Current Y \n";
    std::cin >> y;
    std::cout << "Please Enter Current Theta \n";
    std::cin >> theta;
    thetaRadians = degreesToRadians(theta);
    std::cout << "How far travel? (Positive is straight, negative is backwards)";
    std::cin >> distance;

    newX = x + distance  * cos(thetaRadians);
    newY = y + distance  * sin(thetaRadians);
    std::cout << "=============================\n";
    std::cout << "New Points \n";
    std::cout << "=============================\n";
    std::cout << "NEWX: " << newX << "\n";
    std::cout << "NEWY: " << newY << "\n";
    std::cout << "=============================\n";

    
}
void curveCalc(){
    double a;
}