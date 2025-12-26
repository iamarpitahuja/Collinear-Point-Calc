#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <iostream>
#include <string>
#include <cstdlib> // For system("clear") or system("CLS")
#include <limits>  // For numeric limits
#include "../globals/globals.hpp"

// ============================================
// Point Structure for coordinate representation
// ============================================
struct Point {
    double x;
    double y;
};

// ============================================
// Constants for numerical stability
// ============================================
const double EPSILON = 1e-9;           // Small value for floating-point comparisons
const double MAX_DLEAD = 1e6;          // Maximum reasonable lookahead distance
const double MIN_DLEAD = 1e-6;         // Minimum lookahead to avoid division issues

// ============================================
// Boomerang Curve Parameters
// ============================================
// The boomerang curve is modeled as a circular arc that curves back
// toward the starting heading. The curvature radius determines how
// tight the curve is.
const double DEFAULT_CURVATURE_RADIUS = 1.0;  // Default radius of curvature

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
    std::cout << "2. Option Two: Boomerang Curve Calculator\n";
    std::cout << "3. Exit\n";
    std::cout << "=============================\n";
    std::cout << "Select an option: ";
}

double degreesToRadians(double degrees) {
    return degrees * M_PI / 180.0;
}

// ============================================
// Boomerang Curve Colinear Point Calculator
// ============================================
/**
 * @brief Calculates the colinear point on a boomerang curve trajectory
 * 
 * The boomerang curve is a smooth circular arc that:
 * - Starts at position (x, y) with heading theta
 * - Curves in a circular arc parameterized by dlead
 * - Returns a point along this curved path
 * 
 * Geometry Explanation:
 * ---------------------
 * The boomerang is modeled as motion along a circular arc. Given:
 * - Current position: (x, y)
 * - Current heading: theta (radians, 0 = +X axis, counterclockwise positive)
 * - Lookahead distance: dlead (arc length along the curve)
 * - Curvature radius: R (determines how tight the curve is)
 * 
 * The arc angle swept is: phi = dlead / R
 * 
 * In the robot's local frame (heading aligned with +X):
 * - The curve center is perpendicular to heading at distance R
 * - Points on the arc are computed using circular geometry
 * 
 * Coordinate Frame Transformation:
 * - Local frame: Robot at origin, heading along +X
 * - World frame: Actual position and heading
 * - Transform: Rotate by theta, then translate by (x, y)
 * 
 * @param x       Current x position in world frame
 * @param y       Current y position in world frame
 * @param theta   Current heading in radians (0 = +X, counterclockwise positive)
 * @param dlead   Lookahead distance along the boomerang curve (arc length)
 * @param radius  Curvature radius of the boomerang (optional, default = 1.0)
 * @return Point  Target (x, y) coordinates on the boomerang curve
 */
Point calculateColinearPoint(
    double x,
    double y,
    double theta,
    double dlead,
    double radius = DEFAULT_CURVATURE_RADIUS
) {
    Point result;
    
    // ========================================
    // Input Validation and Bounds Checking
    // ========================================
    
    // Handle edge case: dlead approaches zero
    // Return current position (no movement along curve)
    if (std::abs(dlead) < MIN_DLEAD) {
        result.x = x;
        result.y = y;
        return result;
    }
    
    // Clamp dlead to reasonable bounds for numerical stability
    if (dlead > MAX_DLEAD) {
        dlead = MAX_DLEAD;
    } else if (dlead < -MAX_DLEAD) {
        dlead = -MAX_DLEAD;
    }
    
    // Ensure radius is positive and non-zero
    if (std::abs(radius) < EPSILON) {
        radius = DEFAULT_CURVATURE_RADIUS;
    }
    radius = std::abs(radius);  // Radius must be positive
    
    // ========================================
    // Boomerang Curve Geometry Calculation
    // ========================================
    
    // Calculate the arc angle (phi) swept along the curve
    // Arc length = radius * angle, so angle = arc_length / radius
    double phi = dlead / radius;
    
    // ========================================
    // Local Frame Calculation (Robot Frame)
    // ========================================
    // In local frame:
    // - Robot is at origin (0, 0)
    // - Robot heading is along +X axis
    // - Curve center is at (0, R) for left turn, (0, -R) for right turn
    // 
    // For a boomerang that curves left (positive curvature):
    // Center of rotation: (0, R)
    // Point on arc after angle phi:
    //   local_x = R * sin(phi)
    //   local_y = R * (1 - cos(phi))
    //
    // This creates a smooth arc starting tangent to +X axis
    
    // Determine curve direction based on dlead sign
    // Positive dlead: forward along curve
    // The boomerang curves to the left by default
    double localX = radius * sin(phi);
    double localY = radius * (1.0 - cos(phi));
    
    // ========================================
    // World Frame Transformation
    // ========================================
    // Transform from robot local frame to world frame:
    // 1. Rotate by theta (current heading)
    // 2. Translate by (x, y) (current position)
    //
    // Rotation matrix for angle theta:
    // | cos(theta)  -sin(theta) |
    // | sin(theta)   cos(theta) |
    //
    // world_x = x + local_x * cos(theta) - local_y * sin(theta)
    // world_y = y + local_x * sin(theta) + local_y * cos(theta)
    
    double cosTheta = cos(theta);
    double sinTheta = sin(theta);
    
    // Apply rotation and translation
    result.x = x + localX * cosTheta - localY * sinTheta;
    result.y = y + localX * sinTheta + localY * cosTheta;
    
    // ========================================
    // Numerical Precision Cleanup
    // ========================================
    // Clean up very small values that should be zero
    // This prevents floating-point noise in output
    if (std::abs(result.x) < EPSILON) {
        result.x = 0.0;
    }
    if (std::abs(result.y) < EPSILON) {
        result.y = 0.0;
    }
    
    return result;
}

