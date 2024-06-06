/**
  ******************************************************************************
  * @file    mcp_types.h
  * @author  MCD Application Team
  * @brief   This file contains type definitions used for Media Control Profile
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MCP_TYPES_H
#define __MCP_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Defines -------------------------------------------------------------------*/

/* #############################################################################
   #       Defines and MACRO used to allocate GATT Services, GATT Attributes   #
   #       and storage area for Attribute values for Media Control Profile     #
   #       in Server role                                                      #
   ############################################################################# */

/*
 * MCP_SRV_NUM_GATT_SERVICES: number of GATT services required for Media Control Profile in Server role
 *
 * @param num_media_players: Maximum number of supported Media Player
 */
#define MCP_SRV_NUM_GATT_SERVICES(num_media_players)    (num_media_players)

/*
 * MCP_SRV_NUM_GATT_ATTRIBUTES: minimum number of GATT attributes required to support Media Players registration
 * Additional GATT attributes could be required according to the MCP Option Features specified in the MCP_SRV_Config_t
 * settings submitted in the MCP_Init() function.
 *
 * @param num_media_players: Maximum number of supported Media Players
 *
 * @param mcp_feature: Supported MCP Option Feature (MCP_OptionFeatures_t type).
 */
#define MCP_SRV_NUM_GATT_ATTRIBUTES(num_media_players,mcp_feature) \
                                    (num_media_players * \
                                    (20u + (((mcp_feature & MCP_FEATURE_PLAYBACK_SPEED) != (0u) ) ? (3) : (0)) \
                                    + (((mcp_feature & MCP_FEATURE_SEEKING_SPEED) != (0u)) ? (3) : (0)) \
                                    + (((mcp_feature & MCP_FEATURE_PLAYING_ORDER) != (0u)) ? (3) : (0)) \
                                    + (((mcp_feature & MCP_FEATURE_PLAYING_ORDERS_SUPPORTED) != (0u)) ? (2) : (0)) \
                                    + (((mcp_feature & MCP_FEATURE_MEDIA_CTRL_OP) != (0u)) ? (6) : (0))))

/*
 * MCP_SRV_ATT_VALUE_ARRAY_SIZE: this macro returns the size of the storage area for Attribute values
 * in MCP Server.
 *
 * @param num_media_players: Maximum number of supported Media Players
 *
 * @param num_ble_links: Maximum number of simultaneous connections that the device will support.
 *
 * @param mcp_feature: Supported MCP Option Feature (MCP_OptionFeatures_t type).
 *
 * @param max_media_name_length: The maximum media name size.
 *
 * @param max_track_title_length: The maximum track title size.
 */
#define MCP_SRV_ATT_VALUE_ARRAY_SIZE(num_media_players,num_ble_links,mcp_feature,max_media_name_length,max_track_title_length) \
                                (num_media_players * \
                                ((45u+(12u*num_ble_links) \
                                  + max_media_name_length) \
                                  + (max_track_title_length) \
                                  + (((mcp_feature & MCP_FEATURE_PLAYBACK_SPEED) != (0x00)) ? (6u+2*num_ble_links) : (0)) \
                                  + (((mcp_feature & MCP_FEATURE_SEEKING_SPEED) != (0x00)) ? (6u+2*num_ble_links) : (0)) \
                                  + (((mcp_feature & MCP_FEATURE_PLAYING_ORDER) != (0x00)) ? (6u+2*num_ble_links) : (0)) \
                                  + (((mcp_feature & MCP_FEATURE_PLAYING_ORDERS_SUPPORTED) != (0x00)) ? (7u) : (0)) \
                                  + (((mcp_feature & MCP_FEATURE_MEDIA_CTRL_OP) != (0x00)) ? (19u+4u*num_ble_links) : (0))))

/* #############################################################################
   #       Defines and MACRO used to allocate memory resource of               #
   #       Media Control Profile in MCP_Config_t                                #
   #      (BLE_MCP_CLT_TOTAL_BUFFER_SIZE , BLE_MCP_SRV_TOTAL_BUFFER_SIZE)      #
   ############################################################################# */

/*
 * MCP_CLT_MEM_PER_CONN_SIZE_BYTES: memory size used per link in MCP Client role
 */
#define MCP_CLT_MEM_PER_CONN_SIZE_BYTES                 (36u)

/*
 * MCP_CLT_MEM_PER_MEDIA_PLAYER_SIZE_BYTES: memory size used per Media Player in MCP Client role
 */
#define MCP_CLT_MEM_PER_MEDIA_PLAYER_SIZE_BYTES         (192u)

/*
 * MCP_SRV_MEM_PER_MEDIA_PLAYER_SIZE_BYTES: memory size used per Media Player in MCP Server role
 */
#define MCP_SRV_MEM_PER_MEDIA_PLAYER_SIZE_BYTES         (84u)

/*
 * MCP_SRV_MEM_PER_READLONG_PROC_SIZE_BYTES: memory size used per Read Long Procedure in MCP Server role
 */
#define MCP_SRV_MEM_PER_READLONG_PROC_SIZE_BYTES        (4u)

/*
 * BLE_MCP_CLT_TOTAL_BUFFER_SIZE: this macro returns the amount of memory, in bytes,needed for
 * the storage of data structures in MCP Client whose size depends on the number of supported
 * connections and the number of supported Media Player Instances.
 *
 * @param num_ble_links: Maximum number of simultaneous connections that the device
 * will support. Valid values are from 1 to 8.
 *
 * @param num_media_player_instances: Maximum number of supported Media Player Instances (without
 * take in account the mandatory Generic Media Player) per connection.
 */
#define BLE_MCP_CLT_TOTAL_BUFFER_SIZE(num_ble_links, num_media_player_instances) \
          ((MCP_CLT_MEM_PER_MEDIA_PLAYER_SIZE_BYTES * num_ble_links * (1 + num_media_player_instances)) + \
           (MCP_CLT_MEM_PER_CONN_SIZE_BYTES * num_ble_links))

