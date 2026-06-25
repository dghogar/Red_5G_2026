##
import requests
import json
##
import connexion
from typing import Dict
from typing import Tuple
from typing import Union

from openapi_server.models.plmn_info import PlmnInfo  # noqa: E501
from openapi_server.models.problem_details import ProblemDetails  # noqa: E501
from openapi_server.models.queries_layer2_meas_get200_response import QueriesLayer2MeasGET200Response  # noqa: E501
from openapi_server.models.queries_rabinfo_get200_response import QueriesRabinfoGET200Response  # noqa: E501
from openapi_server.models.queries_s1_bearerinfo_get200_response import QueriesS1BearerinfoGET200Response  # noqa: E501
from openapi_server import util


def queries_layer2_meas_get(app_ins_id=None, cell_id=None, ue_ipv4_address=None, ue_ipv6_address=None, nated_ip_address=None, gtp_teid=None, dl_gbr_prb_usage_cell=None, ul_gbr_prb_usage_cell=None, dl_nongbr_prb_usage_cell=None, ul_nongbr_prb_usage_cell=None, dl_total_prb_usage_cell=None, ul_total_prb_usage_cell=None, received_dedicated_preambles_cell=None, received_randomly_selecte_preambles_low_range_cell=None, received_randomly_selected_preambles_high_range_cell=None, number_of_active_ue_dl_gbr_cell=None, number_of_active_ue_ul_gbr_cell=None, number_of_active_ue_dl_nongbr_cell=None, number_of_active_ue_ul_nongbr_cell=None, dl_gbr_pdr_cell=None, ul_gbr_pdr_cell=None, dl_nongbr_pdr_cell=None, ul_nongbr_pdr_cell=None, dl_gbr_delay_ue=None, ul_gbr_delay_ue=None, dl_nongbr_delay_ue=None, ul_nongbr_delay_ue=None, dl_gbr_pdr_ue=None, ul_gbr_pdr_ue=None, dl_nongbr_pdr_ue=None, ul_nongbr_pdr_ue=None, dl_gbr_throughput_ue=None, ul_gbr_throughput_ue=None, dl_nongbr_throughput_ue=None, ul_nongbr_throughput_ue=None, dl_gbr_data_volume_ue=None, ul_gbr_data_volume_ue=None, dl_nongbr_data_volume_ue=None, ul_nongbr_data_volume_ue=None):  # noqa: E501
    """Retrieve current status of layer 2 measurements information.

    The GET method is used to query information about the layer 2 measurements. # noqa: E501

    :param app_ins_id: Comma separated list of application instance identifiers.
    :type app_ins_id: str
    :param cell_id: Comma separated list of E-UTRAN Cell Identities each defined as a bit string (size (28)), as defined in ETSI TS 136 413 [i.3].
    :type cell_id: List[str]
    :param ue_ipv4_address: Comma separated list of IE IPv4 addresses as defined for the type for AssociateId in clause 6.5.4.
    :type ue_ipv4_address: List[str]
    :param ue_ipv6_address: Comma separated list of IE IPv6 addresses as defined for the type for AssociateId in clause 6.5.4.
    :type ue_ipv6_address: List[str]
    :param nated_ip_address: Comma separated list of NATed IP addresses as defined for the type for AssociateId in clause 6.5.4.
    :type nated_ip_address: List[str]
    :param gtp_teid: Comma separated list of GTP TEID addresses as defined for the type for AssociateId in clause 6.5.4.
    :type gtp_teid: List[str]
    :param dl_gbr_prb_usage_cell: It indicates the PRB usage for downlink GBR traffic, as defined in ETSI TS 136 314 [i.11] and ETSI TS 136 423 [i.12].
    :type dl_gbr_prb_usage_cell: int
    :param ul_gbr_prb_usage_cell: It indicates (in percentage) the PRB usage for uplink GBR traffic, as defined in ETSI TS 136 314 [i.11] and ETSI TS 136 423 [i.12].
    :type ul_gbr_prb_usage_cell: int
    :param dl_nongbr_prb_usage_cell: It indicates (in percentage) the PRB usage for downlink nonGBR traffic, as defined in ETSI TS 136 314 [i.11] and ETSI TS 136 423 [i.12].
    :type dl_nongbr_prb_usage_cell: int
    :param ul_nongbr_prb_usage_cell: It indicates (in percentage) the PRB usage for uplink nonGBR traffic, as defined in ETSI TS 136 314 [i.11] and ETSI TS 136 423 [i.12].
    :type ul_nongbr_prb_usage_cell: int
    :param dl_total_prb_usage_cell: It indicates (in percentage) the PRB usage for total downlink traffic, as defined in ETSI TS 136 314 [i.11] and ETSI TS 136 423 [i.12].
    :type dl_total_prb_usage_cell: int
    :param ul_total_prb_usage_cell: It indicates (in percentage) the PRB usage for total uplink traffic, as defined in ETSI TS 136 314 [i.11] and  ETSI TS 136 423 [i.12].
    :type ul_total_prb_usage_cell: int
    :param received_dedicated_preambles_cell: It indicates (in percentage) the received dedicated preamples, as defined in ETSI TS 136 314 [i.11].
    :type received_dedicated_preambles_cell: int
    :param received_randomly_selecte_preambles_low_range_cell: It indicates (in percentage) the received randomly selected preambles in the low range, as defined in ETSI TS 136 314 [i.11].
    :type received_randomly_selecte_preambles_low_range_cell: int
    :param received_randomly_selected_preambles_high_range_cell: It indicates (in percentage) the received randomly selected preambles in the high range, as defined in ETSI TS 136 314 [i.11].
    :type received_randomly_selected_preambles_high_range_cell: int
    :param number_of_active_ue_dl_gbr_cell: It indicates the number of active UEs with downlink GBR traffic, as defined in ETSI TS 136 314 [i.11].
    :type number_of_active_ue_dl_gbr_cell: int
    :param number_of_active_ue_ul_gbr_cell: It indicates the number of active UEs with uplink GBR traffic, as defined in ETSI TS 136 314 [i.11].
    :type number_of_active_ue_ul_gbr_cell: int
    :param number_of_active_ue_dl_nongbr_cell: It indicates the number of active UEs with downlink non-GBR traffic, as defined in ETSI TS 136 314 [i.11].
    :type number_of_active_ue_dl_nongbr_cell: int
    :param number_of_active_ue_ul_nongbr_cell: It indicates the number of active UEs with uplink non-GBR traffic, as defined in ETSI TS 136 314 [i.11].
    :type number_of_active_ue_ul_nongbr_cell: int
    :param dl_gbr_pdr_cell: It indicates the packet discard rate in percentage of the downlink GBR traffic in a cell, as defined in ETSI TS 136 314 [i.11].
    :type dl_gbr_pdr_cell: int
    :param ul_gbr_pdr_cell: It indicates the packet discard rate in percentage of the uplink GBR traffic in a cell, as defined in ETSI TS 136 314 [i.11].
    :type ul_gbr_pdr_cell: int
    :param dl_nongbr_pdr_cell: It indicates the packet discard rate in percentage of the downlink non-GBR traffic in a cell, as defined in ETSI TS 136 314 [i.11].
    :type dl_nongbr_pdr_cell: int
    :param ul_nongbr_pdr_cell: It indicates the packet discard rate in percentage of the uplink non-GBR traffic in a cell, as defined in ETSI TS 136 314 [i.11].
    :type ul_nongbr_pdr_cell: int
    :param dl_gbr_delay_ue: It indicates the packet delay of the downlink GBR traffic of a UE, as defined in ETSI TS 136 314 [i.11].
    :type dl_gbr_delay_ue: int
    :param ul_gbr_delay_ue: It indicates the packet delay of the uplink GBR traffic of a UE, as defined in ETSI TS 136 314 [i.11].
    :type ul_gbr_delay_ue: int
    :param dl_nongbr_delay_ue: It indicates the packet delay of the downlink non-GBR traffic of a UE, as defined in ETSI TS 136 314 [i.11].
    :type dl_nongbr_delay_ue: int
    :param ul_nongbr_delay_ue: It indicates the packet delay of the uplink non-GBR traffic of a UE, as defined in ETSI TS 136 314 [i.11].
    :type ul_nongbr_delay_ue: int
    :param dl_gbr_pdr_ue: It indicates the packet discard rate in percentage of the downlink GBR traffic of a UE, as defined in ETSI TS 136 314 [i.11].
    :type dl_gbr_pdr_ue: int
    :param ul_gbr_pdr_ue: It indicates the packet discard rate in percentage of the uplink GBR traffic of a UE, as defined in ETSI TS 136 314 [i.11].
    :type ul_gbr_pdr_ue: int
    :param dl_nongbr_pdr_ue: It indicates the packet discard rate in percentage of the downlink non-GBR traffic of a UE, as defined in ETSI TS 136 314 [i.11].
    :type dl_nongbr_pdr_ue: int
    :param ul_nongbr_pdr_ue: It indicates the packet discard rate in percentage of the uplink non-GBR traffic of a UE, as defined in ETSI TS 136 314 [i.11].
    :type ul_nongbr_pdr_ue: int
    :param dl_gbr_throughput_ue: It indicates the scheduled throughput of the downlink GBR traffic of a UE, as defined in ETSI TS 136 314 [i.11].
    :type dl_gbr_throughput_ue: int
    :param ul_gbr_throughput_ue: It indicates the scheduled throughput of the uplink GBR traffic of a UE, as defined in ETSI TS 136 314 [i.11].
    :type ul_gbr_throughput_ue: int
    :param dl_nongbr_throughput_ue: It indicates the scheduled throughput of the downlink non-GBR traffic of a UE, as defined in ETSI TS 136 314 [i.11].
    :type dl_nongbr_throughput_ue: int
    :param ul_nongbr_throughput_ue: It indicates the scheduled throughput of the uplink non-GBR traffic of a UE, as defined in ETSI TS 136 314 [i.11].
    :type ul_nongbr_throughput_ue: int
    :param dl_gbr_data_volume_ue: It indicates the data volume of the downlink GBR traffic of a UE, as defined in ETSI TS 136 314 [i.11].
    :type dl_gbr_data_volume_ue: int
    :param ul_gbr_data_volume_ue: It indicates the data volume of the uplink GBR traffic of a UE, as defined in ETSI TS 136 314 [i.11].
    :type ul_gbr_data_volume_ue: int
    :param dl_nongbr_data_volume_ue: It indicates the data volume of the downlink non-GBR traffic of a UE, as defined in ETSI TS 136 314 [i.11].
    :type dl_nongbr_data_volume_ue: int
    :param ul_nongbr_data_volume_ue: It indicates the data volume of the uplink non-GBR traffic of a UE, as defined in ETSI TS 136 314 [i.11].
    :type ul_nongbr_data_volume_ue: int

    :rtype: Union[QueriesLayer2MeasGET200Response, Tuple[QueriesLayer2MeasGET200Response, int], Tuple[QueriesLayer2MeasGET200Response, int, Dict[str, str]]
    """
    return 'do some magic!'


