#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <telit_sdk.h>
#include <telit_sdk_nas.h>
#include <tqcm.h>
#include <tqcm_nas.h>

static char *dev;
static char default_rmnet_dev[] = "/dev/cdc-wdm0";
static tqcm_context_t ctx;

static void print_cell_location_info(nas_cell_location_info_t *out)
{
    int i, j;
    uint8_t mcc, mnc;

    if (out->geran_info_available) {
        RLOGI("GERAN Info");
        RLOGI("cell_id %u", out->geran_info.cell_id);

        RLOGI("plmn %d %d %d", out->geran_info.plmn[0],
                out->geran_info.plmn[1],
                out->geran_info.plmn[2]);
        mcc =
                ((out->geran_info.plmn[0] & 0xF) +
                (out->geran_info.plmn[0] >> 4) * 10 + (out->geran_info.plmn[1] & 0xF) * 100);
        mnc = ((out->geran_info.plmn[2] & 0xF) + (out->geran_info.plmn[2] >> 4) * 10 +
                ((out->geran_info.plmn[1] >> 4 != 0b1111) ? (out->geran_info.plmn[1] >> 4) * 100 : 0));
        RLOGI("derived MCC %u, MNC %u", mcc, mnc);
        RLOGI("lac %u", out->geran_info.lac);
        RLOGI("arfcn %d", out->geran_info.arfcn);
        RLOGI("bsic %d", out->geran_info.bsic);
        RLOGI("timing_advance %d", out->geran_info.timing_advance);
        RLOGI("rx_lev %d", out->geran_info.rx_lev);
        RLOGI("nmr_inst %d", out->geran_info.nmr_cell_info_size);

        for (i = 0; i < out->geran_info.nmr_cell_info_size; i++) {
            RLOGI("GERAN Info - NMR Cell %d", i);
            RLOGI("nmr_cell_id[%d] %d", i, out->geran_info.nmr_cell_info[i].nmr_cell_id);

            RLOGI("nmr_plmn[%d] %d %d %d", i, out->geran_info.nmr_cell_info[i].nmr_plmn[0],
                    out->geran_info.nmr_cell_info[i].nmr_plmn[1],
                    out->geran_info.nmr_cell_info[i].nmr_plmn[2]);
            RLOGI("nmr_lac[%d] %d", i, out->geran_info.nmr_cell_info[i].nmr_lac);
            RLOGI("nmr_arfcn[%d] %d", i, out->geran_info.nmr_cell_info[i].nmr_arfcn);
            RLOGI("nmr_bsic[%d] %d", i, out->geran_info.nmr_cell_info[i].nmr_bsic);
            RLOGI("nmr_rx_lev[%d] %d", i, out->geran_info.nmr_cell_info[i].nmr_rx_lev);
        }
    }

    if (out->umts_info_available) {
        RLOGI("UMTS Info");
        RLOGI("cell_id %u", out->umts_info.cell_id);
        RLOGI("plmn %d %d %d", out->umts_info.plmn[0],
                out->umts_info.plmn[1],
                out->umts_info.plmn[2]);
        mcc =
                ((out->umts_info.plmn[0] & 0xF) +
                (out->umts_info.plmn[0] >> 4) * 10 + (out->umts_info.plmn[1] & 0xF) * 100);
        mnc = ((out->umts_info.plmn[2] & 0xF) + (out->umts_info.plmn[2] >> 4) * 10 +
                ((out->umts_info.plmn[1] >> 4 != 0b1111) ? (out->umts_info.plmn[1] >> 4) * 100 : 0));
        RLOGI("derived MCC %u, MNC %u", mcc, mnc);
        RLOGI("lac %u", out->umts_info.lac);
        RLOGI("uarfcn %d", out->umts_info.uarfcn);
        RLOGI("psc %d", out->umts_info.psc);
        RLOGI("rscp %d", out->umts_info.rscp);
        RLOGI("ecio %d", out->umts_info.ecio);
        RLOGI("umts_insts_size %d", out->umts_info.umts_insts_size);

        for (i = 0; i < out->umts_info.umts_insts_size; i++) {
            RLOGI("UMTS Info - instance %d", i);
            RLOGI("umts_uarfcn[%d] %d", i, out->umts_info.umts_insts[i].umts_uarfcn);
            RLOGI("umts_psc[%d] %d", i, out->umts_info.umts_insts[i].umts_psc);
            RLOGI("umts_rscp[%d] %d", i, out->umts_info.umts_insts[i].umts_rscp);
            RLOGI("umts_ecio[%d] %d", i, out->umts_info.umts_insts[i].umts_ecio);
        }

        RLOGI("geran_insts_size %d", out->umts_info.geran_insts_size);

        for (i = 0; i < out->umts_info.geran_insts_size; i++) {
            RLOGI("UMTS Info - GERAN instance %d", i);
            RLOGI("geran_arfcn[%d] %d", i, out->umts_info.geran_insts[i].geran_arfcn);
            RLOGI("geran_bsic_ncc[%d] %d", i, out->umts_info.geran_insts[i].geran_bsic_ncc);
            RLOGI("geran_bsic_bcc[%d] %d", i, out->umts_info.geran_insts[i].geran_bsic_bcc);
            RLOGI("geran_rssi[%d] %d", i, out->umts_info.geran_insts[i].geran_rssi);
        }
    }

    if (out->lte_info_intrafreq_available) {
        RLOGI("LTE Info - Intrafrequency");
        RLOGI("ue_in_idle %u", out->lte_info_intrafreq.ue_in_idle);

        RLOGI("plmn %d %d %d", out->lte_info_intrafreq.plmn[0],
                out->lte_info_intrafreq.plmn[1],
                out->lte_info_intrafreq.plmn[2]);
        mcc =
                ((out->lte_info_intrafreq.plmn[0] & 0xF) +
                (out->lte_info_intrafreq.plmn[0] >> 4) * 10 + (out->lte_info_intrafreq.plmn[1] & 0xF) * 100);
        mnc = ((out->lte_info_intrafreq.plmn[2] & 0xF) + (out->lte_info_intrafreq.plmn[2] >> 4) * 10 +
                ((out->lte_info_intrafreq.plmn[1] >> 4 !=
                0b1111) ? (out->lte_info_intrafreq.plmn[1] >> 4) * 100 : 0));
        RLOGI("derived MCC %u, MNC %u", mcc, mnc);
        RLOGI("tac %u", out->lte_info_intrafreq.tac);
        RLOGI("global_cell_id %d", out->lte_info_intrafreq.global_cell_id);
        RLOGI("earfcn %d", out->lte_info_intrafreq.earfcn);
        RLOGI("serving_cell_id %d", out->lte_info_intrafreq.serving_cell_id);
        RLOGI("cell_resel_priority %d", out->lte_info_intrafreq.cell_resel_priority);
        RLOGI("s_non_intra_search %d", out->lte_info_intrafreq.s_non_intra_search);
        RLOGI("thresh_serving_low %d", out->lte_info_intrafreq.thresh_serving_low);
        RLOGI("s_intra_search %d", out->lte_info_intrafreq.s_intra_search);
        RLOGI("cell_intra_freq_params_size %d", out->lte_info_intrafreq.cell_intra_freq_params_size);

        for (i = 0; i < out->lte_info_intrafreq.cell_intra_freq_params_size; i++) {
            RLOGI("LTE Info - Intrafrequency - params %d", i);
            RLOGI("pci[%d] %d", i, out->lte_info_intrafreq.cell_intra_freq_params[i].pci);
            RLOGI("rsrq[%d] %d", i, out->lte_info_intrafreq.cell_intra_freq_params[i].rsrq);
            RLOGI("rsrp[%d] %d", i, out->lte_info_intrafreq.cell_intra_freq_params[i].rsrp);
            RLOGI("rssi[%d] %d", i, out->lte_info_intrafreq.cell_intra_freq_params[i].rssi);
            RLOGI("srxlev[%d] %d", i, out->lte_info_intrafreq.cell_intra_freq_params[i].srxlev);
        }
    }

    if (out->lte_info_interfreq_available) {
        RLOGI("LTE Info - Interfrequency");
        RLOGI("ue_in_idle %u", out->lte_info_interfreq.ue_in_idle);
        RLOGI("inter_freqs_size %d", out->lte_info_interfreq.inter_freqs_size);

        for (i = 0; i < out->lte_info_interfreq.inter_freqs_size; i++) {
            RLOGI("LTE Info - Interfrequency - inter freq %d", i);
            RLOGI("earfcn[%d] %d", i, out->lte_info_interfreq.inter_freqs[i].earfcn);
            RLOGI("threshX_low[%d] %d", i, out->lte_info_interfreq.inter_freqs[i].threshX_low);
            RLOGI("threshX_high[%d] %d", i, out->lte_info_interfreq.inter_freqs[i].threshX_high);
            RLOGI("rssi[%d] %d", i, out->lte_info_interfreq.inter_freqs[i].cell_resel_priority);
            RLOGI("cell_inter_freq_params_size[%d] %d", i,
                    out->lte_info_interfreq.inter_freqs[i].cell_inter_freq_params_size);

            for (j = 0; j < out->lte_info_interfreq.inter_freqs[i].cell_inter_freq_params_size; j++) {
                RLOGI("LTE Info - Interfrequency - inter freq %d - params %d", i, j);
                RLOGI("pci[%d][%d] %d", i, j,
                        out->lte_info_interfreq.inter_freqs[i].cell_inter_freq_params[j].pci);
                RLOGI("rsrq[%d][%d] %d", i, j,
                        out->lte_info_interfreq.inter_freqs[i].cell_inter_freq_params[j].rsrq);
                RLOGI("rsrp[%d][%d] %d", i, j,
                        out->lte_info_interfreq.inter_freqs[i].cell_inter_freq_params[j].rsrp);
                RLOGI("rssi[%d][%d] %d", i, j,
                        out->lte_info_interfreq.inter_freqs[i].cell_inter_freq_params[j].rssi);
                RLOGI("srxlev[%d][%d] %d", i, j,
                        out->lte_info_interfreq.inter_freqs[i].cell_inter_freq_params[j].srxlev);
            }
        }
    }

    if (out->lte_info_neighboring_gsm_available) {
        RLOGI("LTE Info - Neighboring GSM");
        RLOGI("ue_in_idle %u", out->lte_info_neighboring_gsm.ue_in_idle);
        RLOGI("lte_gsm_cells_size %d", out->lte_info_neighboring_gsm.lte_gsm_cells_size);

        for (i = 0; i < out->lte_info_neighboring_gsm.lte_gsm_cells_size; i++) {
            RLOGI("LTE Info - Neighboring GSM - lte cell %d", i);
            RLOGI("cell_resel_priority[%d] %d", i,
                    out->lte_info_neighboring_gsm.lte_gsm_cells[i].cell_resel_priority);
            RLOGI("thresh_gsm_high[%d] %d", i,
                    out->lte_info_neighboring_gsm.lte_gsm_cells[i].thresh_gsm_high);
            RLOGI("thresh_gsm_low[%d] %d", i, out->lte_info_neighboring_gsm.lte_gsm_cells[i].thresh_gsm_low);
            RLOGI("ncc_permitted[%d] %d", i, out->lte_info_neighboring_gsm.lte_gsm_cells[i].ncc_permitted);
            RLOGI("gsm_cells_size[%d] %d", i, out->lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells_size);

            for (j = 0; j < out->lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells_size; j++) {
                RLOGI("LTE Info - Neighboring GSM - lte cell %d - gms cell %d", i, j);
                RLOGI("arfcn[%d][%d] %d", i, j,
                        out->lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells[j].arfcn);
                RLOGI("band_1900[%d][%d] %d", i, j,
                        out->lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells[j].band_1900);
                RLOGI("cell_id_valid[%d][%d] %d", i, j,
                        out->lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells[j].cell_id_valid);
                RLOGI("bsic_id[%d][%d] %d", i, j,
                        out->lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells[j].bsic_id);
                RLOGI("rssi[%d][%d] %d", i, j,
                        out->lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells[j].rssi);
                RLOGI("srxlev[%d][%d] %d", i, j,
                        out->lte_info_neighboring_gsm.lte_gsm_cells[i].gsm_cells[j].srxlev);
            }
        }
    }

    if (out->lte_info_neighboring_wcdma_available) {
        RLOGI("LTE Info - Neighboring WCDMA");
        RLOGI("ue_in_idle %u", out->lte_info_neighboring_wcdma.ue_in_idle);
        RLOGI("lte_wcdma_cells_size %d", out->lte_info_neighboring_wcdma.lte_wcdma_cells_size);

        for (i = 0; i < out->lte_info_neighboring_wcdma.lte_wcdma_cells_size; i++) {
            RLOGI("LTE Info - Neighboring WCDMA - lte cell %d", i);
            RLOGI("uarfcn[%d] %d", i, out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].uarfcn);
            RLOGI("cell_resel_priority[%d] %d", i,
                    out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].cell_resel_priority);
            RLOGI("thresh_Xhigh[%d] %d", i, out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].thresh_Xhigh);
            RLOGI("threshXlow[%d] %d", i, out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].threshXlow);
            RLOGI("wcdma_cells_size[%d] %d", i,
                    out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].wcdma_cells_size);

            for (j = 0; j < out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].wcdma_cells_size; j++) {
                RLOGI("LTE Info - Neighboring WCDMA - lte cell %d - WCDMA cell %d", i, j);
                RLOGI("psc[%d][%d] %d", i, j,
                        out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].wcdma_cells[j].psc);
                RLOGI("cpich_rscp[%d][%d] %d", i, j,
                        out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].wcdma_cells[j].cpich_rscp);
                RLOGI("cpich_ecno[%d][%d] %d", i, j,
                        out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].wcdma_cells[j].cpich_ecno);
                RLOGI("srxlev[%d][%d] %d", i, j,
                        out->lte_info_neighboring_wcdma.lte_wcdma_cells[i].wcdma_cells[j].srxlev);
            }
        }
    }

    if (out->umts_cell_id_available) {
        RLOGI("umts_cell_id %u", out->umts_cell_id);
    }

    if (out->wcdma_info_lte_neighbor_cell_available) {
        RLOGI("WCDMA Info - LTE Neighbor Cell Info");
        RLOGI("wcdma_rrc_state %u", out->wcdma_info_lte_neighbor_cell.wcdma_rrc_state);
        RLOGI("umts_lte_nbr_cell_size %d", out->wcdma_info_lte_neighbor_cell.umts_lte_nbr_cell_size);

        for (i = 0; i < out->wcdma_info_lte_neighbor_cell.umts_lte_nbr_cell_size; i++) {
            RLOGI("WCDMA Info - LTE Neighbor Cell Info - umts cell %d", i);
            RLOGI("earfcn[%d] %d", i, out->wcdma_info_lte_neighbor_cell.umts_lte_nbr_cells[i].pci);
            RLOGI("rsrp[%d] %d", i, out->wcdma_info_lte_neighbor_cell.umts_lte_nbr_cells[i].rsrp);
            RLOGI("rsrq[%d] %d", i, out->wcdma_info_lte_neighbor_cell.umts_lte_nbr_cells[i].rsrq);
            RLOGI("srxlev[%d] %d", i, out->wcdma_info_lte_neighbor_cell.umts_lte_nbr_cells[i].srxlev);
            RLOGI("cell_is_tdd[%d] %d", i,
                    out->wcdma_info_lte_neighbor_cell.umts_lte_nbr_cells[i].cell_is_tdd);
        }
    }
}

