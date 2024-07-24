/*==========================================================================*/
/*  DriverModel.cpp                                  DLL Module for VISSIM  */
/*                                                                          */
/*  Interface module for external driver models.                            */
/*  Dummy version that does nothing (uses Vissim's internal model).         */
/*                                                                          */
/*  Version of 2023-5-9                                   Chengyuan Ma      */
/*==========================================================================*/

#include "DriverModel.h"
#include <algorithm>
#include<cmath>

/*==========================================================================*/

double  desired_acceleration = 0.0;
double  desired_lane_angle   = 0.0;
long    active_lane_change   = 0;
long    rel_target_lane      = 0;
double  desired_velocity     = 0.0;
double timestep = 1;
long    turning_indicator    = 0;
long    vehicle_color        = RGB(225,0,0);

/*==========================================================================*/
/*Create Variables*/
long  vehicle_type;
double  vehicle_length=4.2;

double current_time;
long current_vehicle;
double  current_velocity;
double  current_acceleration;

double ahead_vehicle;
double  gross_distance;// = 0;
double  speed_difference;
double FrontVehAcc;

double car_following_acceleration;

static int vehicle_count = 0;
static int vehicle_to_stop = 0;
double jam_distance = 0.6;                 // minimum gross distance (m)
double speedlimit = 29.0576;
double freeflowspeed = 100/3.6;//m/s
/*==========================================================================*/

BOOL APIENTRY DllMain (HANDLE  hModule,
                       DWORD   ul_reason_for_call,
                       LPVOID  lpReserved)
{
  switch (ul_reason_for_call) {
      case DLL_PROCESS_ATTACH:
      case DLL_THREAD_ATTACH:
      case DLL_THREAD_DETACH:
      case DLL_PROCESS_DETACH:
         break;
  }
  return TRUE;
}

/*==========================================================================*/