/*
 * BLE_MCP_SRV_TOTAL_BUFFER_SIZE: this macro returns the amount of memory, in bytes,needed for
 * the storage of data structures in MCP Server whose size depends on the number of supported
 * connections, the number of supported Media Player Instances, the maximum media name size and
 * the maximum track title size.
 *
 * @param num_ble_links: Maximum number of simultaneous connections that the device
 * will support. Valid values are from 1 to 8.
 *
 * @param num_media_player_instances: Maximum number of supported Media Player Instances (without
 * take in account the mandatory Generic Media Player) per connection.
 *
 * @param max_media_name_length: The maximum media name size.
 *
 * @param max_track_title_length: The maximum track title size.
 */
#define BLE_MCP_SRV_TOTAL_BUFFER_SIZE(num_ble_links,num_mp_instances,read_long_en,max_media_name_length,max_track_title_length) \
                                      ((MCP_SRV_MEM_PER_MEDIA_PLAYER_SIZE_BYTES * (1+num_mp_instances)) + \
                                       (((read_long_en) == (0x01)) ? (MCP_SRV_MEM_PER_READLONG_PROC_SIZE_BYTES * num_ble_links * (1 + num_mp_instances)) : (0)) + \
                                        (DIVC((max_media_name_length * (1+num_mp_instances)),4u) * 4u) + \
                                        (DIVC((max_track_title_length * (1+num_mp_instances)),4u) * 4u))


/* Types ---------------------------------------------------------------------*/
/* Types of mask for roles of the Media Control Profile */
typedef uint8_t MCP_Role_t;
#define MCP_ROLE_SERVER			        (0x01)
#define MCP_ROLE_CLIENT			        (0x02)


/* Media Control Optional features*/
typedef uint16_t MCP_OptionFeatures_t;
#define MCP_FEATURE_PLAYBACK_SPEED              (0x0001)        /* Playback Speed feature is supported.*/
#define MCP_FEATURE_SEEKING_SPEED               (0x0002)        /* Seeking Speed feature is supported.*/
#define MCP_FEATURE_PLAYING_ORDER               (0x0004)        /* Playing Order feature is supported.*/
#define MCP_FEATURE_PLAYING_ORDERS_SUPPORTED    (0x0008)        /* Playing Orders Supported feature is supported.*/
#define MCP_FEATURE_MEDIA_CTRL_OP               (0x0010)        /* Media Control Operation feature is supported.*/

/* Media Control Notification Features type in MCP Server side.
 * If notification is supported, an Updated in MCP Server side would be notified to MCP Client in case of MCP Client
 * has enabled the notification
 */
typedef uint16_t MCP_NotificationUpdateFeatures_t;
#define MCP_NOTIFICATION_MEDIA_PLAYER_NAME         (0x0001)     /* Notification of Media Player Name Update is supported*/
#define MCP_NOTIFICATION_TRACK_TITLE               (0x0002)     /* Notification of Track Title Update is supported.*/
#define MCP_NOTIFICATION_TRACK_DURATION            (0x0004)     /* Notification of Track Duration Update is supported.*/
#define MCP_NOTIFICATION_TRACK_POSITION            (0x0008)     /* Notification of Track Position Update is supported.*/
#define MCP_NOTIFICATION_PLAYBACK_SPEED            (0x0010)     /* Notification of Playback Speed Update is supported.*/
#define MCP_NOTIFICATION_SEEKING_SPEED             (0x0020)     /* Notification of Seeking Speed Update is supported.*/
#define MCP_NOTIFICATION_PLAYING_ORDER             (0x0040)     /* Notification of Playing Order Update is supported.*/
#define MCP_NOTIFICATION_MEDIA_CTRL_OP_SUPPORTED   (0x0080)     /* Notification of Media Control Operation Supported
                                                                 * Update is supported.
                                                                 */

/*Media State Type*/
typedef uint8_t MCP_MediaState_t;
#define MCP_MEDIA_STATE_INACTIVE                (0x00)  /* The current track is invalid,
                                                         * and no track has been selected.
                                                         */
#define MCP_MEDIA_STATE_PLAYING                 (0x01)  /* The media player is playing the current track. */
#define MCP_MEDIA_STATE_PAUSED                  (0x02)  /* The current track is paused.
                                                         * The media player has a current track, but it is not
                                                         * being played.
                                                         */
#define MCP_MEDIA_STATE_SEEKING                 (0x03)  /* The current track is fast forwarding or fast rewinding. */



/* Types describing the Media Control Operation feature support */
typedef uint32_t MCP_MediaCtrlOpFeature_t;
#define MCP_MEDIA_CTRL_FEATURE_PLAY             (0x00000001)    /* Start playing the current track.*/
#define MCP_MEDIA_CTRL_FEATURE_PAUSE            (0x00000002)    /* Pause playing the current track.*/
#define MCP_MEDIA_CTRL_FEATURE_FAST_REWIND      (0x00000004)    /* Fast rewind the current track.*/
#define MCP_MEDIA_CTRL_FEATURE_FAST_FORWARD     (0x00000008)    /* Fast forward the current track.*/
#define MCP_MEDIA_CTRL_FEATURE_STOP             (0x00000010)    /* Stop current activity and return to the paused state
                                                                 * and set the current track position to the start of
                                                                 * the current track.
                                                                 */
#define MCP_MEDIA_CTRL_FEATURE_MOVE_RELATIVE    (0x00000020)    /* Set a new current track position relative to
                                                                 * the current track position.
                                                                 */
#define MCP_MEDIA_CTRL_FEATURE_PREVIOUS_SEGMENT (0x00000040)    /* Set the current track position to the
                                                                 * starting position of the previous segment of the
                                                                 * current track.
                                                                 */
#define MCP_MEDIA_CTRL_FEATURE_NEXT_SEGMENT     (0x00000080)    /* Set the current track position to the
                                                                 * starting position of the next segment of the
                                                                 * current track.
                                                                 */
#define MCP_MEDIA_CTRL_FEATURE_FIRST_SEGMENT    (0x00000100)    /* Set the current track position to the
                                                                 * starting position of the first segment of the
                                                                 * current track.
                                                                 */
