/**
  ******************************************************************************
  * @file    mcp.h
  * @author  MCD Application Team
  * @brief   This file contains definitions used for Media Control Profile
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
#ifndef __MCP_H
#define __MCP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ble_types.h"
#include "mcp_types.h"

/* Exported functions --------------------------------------------------------*/
/**
  * @brief  Set the Name of the Media Player
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  pName: pointer on the Media Player name
  * @param  NameLen: Media Player name length
  * @retval status of the operation
  */
tBleStatus MCP_SERVER_SetMediaPlayerName(uint8_t ContentControlID, uint8_t *pName, uint16_t NameLen);

/**
  * @brief  Notify the remote MCP Client that the track has changed.
  * @note   This function shall be called after all the information, which change during the transition between
  *         the current track and the next track, have been updated (Track Title, Track Duration,Track Position...)
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_SERVER_NotifyTrackChanged(uint8_t ContentControlID);

/**
  * @brief  Set the title of the current track.
  * @note   If the media player has no current track or the track title is unavailable, the Track Title parameter
  *         shall be a zero-length string.
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  pTitle: pointer on the Track Title (UTF-8 string.)
  * @param  TitleLen: Track Title length
  * @retval status of the operation
  */
tBleStatus MCP_SERVER_SetTrackTitle(uint8_t ContentControlID, uint8_t *pTitle, uint16_t TitleLen);

/**
  * @brief  Set the length of the current track.
  * @note   If the media player has no current track or the duration of the current track is unknown,
  *         the Track Duration value shall be 0xFFFFFFFF.Otherwise, the duration of the track shall be zero or greater.
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  Duration: duration of the track in 0.01-second resolution ( 32-bit signed integer).
  * @retval status of the operation
  */
tBleStatus MCP_SERVER_SetTrackDuration(uint8_t ContentControlID, int32_t Duration);

/**
  * @brief  Set the current track position of the current track.
  * @note   The Track Position corresponds to the offset from the start of the track to the current playing position.
  *         If the start of the track is not well defined (such as in a live stream), the server sets a starting
  *         position (where Track Position equals 0) or sets the value to UNAVAILABLE (0xFFFFFFFF).
  * @note   If the media player has no current track, the Track Position characteristic value shall be 0xFFFFFFFF.
  * @note   The Media Control Server shall call MCP_SERVER_SetTrackPosition() when the Track Position  value is changed,
  *         when the track is paused, or when the playback speed is changed.
  *         As an exception to this, to avoid an excessive number of notifications, the Track Position should not be
  *         notified when the Media State is set to “Playing” and playback happens at a constant speed.
  *         The Track Position should be notified when the Media State is set to “Seeking”. The notification
  *         frequency while seeking is determined by the server implementation.
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  Position: position of the track in 0.01-second resolution ( 32-bit signed integer).
  * @retval status of the operation
  */
tBleStatus MCP_SERVER_SetTrackPosition(uint8_t ContentControlID, int32_t Position);

/**
  * @brief  Set the Playback Speed.
  * @note  For streaming media, the playback speed may be fixed to 1x normal speed.
  * @param  ContentControlID: Content Control Identifier of the Media Player ID
  * @param  Speed: playback speed in the range from .25x normal speed up to nearly 4x normal speed.
  * @retval status of the operation
  */
tBleStatus MCP_SERVER_SetPlaybackSpeed(uint8_t ContentControlID, float Speed);

/**
  * @brief  Set the seeking speed of the current track.
  * @note   The seeking speed value is the current multiple of the real-time playback speed at which the track
  *         is being fast forwarded or fast rewound. (“Real-time” is the point at which Playback Speed  = 1).
  *         Fast forward is a positive current track-seeking speed.
  *         Fast rewind is a negative current track-seeking speed
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  SeekingSpeed: current multiple of the real-time playback speed.
  * @retval status of the operation
  */
tBleStatus MCP_SERVER_SetSeekingSpeed(uint8_t ContentControlID, int8_t SeekingSpeed);

