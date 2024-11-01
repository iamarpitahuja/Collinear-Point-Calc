#pragma once
#include <iostream>
#include <string>

//displayscreen clearer
extern void clearScreen();

//display screen global
extern void displayScreen(const std::string &title);

extern void collinearCalc();

extern void curveCalc();

extern double degreesToRadians(double degrees);