DRIVERMODEL_API  int  DriverModelSetValue (long   type,
                                           long   index1,
                                           long   index2,
                                           long   long_value,
                                           double double_value,
                                           char   *string_value)
{
  /* Sets the value of a data object of type <type>, selected by <index1> */
  /* and possibly <index2>, to <long_value>, <double_value> or            */
  /* <*string_value> (object and value selection depending on <type>).    */
  /* Return value is 1 on success, otherwise 0.                           */

  switch (type) {
    case DRIVER_DATA_PATH                   :
    case DRIVER_DATA_TIMESTEP               :
    case DRIVER_DATA_TIME                   :
      return 1;
    case DRIVER_DATA_USE_UDA                :
      return 0; /* doesn't use any UDAs */
                /* must return 1 for desired values of index1 if UDA values are to be sent from/to Vissim */
    case DRIVER_DATA_VEH_ID                 :
        current_vehicle = long_value;
        return 1;
    case DRIVER_DATA_VEH_LANE               :
    case DRIVER_DATA_VEH_ODOMETER           :
    case DRIVER_DATA_VEH_LANE_ANGLE         :
    case DRIVER_DATA_VEH_LATERAL_POSITION   :
    case DRIVER_DATA_VEH_VELOCITY           :
        current_velocity = double_value;
        return 1;
    case DRIVER_DATA_VEH_ACCELERATION       :
        current_acceleration = double_value;
        return 1;
    case DRIVER_DATA_VEH_LENGTH             :
        vehicle_length = double_value;
        return 1;
    case DRIVER_DATA_VEH_WIDTH              :
    case DRIVER_DATA_VEH_WEIGHT             :
    case DRIVER_DATA_VEH_MAX_ACCELERATION   :
      return 1;
    case DRIVER_DATA_VEH_TURNING_INDICATOR  :
      turning_indicator = long_value;
      return 1;
    case DRIVER_DATA_VEH_CATEGORY           :
    case DRIVER_DATA_VEH_PREFERRED_REL_LANE :
    case DRIVER_DATA_VEH_USE_PREFERRED_LANE :
      return 1;
    case DRIVER_DATA_VEH_DESIRED_VELOCITY   :
      desired_velocity = double_value;
      return 1;
    case DRIVER_DATA_VEH_X_COORDINATE       :
    case DRIVER_DATA_VEH_Y_COORDINATE       :
    case DRIVER_DATA_VEH_Z_COORDINATE       :
    case DRIVER_DATA_VEH_REAR_X_COORDINATE  :
    case DRIVER_DATA_VEH_REAR_Y_COORDINATE  :
    case DRIVER_DATA_VEH_REAR_Z_COORDINATE  :
    case DRIVER_DATA_VEH_TYPE               :
        vehicle_type = long_value;
        return 1;
    case DRIVER_DATA_VEH_COLOR              :
      vehicle_color = long_value;
      return 1;
    case DRIVER_DATA_VEH_CURRENT_LINK       :
      return 0; /* (To avoid getting sent lots of DRIVER_DATA_VEH_NEXT_LINKS messages) */
                /* Must return 1 if these messages are to be sent from VISSIM!         */
    case DRIVER_DATA_VEH_NEXT_LINKS         :
    case DRIVER_DATA_VEH_ACTIVE_LANE_CHANGE :
    case DRIVER_DATA_VEH_REL_TARGET_LANE    :
    case DRIVER_DATA_VEH_INTAC_STATE        :
    case DRIVER_DATA_VEH_INTAC_TARGET_TYPE  :
    case DRIVER_DATA_VEH_INTAC_TARGET_ID    :
    case DRIVER_DATA_VEH_INTAC_HEADWAY      :
    case DRIVER_DATA_VEH_UDA                :
    case DRIVER_DATA_NVEH_ID                :
        if (index1 == 0 && index2 == 1) {
            ahead_vehicle = long_value;
        }
        return 1;
    case DRIVER_DATA_NVEH_LANE_ANGLE        :
    case DRIVER_DATA_NVEH_LATERAL_POSITION  :
    case DRIVER_DATA_NVEH_DISTANCE          :
        if (index1 == 0 && index2 == 1)
        {
            gross_distance = double_value;
        }
        return 1;
    case DRIVER_DATA_NVEH_REL_VELOCITY      :
        if (index1 == 0 && index2 == 1)
        {
            speed_difference = double_value;
        }
        return 1;
    case DRIVER_DATA_NVEH_ACCELERATION      :
        if (index1 == 0 && index2 == 1)
        {
            FrontVehAcc = double_value;
        }
        return 1;
    case DRIVER_DATA_NVEH_LENGTH            :
    case DRIVER_DATA_NVEH_WIDTH             :
    case DRIVER_DATA_NVEH_WEIGHT            :
    case DRIVER_DATA_NVEH_TURNING_INDICATOR :
    case DRIVER_DATA_NVEH_CATEGORY          :
    case DRIVER_DATA_NVEH_LANE_CHANGE       :
    case DRIVER_DATA_NVEH_TYPE              :
    case DRIVER_DATA_NVEH_UDA               :
    case DRIVER_DATA_NVEH_X_COORDINATE      :
    case DRIVER_DATA_NVEH_Y_COORDINATE      :
    case DRIVER_DATA_NVEH_Z_COORDINATE      :
    case DRIVER_DATA_NVEH_REAR_X_COORDINATE :
    case DRIVER_DATA_NVEH_REAR_Y_COORDINATE :
    case DRIVER_DATA_NVEH_REAR_Z_COORDINATE :
    case DRIVER_DATA_NO_OF_LANES            :
    case DRIVER_DATA_LANE_WIDTH             :
    case DRIVER_DATA_LANE_END_DISTANCE      :
    case DRIVER_DATA_CURRENT_LANE_POLY_N    :
    case DRIVER_DATA_CURRENT_LANE_POLY_X    :
    case DRIVER_DATA_CURRENT_LANE_POLY_Y    :
    case DRIVER_DATA_CURRENT_LANE_POLY_Z    :
    case DRIVER_DATA_RADIUS                 :
    case DRIVER_DATA_MIN_RADIUS             :
    case DRIVER_DATA_DIST_TO_MIN_RADIUS     :
    case DRIVER_DATA_SLOPE                  :
    case DRIVER_DATA_SLOPE_AHEAD            :
    case DRIVER_DATA_SIGNAL_DISTANCE        :
    case DRIVER_DATA_SIGNAL_STATE           :
    case DRIVER_DATA_SIGNAL_STATE_START     :
    case DRIVER_DATA_SPEED_LIMIT_DISTANCE   :
    case DRIVER_DATA_SPEED_LIMIT_VALUE      :
    case DRIVER_DATA_PRIO_RULE_DISTANCE     :
    case DRIVER_DATA_PRIO_RULE_STATE        :
    case DRIVER_DATA_ROUTE_SIGNAL_DISTANCE  :
    case DRIVER_DATA_ROUTE_SIGNAL_STATE     :
    case DRIVER_DATA_ROUTE_SIGNAL_CYCLE     :
      return 1;
    case DRIVER_DATA_ROUTE_SIGNAL_SWITCH    :
      return 0;  /* (to avoid getting sent lots of signal switch data) */
    case DRIVER_DATA_CONFL_AREAS_COUNT      :
      return 0;  /* (to avoid getting sent lots of conflict area data) */
    case DRIVER_DATA_CONFL_AREA_TYPE        :
    case DRIVER_DATA_CONFL_AREA_YIELD       :
    case DRIVER_DATA_CONFL_AREA_DISTANCE    :
    case DRIVER_DATA_CONFL_AREA_LENGTH      :
    case DRIVER_DATA_CONFL_AREA_VEHICLES    :
    case DRIVER_DATA_CONFL_AREA_TIME_ENTER  :
    case DRIVER_DATA_CONFL_AREA_TIME_IN     :
    case DRIVER_DATA_CONFL_AREA_TIME_EXIT   :
      return 1;
    case DRIVER_DATA_DESIRED_ACCELERATION   :
      desired_acceleration = double_value;
      return 1;
    case DRIVER_DATA_DESIRED_LANE_ANGLE     :
      desired_lane_angle = double_value;
      return 1;
    case DRIVER_DATA_ACTIVE_LANE_CHANGE     :
      active_lane_change = long_value;
      return 1;
    case DRIVER_DATA_REL_TARGET_LANE        :
      rel_target_lane = long_value;
      return 1;
    default :
      return 0;
  }
}

