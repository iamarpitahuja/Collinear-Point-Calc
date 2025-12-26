#pragma once
#include <iostream>
#include <string>

// ============================================
// Point Structure Declaration
// ============================================
struct Point;

// ============================================
// Screen Functions
// ============================================
//displayscreen clearer
extern void clearScreen();

//display screen global
extern void displayScreen(const std::string &title);

// ============================================
// Calculator Functions
// ============================================
extern void collinearCalc();

extern void curveCalc();

// ============================================
// Utility Functions
// ============================================
extern double degreesToRadians(double degrees);

// ============================================
// Boomerang Curve Functions
// ============================================
/**
 * @brief Calculate colinear point on boomerang curve
 * @param x       Current x position
 * @param y       Current y position
 * @param theta   Current heading (radians)
 * @param dlead   Lookahead distance along curve
 * @param radius  Curvature radius (default = 1.0)
 * @return Point  Target coordinates on boomerang curve
 */
extern Point calculateColinearPoint(
    double x,
    double y,
    double theta,
    double dlead,
    double radius
);

/**
 * @brief Calculate colinear point using curvature instead of radius
 * @param x          Current x position
 * @param y          Current y position
 * @param theta      Current heading (radians)
 * @param dlead      Lookahead distance
 * @param curvature  Path curvature (1/radius)
 * @return Point     Target coordinates on boomerang curve
 */
extern Point calculateColinearPointWithCurvature(
    double x,
    double y,
    double theta,
    double dlead,
    double curvature
);