/**
  * @brief  Set the playing order of the media player.
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  PlayingOrder: Playing Order.
  * @retval status of the operation
  */
tBleStatus MCP_SERVER_SetPlayingOrder(uint8_t ContentControlID, MCP_PlayingOrder_t PlayingOrder);

/**
  * @brief  Set the supported playing orders of the media player.
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  PlayingOrdersMask: Supported Playing Orders mask.
  * @retval status of the operation
  */
tBleStatus MCP_SERVER_SetSupportedPlayingOrders(uint8_t ContentControlID,MCP_PlayingOrdersSupported_t PlayingOrdersMask);

/**
  * @brief  Set the media state of the media player.
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  MediaState: Media State.
  * @retval status of the operation
  */
tBleStatus MCP_SERVER_SetMediaState(uint8_t ContentControlID, MCP_MediaState_t MediaState);

/**
  * @brief  Response to a Media Control Request
  * @param  ConnHandle: Connection Handle of the remote MCP Client
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  MediaCtrlOp: Media Control opcode.
  * @param  Response: Response of the operation :
  *                             BLE_STATUS_SUCCESS if action requested by the operation was completed successfully.
  *                             BLE_STATUS_FAILED if action requested by the operation cannot be completed successfully
  *                                               because of a condition within the player.
  * @retval status of the function
  */
tBleStatus MCP_SERVER_CtrlOpRsp(uint8_t ConnHandle,
                                uint8_t ContentControlID,
                                MCP_MediaControlOpcode_t MediaCtrlOp,
                                tBleStatus Response);


/**
  * @brief  Set the supported Media Control Features.
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  MediaCtrlOpFeatureMask: Mask of supported Media Control Operation Features.
  * @retval status of the operation
  */
tBleStatus MCP_SERVER_SetSupportedMediaCtrlFeatures(uint8_t ContentControlID,
                                                    MCP_MediaCtrlOpFeature_t MediaCtrlOpFeatureMask);