/*--------------------------------------------------------------------------*/

DRIVERMODEL_API  int  DriverModelSetValue3 (long   type,
                                            long   index1,
                                            long   index2,
                                            long   index3,
                                            long   long_value,
                                            double double_value,
                                            char   *string_value)
{
  /* Sets the value of a data object of type <type>, selected by <index1>, */
  /* <index2> and <index3>, to <long_value>, <double_value> or             */
  /* <*string_value> (object and value selection depending on <type>).     */
  /* Return value is 1 on success, otherwise 0.                            */
  /* DriverModelGetValue (DRIVER_DATA_MAX_NUM_INDICES, ...) needs to set   */
  /* *long_value to 3 or greater in order to activate this function!       */

  switch (type) {
    case DRIVER_DATA_ROUTE_SIGNAL_SWITCH:
      return 0; /* don't send any more switch values */
    default:
      return 0;
  }
}

/*--------------------------------------------------------------------------*/

DRIVERMODEL_API  int  DriverModelGetValue (long   type,
                                           long   index1,
                                           long   index2,
                                           long   *long_value,
                                           double *double_value,
                                           char   **string_value)
{
  /* Gets the value of a data object of type <type>, selected by <index1> */
  /* and possibly <index2>, and writes that value to <*long_value>,       */
  /* <*double_value> or <**string_value> (object and value selection      */
  /* depending on <type>).                                                */
  /* Return value is 1 on success, otherwise 0.                           */

  switch (type) {
    case DRIVER_DATA_STATUS :
      *long_value = 0;
      return 1;
    case DRIVER_DATA_TIME:
        *double_value = current_time;
        return 1;
    case DRIVER_DATA_VEH_ID:
        *long_value = current_vehicle;
    case DRIVER_DATA_WANTS_ALL_SIGNALS:
      *long_value = 1; /* needs to be set to zero if no global signal data is required */
      return 1;
    case DRIVER_DATA_MAX_NUM_INDICES:
      *long_value = 3; /* because DriverModelSetValue3() and DriverModelSetValue3() exist in this DLL */
      return 1;
    case DRIVER_DATA_VEH_TURNING_INDICATOR :
      *long_value = turning_indicator;
      return 1;
    case DRIVER_DATA_VEH_DESIRED_VELOCITY   :
      *double_value = desired_velocity;
      return 1;
    case DRIVER_DATA_VEH_COLOR :
      *long_value = vehicle_color;
      return 1;
    case DRIVER_DATA_VEH_UDA :
      return 0; /* doesn't set any UDA values */
    case DRIVER_DATA_WANTS_SUGGESTION :
      *long_value = 1;
      return 1;
    case DRIVER_DATA_DESIRED_ACCELERATION :
      *double_value = desired_acceleration;
      return 1;
    case DRIVER_DATA_DESIRED_LANE_ANGLE :
      *double_value = desired_lane_angle;
      return 1;
    case DRIVER_DATA_ACTIVE_LANE_CHANGE :
      *long_value = active_lane_change;
      return 1;
    case DRIVER_DATA_REL_TARGET_LANE :
      *long_value = rel_target_lane;
      return 1;
    case DRIVER_DATA_SIMPLE_LANECHANGE :
      *long_value = 1;
      return 1;
    case DRIVER_DATA_USE_INTERNAL_MODEL:
      *long_value = 0; /* must be set to 0 if external model is to be applied */
      return 1;
    case DRIVER_DATA_WANTS_ALL_NVEHS:
      *long_value = 0; /* must be set to 1 if data for more than 2 nearby vehicles per lane and upstream/downstream is to be passed from Vissim */
      return 1;
    case DRIVER_DATA_ALLOW_MULTITHREADING:
      *long_value = 0; /* must be set to 1 to allow a simulation run to be started with multiple cores used in the simulation parameters */
      return 1;
    default:
      return 0;
  }
}

