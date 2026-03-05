#ifndef TQCM_CTL_H_
#define TQCM_CTL_H_

#include <telit_sdk.h>
#include <telit_sdk_ctl.h>
#include "tqcm.h"

/* The following are tqcm private functions, since the user should not interact
 * with the ctl service.
 *
 * API can be synchronous or asynchronous
 * synchronous: out should not be NULL, cb should be NULL
 * asynchronous: out should be NULL, cb should not be NULL
 *
 * return value
 *  0 if successful (synchronous)
 *  EINPROGRESS if successful (asynchronous)
 *  != 0 if failed
 *
 * request timeout is managed only in the synchronous way
 *
 * TODO: async timeout can be implemented with timers?
 */
int tqcm_ctl_get_client_id_send(tqcm_client_context_t *ctx,
        ctl_get_client_id_t *input,
        ctl_get_client_id_info_t *out,
        tqcm_callback_t cb,
        uint64_t token);

int tqcm_ctl_release_client_id_send(tqcm_client_context_t *ctx,
        ctl_get_client_id_info_t *input,
        ctl_get_client_id_info_t *out,
        tqcm_callback_t cb,
        uint64_t token);

int tqcm_ctl_get_version_info_send(tqcm_client_context_t *ctx,
        ctl_get_version_info_t *out,
        tqcm_callback_t cb,
        uint64_t token);

#endif  /* TQCM_CTL_H_ */