#define MCP_MEDIA_CTRL_FEATURE_LAST_SEGMENT     (0x00000200)    /* Set the current track position to the
                                                                 * starting position of the last segment of the
                                                                 * current track.
                                                                 */
#define MCP_MEDIA_CTRL_FEATURE_GOTO_SEGMENT     (0x00000400)    /* Set the current track position to the
                                                                 * starting position of the nth segment of the
                                                                 * current track.
                                                                 */
#define MCP_MEDIA_CTRL_FEATURE_PREVIOUS_TRACK   (0x00000800)    /* Set the current track to the previous track
                                                                 * based on the playing order.
                                                                 */
#define MCP_MEDIA_CTRL_FEATURE_NEXT_TRACK       (0x00001000)    /* Set the current track to the next track
                                                                 * based on the playing order.
                                                                 */
#define MCP_MEDIA_CTRL_FEATURE_FIRST_TRACK      (0x00002000)    /* Set the current track to the first track
                                                                 * based on the playing order.
                                                                 */
#define MCP_MEDIA_CTRL_FEATURE_LAST_TRACK       (0x00004000)    /* Set the current track to the last track
                                                                 * based on the playing order.
                                                                 */
#define MCP_MEDIA_CTRL_FEATURE_GOTO_TRACK       (0x00008000)    /* Set the current track to the nth track
                                                                 * based on the playing order.
                                                                 */
#define MCP_MEDIA_CTRL_FEATURE_PREVIOUS_GROUP   (0x00010000)    /* Set the current group to the previous group
                                                                 * in the sequence of groups.
                                                                 */
#define MCP_MEDIA_CTRL_FEATURE_NEXT_GROUP       (0x00020000)    /* Set the current group to the next group
                                                                 * in the sequence of groups.
                                                                 */
#define MCP_MEDIA_CTRL_FEATURE_FIRST_GROUP      (0x00040000)    /* Set the current group to the first group
                                                                 * in the sequence of groups.
                                                                 */
#define MCP_MEDIA_CTRL_FEATURE_LAST_GROUP       (0x00080000)    /* Set the current group to the last group
                                                                 * in the sequence of groups.
                                                                 */
#define MCP_MEDIA_CTRL_FEATURE_GOTO_GROUP       (0x00100000)    /* Set the current group to the nth group
                                                                 * in the sequence of groups.
                                                                 */

/*Types describing the Playing Order*/
typedef uint8_t MCP_PlayingOrder_t;
#define MCP_PLAYING_ORDER_SINGLE_ONCE                   (0x01)  /* A single track is played once.
                                                                 * There is no next track
                                                                 */
#define MCP_PLAYING_ORDER_SINGLE_REPEAT                 (0x02)  /* A single track is played repeatedly.
                                                                 * Next track is the current track.
                                                                 */
#define MCP_PLAYING_ORDER_IN_ORDER_ONCE                 (0x03)  /* The tracks within a group are played once
                                                                 * in track order.
                                                                 */
#define MCP_PLAYING_ORDER_IN_ORDER_REPEAT               (0x04)  /* The tracks within a group are played in
                                                                 * order repeatedly.
                                                                 */
#define MCP_PLAYING_ORDER_OLDEST_ONCE                   (0x05)  /* The tracks within a group are played once
                                                                 * only from the oldest first.
                                                                 */
#define MCP_PLAYING_ORDER_OLDEST_REPEAT                 (0x06)  /* The tracks within a group are played from the
                                                                 * oldest first repeatedly.
                                                                 */
#define MCP_PLAYING_ORDER_NEWEST_ONCE                   (0x07)  /* The tracks within a group are played once only
                                                                 * from the newest first.
                                                                 */
#define MCP_PLAYING_ORDER_NEWEST_REPEAT                 (0x08)  /* The tracks within a group are played from the
                                                                 * newest first repeatedly.
                                                                 */
#define MCP_PLAYING_ORDER_SHUFFLE_ONCE                  (0x09)  /* The tracks within a group are played in
                                                                 * random order once.
                                                                 */
#define MCP_PLAYING_ORDER_SHUFFLE_REPEAT                (0x0A)  /* The tracks within a group are played in
                                                                 * random order repeatedly.
                                                                 */

/*Mask of Playing Orders Type*/
typedef uint16_t MCP_PlayingOrdersSupported_t;
#define MCP_SUPPORT_PLAYING_ORDER_SINGLE_ONCE           (0x0001)        /* A single track is played once.
                                                                         * There is no next track
                                                                         */
#define MCP_SUPPORT_PLAYING_ORDER_SINGLE_REPEAT         (0x0002)        /* A single track is played repeatedly.
                                                                         * Next track is the current track.
                                                                         */
#define MCP_SUPPORT_PLAYING_ORDER_IN_ORDER_ONCE         (0x0004)        /* The tracks within a group are played once
                                                                         * in track order.
                                                                         */
#define MCP_SUPPORT_PLAYING_ORDER_IN_ORDER_REPEAT       (0x0008)        /* The tracks within a group are played in
                                                                         * order repeatedly.
                                                                         */
#define MCP_SUPPORT_PLAYING_ORDER_OLDEST_ONCE           (0x0010)        /* The tracks within a group are played once
                                                                         * only from the oldest first.
                                                                         */
#define MCP_SUPPORT_PLAYING_ORDER_OLDEST_REPEAT         (0x0020)        /* The tracks within a group are played from
                                                                         * the oldest first repeatedly.
                                                                         */
#define MCP_SUPPORT_PLAYING_ORDER_NEWEST_ONCE           (0x0040)        /* The tracks within a group are played once
                                                                         * only from the newest first.
                                                                         */
#define MCP_SUPPORT_PLAYING_ORDER_NEWEST_REPEAT         (0x0080)        /* The tracks within a group are played from
                                                                         * the newest first repeatedly.
                                                                         */
#define MCP_SUPPORT_PLAYING_ORDER_SHUFFLE_ONCE          (0x0100)        /* The tracks within a group are played in
                                                                         * random order once.
                                                                         */