/*--------------------------------------------------------------------------*/

DRIVERMODEL_API  int  DriverModelGetValue3 (long   type,
                                            long   index1,
                                            long   index2,
                                            long   index3,
                                            long   *long_value,
                                            double *double_value,
                                            char   **string_value)
{
  /* Gets the value of a data object of type <type>, selected by <index1>, */
  /* <index2> and <index3>, and writes that value to <*long_value>,        */
  /* <*double_value> or <**string_value> (object and value selection       */
  /* depending on <type>).                                                 */
  /* Return value is 1 on success, otherwise 0.                            */
  /* DriverModelGetValue (DRIVER_DATA_MAX_NUM_INDICES, ...) needs to set   */
  /* *long_value to 3 or greater in order to activate this function!       */

  switch (type) {
    default:
      return 0;
    }
}
// Sigmoid function
double Sigmoid(double x, double k, double d1) {
    return 1.0 / (1.0 + exp(-k * (x - d1)));
}

// Calculate acceleration for each piece
double CalcAccel(double vf, double v_delta, double d, int piece) {
    switch (piece) {
    case 1:
        return -6.6713 + 0.024 * vf + 0.4313 * v_delta + 0.0936 * d;
    case 2:
        return -0.1768 - 0.04134 * vf + 0.346 * v_delta + 0.015 * d;
    case 3:
        return 6.2518 - 0.0422 * vf + 0.1253 * v_delta - 0.0079 * d;
    case 4:
        return 12.1340 - 0.0498 * vf - 0.0079 * v_delta - 0.0100 * d;
    default:
        return 0; // Default case
    }
}

