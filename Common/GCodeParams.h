/*
 * Parameters for marlin flavor gcode
 */

struct POINT {
  double x = 0.0;
  double y = 0.0;
  double z = 0.0;
};

struct GCODE_PARAMS {
  static double MM_PER_ARC_SEGMENT = 1.0;
  static bool ARC_P_CIRCLES = false;
  static bool FWRETRACT = false;
  static int NOZZLE_CLEAN_STROKES = 0;
  static double NOZZLE_CLEAN_START_POINT = POINT();
  static double NOZZLE_CLEAN_END_POINT = POINT();
  static bool NOZZLE_CLEAN_TRIANGLES = false;
  static bool NOZZLE_CLEAN_CIRCLE_MIDDLE = false;
  static double NOZZLE_CLEAN_CIRCLE_RADIUS = 0.0;
  static bool NOZZLE_CLEAN_GOBACK = false;
  static bool CNC_WORKSPACE_PLANES = false;
  static bool NOZZLE_PARK_FEATURE = false;
  static double NOZZLE_PARK_POINT = POINT();
  static double X_HOME_BUMP_MM = 0.0;
  static double Y_HOME_BUMP_MM = 0.0;
  static double Z_HOME_BUMP_MM = 0.0;
  static int HOMING_BUMP_DIVISOR = 0.0;
  static bool RESTORE_LEVELING_AFTER_G28 = false;
  static bool Z_SAFE_HOMING = false;
  static bool QUICK_HOME = false;
  static bool HOME_Y_BEFORE_X = false;
}