/**
  * @brief  Configure the Media Player Name update Notification
  * @note   The MCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @note   If notification is enabled, MCP_CLT_MEDIA_PLAYER_NAME_EVT event would be notified when
  *         Media Player Name change in MCP Server.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  Enabled: if 0, Media Player Name change in MCP Server is never notified.
  *                  if 1, Media Player Name change in MCP Server is notified.
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_ConfigureMediaPlayerNameNotification(uint16_t ConnHandle,uint8_t ContentControlID,uint8_t Enabled);

/**
  * @brief  Configure the Track Title update Notification
  * @note   The MCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @note   If notification is enabled, MCP_CLT_TRACK_TITLE_EVT event would be notified when
  *         the track title change in MCP Server.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  Enabled: if 0, Track Title change in MCP Server is never notified.
  *                  if 1, Track Title change in MCP Server is notified.
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_ConfigureTrackTitleNotification(uint16_t ConnHandle,uint8_t ContentControlID,uint8_t Enabled);

/**
  * @brief  Configure the Track Duration update Notification
  * @note   The MCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @note   If notification is enabled, MCP_CLT_TRACK_DURATION_EVT event would be notified when
  *         the track duration change in MCP Server.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  Enabled: if 0, Track Duration change in MCP Server is never notified.
  *                  if 1, Track Duration change in MCP Server is notified.
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_ConfigureTrackDurationNotification(uint16_t ConnHandle,uint8_t ContentControlID,uint8_t Enabled);

/**
  * @brief  Configure the Track Position update Notification
  * @note   The MCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @note   If notification is enabled, MCP_CLT_TRACK_POSITION_EVT event would be notified when
  *         the track position change in MCP Server.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  Enabled: if 0, Track Position change in MCP Server is never notified.
  *                  if 1, Track Position change in MCP Server is notified.
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_ConfigureTrackPositionNotification(uint16_t ConnHandle,uint8_t ContentControlID,uint8_t Enabled);

/**
  * @brief  Configure the Playback Speed update Notification
  * @note   The MCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @note   If notification is enabled, MCP_CLT_PLAYBACK_SPEED_EVT event would be notified when
  *         the Playback Speed change in MCP Server.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  Enabled: if 0, Playback Speed change in MCP Server is never notified.
  *                  if 1, Playback Speed change in MCP Server is notified.
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_ConfigurePlaybackSpeedNotification(uint16_t ConnHandle,uint8_t ContentControlID,uint8_t Enabled);

/**
  * @brief  Configure the Seeking Speed update Notification
  * @note   The MCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @note   If notification is enabled, MCP_CLT_SEEKING_SPEED_EVT event would be notified when
  *         the Seeking Speed change in MCP Server.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  Enabled: if 0, Seeking Speed change in MCP Server is never notified.
  *                  if 1, Seeking Speed change in MCP Server is notified.
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_ConfigureSeekingSpeedNotification(uint16_t ConnHandle,uint8_t ContentControlID,uint8_t Enabled);

/**
  * @brief  Configure the Playing Order update Notification
  * @note   The MCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @note   If notification is enabled, MCP_CLT_PLAYING_ORDER_EVT event would be notified when
  *         the Playing Order change in MCP Server.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  Enabled: if 0, Playing Order change in MCP Server is never notified.
  *                  if 1, Playing Order change in MCP Server is notified.
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_ConfigurePlayingOrderNotification(uint16_t ConnHandle,uint8_t ContentControlID,uint8_t Enabled);

/**
  * @brief  Configure the Supported Media Control Operations update Notification
  * @note   The MCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @note   If notification is enabled, MCP_CLT_MEDIA_CTRL_OP_FEATURES_EVT event would be notified when
  *         the Supported Media Control Operations change in MCP Server.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  Enabled: if 0, Supported Media Control Operations change in MCP Server is never notified.
  *                  if 1, Supported Media Control Operations change in MCP Server is notified.
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_ConfigureMediaCtrlOpSupportedNotification(uint16_t ConnHandle,
                                                                uint8_t ContentControlID,
                                                                uint8_t Enabled);

/**
  * @brief  Read the Media Player Name
  * @note   The MCP_CLT_MEDIA_PLAYER_NAME_EVT event will be notified to report the operation result.
  * @note   The MCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_ReadMediaPlayerName(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Read the Track Title
  * @note   The MCP_CLT_TRACK_TITLE_EVT event will be notified to report the operation result.
  * @note   The MCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_ReadTrackTitle(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Read the Track Duration
  * @note   The MCP_CLT_TRACK_DURATION_EVT event will be notified to report the operation result.
  * @note   The MCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_ReadTrackDuration(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Read the Track Position
  * @note   The MCP_CLT_TRACK_POSITION_EVT event will be notified to report the operation result.
  * @note   The MCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_ReadTrackPosition(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Read the Playback Speed
  * @note   The MCP_CLT_PLAYBACK_SPEED_EVT event will be notified to report the operation result.
  * @note   The MCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_ReadPlaybackSpeed(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Read the Seeking Speed
  * @note   The MCP_CLT_SEEKING_SPEED_EVT event will be notified to report the operation result.
  * @note   The MCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_ReadSeekingSpeed(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Read the Playing Order
  * @note   The MCP_CLT_PLAYING_ORDER_EVT event will be notified to report the operation result.
  * @note   The MCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_ReadPlayingOrder(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Read the Supported Playing Orders
  * @note   The MCP_CLT_PLAYING_ORDERS_SUPPORTED_EVT event will be notified to report the operation result.
  * @note   The MCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_ReadSupportedPlayingOrders(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Read the Media State
  * @note   The MCP_MEDIA_STATE_EVT event will be notified to report the operation result.
  * @note   The MCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_ReadMediaState(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Read the Media Control Features
  * @note   The MCP_CLT_MEDIA_CTRL_OP_FEATURES_EVT event will be notified to report the operation result.
  * @note   The MCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_ReadMediaCtrlFeatures(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Set the current track position on the remote MCP Server to an absolute position
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  Position: Position of the track in 0.01-second resolution (offset from the start of the track)
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_SetAbsoluteTrackPosition(uint16_t ConnHandle,uint8_t ContentControlID, int32_t Position);

/**
  * @brief  Set the Playback Speed of the current track on the remote MCP Server
  * @note   For streaming media, the playback speed may be fixed to 1x normal speed.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  Speed: playback speed in the range from .25x normal speed up to nearly 4x normal speed.
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_SetPlaybackSpeed(uint16_t ConnHandle,uint8_t ContentControlID, float Speed);

/**
  * @brief  Set the Playing Order on the remote MCP Server
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  PlayingOrder: Playing Order.
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_SetPlayingOrder(uint16_t ConnHandle,uint8_t ContentControlID, MCP_PlayingOrder_t PlayingOrder);

/**
  * @brief  Request the remote MCP Server to start playing the current track
  * @note   The MCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the control operation request is
  *         transmitted to the remote MCP Server.
  * @note   The MCP_CLT_RESPONSE_EVT event will be notified once the response of the control request is received
  *         from the remote MCP Server for operation MCP_MEDIA_CTRL_PLAY.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_PlayTrack(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Request the remote MCP Server to pause the playing current track
  * @note   The MCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the control operation request is
  *         transmitted to the remote MCP Server.
  * @note   The MCP_CLT_RESPONSE_EVT event will be notified once the response of the control request is received
  *         from the remote MCP Server for operation MCP_MEDIA_CTRL_PAUSE.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_PauseTrack(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Request the remote MCP Server to fast rewind the current track
  * @note   The MCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the control operation request is
  *         transmitted to the remote MCP Server.
  * @note   The MCP_CLT_RESPONSE_EVT event will be notified once the response of the control request is received
  *         from the remote MCP Server for operation MCP_MEDIA_CTRL_FAST_REWIND.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_FastRewindTrack(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Request the remote MCP Server to fast forward the current track
  * @note   The MCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the control operation request is
  *         transmitted to the remote MCP Server.
  * @note   The MCP_CLT_RESPONSE_EVT event will be notified once the response of the control request is received
  *         from the remote MCP Server for operation MCP_MEDIA_CTRL_FAST_FORWARD.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_FastForwardTrack(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Request the remote MCP Server to stop current activity (Playing, Paused, Seeking) and return to
  *         the paused state and set the current track position to the start of the current track.
  * @note   The MCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the control operation request is
  *         transmitted to the remote MCP Server.
  * @note   The MCP_CLT_RESPONSE_EVT event will be notified once the response of the control request is received
  *         from the remote MCP Server for operation MCP_MEDIA_CTRL_STOP.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_StopTrack(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Set the current track position on the remote MCP Server to a position relative to the current track position.
  * @note   The MCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the control operation request is
  *         transmitted to the remote MCP Server.
  * @note   The MCP_CLT_RESPONSE_EVT event will be notified once the response of the control request is received
  *         from the remote MCP Server for operation MCP_MEDIA_CTRL_MOVE_RELATIVE.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  Position: Position of the track in 0.01-second resolution (offset from the current position of the track)
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_SetRelativeTrackPosition(uint16_t ConnHandle,uint8_t ContentControlID, int32_t Position);

/**
  * @brief  Request the remote MCP Server to move to the previous segment within the current track.
  * @note   The MCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the control operation request is
  *         transmitted to the remote MCP Server.
  * @note   The MCP_CLT_RESPONSE_EVT event will be notified once the response of the control request is received
  *         from the remote MCP Server for operation MCP_MEDIA_CTRL_PREVIOUS_SEGMENT.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_MovePreviousSegment(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Request the remote MCP Server to move to the next segment within the current track.
  * @note   The MCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the control operation request is
  *         transmitted to the remote MCP Server.
  * @note   The MCP_CLT_RESPONSE_EVT event will be notified once the response of the control request is received
  *         from the remote MCP Server for operation MCP_MEDIA_CTRL_NEXT_SEGMENT.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_MoveNextSegment(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Request the remote MCP Server to move to the first segment within the current track.
  * @note   The MCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the control operation request is
  *         transmitted to the remote MCP Server.
  * @note   The MCP_CLT_RESPONSE_EVT event will be notified once the response of the control request is received
  *         from the remote MCP Server for operation MCP_MEDIA_CTRL_FIRST_SEGMENT.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_MoveFirstSegment(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Request the remote MCP Server to move to the last segment within the current track.
  * @note   The MCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the control operation request is
  *         transmitted to the remote MCP Server.
  * @note   The MCP_CLT_RESPONSE_EVT event will be notified once the response of the control request is received
  *         from the remote MCP Server for operation MCP_MEDIA_CTRL_LAST_SEGMENT.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_MoveLastSegment(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Request the remote MCP Server to move to the specified segment within the current track.
  * @note   If the Segment number (n) parameter is a positive number, setting the current track position is equivalent
  *         to request Move to First Segment once, followed by the Next Segment operation n-1 times.
  *         If the Segment number (n) parameter is a negative number setting, the current track position is equivalent
  *         to request Move Last Segment once, followed by the Previous Segment operation n-1 times.
  *         If the value Segment number (n) parameter is zero, the current track position shall not change.
  * @note   The MCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the control operation request is
  *         transmitted to the remote MCP Server.
  * @note   The MCP_CLT_RESPONSE_EVT event will be notified once the response of the control request is received
  *         from the remote MCP Server for operation MCP_MEDIA_CTRL_GOTO_SEGMENT.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  SegmentNumber: Segment number.
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_MoveSegmentNumber(uint16_t ConnHandle,uint8_t ContentControlID, int32_t SegmentNumber);

/**
  * @brief  Request the remote MCP Server to move to the previous track.
  * @note   The MCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the control operation request is
  *         transmitted to the remote MCP Server.
  * @note   The MCP_CLT_RESPONSE_EVT event will be notified once the response of the control request is received
  *         from the remote MCP Server for operation MCP_MEDIA_CTRL_PREVIOUS_TRACK.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_MovePreviousTrack(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Request the remote MCP Server to move to the next track.
  * @note   The MCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the control operation request is
  *         transmitted to the remote MCP Server.
  * @note   The MCP_CLT_RESPONSE_EVT event will be notified once the response of the control request is received
  *         from the remote MCP Server for operation MCP_MEDIA_CTRL_NEXT_TRACK.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_MoveNextTrack(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Request the remote MCP Server to move to the first track within the current group.
  * @note   The MCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the control operation request is
  *         transmitted to the remote MCP Server.
  * @note   The MCP_CLT_RESPONSE_EVT event will be notified once the response of the control request is received
  *         from the remote MCP Server for operation MCP_MEDIA_CTRL_FIRST_TRACK.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_MoveFirstTrack(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Request the remote MCP Server to move to the last track within the current group.
  * @note   The MCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the control operation request is
  *         transmitted to the remote MCP Server.
  * @note   The MCP_CLT_RESPONSE_EVT event will be notified once the response of the control request is received
  *         from the remote MCP Server for operation MCP_MEDIA_CTRL_LAST_TRACK.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_MoveLastTrack(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Request the remote MCP Server to move to the specified track within the current group.
  * @note   If the Track number (n) parameter is a positive number, setting the current track position is equivalent
  *         to request Move to First Track once, followed by the Next Track operation n-1 times.
  *         If the Track number (n) parameter is a negative number setting, the current track position is equivalent
  *         to request Move Last Track once, followed by the Previous Track operation n-1 times.
  *         If the value Track number (n) parameter is zero, the current track position shall not change.
  * @note   The MCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the control operation request is
  *         transmitted to the remote MCP Server.
  * @note   The MCP_CLT_RESPONSE_EVT event will be notified once the response of the control request is received
  *         from the remote MCP Server for operation MCP_MEDIA_CTRL_GOTO_TRACK.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  TrackNumber: Track number.
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_MoveTrackNumber(uint16_t ConnHandle,uint8_t ContentControlID, int32_t TrackNumber);

/**
  * @brief  Request the remote MCP Server to move to the previous group.
  * @note   The MCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the control operation request is
  *         transmitted to the remote MCP Server.
  * @note   The MCP_CLT_RESPONSE_EVT event will be notified once the response of the control request is received
  *         from the remote MCP Server for operation MCP_MEDIA_CTRL_PREVIOUS_GROUP.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_MovePreviousGroup(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Request the remote MCP Server to move to the next group.
  * @note   The MCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the control operation request is
  *         transmitted to the remote MCP Server.
  * @note   The MCP_CLT_RESPONSE_EVT event will be notified once the response of the control request is received
  *         from the remote MCP Server for operation MCP_MEDIA_CTRL_NEXT_GROUP.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_MoveNextGroup(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Request the remote MCP Server to move to the first group.
  * @note   The MCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the control operation request is
  *         transmitted to the remote MCP Server.
  * @note   The MCP_CLT_RESPONSE_EVT event will be notified once the response of the control request is received
  *         from the remote MCP Server for operation MCP_MEDIA_CTRL_FIRST_GROUP.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_MoveFirstGroup(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Request the remote MCP Server to move to the last group.
  * @note   The MCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the control operation request is
  *         transmitted to the remote MCP Server.
  * @note   The MCP_CLT_RESPONSE_EVT event will be notified once the response of the control request is received
  *         from the remote MCP Server for operation MCP_MEDIA_CTRL_LAST_GROUP.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_MoveLastGroup(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Request the remote MCP Server to move to the specified group.
  * @note   If the Group number (n) parameter is a positive number, setting the current track position is equivalent
  *         to request Move to First Group once, followed by the Next Group operation n-1 times.
  *         If the Group number (n) parameter is a negative number setting, the current group position is equivalent
  *         to request Move Last Group once, followed by the Previous Group operation n-1 times.
  *         If the value Group number (n) parameter is zero, the current group position shall not change.
  * @note   The MCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the control operation request is
  *         transmitted to the remote MCP Server.
  * @note   The MCP_CLT_RESPONSE_EVT event will be notified once the response of the control request is received
  *         from the remote MCP Server for operation MCP_MEDIA_CTRL_GOTO_GROUP.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param  GroupNumber: Group number.
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_MoveGroupNumber(uint16_t ConnHandle,uint8_t ContentControlID, int32_t GroupNumber);

/**
  * @brief  Check if the specified CCID corresponds to a Media Player discovered on the remote Media Control Server.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @retval if status is BLE_STATUS_SUCCESS, specified connection handle and Content Control ID corresponds to a
  *         discovered Media Player, else not.
  */