// NewModel function with smoothed transitions
double NewModel(double vf, double v_delta, double d) {
    const double w = 1;
    const double k = 2;
    const double bp1 = 11.524;
    const double bp2 = 98.5670;
    double value, a_hat1, a_hat2, x;
    bool smooth = false;

    if (v_delta < bp1 - w && d < bp2 - w) {
        // Piece 1
        value = CalcAccel(vf, v_delta, d, 1);
    }
    else if (v_delta < bp1 - w && d >= bp2 + w) {
        // Piece 2
        value = CalcAccel(vf, v_delta, d, 2);
    }
    else if (v_delta >= bp1 + w && d < bp2 - w) {
        // Piece 3
        value = CalcAccel(vf, v_delta, d, 3);
    }
    else if (v_delta >= bp1 + w && d >= bp2 + w) {
        // Piece 4
        value = CalcAccel(vf, v_delta, d, 4);
    }
    else {
        // Smooth transitions
        smooth = true;
        if (v_delta < bp1 - w && d >= bp2 - w && d < bp2 + w) {
            // Transition between Piece 1 and Piece 2 (A)
            a_hat1 = CalcAccel(vf, v_delta, bp2 - w, 1);
            a_hat2 = CalcAccel(vf, v_delta, bp2 + w, 2);
            x = d;
        }
        else if (v_delta >= bp1 + w && d >= bp2 - w && d < bp2 + w) {
            // Transition between Piece 3 and Piece 4 (B)
            a_hat1 = CalcAccel(vf, v_delta, bp2 - w, 3);
            a_hat2 = CalcAccel(vf, v_delta, bp2 + w, 4);
            x = d;
        }
        else if ((v_delta >= bp1 - w && v_delta < bp1 + w) && d >= bp2 + w) {
            // Transition between Piece 2 and Piece 4 (C)
            a_hat1 = CalcAccel(vf, bp1 - w, d, 2);
            a_hat2 = CalcAccel(vf, bp1 + w, d, 4);
            x = v_delta;
        }
        else if ((v_delta >= bp1 - w && v_delta < bp1 + w) && d < bp2 - w) {
            // Transition between Piece 1 and Piece 3 (D)
            a_hat1 = CalcAccel(vf, bp1 - w, d, 1);
            a_hat2 = CalcAccel(vf, bp1 + w, d, 3);
            x = v_delta;
        }
        else {
            // Smoothed transition for all four pieces
            double A = CalcAccel(vf, bp1 - w, bp2 - w, 1);
            double B = CalcAccel(vf, bp1 + w, bp2 - w, 3);
            double C = CalcAccel(vf, bp1 + w, bp2 + w, 4);
            double D = CalcAccel(vf, bp1 - w, bp2 + w, 2);
            value = 0.25 * (A + B + C + D);
            return value;
        }
    }

    if (smooth) {
        double s = Sigmoid(x, k, (x < bp1) ? bp2 : bp1);
        value = a_hat1 + (a_hat2 - a_hat1) * s;
    }

    return value;
}

DRIVERMODEL_API  int  DriverModelExecuteCommand (long number)
{
  /* Executes the command <number> if that is available in the driver */
  /* module. Return value is 1 on success, otherwise 0.               */

  switch (number) {
    case DRIVER_COMMAND_INIT :
      return 1;
    case DRIVER_COMMAND_CREATE_DRIVER :
      return 1;
    case DRIVER_COMMAND_KILL_DRIVER :
      return 1;
    case DRIVER_COMMAND_MOVE_DRIVER :
        if (vehicle_type == 690)
        {
            //car following model
            if (ahead_vehicle < 0 || gross_distance - vehicle_length>120) {

                vehicle_to_stop = current_vehicle;//leading vehicle
                //free flow
                if (speedlimit - current_velocity > 1) {
                    desired_acceleration = 2.69 * exp(-0.02 * 3.6 * current_velocity);;
                }
                else
                {
                    desired_acceleration = speedlimit - current_velocity;
                }

            }
            if (current_vehicle != vehicle_to_stop)
            {
                desired_velocity = speedlimit;//m/s
                double d = gross_distance - vehicle_length;//distance headway
                double v = current_velocity;//current speed
                double v_delta = - speed_difference;//speed distance
                double feet2m = 0.3048;//unit conversion
                car_following_acceleration = NewModel(v / feet2m, v_delta / feet2m, d / feet2m) * feet2m;//get the MPL acceleration

                desired_acceleration = min(speedlimit - current_velocity, car_following_acceleration);// setting the MPL acceleration to the desired acceleration 
                //plugin emergency braking
                double cc0 = 1.5;
                double cc1 = 0.9;
                double vslow = 7;
                double cc4 = -0.35;
                double cc6 = 11.44;
                double OPTV = cc4 - cc6 / 17000 * d * d;
                double ABX = cc0 + cc1 * sqrt(min(v, v - speed_difference));
                if (d <= ABX && d > cc0 && speed_difference > OPTV) {
                    desired_acceleration = max(-0.5 * speed_difference * speed_difference / (d - cc0) + FrontVehAcc, -10 + sqrt(v));
                }
                if (d / (speed_difference - FrontVehAcc + current_acceleration) <= 2 && speed_difference > 0) {
                    desired_acceleration = -1.1*speed_difference + FrontVehAcc - current_acceleration;
                }
                if (d / (speed_difference - FrontVehAcc + current_acceleration) <= 1.5 && speed_difference > 0) {
                    desired_acceleration = -10 + sqrt(v);
                }
            }
            if (current_velocity > speedlimit)//m/s
            {
                desired_acceleration = speedlimit - current_velocity;
            }
        }
      return 1;
    default :
      return 0;
  }
}

/*==========================================================================*/
/*  End of DriverModel.cpp                                                  */
/*==========================================================================*/