#define MCP_SUPPORT_PLAYING_ORDER_SHUFFLE_REPEAT        (0x0200)        /* The tracks within a group are played in
                                                                         * random order repeatedly.
                                                                         */

/*Types describing the Media Control Operation*/
typedef uint8_t MCP_MediaControlOpcode_t;
#define MCP_MEDIA_CTRL_PLAY                             (0x01)  /* Start playing the current track.*/
#define MCP_MEDIA_CTRL_PAUSE                            (0x02)  /* Pause playing the current track.*/
#define MCP_MEDIA_CTRL_FAST_REWIND                      (0x03)  /* Fast rewind the current track.*/
#define MCP_MEDIA_CTRL_FAST_FORWARD                     (0x04)  /* Fast forward the current track.*/
#define MCP_MEDIA_CTRL_STOP                             (0x05)  /* Stop current activity and return to the paused state
                                                                 * and set the current track position to the start of
                                                                 * the current track.
                                                                 */
#define MCP_MEDIA_CTRL_MOVE_RELATIVE                    (0x10)  /* Set a new current track position relative to
                                                                 * the current track position.
                                                                 */
#define MCP_MEDIA_CTRL_PREVIOUS_SEGMENT                 (0x20)  /* Set the current track position to the
                                                                 * starting position of the previous segment of the
                                                                 * current track.
                                                                 */
#define MCP_MEDIA_CTRL_NEXT_SEGMENT                     (0x21)  /* Set the current track position to the
                                                                 * starting position of the next segment of the
                                                                 * current track.
                                                                 */
#define MCP_MEDIA_CTRL_FIRST_SEGMENT                    (0x22)  /* Set the current track position to the
                                                                 * starting position of the first segment of the
                                                                 * current track.
                                                                 */
#define MCP_MEDIA_CTRL_LAST_SEGMENT                     (0x23)  /* Set the current track position to the
                                                                 * starting position of the last segment of the
                                                                 * current track.
                                                                 */
#define MCP_MEDIA_CTRL_GOTO_SEGMENT                     (0x24)  /* Set the current track position to the
                                                                 * starting position of the nth segment of the
                                                                 * current track.
                                                                 */
#define MCP_MEDIA_CTRL_PREVIOUS_TRACK                   (0x30)  /* Set the current track to the previous track
                                                                 * based on the playing order.
                                                                 */
#define MCP_MEDIA_CTRL_NEXT_TRACK                       (0x31)  /* Set the current track to the next track
                                                                 * based on the playing order.
                                                                 */
#define MCP_MEDIA_CTRL_FIRST_TRACK                      (0x32)  /* Set the current track to the first track
                                                                 * based on the playing order.
                                                                 */
#define MCP_MEDIA_CTRL_LAST_TRACK                       (0x33)  /* Set the current track to the last track
                                                                 * based on the playing order.
                                                                 */
#define MCP_MEDIA_CTRL_GOTO_TRACK                       (0x34)  /* Set the current track to the nth track
                                                                 * based on the playing order.
                                                                 */
#define MCP_MEDIA_CTRL_PREVIOUS_GROUP                   (0x40)  /* Set the current group to the previous group
                                                                 * in the sequence of groups.
                                                                 */
#define MCP_MEDIA_CTRL_NEXT_GROUP                       (0x41)  /* Set the current group to the next group
                                                                 * in the sequence of groups.
                                                                 */
#define MCP_MEDIA_CTRL_FIRST_GROUP                      (0x42)  /* Set the current group to the first group
                                                                 * in the sequence of groups.
                                                                 */
#define MCP_MEDIA_CTRL_LAST_GROUP                       (0x43)  /* Set the current group to the last group
                                                                 * in the sequence of groups.
                                                                 */
#define MCP_MEDIA_CTRL_GOTO_GROUP                       (0x44)  /* Set the current group to the nth group
                                                                 * in the sequence of groups.
                                                                 */

/*Media Control Response Code Type*/
typedef uint8_t MCP_MediaControlRespCode_t;
#define MCP_MEDIA_CTRL_RSP_SUCCESS                      (0x01)  /* Action requested by the opcode write
                                                                 * was completed successfully.
                                                                 */
#define MCP_MEDIA_CTRL_RSP_NOT_SUPPORTED                (0x02)  /* An invalid or unsupported operation was sent
                                                                 */
#define MCP_MEDIA_CTRL_RSP_MEDIA_PLAYER_INACTIVE        (0x03)  /* The Media Player State value is Inactive when the
                                                                 * operation request is received or the result
                                                                 * of the requested action of the operation results
                                                                 * in the Media Player State being set to Inactive.
                                                                 */
#define MCP_MEDIA_CTRL_RSP_CMD_CANNOT_BE_COMPLETED      (0x04)  /* The requested action of any Media Control Operation
                                                                 * cannot be completed successfully because of
                                                                 * a condition within the player.
                                                                 */

