/* Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved. */

#include "zcl/se/zcl.price.h"

#ifndef ZCL_PRICE_ATTR_CALLBACK
# define ZCL_PRICE_ATTR_CALLBACK                    NULL
# define ZCL_PRICE_ATTR_FLAGS                       ZCL_ATTR_FLAG_NONE
/* By default, disable reporting */
# define ZCL_PRICE_ATTR_REPORT_INTVL_MIN            0x0000U
# define ZCL_PRICE_ATTR_REPORT_INTVL_MAX            0xffffU

#else
# ifndef ZCL_PRICE_ATTR_FLAGS
/* If callback is defined, must also define read/write callback flags:
 * ZCL_ATTR_FLAG_CB_READ | ZCL_ATTR_FLAG_CB_WRITE */
#  error "ZCL_PRICE_ATTR_FLAGS not defined"
# endif
# ifndef ZCL_PRICE_ATTR_REPORT_INTVL_MIN
#  error "ZCL_PRICE_ATTR_REPORT_INTVL_MIN not defined"
# endif
# ifndef ZCL_PRICE_ATTR_REPORT_INTVL_MAX
#  error "ZCL_PRICE_ATTR_REPORT_INTVL_MAX not defined"
# endif
#endif

static const struct ZbZclAttrT zcl_price_server_gbcs_attr_list[] = {
    /* CommodityType (Read-only) */
    {
        ZCL_PRICE_SVR_ATTR_COMMODITY_TYPE, ZCL_DATATYPE_ENUMERATION_8BIT, ZCL_PRICE_ATTR_FLAGS, 0,
        ZCL_PRICE_ATTR_CALLBACK, {0, 0},
        {ZCL_PRICE_ATTR_REPORT_INTVL_MIN, ZCL_PRICE_ATTR_REPORT_INTVL_MAX}
    },
};
