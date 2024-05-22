/**
 * @file zcl.tunnel.h
 * @heading Tunneling
 * @brief ZCL Tunneling cluster header
 * ZCL 7 section 10.6
 * ZCL 8 section 10.6
 * @copyright Copyright [2009 - 2023] Exegin Technologies Limited. All rights reserved.
 */

/* EXEGIN - removed '@'PICS escape sequence, since these are not the
 * official PICS codes. These were added with the assumption that
 * SE clusters are being brought into the Zigbee 3.0 fold and will
 * use the same PICS codes and PICS format style. However, this has
 * yet to occur. */

/* ZCL.Tunnel
 * SETUN.S | Server | True
 * SETUN.C | Client | True
 *
 * Server Attributes
 * SETUN.S.A0000 | CloseTunnelTimeout | True
 * SETUN.S.Afffd | ClusterRevision | True
 * SETUN.S.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * SETUN.S.C00.Rsp | RequestTunnel | True
 * SETUN.S.C01.Rsp | CloseTunnel | True
 * SETUN.S.C02.Rsp | TransferData | True
 * SETUN.S.C03.Rsp | TransferDataError | True
 * SETUN.S.C04.Rsp | AckTransferData | False | Optional
 * SETUN.S.C05.Rsp | ReadyData | False | Optional
 * SETUN.S.C06.Rsp | GetSupportedTunnelProtocols | True | Optional
 *
 * Commands Generated
 * SETUN.S.C00.Tx | RequestTunnelResponse | True
 * SETUN.S.C01.Tx | TransferData | True
 * SETUN.S.C02.Tx | TransferDataError | True
 * SETUN.S.C03.Tx | AckTransferData | True | Optional
 * SETUN.S.C04.Tx | ReadyData | False | Optional
 * SETUN.S.C05.Tx | Supported Tunnel Protocols Response | True | Optional
 * SETUN.S.C06.Tx | TunnelClosureNotification | False | Optional
 *
 * Client Attributes
 * SETUN.C.Afffd | ClusterRevision | True
 * SETUN.C.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * SETUN.C.C00.Rsp | RequestTunnelResponse | True
 * SETUN.C.C01.Rsp | TransferData | True
 * SETUN.C.C02.Rsp | TransferDataError | True
 * SETUN.C.C03.Rsp | AckTransferData | False | Optional
 * SETUN.C.C04.Rsp | ReadyData | False | Optional
 * SETUN.C.C05.Rsp | Supported Tunnel Protocols Response | True | Optional
 * SETUN.C.C06.Rsp | TunnelClosureNotification | True | Optional
 *
 * Commands Generated
 * SETUN.C.C00.Tx | RequestTunnel | True
 * SETUN.C.C01.Tx | CloseTunnel | True
 * SETUN.C.C02.Tx | TransferData | True
 * SETUN.C.C03.Tx | TransferDataError | True
 * SETUN.C.C04.Tx | AckTransferData | True | Optional
 * SETUN.C.C05.Tx | ReadyData | False | Optional
 * SETUN.C.C06.Tx | GetSupportedTunnelProtocols | True | Optional
 */

#ifndef ZCL_TUNNEL_H
# define ZCL_TUNNEL_H

#include "zcl/zcl.h"

/** Manufacturer code unused. */
#define ZCL_TUNNEL_MANUFACTURER_CODE_UNUSED                 0xffffU

/** Tunneling Server Attribute IDs */
enum ZbZclTunnelSvrAttrT {
    ZCL_TUNNEL_SVR_ATTR_TIMEOUT = 0x0000, /**< CloseTunnelTimeout */
    /* discontinuity */
    ZCL_TUNNEL_SVR_ATTR_NUM_TUNNELS = 0x7ffeU,
    /**< This is an internal attribute that can be used by the application to set the number of tunnels that
     * tunnel server shall support. After this, the server shall return NO_RESOURCES error for any request
     * to create the tunnel. by default it is set to ZCL_TUNNEL_SVR_NUM_TUNNELS_MAX. */
    ZCL_TUNNEL_SVR_ATTR_PERSIST = 0x7fffU
        /**< This is an internal attribute used to read/write the tunnel data from/to tunnel server. */
};