/**
 * @brief Overloaded version with curvature specification
 * 
 * This version allows specifying the curvature (1/radius) directly,
 * which is often more intuitive for motion planning.
 * 
 * @param x          Current x position
 * @param y          Current y position
 * @param theta      Current heading (radians)
 * @param dlead      Lookahead distance
 * @param curvature  Curvature of the path (1/radius). Positive = left turn.
 * @return Point     Target coordinates on boomerang curve
 */
Point calculateColinearPointWithCurvature(
    double x,
    double y,
    double theta,
    double dlead,
    double curvature
) {
    // Convert curvature to radius
    // Curvature = 1/radius, so radius = 1/curvature
    // Handle zero curvature (straight line) case
    if (std::abs(curvature) < EPSILON) {
        // Straight line: no curve, just move forward
        Point result;
        result.x = x + dlead * cos(theta);
        result.y = y + dlead * sin(theta);
        return result;
    }
    
    double radius = 1.0 / std::abs(curvature);
    
    // If curvature is negative, flip the dlead to curve right instead of left
    if (curvature < 0) {
        dlead = -dlead;
    }
    
    return calculateColinearPoint(x, y, theta, dlead, radius);
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

// ============================================
// Boomerang Curve Calculator - User Interface
// ============================================
/**
 * @brief Interactive calculator for boomerang curve colinear points
 * 
 * This function provides a user-friendly interface to:
 * 1. Input current robot state (x, y, theta)
 * 2. Specify lookahead distance (dlead)
 * 3. Optionally specify curvature radius
 * 4. Display the calculated target point on the boomerang curve
 */
void curveCalc(){
    clearScreen();
    
    // ========================================
    // Variable Declarations
    // ========================================
    double x;              // Current X position
    double y;              // Current Y position
    double theta;          // Current heading (degrees, user input)
    double thetaRadians;   // Current heading (radians, internal)
    double dlead;          // Lookahead distance along curve
    double radius;         // Curvature radius
    int useCustomRadius;   // Flag for custom radius input
    Point targetPoint;     // Result from calculation
    
    // ========================================
    // Display Header
    // ========================================
    std::cout << "========================================\n";
    std::cout << "   BOOMERANG CURVE COLINEAR CALCULATOR  \n";
    std::cout << "========================================\n\n";
    
    std::cout << "This calculates a target point along a\n";
    std::cout << "boomerang (circular arc) trajectory.\n\n";
    
    // ========================================
    // User Input: Current State
    // ========================================
    std::cout << "--- Current Robot State ---\n";
    
    std::cout << "Enter Current X position: ";
    std::cin >> x;
    
    std::cout << "Enter Current Y position: ";
    std::cin >> y;
    
    std::cout << "Enter Current Theta (degrees): ";
    std::cin >> theta;
    
    // Convert theta from degrees to radians
    thetaRadians = degreesToRadians(theta);
    
    // ========================================
    // User Input: Curve Parameters
    // ========================================
    std::cout << "\n--- Boomerang Curve Parameters ---\n";
    
    std::cout << "Enter Lookahead Distance (dlead):\n";
    std::cout << "  (Positive = forward curve, Negative = backward)\n";
    std::cout << "  dlead: ";
    std::cin >> dlead;
    
    // Ask about custom curvature radius
    std::cout << "\nUse custom curvature radius? (1=Yes, 0=No): ";
    std::cin >> useCustomRadius;
    
    if (useCustomRadius == 1) {
        std::cout << "Enter Curvature Radius (larger = gentler curve):\n";
        std::cout << "  radius: ";
        std::cin >> radius;
        
        // Validate radius input
        if (radius <= 0) {
            std::cout << "\nWarning: Radius must be positive. Using default (1.0).\n";
            radius = DEFAULT_CURVATURE_RADIUS;
        }
    } else {
        radius = DEFAULT_CURVATURE_RADIUS;
    }
    
    // ========================================
    // Calculate Colinear Point
    // ========================================
    targetPoint = calculateColinearPoint(x, y, thetaRadians, dlead, radius);
    
    // ========================================
    // Display Results
    // ========================================
    std::cout << "\n========================================\n";
    std::cout << "         CALCULATION RESULTS            \n";
    std::cout << "========================================\n";
    
    std::cout << "\n--- Input Summary ---\n";
    std::cout << "  Start Position: (" << x << ", " << y << ")\n";
    std::cout << "  Heading: " << theta << " degrees (" << thetaRadians << " rad)\n";
    std::cout << "  Lookahead Distance: " << dlead << "\n";
    std::cout << "  Curvature Radius: " << radius << "\n";
    
    std::cout << "\n--- Target Colinear Point ---\n";
    std::cout << "  Target X: " << targetPoint.x << "\n";
    std::cout << "  Target Y: " << targetPoint.y << "\n";
    
    // ========================================
    // Additional Geometric Information
    // ========================================
    // Calculate arc angle for reference
    double arcAngle = dlead / radius;
    double arcAngleDegrees = arcAngle * 180.0 / M_PI;
    
    // Calculate straight-line distance from start to target
    double dx = targetPoint.x - x;
    double dy = targetPoint.y - y;
    double chordLength = sqrt(dx * dx + dy * dy);
    
    // Calculate bearing to target point
    double bearingToTarget = atan2(dy, dx);
    double bearingDegrees = bearingToTarget * 180.0 / M_PI;
    
    std::cout << "\n--- Geometry Details ---\n";
    std::cout << "  Arc Angle Swept: " << arcAngleDegrees << " degrees\n";
    std::cout << "  Chord Length: " << chordLength << "\n";
    std::cout << "  Bearing to Target: " << bearingDegrees << " degrees\n";
    
    std::cout << "\n========================================\n";
}