/* Types the Media Control Events */
typedef enum
{
  MCP_SRV_TRACK_POSITION_REQ_EVT,       /* Track Position has been requested from remote MCP Client.
                                         * The Track Position field in the pInfo of MCP_SRV_TrackPositionReq_Evt_t
                                         * type shall be setting to the current Track Position.
                                         */
  MCP_SRV_TRACK_POSITION_UPDATED_EVT,   /* Track Position has been received from remote MCP Client.
                                         * The MCP Server may adjust the current playing position of the current track
                                         * thanks to the MCP_SERVER_SetTrackPosition() function.
                                         * If the value is zero or greater, then the current playing position shall be
                                         * set to the offset from the start of the track.
                                         * If the value is less than zero, then the current playing position shall be
                                         * set to the offset from the end of the track and the value of the
                                         * Track Position specified by MCP_SERVER_SetTrackPosition() shall be set to
                                         * the offset from the start of the track to the new playing position.
                                         * If the value written by the MCP Client  does not correspond to a valid
                                         * track position, the server shall set the Track Position to a valid value.
                                         * The pInfo field indicates updated Track Position through the
                                         * MCP_SRV_UpdatedTrackPosition_Evt_t type.
                                         */

  MCP_SRV_PLAYBACK_SPEED_WRITTEN_EVT,   /* Playback Speed has been received from remote MCP Client.
                                         * The MCP Server shall adjust the playback speed of the current track.
                                         * If the MCP Server supports the value written, the MCP Server shall set the
                                         * Playback Speed to the value written. If the MCP Server does not support the
                                         * value written, the server shall set the Playback Speed to a supported value.
                                         * If the value written is not supported and is greater than the existing
                                         * Playback Speed, then the server should set the Playback Speed to the next
                                         * higher supported playback speed. If the value written is not supported and is
                                         * less than the existing Playback Speed, then the server should set the
                                         * Playback Speed to the next lower supported playback speed.
                                         * The pInfo field indicates updated Playback Speed through the
                                         * MCP_SRV_PlaybackSpeed_Evt_t type.
                                         */

  MCP_SRV_PLAYING_ORDER_WRITTEN_EVT,    /*Playing Order has been received remote MCP Client.*/

  MCP_SRV_CTRL_REQ_EVT,                 /* Media Control Request has been received remote MCP Client.
                                         * The MCP Server shall accept or reject to the operation request thanks to the
                                         * MCP_SERVER_CtrlOpRsp() function and then perform the requested operation if
                                         * accepted.
                                         * The pInfo field indicates the requested operation and its associated value
                                         * through the MCP_SRV_MediaCtrlReq_Evt_t type.
                                         */
  MCP_CLT_GMP_INFO_EVT,                 /* This event is notified by MCP Client during the link up process for the
                                         * Generic Media Player.
                                         * The pInfo field indicates information through
                                         * the MCP_CLT_MediaPlayerInfo_Evt_t type.
                                         */
  MCP_CLT_MPI_INFO_EVT,                 /* This event is notified by MCP Client during the link up process for a
                                         * Media Player Instance.
                                         * Note this event is notified for each Media Player Instance
                                         * The pInfo field indicates information through
                                         * the MCP_CLT_MediaPlayerInfo_Evt_t type.
                                         */
  MCP_CLT_OPERATION_COMPLETE_EVT,       /* This event is notified when a MCP Client Operation is complete.
                                         * A new MCP Client operation could be started
                                         */
  MCP_CLT_OPERATION_TRANSMITTED_EVT,    /* This event is notified when a MCP Client Media Control Operation is
                                         * transmitted.
                                         * pInfo field indicates the transmitted Media Control Operation to the remote
                                         * Media Control Server through the MCP_CLT_MediaCtrlOpTransmitted_Evt_t type.
                                         * After receiving this event, a new MCP Client operation could be started.
                                         * A MCP_CLT_RESPONSE_EVT event should be received after this event.
                                         */
  MCP_CLT_RESPONSE_EVT,                 /* This event is notified once the response of a Media Control Operation is
                                         * received from the remote Media Control Server.
                                         * pInfo field indicates the information response from the remote
                                         * Media Control Server through the MCP_CLT_MediaCtrlRsp_Evt_t type.
                                         */
  MCP_CLT_MEDIA_PLAYER_NAME_EVT,        /* This event is notified by MCP when a Media Player Name is read
                                         * thanks to the MCP_CLIENT_ReadMediaPlayerName() function or if
                                         * the Media Player Name has changed in MCP Server side.
                                         * pInfo field indicates the Media Player Name through the
                                         *  MCP_CLT_MediaPlayerName_Evt_t type.
                                         */
  MCP_CLT_TRACK_CHANGED_EVT,            /* This event is notified by MCP when the track has changed in MCP Server side */
  MCP_CLT_TRACK_TITLE_EVT,              /* This event is notified by MCP when a Track Title is read thanks to the
                                         * MCP_CLIENT_ReadTrackTitle() function or if the Track Title has changed
                                         * in MCP Server side.
                                         * pInfo field indicates the Track Title through the MCP_CLT_TrackTitle_Evt_t type.
                                         */
  MCP_CLT_TRACK_DURATION_EVT,           /* This event is notified by MCP when a Track Duration is read thanks to the
                                         * MCP_CLIENT_ReadTrackDuration() function or if the Track Duration has changed
                                         * in MCP Server side.
                                         * pInfo field indicates the Track Duration through the
                                         *  MCP_CLT_TrackDuration_Evt_t type.
                                         */
  MCP_CLT_TRACK_POSITION_EVT,           /* This event is notified by MCP when a Track Position is read thanks to the
                                         * MCP_CLIENT_ReadTrackPosition() function or if the Track Position has changed
                                         * in MCP Server side.
                                         * pInfo field indicates the Track Position through the
                                         *  MCP_CLT_TrackPosition_Evt_t type.
                                         */
  MCP_CLT_PLAYBACK_SPEED_EVT,           /* This event is notified by MCP when a Playback Speed is read thanks to the
                                         * MCP_CLIENT_ReadPlaybackSpeed() function or if the Playback Speed has changed
                                         * in MCP Server side.
                                         * pInfo field indicates the Playback Speed through the
                                         *  MCP_CLT_PlaybackSpeed_Evt_t type.
                                         */
  MCP_CLT_SEEKING_SPEED_EVT,            /* This event is notified by MCP when a Seeking Speed is read thanks to the
                                         * MCP_CLIENT_ReadSeekingSpeed() function or if the Seeking Speed has changed
                                         * in MCP Server side.
                                         * pInfo field indicates the Seeking Speed through the
                                         *  MCP_CLT_SeekingSpeed_Evt_t type.
                                         */
  MCP_CLT_PLAYING_ORDER_EVT,            /* This event is notified by MCP when a Playing Order is read thanks to the
                                         * MCP_CLIENT_ReadPlayingOrder() function or if the Playing Order has changed
                                         * in MCP Server side.
                                         * pInfo field indicates the Seeking Speed through the
                                         *  MCP_CLT_PlayingOrder_Evt_t type.
                                         */
  MCP_CLT_PLAYING_ORDERS_SUPPORTED_EVT, /* This event is notified by MCP when a Supported Playing Orders is read
                                         * thanks to the MCP_CLIENT_ReadSupportedPlayingOrders() function.
                                         * pInfo field indicates the Supported Playing Orders through the
                                         *  MCP_CLT_SupportedPlayingOrders_Evt_t type.
                                         */
  MCP_MEDIA_STATE_EVT,                  /* This event is notified by MCP when the Media State is read thanks to the
                                         * MCP_CLIENT_ReadMediaState() function or if the Media State has changed
                                         * in MCP Server side.
                                         * The pInfo field indicates the Media State
                                         * through the MCP_MediaState_Evt_t type.
                                         */
  MCP_CLT_MEDIA_CTRL_OP_FEATURES_EVT,   /* This event is notified by MCP when the Media Control Features are read
                                         * thanks to the MCP_CLIENT_ReadMediaCtrlFeatures() function or if
                                         * the Media Control Features have changed in MCP Server side.
                                         * The pInfo field indicates the supported Media Control Features
                                         * through the MCP_CLT_MediaCtrlOpFeatures_Evt_t type.
                                         */

} MCP_NotCode_t;