tBleStatus MCP_CLIENT_IsDiscoveredMediaPlayer(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Get the MCP Option features and MCP Notifications features supported by the specified Media Player
  *         on the remote Media Control Server.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Media Player
  * @param[out] FeaturesMask : returned Mask of Media Player features supported on the remote Media Control Server
  * @param[out] NotifUpdateMask : returned Mask of Media Player Notifications features supported on the remote
  *                             Media Control Server
  * @retval if status is BLE_STATUS_SUCCESS, specified connection handle and Content Control ID corresponds to a
  *         discovered Media Player, else not.
  */
tBleStatus MCP_CLIENT_GetMediaPlayerFeatures(uint16_t ConnHandle,
                                            uint8_t ContentControlID,
                                            MCP_OptionFeatures_t *FeaturesMask,
                                            MCP_NotificationUpdateFeatures_t *NotifUpdateMask);

/**
  * @brief  Remove the record of the Media Control Profile Client stored in the Non Volatile memory.
  * @param  PeerIdentityAddressType: Identity address type.
  *                                  Values:
  *                                     - 0x00: Public Identity Address
  *                                     - 0x01: Random (static) Identity Address
  * @param  PeerIdentityAddress : Public or Random (static) Identity address of the peer device
  * @retval status of the operation
  */
tBleStatus MCP_CLIENT_RemoveRecord(uint8_t PeerIdentityAddressType,const uint8_t PeerIdentityAddress[6]);

#ifdef __cplusplus
}
#endif

#endif /* __MCP_H */
