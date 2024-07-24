/*==========================================================================*/
/*  DriverModelExampleConflArea.cpp                  DLL Module for VISSIM  */
/*                                                                          */
/*  Interface module for external driver models.                            */
/*  Example version with a very simple driver model.                        */
/*                                                                          */
/*  Version of 2018-08-30                                   Lukas Kautzsch  */
/*==========================================================================*/

#include "DriverModel.h"

/*==========================================================================*/

double  desired_velocity     = 0.0;
long    ca_count, ca_no[3], ca_type[3], ca_yield[3], ca_veh[3], ca_veh_no[3][3];
double  ca_dist[3], ca_length[3], ca_veh_tEn[3][3], ca_veh_tIn[3][3], ca_veh_tEx[3][3];

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
      return 1; /* Use all UDAs */
                /* must return 1 for desired values of index1 if UDA values are to be sent from/to Vissim */
    case DRIVER_DATA_VEH_ID:
      /* reset leading vehicle's data for this new vehicle */
      return 1;
    case DRIVER_DATA_VEH_LANE               :
    case DRIVER_DATA_VEH_ODOMETER           :
    case DRIVER_DATA_VEH_LANE_ANGLE         :
    case DRIVER_DATA_VEH_LATERAL_POSITION   :
    case DRIVER_DATA_VEH_VELOCITY           :
    case DRIVER_DATA_VEH_ACCELERATION       :
    case DRIVER_DATA_VEH_LENGTH             :
    case DRIVER_DATA_VEH_WIDTH              :
    case DRIVER_DATA_VEH_WEIGHT             :
    case DRIVER_DATA_VEH_MAX_ACCELERATION   :
    case DRIVER_DATA_VEH_TURNING_INDICATOR  :
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
    case DRIVER_DATA_VEH_COLOR              :
      return 1;
    case DRIVER_DATA_VEH_CURRENT_LINK       :
      return 0; /* (to avoid getting sent lots of DRIVER_DATA_VEH_NEXT_LINKS messages) */
    case DRIVER_DATA_VEH_NEXT_LINKS         :
    case DRIVER_DATA_VEH_ACTIVE_LANE_CHANGE :
    case DRIVER_DATA_VEH_REL_TARGET_LANE    :
    case DRIVER_DATA_VEH_INTAC_STATE        :
    case DRIVER_DATA_VEH_INTAC_TARGET_TYPE  :
    case DRIVER_DATA_VEH_INTAC_TARGET_ID    :
    case DRIVER_DATA_VEH_INTAC_HEADWAY      :
    case DRIVER_DATA_VEH_UDA                :
    case DRIVER_DATA_NVEH_ID                :
    case DRIVER_DATA_NVEH_LANE_ANGLE        : /*leading vehicle's angle*/
    case DRIVER_DATA_NVEH_LATERAL_POSITION  :
    case DRIVER_DATA_NVEH_DISTANCE          :
    case DRIVER_DATA_NVEH_REL_VELOCITY      :
    case DRIVER_DATA_NVEH_ACCELERATION      :
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
      return 1;
    case DRIVER_DATA_ROUTE_SIGNAL_SWITCH    :
      return 0;  /* (to avoid getting sent lots of signal switch data) */
    case DRIVER_DATA_CONFL_AREAS_COUNT      :
      ca_count = long_value;
      return 1;
    case DRIVER_DATA_CONFL_AREA_TYPE        :
      ca_no[index1] = index2;
      ca_type[index1] = long_value;
      return 1;
    case DRIVER_DATA_CONFL_AREA_YIELD       :
      ca_yield[index1] = long_value;
      return 1;
    case DRIVER_DATA_CONFL_AREA_DISTANCE    :
      ca_dist[index1] = double_value;
      return 1;
    case DRIVER_DATA_CONFL_AREA_LENGTH      :
      ca_length[index1] = double_value;
      return 1;
    case DRIVER_DATA_CONFL_AREA_VEHICLES    :
      ca_veh[index1] = long_value;
      return 1;
    case DRIVER_DATA_CONFL_AREA_TIME_ENTER  :
      ca_veh_no[index1][index2] = long_value;
      ca_veh_tEn[index1][index2] = double_value;
      return 1;
    case DRIVER_DATA_CONFL_AREA_TIME_IN     :
      ca_veh_tIn[index1][index2] = double_value;
      return 1;
    case DRIVER_DATA_CONFL_AREA_TIME_EXIT   :
      ca_veh_tEx[index1][index2] = double_value;
      return 1;
    case DRIVER_DATA_DESIRED_ACCELERATION :
    case DRIVER_DATA_DESIRED_LANE_ANGLE :
    case DRIVER_DATA_ACTIVE_LANE_CHANGE :
    case DRIVER_DATA_REL_TARGET_LANE :
      return 1;
    default :
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
  /* and possibly <index2>, and writes that value to <*double_value>,     */
  /* <*float_value> or <**string_value> (object and value selection       */
  /* depending on <type>).                                                */
  /* Return value is 1 on success, otherwise 0.                           */

  switch (type) {
    case DRIVER_DATA_STATUS :
      return 1;
    case DRIVER_DATA_VEH_TURNING_INDICATOR :
      return 1;
    case DRIVER_DATA_VEH_DESIRED_VELOCITY   :
      *double_value = desired_velocity;
      return 1;
    case DRIVER_DATA_VEH_COLOR :
      return 1;
    case DRIVER_DATA_VEH_UDA :
    {
      if (index1 == 1099) {
        *long_value = ca_count;
        return 1;
      }
      if ((index1 - 1000) / 100 > ca_count) {
        return 0;
      }
      int ca_index = (index1 - 1000) / 100 - 1;
      if (index1 - 1000 - 100 * (ca_index+1) > 9) {
        int veh_index = (index1 - 1000 - 100 * (ca_index+1)) / 10 - 1;
        if (veh_index >= ca_veh[ca_index]) {
          return 0;
        }
        switch (index1 - 1000 - 100 * (ca_index+1) - 10 * (veh_index+1)) {
          case 0 :
            *long_value = ca_veh_no[ca_index][veh_index];
            return 1;
          case 1 :
            *double_value = ca_veh_tEn[ca_index][veh_index];
            return 1;
          case 2 :
            *double_value = ca_veh_tIn[ca_index][veh_index];
            return 1;
          case 3 :
            *double_value = ca_veh_tEx[ca_index][veh_index];
            return 1;
          default :
            return 0;
        }
      }
      switch (index1 - 1000 - 100 * (ca_index+1)) {
        case 0 :
          *long_value = ca_no[ca_index];
          return 1;
        case 1 :
          *long_value = ca_type[ca_index];
          return 1;
        case 2 :
          *long_value = ca_yield[ca_index];
          return 1;
        case 3 :
          *double_value = ca_dist[ca_index];
          return 1;
        case 4 :
          *double_value = ca_length[ca_index];
          return 1;
        case 5 :
          *long_value = ca_veh[ca_index];
          return 1;
        default :
          return 0;
      }
    }
    case DRIVER_DATA_WANTS_SUGGESTION :
      *long_value = 1;
      return 1;
    case DRIVER_DATA_DESIRED_ACCELERATION : 
      return 1;
    case DRIVER_DATA_DESIRED_LANE_ANGLE :
      return 1;
    case DRIVER_DATA_ACTIVE_LANE_CHANGE :
      return 1;
    case DRIVER_DATA_REL_TARGET_LANE :
      return 1;
    case DRIVER_DATA_SIMPLE_LANECHANGE :
      return 1;
    case DRIVER_DATA_USE_INTERNAL_MODEL:
      *long_value = 1; /* must be set to 1 if the internal behavior model of Vissim is to be applied */
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

/*==========================================================================*/

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
      return 1;
    default :
      return 0;
  }
}

/*==========================================================================*/
/*  End of DriverModel.cpp                                                  */
/*==========================================================================*/