/* Tunnel Server Generated Commands */
enum ZbZclTunnelServerCommandT {
    ZCL_TUNNEL_SVR_CMD_RESPONSE = 0x00,
    ZCL_TUNNEL_SVR_CMD_DATA = 0x01,
    ZCL_TUNNEL_SVR_CMD_ERROR = 0x02,
    ZCL_TUNNEL_SVR_CMD_ACK = 0x03,
    ZCL_TUNNEL_SVR_CMD_READY = 0x04,
    ZCL_TUNNEL_SVR_CMD_SUPPORTED_RSP = 0x05,
    ZCL_TUNNEL_SVR_CMD_CLOSE_NOTIFY = 0x06
};

/* Tunnel Client Generated Commands */
enum ZbZclTunnelClientCommandT {
    ZCL_TUNNEL_CLI_CMD_REQUEST = 0x00,
    ZCL_TUNNEL_CLI_CMD_CLOSE = 0x01,
    ZCL_TUNNEL_CLI_CMD_DATA = 0x02,
    ZCL_TUNNEL_CLI_CMD_ERROR = 0x03,
    ZCL_TUNNEL_CLI_CMD_ACK = 0x04, /* for flow control */
    ZCL_TUNNEL_CLI_CMD_READY = 0x05, /* for flow control */
    ZCL_TUNNEL_CLI_CMD_SUPPORTED_REQ = 0x06
};

/** Tunneling Protocol ID enumerations */
enum ZbZclTunnelProtocolT {
    ZCL_TUNNEL_PROTO_DLMS_COSEM = 0, /**< DLMS/COSEM (IEC 62056) */
    ZCL_TUNNEL_PROTO_IEC_61107 = 1, /**< IEC 61107 */
    ZCL_TUNNEL_PROTO_ANSI_C12 = 2, /**< ANSI C12 */
    ZCL_TUNNEL_PROTO_M_BUS = 3, /**< M-BUS */
    ZCL_TUNNEL_PROTO_SML = 4, /**< SML */
    ZCL_TUNNEL_PROTO_CLIMATE_TALK = 5, /**< ClimateTalk */
    ZCL_TUNNEL_PROTO_GB_HRGP = 6, /**< GB-HRGP */
    ZCL_TUNNEL_PROTO_IPV4 = 7, /**< IP v4 */
    ZCL_TUNNEL_PROTO_IPV6 = 8, /**< IP v6 */
    ZCL_TUNNEL_PROTO_MANUFACTURER = 200, /**< Manufacturer-defined protocols */
    ZCL_TUNNEL_PROTO_RESERVED = 0xff /**< Reserved */
};

/** Tunneling Status Values */
enum ZbZclTunnelStatusT {
    ZCL_TUNNEL_STATUS_SUCCESS = 0x00, /**< Success */
    ZCL_TUNNEL_STATUS_BUSY = 0x01, /**< Busy */
    ZCL_TUNNEL_STATUS_NO_RESOURCES = 0x02, /**< No more tunnel IDs */
    ZCL_TUNNEL_STATUS_PROTO_UNSUPPORTED = 0x03, /**< Protocol not supported */
    ZCL_TUNNEL_STATUS_FLOW_UNSUPPORTED = 0x04 /**< Flow control not supported */
};

/** Tunneling Transfer Data Status Values */
enum ZbZclTunnelXferStatusT {
    ZCL_TUNNEL_XFER_STATUS_NO_TUNNEL = 0x00, /**< No such tunnel */
    ZCL_TUNNEL_XFER_STATUS_WRONG_DEVICE = 0x01, /**< Wrong device */
    ZCL_TUNNEL_XFER_STATUS_OVERFLOW = 0x02, /**< Data overflow */
    ZCL_TUNNEL_XFER_STATUS_NO_RESPONSE = 0xff /**< No Response - If zcl_tunnel_handle_data returns
     * ZCL_TUNNEL_XFER_STATUS_NO_RESPONSE, don't send a response
     */
};

