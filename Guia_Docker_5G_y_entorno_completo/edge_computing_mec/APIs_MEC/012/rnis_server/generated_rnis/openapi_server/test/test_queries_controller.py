import unittest

from flask import json

from openapi_server.models.plmn_info import PlmnInfo  # noqa: E501
from openapi_server.models.problem_details import ProblemDetails  # noqa: E501
from openapi_server.models.queries_layer2_meas_get200_response import QueriesLayer2MeasGET200Response  # noqa: E501
from openapi_server.models.queries_rabinfo_get200_response import QueriesRabinfoGET200Response  # noqa: E501
from openapi_server.models.queries_s1_bearerinfo_get200_response import QueriesS1BearerinfoGET200Response  # noqa: E501
from openapi_server.test import BaseTestCase


class TestQueriesController(BaseTestCase):
    """QueriesController integration test stubs"""

    def test_queries_layer2_meas_get(self):
        """Test case for queries_layer2_meas_get

        Retrieve current status of layer 2 measurements information.
        """
        query_string = [('app_ins_id', 'app_ins_id_example'),
                        ('cell_id', ['cell_id_example']),
                        ('ue_ipv4_address', ['ue_ipv4_address_example']),
                        ('ue_ipv6_address', ['ue_ipv6_address_example']),
                        ('nated_ip_address', ['nated_ip_address_example']),
                        ('gtp_teid', ['gtp_teid_example']),
                        ('dl_gbr_prb_usage_cell', 56),
                        ('ul_gbr_prb_usage_cell', 56),
                        ('dl_nongbr_prb_usage_cell', 56),
                        ('ul_nongbr_prb_usage_cell', 56),
                        ('dl_total_prb_usage_cell', 56),
                        ('ul_total_prb_usage_cell', 56),
                        ('received_dedicated_preambles_cell', 56),
                        ('received_randomly_selecte_preambles_low_range_cell', 56),
                        ('received_randomly_selected_preambles_high_range_cell', 56),
                        ('number_of_active_ue_dl_gbr_cell', 56),
                        ('number_of_active_ue_ul_gbr_cell', 56),
                        ('number_of_active_ue_dl_nongbr_cell', 56),
                        ('number_of_active_ue_ul_nongbr_cell', 56),
                        ('dl_gbr_pdr_cell', 56),
                        ('ul_gbr_pdr_cell', 56),
                        ('dl_nongbr_pdr_cell', 56),
                        ('ul_nongbr_pdr_cell', 56),
                        ('dl_gbr_delay_ue', 56),
                        ('ul_gbr_delay_ue', 56),
                        ('dl_nongbr_delay_ue', 56),
                        ('ul_nongbr_delay_ue', 56),
                        ('dl_gbr_pdr_ue', 56),
                        ('ul_gbr_pdr_ue', 56),
                        ('dl_nongbr_pdr_ue', 56),
                        ('ul_nongbr_pdr_ue', 56),
                        ('dl_gbr_throughput_ue', 56),
                        ('ul_gbr_throughput_ue', 56),
                        ('dl_nongbr_throughput_ue', 56),
                        ('ul_nongbr_throughput_ue', 56),
                        ('dl_gbr_data_volume_ue', 56),
                        ('ul_gbr_data_volume_ue', 56),
                        ('dl_nongbr_data_volume_ue', 56),
                        ('ul_nongbr_data_volume_ue', 56)]
        headers = { 
            'Accept': 'application/json',
        }
        response = self.client.open(
            '/rni/v2/queries/layer2_meas',
            method='GET',
            headers=headers,
            query_string=query_string)
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_queries_plmninfo_get(self):
        """Test case for queries_plmninfo_get

        Retrieve current status of PLMN information.
        """
        query_string = [('app_ins_id', ['app_ins_id_example'])]
        headers = { 
            'Accept': 'application/json',
        }
        response = self.client.open(
            '/rni/v2/queries/plmn_info',
            method='GET',
            headers=headers,
            query_string=query_string)
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_queries_rabinfo_get(self):
        """Test case for queries_rabinfo_get

        Retrieve current status of Radio Access Bearer information .
        """
        query_string = [('app_ins_id', 'app_ins_id_example'),
                        ('cell_id', 'cell_id_example'),
                        ('ue_ipv4_address', ['ue_ipv4_address_example']),
                        ('ue_ipv6_address', ['ue_ipv6_address_example']),
                        ('nated_ip_address', ['nated_ip_address_example']),
                        ('gtp_teid', ['gtp_teid_example']),
                        ('erab_id', 56),
                        ('qci', 56),
                        ('erab_mbr_dl', 56),
                        ('erab_mbr_ul', 56),
                        ('erab_gbr_dl', 56),
                        ('erab_gbr_ul', 56)]
        headers = { 
            'Accept': 'application/json',
        }
        response = self.client.open(
            '/rni/v2/queries/rab_info',
            method='GET',
            headers=headers,
            query_string=query_string)
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_queries_s1_bearerinfo_get(self):
        """Test case for queries_s1_bearerinfo_get

        Retrieve current status of S1 bearer information.
        """
        query_string = [('temp_ue_id', ['temp_ue_id_example']),
                        ('ue_ipv4_address', ['ue_ipv4_address_example']),
                        ('ue_ipv6_address', ['ue_ipv6_address_example']),
                        ('nated_ip_address', ['nated_ip_address_example']),
                        ('gtp_teid', ['gtp_teid_example']),
                        ('cell_id', 'cell_id_example'),
                        ('erab_id', [56])]
        headers = { 
            'Accept': 'application/json',
        }
        response = self.client.open(
            '/rni/v2/queries/s1_bearer_info',
            method='GET',
            headers=headers,
            query_string=query_string)
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))


if __name__ == '__main__':
    unittest.main()
