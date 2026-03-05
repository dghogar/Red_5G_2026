#ifndef TELIT_SDK_PRIVATE_DPM_H
#define TELIT_SDK_PRIVATE_DPM_H

#pragma pack( push, 1 )

/*****************************************************************************
 * QMI_DPM_OPEN_PORT (32 - 0x0020)
 */

/* In theory this should have an array of port_details, but at the moment
 * this is only needed to configure IPA when using QRTR, so it's kept simple
 */
typedef struct {
    uint8_t size;
    uint32_t ep_type;
    uint32_t iface_id;
    uint32_t consumer_pipe_num;
    uint32_t producer_pipe_num;
} QMI_DPM_generic_port_details_t;

#pragma pack( pop )

#endif  /* TELIT_SDK_PRIVATE_DPM_H */