/** Request Tunnel command structure */
struct ZbZclTunnelConnectT {
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD via a Mirror device. */
};

/** Close Tunnel command structure */
struct ZbZclTunnelCloseT {
    uint8_t mirror_ep; /**< ESI Mirror endpoint to send Tunnel data in case the
     * destination is BOMD and to_bomd flag is set to 'true'. */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD via a Mirror device. */
};

/** Tunnel Data command structure */
struct ZbZclTunnelDataT {
    uint8_t *data; /**< Pointer to the data being sent over the Tunnel. */
    uint16_t len; /**< Data length. */
    uint8_t mirror_ep; /**< ESI Mirror endpoint to send Tunnel data in case the
     * destination is BOMD and to_bomd flag is set to 'true'. */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD via a Mirror device. */
};

/** Get Supported Protocols command structure */
struct ZbZclTunnelGetSupportedProtocolsT {
    uint8_t protocol_offset; /**< Protocol offset */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD via a Mirror device. */
};

struct ZbZclTunnelStateT;

/** ZbZclTunnelServerAddProto and ZbZclTunnelClientAddProto callback data structure */
struct ZbZclTunnelProtoCbT {
    enum ZbZclTunnelStatusT (*request)(struct ZbZclClusterT *clusterPtr,
        struct ZbZclTunnelStateT *statePtr, void *priv);
    /**< Callback to the request handler. Only applicable for Tunnel Server cluster. */
    void (*input)(struct ZbZclClusterT *clusterPtr, struct ZbZclTunnelStateT *statePtr, void *priv);
    /**< Callback to the input handler */
    void (*close)(struct ZbZclClusterT *clusterPtr, struct ZbZclTunnelStateT *statePtr, void *priv);
    /**< Callback to the close handler (Optional). Required if the application needs to keep track of
     * open tunnels, e.g., for persistence */
    bool (*error)(struct ZbZclClusterT *clusterPtr, struct ZbZclTunnelStateT *statePtr, void *priv,
        enum ZbZclTunnelXferStatusT status);
    /**< Callback to the error handler (Optional). To handle ZCL_TUNNEL_SVR_CMD_ERROR.
     * Return false if tunnel should be closed */
    void *priv;
    /**< Application private data pointer */
};

/* Tunneling Server */

/**
 * Create a new instance of the Tunneling Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclTunnelServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Add a protocol tunnel to the server
 * @param cluster Cluster instance from which to send this command
 * @param protocol Protocol ID enumeration
 * @param mfr Manufacturer ID
 * @param mtu Protocol's MTU
 * @param callbacks Protocol callbacks
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclTunnelServerAddProto(struct ZbZclClusterT *cluster, enum ZbZclTunnelProtocolT protocol,
    uint16_t mfr, uint16_t mtu, struct ZbZclTunnelProtoCbT *callbacks);

/**
 * Get tunnel server state by tunnel ID
 * @param cluster Cluster instance from which to send this command
 * @param tunnel_id Tunnel ID
 * @return State of the tunnel
 */
struct ZbZclTunnelStateT * ZbZclTunnelServerStateFindById(struct ZbZclClusterT *cluster, uint16_t tunnel_id);

/* Tunneling Client */