def queries_plmninfo_get(app_ins_id):  # noqa: E501
    """Retrieve current status of PLMN information.

    The GET method is used to query information about the Mobile Network. # noqa: E501

    :param app_ins_id: Comma separated list of application instance identifiers.
    :type app_ins_id: List[str]

    :rtype: Union[List[PlmnInfo], Tuple[List[PlmnInfo], int], Tuple[List[PlmnInfo], int, Dict[str, str]]
    """
    return 'do some magic!'


def queries_rabinfo_get(**kwargs):  # noqa: E501 #queries_rabinfo_get(app_ins_id=None, cell_id=None, ue_ipv4_address=None, ue_ipv6_address=None, nated_ip_address=None, gtp_teid=None, erab_id=None, qci=None, erab_mbr_dl=None, erab_mbr_ul=None, erab_gbr_dl=None, erab_gbr_ul=None):  # noqa: E501
    """Retrieve current status of Radio Access Bearer information .

    The GET method is used to query information about the Radio Access Bearers. # noqa: E501

    :param app_ins_id: Comma separated list of application instance identifiers.
    :type app_ins_id: str
    :param cell_id: Comma separated list of E-UTRAN Cell Identities.
    :type cell_id: str
    :param ue_ipv4_address: Comma separated list of IE IPv4 addresses as defined for the type for AssociateId in clause 6.5.4.
    :type ue_ipv4_address: List[str]
    :param ue_ipv6_address: Comma separated list of IE IPv6 addresses as defined for the type for AssociateId in clause 6.5.4.
    :type ue_ipv6_address: List[str]
    :param nated_ip_address: Comma separated list of NATed IP addresses as defined for the type for AssociateId in clause 6.5.4.
    :type nated_ip_address: List[str]
    :param gtp_teid: Comma separated list of GTP TEID addresses as defined for the type for AssociateId in clause 6.5.4.
    :type gtp_teid: List[str]
    :param erab_id: E-RAB identifier.
    :type erab_id: int
    :param qci: QoS Class Identifier as defined in ETSI TS 123 401 [i.4].
    :type qci: int
    :param erab_mbr_dl: Maximum downlink E-RAB Bit Rate as defined in ETSI TS 123 401 [i.4].
    :type erab_mbr_dl: int
    :param erab_mbr_ul: Maximum uplink E-RAB Bit Rate as defined in ETSI TS 123 401 [i.4].
    :type erab_mbr_ul: int
    :param erab_gbr_dl: Guaranteed downlink E-RAB Bit Rate as defined in ETSI TS 123 401 [i.4].
    :type erab_gbr_dl: int
    :param erab_gbr_ul: Guaranteed uplink E-RAB Bit Rate as defined in ETSI TS 123 401 [i.4].
    :type erab_gbr_ul: int

    :rtype: Union[QueriesRabinfoGET200Response, Tuple[QueriesRabinfoGET200Response, int], Tuple[QueriesRabinfoGET200Response, int, Dict[str, str]]
    """
    #return 'do some magic!'

    ue_ipv4_address = kwargs.get('ue_ipv4_address', '10.47.0.3') ##

    print(f"[RNIS] Petición recibida para UE IP: {ue_ipv4_address}")
    
    # URL de tu Prometheus real
    PROMETHEUS_URL = "http://192.168.0.112:9090/api/v1/query"
    
    # Usamos la métrica de usuarios activos de tu lista
    METRICA_BUSCADA = 'upf_pfcp_sessions' #'ues_active'
    
    # Variables por defecto
    calidad_qci = 9 # Por defecto asumimos peor calidad
    usuarios_activos = 0
    
    try:
        print(f"[RNIS] Consultando métrica '{METRICA_BUSCADA}' en {PROMETHEUS_URL}...")
        respuesta = requests.get(PROMETHEUS_URL, params={'query': METRICA_BUSCADA}, timeout=3)
        
        if respuesta.status_code == 200:
            datos_prom = respuesta.json()
            resultados = datos_prom.get('data', {}).get('result', [])
            
            if resultados:
                # Extraemos el valor real de Prometheus
                usuarios_activos = int(float(resultados[0]['value'][1]))
                print(f"[RNIS] ✅ Dato real extraído: {usuarios_activos} usuarios activos en la red")
                
                # =======================================================
                # LÓGICA MEC: Congestión -> Calidad
                # =======================================================
                if usuarios_activos <= 1: 
                    # Celda vacía -> Máxima prioridad para vídeo
                    calidad_qci = 1 
                else:
                    # Celda congestionada -> Bajamos prioridad
                    calidad_qci = 9 
            else:
                print("[RNIS] ⚠️ Prometheus respondió, pero la métrica está vacía.")
        else:
            print(f"[RNIS] ❌ Error HTTP al contactar Prometheus: {respuesta.status_code}")
            
    except Exception as e:
        print(f"[RNIS] ❌ Fallo conectando a Prometheus: {str(e)}")

    # Empaquetado en el estándar ETSI MEC 012
    datos_radio = {
        "rabInfo": {
            "appInstanceId": "App-TFG",
            "cellUserInfo": [
                {
                    "ecgi": {
                        "cellId": "srsRAN-Cell-01",
                        "plmn": {"mcc": "001", "mnc": "01"}
                    },
                    "ueInfo": [
                        {
                            "associateId": [
                                {"type": 1, "value": ue_ipv4_address}
                            ],
                            "erabInfo": [
                                {
                                    "erabId": 1,
                                    "erabQosParameters": {
                                        "qci": calidad_qci
                                    }
                                }
                            ]
                        }
                    ]
                }
            ]
        }
    }
    
    return datos_radio, 200

