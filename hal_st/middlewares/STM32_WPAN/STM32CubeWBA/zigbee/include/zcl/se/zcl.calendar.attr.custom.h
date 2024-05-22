/* Copyright [2022] Exegin Technologies Limited. All rights reserved. */

#include "zcl/se/zcl.calendar.h"

#ifndef ZCL_CALENDAR_ATTR_CALLBACK
# define ZCL_CALENDAR_ATTR_CALLBACK              NULL
# define ZCL_CALENDAR_ATTR_FLAGS                 ZCL_ATTR_FLAG_NONE
/* By default, disable reporting */
# define ZCL_CALENDAR_ATTR_REPORT_INTVL_MIN      0x0000U
# define ZCL_CALENDAR_ATTR_REPORT_INTVL_MAX      0xffffU

#else
# ifndef ZCL_CALENDAR_ATTR_FLAGS
/* If callback is defined, must also define read/write callback flags:
 * ZCL_ATTR_FLAG_CB_READ | ZCL_ATTR_FLAG_CB_WRITE */
#  error "ZCL_CALENDAR_ATTR_FLAGS not defined"
# endif
# ifndef ZCL_CALENDAR_ATTR_REPORT_INTVL_MIN
#  error "ZCL_CALENDAR_ATTR_REPORT_INTVL_MIN not defined"
# endif
# ifndef ZCL_CALENDAR_ATTR_REPORT_INTVL_MAX
#  error "ZCL_CALENDAR_ATTR_REPORT_INTVL_MAX not defined"
# endif
#endif

static const struct ZbZclAttrT zcl_cal_server_custom_attr_list[] = {
    {
        ZCL_CAL_SVR_ATTR_AuxSwitch1Label, ZCL_DATATYPE_STRING_OCTET,
        ZCL_CALENDAR_ATTR_FLAGS, 23U, ZCL_CALENDAR_ATTR_CALLBACK, {0, 0},
        {ZCL_CALENDAR_ATTR_REPORT_INTVL_MIN, ZCL_CALENDAR_ATTR_REPORT_INTVL_MAX}
    },
    {
        ZCL_CAL_SVR_ATTR_AuxSwitch2Label, ZCL_DATATYPE_STRING_OCTET,
        ZCL_CALENDAR_ATTR_FLAGS, 23U, ZCL_CALENDAR_ATTR_CALLBACK, {0, 0},
        {ZCL_CALENDAR_ATTR_REPORT_INTVL_MIN, ZCL_CALENDAR_ATTR_REPORT_INTVL_MAX}
    }
};