/**
 * Create a new instance of the Tunneling Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclTunnelClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Add a protocol tunnel to the server
 * @param cluster Cluster instance from which to send this command
 * @param protocol Protocol ID enumeration
 * @param mfr Manufacturer ID
 * @param mtu Protocol's MTU
 * @param callbacks Protocol callbacks
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclTunnelClientAddProto(struct ZbZclClusterT *cluster, enum ZbZclTunnelProtocolT protocol,
    uint16_t mfr, uint16_t mtu, struct ZbZclTunnelProtoCbT *callbacks);

/**
 * Send a Request Tunnel command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command
 * @param connect Request Tunnel command structure.
 * @param callback Callback function to handle response
 * @param state Tunneling Cluster State structure
 * @param status Status of the tunnel
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclTunnelClientConnectReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclTunnelConnectT *connect,
    void (*callback)(struct ZbZclClusterT *cluster, struct ZbZclTunnelStateT *state,
        enum ZbZclTunnelStatusT status, void *arg), void *arg);

/**
 * Send a Close Tunnel command
 * @param cluster Cluster instance from which to send this command
 * @param close Close Tunnel command structure.
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclTunnelClientCloseReq(struct ZbZclClusterT *cluster, struct ZbZclTunnelCloseT *close,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/**
 * Close the local tunnel without informing the server
 * @param cluster Cluster instance from which to send this command
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclTunnelClientCloseQuietReq(struct ZbZclClusterT *cluster);

/**
 * Send a Get Supported Protocols command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for this command.
 * @param suppported_proto Get Supported Protocol command structure.
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclTunnelClientGetSupportedProtocols(struct ZbZclClusterT *cluster,
    struct ZbApsAddrT *dst, struct ZbZclTunnelGetSupportedProtocolsT *suppported_proto,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/**
 * Send a Transfer Data command
 * @param cluster Cluster instance from which to send this command
 * @param data Tunnel Data information structure.
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclTunnelClientSendReq(struct ZbZclClusterT *cluster,
    struct ZbZclTunnelDataT *data,
    void (*callback)(struct ZbZclCommandRspT *rspPr, void *arg), void *arg);

/* Tunneling Helper Functions */

/**
 * Send data via the tunnel. If the input cluster is server then the data will be
 * sent to the client and vice versa.
 * @param cluster Cluster instance from which to send this command
 * @param state State of the tunnel
 * @param data Tunnel Data information structure.
 * @param callback Callback function to handle response
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclTunnelSendData(struct ZbZclClusterT *cluster,
    struct ZbZclTunnelStateT *state, struct ZbZclTunnelDataT *data,
    void (*callback)(struct ZbZclCommandRspT *rspPtr, void *arg), void *arg);

/**
 * Get ID using Tunneling Cluster State structure
 * @param state Tunneling Cluster State structure
 * @return Allocated tunnel ID
 */
uint16_t ZbZclTunnelStateGetId(struct ZbZclTunnelStateT *state);

/**
 * Get Tunneling Protocol Instance using Tunneling Cluster State structure
 * @param state Tunneling Cluster State structure
 * @return Tunneling Protocol Instance structure
 */
enum ZbZclTunnelProtocolT ZbZclTunnelStateGetProtocol(struct ZbZclTunnelStateT *state);

/**
 * Get received data using Tunneling Cluster State structure
 * @param state Tunneling Cluster State structure
 * @return Data receive buffer
 */
uint8_t * ZbZclTunnelStateGetDataPtr(struct ZbZclTunnelStateT *state);

/**
 * Get received data length using Tunneling Cluster State structure
 * @param state Tunneling Cluster State structure
 * @return Length of data in the buffer
 */
uint32_t ZbZclTunnelStateGetDataLen(struct ZbZclTunnelStateT *state, bool clear_data);

/*-----------------------------------------------------------------------------
 * Mirror Cluster
 *-----------------------------------------------------------------------------
 */

/**
 * Allocates a Tunneling Client/Server Mirror cluster. The caller (application) must
 *  attach any attributes to the cluster you want to mirror.
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param events_client Pointer to Events Client cluster. This is used by the
 *  Metering Mirror to send mirrored commands to the BOMD.
 * @param meter_mirror Pointer to Metering Mirror cluster. This is used to queue
 *  any commands that need to be forwarded to the BOMD when it wakes up.
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclTunnelMirrorAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclClusterT *tunnel, struct ZbZclClusterT *meter_mirror);

#endif
