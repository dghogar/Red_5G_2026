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
    nas_cell_location_info_t out;

    memset(&out, 0, sizeof(out));
    memset(&req_ctx, 0, sizeof(req_ctx));
    req_ctx.xid = xid;

    telit_set_log_level(LOG_INFO);

    rtn = telit_nas_get_cell_location_info_pack(&req_ctx, req, &req_size);

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
                        rtn = telit_nas_get_cell_location_info_unpack(rsp, rsp_size, &out);

                        if (rtn != QMI_SDK_ERR_NONE) {
                            RLOGE("%s unpack pack fail %d ", SVC_STR, rtn);
                        } else {
                            int i, j;

                            if (out.geran_info_available) {
                                RLOGI("GERAN Info");
                                RLOGI("cell_id %u, ", out.geran_info.cell_id);

                                RLOGI("plmn %d %d %d, ", out.geran_info.plmn[0],
                                        out.geran_info.plmn[1],
                                        out.geran_info.plmn[2]);
                                RLOGI("lac %u, ", out.geran_info.lac);
                                RLOGI("arfcn %d, ", out.geran_info.arfcn);
                                RLOGI("bsic %d, ", out.geran_info.bsic);
                                RLOGI("timing_advance %d, ", out.geran_info.timing_advance);
                                RLOGI("rx_lev %d, ", out.geran_info.rx_lev);
                                RLOGI("nmr_inst %d, ", out.geran_info.nmr_cell_info_size);

                                for (i = 0; i < out.geran_info.nmr_cell_info_size; i++) {
                                    RLOGI("GERAN Info - NMR Cell %d", i);
                                    RLOGI("nmr_cell_id[%d] %d, ", i,
                                            out.geran_info.nmr_cell_info[i].nmr_cell_id);

                                    RLOGI("nmr_plmn[%d] %d %d %d, ", i,
                                            out.geran_info.nmr_cell_info[i].nmr_plmn[0],
                                            out.geran_info.nmr_cell_info[i].nmr_plmn[1],
                                            out.geran_info.nmr_cell_info[i].nmr_plmn[2]);
                                    RLOGI("nmr_lac[%d] %d, ", i, out.geran_info.nmr_cell_info[i].nmr_lac);
                                    RLOGI("nmr_arfcn[%d] %d, ", i, out.geran_info.nmr_cell_info[i].nmr_arfcn);
                                    RLOGI("nmr_bsic[%d] %d, ", i, out.geran_info.nmr_cell_info[i].nmr_bsic);
                                    RLOGI("nmr_rx_lev[%d] %d, ", i,
                                            out.geran_info.nmr_cell_info[i].nmr_rx_lev);
                                }
                            }

                            if (out.umts_info_available) {
                                RLOGI("UMTS Info");
                                RLOGI("cell_id %u, ", out.umts_info.cell_id);
                                RLOGI("plmn %d %d %d, ", out.umts_info.plmn[0],
                                        out.umts_info.plmn[1],
                                        out.umts_info.plmn[2]);
                                RLOGI("lac %u, ", out.umts_info.lac);
                                RLOGI("uarfcn %d, ", out.umts_info.uarfcn);
                                RLOGI("psc %d, ", out.umts_info.psc);
                                RLOGI("rscp %d, ", out.umts_info.rscp);
                                RLOGI("ecio %d, ", out.umts_info.ecio);
                                RLOGI("umts_insts_size %d, ", out.umts_info.umts_insts_size);

                                for (i = 0; i < out.umts_info.umts_insts_size; i++) {
                                    RLOGI("UMTS Info - instance %d", i);
                                    RLOGI("umts_uarfcn[%d] %d, ", i, out.umts_info.umts_insts[i].umts_uarfcn);
                                    RLOGI("umts_psc[%d] %d, ", i, out.umts_info.umts_insts[i].umts_psc);
                                    RLOGI("umts_rscp[%d] %d, ", i, out.umts_info.umts_insts[i].umts_rscp);
                                    RLOGI("umts_ecio[%d] %d, ", i, out.umts_info.umts_insts[i].umts_ecio);
                                }

                                RLOGI("geran_insts_size %d, ", out.umts_info.geran_insts_size);

                                for (i = 0; i < out.umts_info.geran_insts_size; i++) {
                                    RLOGI("UMTS Info - GERAN instance %d", i);
                                    RLOGI("geran_arfcn[%d] %d, ", i,
                                            out.umts_info.geran_insts[i].geran_arfcn);
                                    RLOGI("geran_bsic_ncc[%d] %d, ", i,
                                            out.umts_info.geran_insts[i].geran_bsic_ncc);
                                    RLOGI("geran_bsic_bcc[%d] %d, ", i,
                                            out.umts_info.geran_insts[i].geran_bsic_bcc);
                                    RLOGI("geran_rssi[%d] %d, ", i, out.umts_info.geran_insts[i].geran_rssi);
                                }
                            }

                            if (out.lte_info_intrafreq_available) {
                                RLOGI("LTE Info - Intrafrequency");
                                RLOGI("ue_in_idle %u, ", out.lte_info_intrafreq.ue_in_idle);

                                RLOGI("plmn %d %d %d, ", out.lte_info_intrafreq.plmn[0],
                                        out.lte_info_intrafreq.plmn[1],
                                        out.lte_info_intrafreq.plmn[2]);
                                RLOGI("tac %u, ", out.lte_info_intrafreq.tac);
                                RLOGI("global_cell_id %d, ", out.lte_info_intrafreq.global_cell_id);
                                RLOGI("earfcn %d, ", out.lte_info_intrafreq.earfcn);
                                RLOGI("serving_cell_id %d, ", out.lte_info_intrafreq.serving_cell_id);
                                RLOGI("cell_resel_priority %d, ", out.lte_info_intrafreq.cell_resel_priority);
                                RLOGI("s_non_intra_search %d, ", out.lte_info_intrafreq.s_non_intra_search);
                                RLOGI("thresh_serving_low %d, ", out.lte_info_intrafreq.thresh_serving_low);
                                RLOGI("s_intra_search %d, ", out.lte_info_intrafreq.s_intra_search);
                                RLOGI("cell_intra_freq_params_size %d, ",
                                        out.lte_info_intrafreq.cell_intra_freq_params_size);

                                for (i = 0; i < out.lte_info_intrafreq.cell_intra_freq_params_size; i++) {
                                    RLOGI("LTE Info - Intrafrequency - params %d", i);
                                    RLOGI("pci[%d] %d, ", i,
                                            out.lte_info_intrafreq.cell_intra_freq_params[i].pci);
                                    RLOGI("rsrq[%d] %d, ", i,
                                            out.lte_info_intrafreq.cell_intra_freq_params[i].rsrq);
                                    RLOGI("rsrp[%d] %d, ", i,
                                            out.lte_info_intrafreq.cell_intra_freq_params[i].rsrp);
                                    RLOGI("rssi[%d] %d, ", i,
                                            out.lte_info_intrafreq.cell_intra_freq_params[i].rssi);
                                    RLOGI("srxlev[%d] %d, ", i,
                                            out.lte_info_intrafreq.cell_intra_freq_params[i].srxlev);
                                }
                            }

                            if (out.lte_info_interfreq_available) {
                                RLOGI("LTE Info - Interfrequency");
                                RLOGI("ue_in_idle %u, ", out.lte_info_interfreq.ue_in_idle);
                                RLOGI("inter_freqs_size %d, ", out.lte_info_interfreq.inter_freqs_size);

                                for (i = 0; i < out.lte_info_interfreq.inter_freqs_size; i++) {
                                    RLOGI("LTE Info - Interfrequency - inter freq %d", i);
                                    RLOGI("earfcn[%d] %d, ", i, out.lte_info_interfreq.inter_freqs[i].earfcn);
                                    RLOGI("threshX_low[%d] %d, ", i,
                                            out.lte_info_interfreq.inter_freqs[i].threshX_low);
                                    RLOGI("threshX_high[%d] %d, ", i,
                                            out.lte_info_interfreq.inter_freqs[i].threshX_high);
                                    RLOGI("rssi[%d] %d, ", i,
                                            out.lte_info_interfreq.inter_freqs[i].cell_resel_priority);
                                    RLOGI("cell_inter_freq_params_size[%d] %d, ", i,
                                            out.lte_info_interfreq.inter_freqs[i].cell_inter_freq_params_size);

                                    for (j = 0;
                                            j <
                                            out.lte_info_interfreq.inter_freqs[i].cell_inter_freq_params_size;
                                            j++) {
                                        RLOGI("LTE Info - Interfrequency - inter freq %d - params %d", i, j);
                                        RLOGI("pci[%d][%d] %d, ", i, j,
                                                out.lte_info_interfreq.inter_freqs[i].cell_inter_freq_params[j]
                                                .pci);
                                        RLOGI("rsrq[%d][%d] %d, ", i, j,
                                                out.lte_info_interfreq.inter_freqs[i].cell_inter_freq_params[j]
                                                .rsrq);
                                        RLOGI("rsrp[%d][%d] %d, ", i, j,
                                                out.lte_info_interfreq.inter_freqs[i].cell_inter_freq_params[j]
                                                .rsrp);
                                        RLOGI("rssi[%d][%d] %d, ", i, j,
                                                out.lte_info_interfreq.inter_freqs[i].cell_inter_freq_params[j]
                                                .rssi);
                                        RLOGI("srxlev[%d][%d] %d, ", i, j,
                                                out.lte_info_interfreq.inter_freqs[i].cell_inter_freq_params[j]
                                                .srxlev);
                                    }
                                }
                            }

                            if (out.lte_info_neighboring_gsm_available) {
                                RLOGI("LTE Info - Neighboring GSM");
                                RLOGI("ue_in_idle %u, ", out.lte_info_neighboring_gsm.ue_in_idle);
                                RLOGI("lte_gsm_cells_size %d, ",
                                        out.lte_info_neighboring_gsm.lte_gsm_cells_size);

                                for (i = 0; i < out.lte_info_neighboring_gsm.lte_gsm_cells_size; i++) {
                                    RLOGI("LTE Info - Neighboring GSM - lte cell %d", i);
                                    RLOGI("cell_resel_priority[%d] %d, ", i,
                                            out.lte_info_neighboring_gsm.lte_gsm_cells[i].cell_resel_priority);
                                    RLOGI("thresh_gsm_high[%d] %d, ", i,
                                            out.lte_info_neighboring_gsm.lte_gsm_cells[i].thresh_gsm_high);
                                    RLOGI("thresh_gsm_low[%d] %d, ", i,
                                            out.lte_info_neighboring_gsm.lte_gsm_cells[i].thresh_gsm_low);
                                    RLOGI("ncc_permitted[%d] %d, ", i,
                                            out.lte_info_neighboring_gsm.lte_gsm_cells[i].ncc_permitted);
                                    RLOGI("gsm_cells_size[%d] %d, ", i,
                                            out.lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells_size);

                                    for (j = 0;
                                            j < out.lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells_size;
                                            j++) {
                                        RLOGI("LTE Info - Neighboring GSM - lte cell %d - gms cell %d", i, j);
                                        RLOGI("arfcn[%d][%d] %d, ", i, j,
                                                out.lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells[j].
                                                arfcn);
                                        RLOGI("band_1900[%d][%d] %d, ", i, j,
                                                out.lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells[j].
                                                band_1900);
                                        RLOGI("cell_id_valid[%d][%d] %d, ", i, j,
                                                out.lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells[j].
                                                cell_id_valid);
                                        RLOGI("bsic_id[%d][%d] %d, ", i, j,
                                                out.lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells[j].
                                                bsic_id);
                                        RLOGI("rssi[%d][%d] %d, ", i, j,
                                                out.lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells[j].
                                                rssi);
                                        RLOGI("srxlev[%d][%d] %d, ", i, j,
                                                out.lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells[j].
                                                srxlev);
                                    }
                                }
                            }

                            if (out.lte_info_neighboring_wcdma_available) {
                                RLOGI("LTE Info - Neighboring WCDMA");
                                RLOGI("ue_in_idle %u, ", out.lte_info_neighboring_wcdma.ue_in_idle);
                                RLOGI("lte_wcdma_cells_size %d, ",
                                        out.lte_info_neighboring_wcdma.lte_wcdma_cells_size);

                                for (i = 0; i < out.lte_info_neighboring_wcdma.lte_wcdma_cells_size; i++) {
                                    RLOGI("LTE Info - Neighboring WCDMA - lte cell %d", i);
                                    RLOGI("uarfcn[%d] %d, ", i,
                                            out.lte_info_neighboring_wcdma.lte_wcdma_cells[i].uarfcn);
                                    RLOGI("cell_resel_priority[%d] %d, ", i,
                                            out.lte_info_neighboring_wcdma.lte_wcdma_cells[i].
                                            cell_resel_priority);
                                    RLOGI("thresh_Xhigh[%d] %d, ", i,
                                            out.lte_info_neighboring_wcdma.lte_wcdma_cells[i].thresh_Xhigh);
                                    RLOGI("threshXlow[%d] %d, ", i,
                                            out.lte_info_neighboring_wcdma.lte_wcdma_cells[i].threshXlow);
                                    RLOGI("wcdma_cells_size[%d] %d, ", i,
                                            out.lte_info_neighboring_wcdma.lte_wcdma_cells[i].wcdma_cells_size);

                                    for (j = 0;
                                            j <
                                            out.lte_info_neighboring_wcdma.lte_wcdma_cells[i].wcdma_cells_size;
                                            j++) {
                                        RLOGI("LTE Info - Neighboring WCDMA - lte cell %d - WCDMA cell %d", i,
                                                j);
                                        RLOGI("psc[%d][%d] %d, ", i, j,
                                                out.lte_info_neighboring_wcdma.lte_wcdma_cells[i].wcdma_cells[
                                                    j].psc);
                                        RLOGI("cpich_rscp[%d][%d] %d, ", i, j,
                                                out.lte_info_neighboring_wcdma.lte_wcdma_cells[i].wcdma_cells[
                                                    j].cpich_rscp);
                                        RLOGI("cpich_ecno[%d][%d] %d, ", i, j,
                                                out.lte_info_neighboring_wcdma.lte_wcdma_cells[i].wcdma_cells[
                                                    j].cpich_ecno);
                                        RLOGI("srxlev[%d][%d] %d, ", i, j,
                                                out.lte_info_neighboring_wcdma.lte_wcdma_cells[i].wcdma_cells[
                                                    j].srxlev);
                                    }
                                }
                            }

                            if (out.umts_cell_id_available) {
                                RLOGI("umts_cell_id %u, ", out.umts_cell_id);
                            }

                            if (out.wcdma_info_lte_neighbor_cell_available) {
                                RLOGI("WCDMA Info - LTE Neighbor Cell Info");
                                RLOGI("wcdma_rrc_state %u, ",
                                        out.wcdma_info_lte_neighbor_cell.wcdma_rrc_state);
                                RLOGI("umts_lte_nbr_cell_size %d, ",
                                        out.wcdma_info_lte_neighbor_cell.umts_lte_nbr_cell_size);

                                for (i = 0; i < out.wcdma_info_lte_neighbor_cell.umts_lte_nbr_cell_size;
                                        i++) {
                                    RLOGI("WCDMA Info - LTE Neighbor Cell Info - umts cell %d", i);
                                    RLOGI("earfcn[%d] %d, ", i,
                                            out.wcdma_info_lte_neighbor_cell.umts_lte_nbr_cells[i].pci);
                                    RLOGI("rsrp[%d] %d, ", i,
                                            out.wcdma_info_lte_neighbor_cell.umts_lte_nbr_cells[i].rsrp);
                                    RLOGI("rsrq[%d] %d, ", i,
                                            out.wcdma_info_lte_neighbor_cell.umts_lte_nbr_cells[i].rsrq);
                                    RLOGI("srxlev[%d] %d, ", i,
                                            out.wcdma_info_lte_neighbor_cell.umts_lte_nbr_cells[i].srxlev);
                                    RLOGI("cell_is_tdd[%d] %d, ", i,
                                            out.wcdma_info_lte_neighbor_cell.umts_lte_nbr_cells[i].cell_is_tdd);
                                }
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