/* Type associated to MCP_SRV_TRACK_POSITION_REQ_EVT event*/
typedef struct
{
  int32_t *TrackPosition;
}MCP_SRV_TrackPositionReq_Evt_t;

/* Type associated to MCP_SRV_TRACK_POSITION_UPDATED_EVT event*/
typedef struct
{
  int32_t TrackPosition;
}MCP_SRV_UpdatedTrackPosition_Evt_t;

/* Type associated to MCP_SRV_PLAYBACK_SPEED_WRITTEN_EVT event*/
typedef struct
{
  float PlaybackSpeed;
}MCP_SRV_PlaybackSpeed_Evt_t;

/* Type associated to MCP_SRV_PLAYING_ORDER_WRITTEN_EVT event*/
typedef struct
{
  MCP_PlayingOrder_t PlayingOrder;
}MCP_SRV_PlayingOrder_Evt_t;

/* Type associated to MCP_SRV_CTRL_REQ_EVT event:
 * If CtrlOp is :
 * - MCP_MEDIA_CTRL_PLAY: The media player shall start playing the current track and call MCP_SERVER_SetMediaState()
 *                        with State MCP_MEDIA_STATE_PLAYING.
 * - MCP_MEDIA_CTRL_PAUSE: * If the media state is in PLAYING state, the media player shall pause playing the current
 *                         track and call MCP_SERVER_SetMediaState() with State MCP_MEDIA_STATE_PAUSED.
 *                         * If the media state is in SEEKING state, the media player shall stop seeking, set the current
 *                         track and track position, thanks to the MCP_SERVER_SetTrackPosition(), as a result of seeking,
 *                         and call MCP_SERVER_SetMediaState() with State MCP_MEDIA_STATE_PAUSED.
 * - MCP_MEDIA_CTRL_FAST_REWIND: * If the media state is in PLAYING ot PAUSED state, the media player shall move the
 *                               current track position backwards thanks to MCP_SERVER_SetTrackPosition().The media
 *                               player should stop playing the current track, call MCP_SERVER_SetMediaState() with State
 *                               MCP_MEDIA_STATE_SEEKING and move the current track position backwards at a speed
 *                               determined by the implementation thanks to MCP_SERVER_SetSeekingSpeed().
 *                               * If the media state is in SEEKING state, the seeking speed should be adjusted in the
 *                               negative direction thanks to MCP_SERVER_SetSeekingSpeed(). The method by which the
 *                               media player handles multiple Fast Rewind request is left up to the implementation.
 * - MCP_MEDIA_CTRL_FAST_FORWARD: * If the media state is in PLAYING ot PAUSED state, the media player shall move the
 *                               current track position forward thanks to MCP_SERVER_SetTrackPosition().The media
 *                               player should stop playing the current track, call MCP_SERVER_SetMediaState() with State
 *                               MCP_MEDIA_STATE_SEEKING and move the current track position forward at a speed
 *                               determined by the implementation thanks to MCP_SERVER_SetSeekingSpeed().
 *                               * If the media state is in SEEKING state, the seeking speed should be adjusted in the
 *                               positive direction thanks to MCP_SERVER_SetSeekingSpeed(). The method by which the
 *                               media player handles multiple Fast Forward request is left up to the implementation.
 * - MCP_MEDIA_CTRL_STOP : The media player shall stop any activity  and call MCP_SERVER_SetMediaState() with State
 *                         MCP_MEDIA_STATE_PAUSED. The track position shall be set to the beginning of the current track
 *                         thanks to MCP_SERVER_SetTrackPosition().
 * - MCP_MEDIA_CTRL_MOVE_RELATIVE : The track position of the current track shall be set, thanks to the function
 *                                  MCP_SERVER_SetTrackPosition(), to the current track position in addition to the
 *                                  offset specified by the 'value' flag associated to the callback event. In case of
 *                                  the operation results negative track position, the track position shall be set to 0,
 *                                  and if the operation results in a track position past the end of the current track,
 *                                  the track position shall be set to the end of the track.
 * - MCP_MEDIA_CTRL_PREVIOUS_SEGMENT : The current track position shall be set, thanks to MCP_SERVER_SetTrackPosition(),
 *                                     to either the starting position of the previous segment of this track or the
 *                                     starting position of the current segment, as determined by the implementation.
 *                                     The resulting track state is left up to the implementation.
 * - MCP_MEDIA_CTRL_NEXT_SEGMENT : The current track position shall be set, thanks to MCP_SERVER_SetTrackPosition(),
 *                                 to either the starting position of the next segment of this track. If operation is
 *                                 received while in last segment, the current track position shall be set to the
 *                                 position of the end of the segment.
 *                                 The resulting track state is left up to the implementation.
 * - MCP_MEDIA_CTRL_FIRST_SEGMENT : The current track position shall be set, thanks to MCP_SERVER_SetTrackPosition(), to
 *                                  the starting position of the first segment of the track.
 *                                  The resulting track state is left up to the implementation.
 * - MCP_MEDIA_CTRL_LAST_SEGMENT : The current track position shall be set, thanks to MCP_SERVER_SetTrackPosition(), to
 *                                 the starting position of the last segment of the track.
 *                                 The resulting track state is left up to the implementation.
 * - MCP_MEDIA_CTRL_GOTO_SEGMENT : The current track position shall be set, thanks to MCP_SERVER_SetTrackPosition(), to
 *                                 the starting position of the segment number (n) of the track specified by 'value'
 *                                 flag associated to the callback event.
 *                                 If the value n is a positive number, setting the current track position is equivalent
 *                                 to sending the First Segment operation once, followed by the Next Segment n-1 times.
 *                                 If the value n is a negative number setting, the current track position is equivalent
 *                                 to sending the Last Segment operation once, followed by the Previous Segment n-1 times.
 *                                 The resulting track state is left up to the implementation.
 * - MCP_MEDIA_CTRL_PREVIOUS_TRACK : The current track shall be set to either the previous track based on the playing
 *                                  order or remain on the current track, as determined by the implementation.
 *                                  The Track Position shall be set, thanks to MCP_SERVER_SetTrackPosition(), to 0.
 *                                  The resulting track state is left up to the implementation.
 * - MCP_MEDIA_CTRL_NEXT_TRACK : The current track shall be set to the next track based on the playing order and
 *                                the Track Position shall be set, thanks to MCP_SERVER_SetTrackPosition(), to 0.
 *                               The resulting track state is left up to the implementation.
 * - MCP_MEDIA_CTRL_FIRST_TRACK : The current track shall be set to the first track based on the playing order and
 *                                the Track Position shall be set, thanks to MCP_SERVER_SetTrackPosition(), to 0.
 *                               The resulting track state is left up to the implementation.
 * - MCP_MEDIA_CTRL_LAST_TRACK : The current track shall be set to the last track based on the playing order and
 *                               the Track Position shall be set, thanks to MCP_SERVER_SetTrackPosition(), to 0.
 *                               The resulting track state is left up to the implementation.
 * - MCP_MEDIA_CTRL_GOTO_TRACK : The current track shall be set to track number, specified by 'value' flag associated
 *                               to the callback event, based on the playing order.
 *                               If the value n is a positive number, setting the current track is equivalent to sending
 *                               the First Track operation once, followed by the Next Track operation n-1 times.
 *                               If the value n is a negative number setting, the current track is equivalent to sending
 *                               the Last Trackoperation once, followed by the Previous Track operation n-1 times.
 *                               If the value n is zero, the current track shall not be changed.
 *                               The Track Position shall be set, thanks to MCP_SERVER_SetTrackPosition(), to 0.
 *                               The resulting track state is left up to the implementation.
 * - MCP_MEDIA_CTRL_PREVIOUS_GROUP : The current group shall be set to the previous group in the current parent group.
 *                                  The current track shall be set to the first track of the resulting current group.
 *                                  The Track Position shall be set, thanks to MCP_SERVER_SetTrackPosition(), to 0.
 *                                  The resulting track state is left up to the implementation.
 * - MCP_MEDIA_CTRL_NEXT_GROUP : The current group shall be set to the next group in the current parent group.
 *                               The current track shall be set to the first track of the resulting current group.
 *                               The Track Position shall be set, thanks to MCP_SERVER_SetTrackPosition(), to 0.
 *                               The resulting track state is left up to the implementation.
 * - MCP_MEDIA_CTRL_FIRST_GROUP : The current group shall be set to the first group in the current parent group.
 *                               The current track shall be set to the first track of the resulting current group.
 *                               The Track Position shall be set, thanks to MCP_SERVER_SetTrackPosition(), to 0.
 *                               The resulting track state is left up to the implementation.
 * - MCP_MEDIA_CTRL_LAST_GROUP : The current group shall be set to the last group in the current parent group.
 *                               The current track shall be set to the first track of the resulting current group.
 *                               The Track Position shall be set, thanks to MCP_SERVER_SetTrackPosition(), to 0.
 *                               The resulting track state is left up to the implementation.
 * - MCP_MEDIA_CTRL_GOTO_GROUP : The current group shall be set to group number, specified by 'value' flag associated
 *                               to the callback event, of the current parent group.
 *                               If the value n is a positive number, setting the current group is equivalent to sending
 *                               the First Group operation once, followed by the Next Group operation n-1 times.
 *                               If the value n is a negative number setting, the current group is equivalent to sending
 *                               the Last Group operation once, followed by the Previous Group operation n-1 times.
 *                               If the value n is zero, the current Group shall not be changed.
 *                               The resulting track state is left up to the implementation.
 */
