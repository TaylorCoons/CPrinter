#ifndef AxisDefines_h
#define AxisDefines_h

// Step Angles
#define X_AXIS_STEP_ANGLE 1.80 / 360.0 * 2.0 * PI // rad
#define Y_AXIS_STEP_ANGLE 1.80 / 360.0 * 2.0 * PI // rad
#define Z_AXIS_STEP_ANGLE 1.80 / 360.0 * 2.0 * PI // rad

// Timing gear radii
#define X_AXIS_RADIUS 16.0 // mm
#define Y_AXIS_RADIUS 16.0 // mm

// Steps per mm (s=r*theta)
#define X_AXIS_STEPS_PER_MM 1.0 / (X_AXIS_STEP_ANGLE * X_AXIS_RADIUS)
#define Y_AXIS_STEPS_PER_MM 1.0 / (Y_AXIS_STEP_ANGLE * Y_AXIS_RADIUS)

#endif
