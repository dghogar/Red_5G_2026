/* */
/* demo program for illustrating GobiNet driver usage */
/* */
/* (a) prepare a message */
/* (b) send to modem. */
/* (c) Wait for response from modem */
/* (d) Process received response message */
/* (e) close QMI service. */
/* */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "telit_sdk.h"
#include "telit_sdk_nas.h"

#define QMI_GET_SERVICE_FILE_IOCTL 0x8BE0 + 1

/* change this define if QMI device name is different */
#define GOBINET_DEVICE "/dev/qcqmi0"

#define SERVICE QMI_SVC_NAS
#define SVC_STR "NAS"

static int telit_client_fd_from_qcqmi(uint8_t svc, const char *qcqmi)
{
    int fd = open(qcqmi, O_RDWR);

    if (fd == -1) {
        RLOGE("%s: error in opening device %d - %s",
                __FUNCTION__, errno, strerror(errno));
        goto end;
    }

    if (ioctl(fd, QMI_GET_SERVICE_FILE_IOCTL, svc) != 0) {
        RLOGE("%s: error in ioctl", __FUNCTION__);
        close(fd);
        fd = -1;
    }

end:
    return fd;
}

int main()
{
    int svc = -1;
    pack_qmi_t req_ctx;
    unpack_qmi_t rsp_ctx;
    uint16_t xid = 1;
    int rtn;
    uint8_t rsp[QMI_MSG_MAX];
    uint8_t req[QMI_MSG_MAX];
    uint16_t req_size = QMI_MSG_MAX;
    uint16_t rsp_size = QMI_MSG_MAX;
    bool got_expected_message = false;
    nas_system_info_t out;

    memset(&out, 0, sizeof(out));
    memset(&req_ctx, 0, sizeof(req_ctx));
    req_ctx.xid = xid;

    telit_set_log_level(LOG_INFO);

    rtn = telit_nas_get_sys_info_pack(&req_ctx, req, &req_size);

    if (rtn != QMI_SDK_ERR_NONE) {
        RLOGE("pack fail %d ", rtn);
        return -1;
    }

    if ((svc = telit_client_fd_from_qcqmi(SERVICE, GOBINET_DEVICE)) < 0) {
        RLOGE("%s Service Not Supported!", SVC_STR);
        return -1;
    }

    rtn = write(svc, req, req_size);

    if (rtn != req_size) {
        RLOGE("%s socket write failed!", SVC_STR);
        goto close_fd;
    }

    while (!got_expected_message) {
        rtn = read(svc, rsp, QMI_MSG_MAX);

        if (rtn > 0) {
            rsp_size = rtn;
        }

        RLOGD("rtn = %d, rspLen = %u", rtn, rsp_size);

        if ((rtn > 0) && (rsp_size > 0)) {
            telit_helper_get_resp_ctx(SERVICE, rsp, rsp_size, &rsp_ctx);

            switch (rsp_ctx.type) {
                case QMI_MSG_RESPONSE:
                    RLOGD("%s response message, msgid 0x%x", SVC_STR, rsp_ctx.msg_id);

                    if (rsp_ctx.xid == xid) {
                        rtn = telit_nas_get_sys_info_unpack(rsp, rsp_size, &out);

                        if (rtn != QMI_SDK_ERR_NONE) {
                            RLOGE("%s unpack pack fail %d ", SVC_STR, rtn);
                        } else {
                            if (out.hdr_service_status_info_available) {
                                RLOGI("hdr preferred_data_path %u",
                                        out.hdr_service_status_info.preferred_data_path);
                                RLOGI("hdr service_status %u", out.hdr_service_status_info.service_status);
                            }
                            if (out.gsm_service_status_info_available) {
                                RLOGI("gsm preferred_data_path %u",
                                        out.gsm_service_status_info.preferred_data_path);
                                RLOGI("gsm service_status %u", out.gsm_service_status_info.service_status);
                                RLOGI("gsm true_service_status %u",
                                        out.gsm_service_status_info.true_service_status);
                            }
                            if (out.wcdma_service_status_info_available) {
                                RLOGI("wcdma preferred_data_path %u",
                                        out.wcdma_service_status_info.preferred_data_path);
                                RLOGI("wcdma service_status %u",
                                        out.wcdma_service_status_info.service_status);
                                RLOGI("wcdma true_service_status %u",
                                        out.wcdma_service_status_info.true_service_status);
                            }
                            if (out.lte_service_status_info_available) {
                                RLOGI("lte preferred_data_path %u",
                                        out.lte_service_status_info.preferred_data_path);
                                RLOGI("lte service_status %u", out.lte_service_status_info.service_status);
                                RLOGI("lte true_service_status %u",
                                        out.lte_service_status_info.true_service_status);
                            }
                            if (out.nr5g_service_status_info_available) {
                                RLOGI("nr5g preferred_data_path %u",
                                        out.nr5g_service_status_info.preferred_data_path);
                                RLOGI("nr5g service_status %u", out.nr5g_service_status_info.service_status);
                                RLOGI("nr5g true_service_status %u",
                                        out.nr5g_service_status_info.true_service_status);
                            }
                            if (out.HDR_sys_info_available) {
                                if (out.HDR_sys_info.srv_domain_available) {
                                    RLOGI("HDR srv_domain %u", out.HDR_sys_info.srv_domain);
                                }
                                if (out.HDR_sys_info.srv_capability_available) {
                                    RLOGI("HDR srv_capability %u", out.HDR_sys_info.srv_capability);
                                }
                                if (out.HDR_sys_info.roam_status_available) {
                                    RLOGI("HDR roam_status %u", out.HDR_sys_info.roam_status);
                                }
                                if (out.HDR_sys_info.is_sys_forbidden_available) {
                                    RLOGI("HDR is_sys_forbidden %u", out.HDR_sys_info.is_sys_forbidden);
                                }
                                if (out.HDR_sys_info.is_sys_prl_match_available) {
                                    RLOGI("HDR is_sys_prl_match %u", out.HDR_sys_info.is_sys_prl_match);
                                }
                                if (out.HDR_sys_info.hdr_personality_available) {
                                    RLOGI("HDR hdr_personality %u", out.HDR_sys_info.hdr_personality);
                                }
                                if (out.HDR_sys_info.hdr_active_prot_available) {
                                    RLOGI("HDR hdr_active_prot %u", out.HDR_sys_info.hdr_active_prot);
                                }
                                if (out.HDR_sys_info.is856_sys_id_available) {
                                    uint8_t i = 0;
                                    char log[32] = {0};
                                    for (i = 0; i < SYS_ID_SIZE; i++) {
                                        char tmp[4] = {0};
                                        RLOGI(tmp, "%02X ", out.HDR_sys_info.is856_sys_id[i]);
                                        strcat(log, tmp);
                                    }
                                    RLOGI("HDR is856_sys_id %s", log);
                                }
                            }
                            if (out.gsm_sys_info_available) {
                                if (out.gsm_sys_info.srv_domain_available) {
                                    RLOGI("GSM srv_domain %u", out.gsm_sys_info.srv_domain);
                                }
                                if (out.gsm_sys_info.srv_capability_available) {
                                    RLOGI("GSM srv_capability %u", out.gsm_sys_info.srv_capability);
                                }
                                if (out.gsm_sys_info.roam_status_available) {
                                    RLOGI("GSM roam_status %u", out.gsm_sys_info.roam_status);
                                }
                                if (out.gsm_sys_info.is_sys_forbidden_available) {
                                    RLOGI("GSM is_sys_forbidden %u", out.gsm_sys_info.is_sys_forbidden);
                                }
                                if (out.gsm_sys_info.lac_available) {
                                    RLOGI("GSM lac %u", out.gsm_sys_info.lac);
                                }
                                if (out.gsm_sys_info.cell_id_available) {
                                    RLOGI("GSM cell_id %u", out.gsm_sys_info.cell_id);
                                }
                                if (out.gsm_sys_info.reg_reject_info_available) {
                                    RLOGI("GSM reject_srv_domain %u", out.gsm_sys_info.reject_srv_domain);
                                    RLOGI("GSM rej_cause %u", out.gsm_sys_info.rej_cause);
                                }
                                if (out.gsm_sys_info.network_id_available) {
                                    RLOGI("GSM mcc %s", out.gsm_sys_info.mcc);

                                    if ((uint8_t)out.gsm_sys_info.mnc[2] == 0xff) {
                                        out.gsm_sys_info.mnc[2] = 0;
                                    }
                                    RLOGI("GSM mnc %s", out.gsm_sys_info.mnc);
                                }
                                if (out.gsm_sys_info.egprs_supp_available) {
                                    RLOGI("GSM egprs_supp %u", out.gsm_sys_info.egprs_supp);
                                }
                                if (out.gsm_sys_info.dtm_supp_available) {
                                    RLOGI("GSM dtm_supp %u", out.gsm_sys_info.dtm_supp);
                                }
                            }
                            if (out.wcdma_sys_info_available) {
                                if (out.wcdma_sys_info.srv_domain_available) {
                                    RLOGI("WCDMA srv_domain %u", out.wcdma_sys_info.srv_domain);
                                }
                                if (out.wcdma_sys_info.srv_capability_available) {
                                    RLOGI("WCDMA srv_capability %u", out.wcdma_sys_info.srv_capability);
                                }
                                if (out.wcdma_sys_info.roam_status_available) {
                                    RLOGI("WCDMA roam_status %u", out.wcdma_sys_info.roam_status);
                                }
                                if (out.wcdma_sys_info.is_sys_forbidden_available) {
                                    RLOGI("WCDMA is_sys_forbidden %u", out.wcdma_sys_info.is_sys_forbidden);
                                }
                                if (out.wcdma_sys_info.lac_available) {
                                    RLOGI("WCDMA lac %u", out.wcdma_sys_info.lac);
                                }
                                if (out.wcdma_sys_info.cell_id_available) {
                                    RLOGI("WCDMA cell_id %u", out.wcdma_sys_info.cell_id);
                                }
                                if (out.wcdma_sys_info.reg_reject_info_available) {
                                    RLOGI("WCDMA reject_srv_domain %u", out.wcdma_sys_info.reject_srv_domain);
                                    RLOGI("WCDMA rej_cause %u", out.wcdma_sys_info.rej_cause);
                                }
                                if (out.wcdma_sys_info.network_id_available) {
                                    RLOGI("WCDMA mcc %s", out.wcdma_sys_info.mcc);

                                    if ((uint8_t)out.wcdma_sys_info.mnc[2] == 0xff) {
                                        out.wcdma_sys_info.mnc[2] = 0;
                                    }
                                    RLOGI("WCDMA mnc %s", out.wcdma_sys_info.mnc);
                                }
                                if (out.wcdma_sys_info.hs_call_status_available) {
                                    RLOGI("WCDMA hs_call_status %u", out.wcdma_sys_info.hs_call_status);
                                }
                                if (out.wcdma_sys_info.hs_ind_available) {
                                    RLOGI("WCDMA hs_ind %u", out.wcdma_sys_info.hs_ind);
                                }
                                if (out.wcdma_sys_info.psc_available) {
                                    RLOGI("WCDMA psc %u", out.wcdma_sys_info.psc);
                                }
                            }
                            if (out.lte_sys_info_available) {
                                if (out.lte_sys_info.srv_domain_available) {
                                    RLOGI("LTE srv_domain %u", out.lte_sys_info.srv_domain);
                                }
                                if (out.lte_sys_info.srv_capability_available) {
                                    RLOGI("LTE srv_capability %u", out.lte_sys_info.srv_capability);
                                }
                                if (out.lte_sys_info.roam_status_available) {
                                    RLOGI("LTE roam_status %u", out.lte_sys_info.roam_status);
                                }
                                if (out.lte_sys_info.is_sys_forbidden_available) {
                                    RLOGI("LTE is_sys_forbidden %u", out.lte_sys_info.is_sys_forbidden);
                                }
                                if (out.lte_sys_info.lac_available) {
                                    RLOGI("LTE lac %u", out.lte_sys_info.lac);
                                }
                                if (out.lte_sys_info.cell_id_available) {
                                    RLOGI("LTE cell_id %u", out.lte_sys_info.cell_id);
                                }
                                if (out.lte_sys_info.reg_reject_info_available) {
                                    RLOGI("LTE reject_srv_domain %u", out.lte_sys_info.reject_srv_domain);
                                    RLOGI("LTE rej_cause %u", out.lte_sys_info.rej_cause);
                                }
                                if (out.lte_sys_info.network_id_available) {
                                    RLOGI("LTE mcc %s", out.lte_sys_info.mcc);

                                    if ((uint8_t)out.lte_sys_info.mnc[2] == 0xff) {
                                        out.lte_sys_info.mnc[2] = 0;
                                    }
                                    RLOGI("LTE mnc %s", out.lte_sys_info.mnc);
                                }
                                if (out.lte_sys_info.tac_available) {
                                    RLOGI("LTE tac %u", out.lte_sys_info.tac);
                                }
                            }
                            if (out.nr5g_sys_info_available) {
                                if (out.nr5g_sys_info.srv_domain_available) {
                                    RLOGI("nr5g srv_domain %u", out.nr5g_sys_info.srv_domain);
                                }
                                if (out.nr5g_sys_info.srv_capability_available) {
                                    RLOGI("nr5g srv_capability %u", out.nr5g_sys_info.srv_capability);
                                }
                                if (out.nr5g_sys_info.roam_status_available) {
                                    RLOGI("nr5g roam_status %u", out.nr5g_sys_info.roam_status);
                                }
                                if (out.nr5g_sys_info.is_sys_forbidden_available) {
                                    RLOGI("nr5g is_sys_forbidden %u", out.nr5g_sys_info.is_sys_forbidden);
                                }
                                if (out.nr5g_sys_info.lac_available) {
                                    RLOGI("nr5g lac %u", out.nr5g_sys_info.lac);
                                }
                                if (out.nr5g_sys_info.cell_id_available) {
                                    RLOGI("nr5g cell_id %u", out.nr5g_sys_info.cell_id);
                                }
                                if (out.nr5g_sys_info.reg_reject_info_available) {
                                    RLOGI("nr5g reject_srv_domain %u", out.nr5g_sys_info.reject_srv_domain);
                                    RLOGI("nr5g rej_cause %u", out.nr5g_sys_info.rej_cause);
                                }
                                if (out.nr5g_sys_info.network_id_available) {
                                    RLOGI("nr5g mcc %s", out.nr5g_sys_info.mcc);

                                    if ((uint8_t)out.nr5g_sys_info.mnc[2] == 0xff) {
                                        out.nr5g_sys_info.mnc[2] = 0;
                                    }
                                    RLOGI("nr5g mnc %s", out.nr5g_sys_info.mnc);
                                }
                                if (out.nr5g_sys_info.tac_available) {
                                    RLOGI("nr5g tac %u", out.nr5g_sys_info.tac);
                                }
                            }
                            if (out.nr5g_cell_status_available) {
                                RLOGI("nr5g_cell_status %u", out.nr5g_cell_status);
                            }
                            if (out.nr5g_tac_available) {
                                RLOGI("nr5g_tac[0] %u", out.nr5g_tac[0]);
                                RLOGI("nr5g_tac[1] %u", out.nr5g_tac[1]);
                                RLOGI("nr5g_tac[2] %u", out.nr5g_tac[2]);
                            }
                            if (out.nr5g_pci_available) {
                                RLOGI("nr5g_pci %u", out.nr5g_pci);
                            }
                            if (out.nr5g_cell_id_available) {
                                RLOGI("nr5g_cell_id %u", out.nr5g_cell_id);
                            }
                            if (out.nr5g_arfcn_available) {
                                RLOGI("nr5g_arfcn %u", out.nr5g_arfcn);
                            }
                            if (out.add_hdr_sys_info_available) {
                                RLOGI("add_hdr_sys_info %u", out.add_hdr_sys_info);
                            }
                            if (out.add_gsm_sys_info_available) {
                                RLOGI("add_gsm_sys_info cell_broadcast_cap %u",
                                        out.add_gsm_sys_info.cell_broadcast_cap);
                                RLOGI("add_gsm_sys_info geo_sys_idx %u", out.add_gsm_sys_info.geo_sys_idx);
                            }
                            if (out.add_wcdma_sys_info_available) {
                                RLOGI("add_wcdma_sys_info cell_broadcast_cap %u",
                                        out.add_wcdma_sys_info.cell_broadcast_cap);
                                RLOGI("add_wcdma_sys_info geo_sys_idx %u",
                                        out.add_wcdma_sys_info.geo_sys_idx);
                            }
                            if (out.lte_geo_sys_idx_available) {
                                RLOGI("add_lte_sys_info %u", out.lte_geo_sys_idx);
                            }
                            if (out.gsm_call_barring_sys_info_available) {
                                RLOGI("gsm_call_barring_sys_info cs_bar_status %u",
                                        out.gsm_call_barring_sys_info.cs_bar_status);
                                RLOGI("gsm_call_barring_sys_info ps_bar_status %u",
                                        out.gsm_call_barring_sys_info.ps_bar_status);
                            }
                            if (out.wcdma_call_barring_sys_info_available) {
                                RLOGI("wcdma_call_barring_sys_info_available cs_bar_status %u",
                                        out.wcdma_call_barring_sys_info.cs_bar_status);
                                RLOGI("wcdma_call_barring_sys_info_available ps_bar_status %u",
                                        out.wcdma_call_barring_sys_info.ps_bar_status);
                            }
                            RLOGI("lte_voice_support_sys_info %u", out.lte_voice_support_sys_info);
                            if (out.gsm_cipher_domain_sys_info_available) {
                                RLOGI("gsm_cipher_domain_sys_info %u", out.gsm_cipher_domain_sys_info);
                            }
                            if (out.wcdma_cipher_domain_sys_info_available) {
                                RLOGI("wcdma_cipher_domain_sys_info %u", out.wcdma_cipher_domain_sys_info);
                            }
                            if (out.endc_availability_available) {
                                RLOGI("endc_availability %u", out.endc_availability);
                            }
                            if (out.restrict_dcnr_available) {
                                RLOGI("restrict_dcnr %u", out.restrict_dcnr);
                            }
                            if (out.plmn_infolist_r15_availability_available) {
                                RLOGI("plmn_infolist_r15_availability %u",
                                        out.plmn_infolist_r15_availability);
                            }
                        }

                        got_expected_message = true;
                    } else {
                        RLOGI("xid %d", xid);
                    }
                    break;

                case QMI_MSG_INDICATION:
                    RLOGD("%s Indication message, msgid 0x%x", SVC_STR, rsp_ctx.msg_id);
                    break;

                default:
                    RLOGD("Unknown QMI message type: %d", rsp_ctx.type);
                    break;
            }
        } else {
            sleep(1);
        }
    }

close_fd:

    if (svc >= 0) {
        close(svc);
    }

    return 0;
}