typedef struct
{
  MCP_MediaControlOpcode_t      CtrlOp;
  int32_t                       Value;
}MCP_SRV_MediaCtrlReq_Evt_t;

/* Type associated to MCP_MEDIA_STATE_EVT event*/
typedef struct
{
  MCP_MediaState_t MediaState;
}MCP_MediaState_Evt_t;

/* Structure used in parameter when MCP_CLT_GMP_INFO_EVT and MCP_CLT_MPI_INFO_EVT events are notified*/
typedef struct
{
  MCP_OptionFeatures_t                  OptionFeaturesMask;     /* Mask of Optional Media Control features
                                                                 * supported on the remote Media Control Server
                                                                 */
  MCP_NotificationUpdateFeatures_t      NotifUpdateMask;        /* Mask of Notification update features
                                                                 * supported on the remote Media Control Server.
                                                                 * If not activated by the MCP Client, the update
                                                                 * notification is disabled by default.
                                                                 */
  uint16_t                              StartAttHandle;         /* ATT Start Handle of the Media Control Player
                                                                 * in the remote MCP Server
                                                                 */
  uint16_t                              EndAttHandle;           /* ATT End Handle of the Media Control Player
                                                                 * in the remote MCP Server
                                                                 */
} MCP_CLT_MediaPlayerInfo_Evt_t;