int main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;
    tqcm_client_context_t ctx_nas;
    nas_cell_location_info_t out;
    uint16_t sdk_error;

    if (argc == 1) {
        dev = default_rmnet_dev;
    } else {
        dev = argv[1];
    }

    ret = tqcm_init(dev, &ctx);
    if (ret != 0) {
        RLOGE("Failed tqcm initialization");
        ret = EXIT_FAILURE;
        goto end;
    }

    ret = tqcm_init_client_context(QMI_SVC_NAS, &ctx, &ctx_nas, NULL);

    if (ret != 0) {
        RLOGE("Failed context opening");
        ret = EXIT_FAILURE;
        goto end;
    }

    RLOGD("Acquired client id %u", ctx_nas.client_id);

    ret = tqcm_nas_get_cell_location_info_send(&ctx_nas, &out, &sdk_error, NULL, 0);

    if (ret != 0) {
        RLOGE("send fail %d", ret);
        if (ret == EPROTO) {
            RLOGE("qmi protocol error 0x%04X", sdk_error);
        }
        ret = EXIT_FAILURE;
        goto context_disposal;
    }

    print_cell_location_info(&out);

context_disposal:
    if (tqcm_deinit_client_context(&ctx, &ctx_nas) != 0) {
        RLOGE("Failed context disposal");
        ret = EXIT_FAILURE;
    }

    if (tqcm_deinit(&ctx) != 0) {
        RLOGE("Failed tqcm deinitialization");
        ret = EXIT_FAILURE;
        goto end;
    }

    /* Needed for valgrind complain about thread stack memory */
    sleep(1);
end:

    return ret;
}