def queries_s1_bearerinfo_get(temp_ue_id=None, ue_ipv4_address=None, ue_ipv6_address=None, nated_ip_address=None, gtp_teid=None, cell_id=None, erab_id=None):  # noqa: E501
    """Retrieve current status of S1 bearer information.

    The GET method is used to query information about the S1 bearer(s). # noqa: E501

    :param temp_ue_id: Comma separated list of temporary identifiers allocated for the specific UEs as defined in ETSI TS 136 413 [i.3].
    :type temp_ue_id: List[str]
    :param ue_ipv4_address: Comma separated list of IE IPv4 addresses as defined for the type for AssociateId in clause 6.5.4.
    :type ue_ipv4_address: List[str]
    :param ue_ipv6_address: Comma separated list of IE IPv6 addresses as defined for the type for AssociateId in clause 6.5.4.
    :type ue_ipv6_address: List[str]
    :param nated_ip_address: Comma separated list of NATed IP addresses as defined for the type for AssociateId in clause 6.5.4.
    :type nated_ip_address: List[str]
    :param gtp_teid: Comma separated list of GTP TEID addresses as defined for the type for AssociateId in clause 6.5.4.
    :type gtp_teid: List[str]
    :param cell_id: Comma separated list of E-UTRAN Cell Identities.
    :type cell_id: str
    :param erab_id: Comma separated list of E-RAB identifiers.
    :type erab_id: List[int]

    :rtype: Union[QueriesS1BearerinfoGET200Response, Tuple[QueriesS1BearerinfoGET200Response, int], Tuple[QueriesS1BearerinfoGET200Response, int, Dict[str, str]]
    """
    return 'do some magic!'