/* Type associated to MCP_CLT_OPERATION_TRANSMITTED_EVT event*/
typedef struct
{
  MCP_MediaControlOpcode_t      CtrlOp;
}MCP_CLT_MediaCtrlOpTransmitted_Evt_t;

/* Type associated to MCP_CLT_RESPONSE_EVT event*/
typedef struct
{
  MCP_MediaControlOpcode_t      CtrlOp;
  MCP_MediaControlRespCode_t    ResultCode;
}MCP_CLT_MediaCtrlRsp_Evt_t;

/* Structure used in parameter when MCP_CLT_MEDIA_PLAYER_NAME_EVT event is notified*/
typedef struct
{
  uint8_t       *pName;         /* Pointer on the Media Player Name string*/
  uint8_t       Offset;         /* Offset of the string*/
  uint8_t       NameLength;     /* length of the Media Player Name*/
} MCP_CLT_MediaPlayerName_Evt_t;

/* Structure used in parameter when MCP_CLT_TRACK_TITLE_EVT event is notified*/
typedef struct
{
  uint8_t       *pTitle;        /* Pointer on the Title string*/
  uint8_t       Offset;         /* Offset of the string*/
  uint8_t       TitleLength;    /* length of the Title*/
} MCP_CLT_TrackTitle_Evt_t;

/* Structure used in parameter when MCP_CLT_TRACK_DURATION_EVT event is notified*/
typedef struct
{
  int32_t       Duration;         /* length of current track in 0.01-second resolution*/
} MCP_CLT_TrackDuration_Evt_t;

/* Structure used in parameter when MCP_CLT_TRACK_POSITION_EVT event is notified*/
typedef struct
{
  int32_t       Position;         /* position of current track in track in 0.01-secondd resolution*/
} MCP_CLT_TrackPosition_Evt_t;

/* Structure used in parameter when MCP_CLT_PLAYBACK_SPEED_EVT event is notified*/
typedef struct
{
  float         Speed;         /* multiple of the playback speed*/
} MCP_CLT_PlaybackSpeed_Evt_t;

/* Structure used in parameter when MCP_CLT_SEEKING_SPEED_EVT event is notified*/
typedef struct
{
  int8_t         Speed;         /* Seeking speed of the current track. The value is the current multiple of the
                                 * real-time playback speed at which the track is being fast forwarded or fast rewound.
                                 * (“Real-time” is the point at which Playback Speed = 1).
                                 * Fast forward is a positive current track-seeking speed.
                                 * Fast rewind is a negative current track-seeking speed.
                                 */
} MCP_CLT_SeekingSpeed_Evt_t;

/* Structure used in parameter when MCP_CLT_PLAYING_ORDER_EVT event is notified*/
typedef struct
{
  MCP_PlayingOrder_t    PlayingOrder;
} MCP_CLT_PlayingOrder_Evt_t;

/* Structure used in parameter when MCP_CLT_PLAYING_ORDERS_SUPPORTED_EVT event is notified*/
typedef struct
{
  MCP_PlayingOrdersSupported_t PlayingOrdersMask;
} MCP_CLT_SupportedPlayingOrders_Evt_t;

/* Structure used in parameter when MCP_CLT_MEDIA_CTRL_OP_FEATURES_EVT event is notified*/
typedef struct
{
  MCP_MediaCtrlOpFeature_t      CtrlOpFeatureMask;
} MCP_CLT_MediaCtrlOpFeatures_Evt_t;


typedef struct
{
  MCP_NotCode_t EvtOpcode;              /* Opcode of the notified event */
  tBleStatus    Status;                 /* Status associated to the event */
  uint16_t      ConnHandle;             /* ACL Connection Handle associated to the event */
  uint8_t       ContentControlID;       /* Content Control ID of the Media Player
                                         * Meaningless for MCP_CLT_LINKUP_COMPLETE_EVT
                                         */
  uint8_t       *pInfo;                 /* Pointer on information associated to the event */
}MCP_Notification_Evt_t;

/* Media Control Server Configuration */
typedef struct
{
  uint8_t               MaxNumMPInstances;              /* Maximum Number of Media Player Instances */
  MCP_OptionFeatures_t  OptionFeatures;                 /* Mask indicating the MCP optional features to support*/
  uint8_t               MaxMediaPlayerNameLength;       /* Maximum Length of the Media Player Name */
  uint8_t               MaxTrackTitleLength;            /* Maximum Length of the Track Title */
  uint8_t               ReadLongEn;                     /* Read Long value procedure configuration :
                                                         * - supported (0x01)
                                                         * - unsupported (0x00)
                                                         */
  uint8_t               *pStartRamAddr;                 /* Start address of the RAM buffer allocated for MCP Server
                                                         * Context memory resource.
                                                         * It must be a 32bit aligned RAM area.
                                                         */
  uint16_t              RamSize;                        /* Size of the RAM allocated at pStartRamAddr
                                                         * pointer.
                                                         */
} MCP_SRV_Config_t;

/* Media Control Client Configuration */
typedef struct
{
  uint8_t               MaxNumMPInstPerConn;            /* Maximum Number of Media Player Instances
                                                         * per MCP connections
                                                         */

  uint8_t               *pStartRamAddr;                 /* Start address of the RAM buffer allocated for MCP Client
                                                         * Context memory resource.
                                                         * It must be a 32bit aligned RAM area.
                                                         */
  uint16_t              RamSize;                        /* Size of the RAM allocated at pStartRamAddr
                                                         * pointer.
                                                         */

} MCP_CLT_Config_t;

/* Structure used for Media Control Profile Configuration*/
typedef struct
{
  MCP_Role_t            Role;                           /* Media Control roles*/
  uint8_t               MaxNumBleLinks;                 /* Maximum number of BLE Links */
  MCP_SRV_Config_t      Srv;                            /* Media Control Server Configuration */
  MCP_CLT_Config_t      Clt;                            /* Media Control Client Configuration */
} MCP_Config_t;


#ifdef __cplusplus
}
#endif

#endif /* __MCP_TYPES_H */
