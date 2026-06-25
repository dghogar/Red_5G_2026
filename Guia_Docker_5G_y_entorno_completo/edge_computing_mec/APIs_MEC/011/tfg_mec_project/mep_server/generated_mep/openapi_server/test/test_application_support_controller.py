import unittest

from flask import json

from openapi_server.models.applications_confirmready_post_appinstanceid_request import ApplicationsConfirmreadyPOSTAppinstanceidRequest  # noqa: E501
from openapi_server.models.applications_confirmtermination_post_appinstanceid_request import ApplicationsConfirmterminationPOSTAppinstanceidRequest  # noqa: E501
from openapi_server.models.applications_dnsrules_get_appinstanceid_dnsruleid200_response import ApplicationsDnsrulesGETAppinstanceidDnsruleid200Response  # noqa: E501
from openapi_server.models.applications_subscriptions_get_appinstanceid200_response import ApplicationsSubscriptionsGETAppinstanceid200Response  # noqa: E501
from openapi_server.models.applications_subscriptions_post_appinstanceid_request import ApplicationsSubscriptionsPOSTAppinstanceidRequest  # noqa: E501
from openapi_server.models.applications_trafficrules_get_appinstanceid_trafficruleid200_response import ApplicationsTrafficrulesGETAppinstanceidTrafficruleid200Response  # noqa: E501
from openapi_server.models.dns_rule import DnsRule  # noqa: E501
from openapi_server.models.problem_details import ProblemDetails  # noqa: E501
from openapi_server.models.registrations_post_request import RegistrationsPOSTRequest  # noqa: E501
from openapi_server.models.timing_currenttime_get200_response import TimingCurrenttimeGET200Response  # noqa: E501
from openapi_server.models.timing_timingcaps_get200_response import TimingTimingcapsGET200Response  # noqa: E501
from openapi_server.models.traffic_rule import TrafficRule  # noqa: E501
from openapi_server.test import BaseTestCase


class TestApplicationSupportController(BaseTestCase):
    """ApplicationSupportController integration test stubs"""

    def test_applications_confirmready_post_appinstanceid(self):
        """Test case for applications_confirmready_post_appinstanceid

        Confirm the application instance is up and running.
        """
        applications_confirmready_post_appinstanceid_request = openapi_server.ApplicationsConfirmreadyPOSTAppinstanceidRequest()
        headers = { 
            'Accept': 'application/problem+json',
            'Content-Type': 'application/json',
        }
        response = self.client.open(
            '/mec_app_support/v1/applications/{app_instance_id}/confirm_ready'.format(app_instance_id='app_instance_id_example'),
            method='POST',
            headers=headers,
            data=json.dumps(applications_confirmready_post_appinstanceid_request),
            content_type='application/json')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_applications_confirmtermination_post_appinstanceid(self):
        """Test case for applications_confirmtermination_post_appinstanceid

        Confirm the application level termination of an App instance.
        """
        applications_confirmtermination_post_appinstanceid_request = openapi_server.ApplicationsConfirmterminationPOSTAppinstanceidRequest()
        headers = { 
            'Accept': 'application/problem+json',
            'Content-Type': 'application/json',
        }
        response = self.client.open(
            '/mec_app_support/v1/applications/{app_instance_id}/confirm_termination'.format(app_instance_id='app_instance_id_example'),
            method='POST',
            headers=headers,
            data=json.dumps(applications_confirmtermination_post_appinstanceid_request),
            content_type='application/json')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_applications_dnsrules_get_appinstanceid(self):
        """Test case for applications_dnsrules_get_appinstanceid

        Retrieve information about a list of mecDnsRule resources for an application instance.
        """
        headers = { 
            'Accept': 'application/json',
        }
        response = self.client.open(
            '/mec_app_support/v1/applications/{app_instance_id}/dns_rules'.format(app_instance_id='app_instance_id_example'),
            method='GET',
            headers=headers)
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_applications_dnsrules_get_appinstanceid_dnsruleid(self):
        """Test case for applications_dnsrules_get_appinstanceid_dnsruleid

        Retrieve information about a mecDnsRule resource.
        """
        headers = { 
            'Accept': 'application/json',
        }
        response = self.client.open(
            '/mec_app_support/v1/applications/{app_instance_id}/dns_rules/{dns_rule_id}'.format(app_instance_id='app_instance_id_example', dns_rule_id='dns_rule_id_example'),
            method='GET',
            headers=headers)
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_applications_dnsrules_put_appinstanceid_dnsruleid(self):
        """Test case for applications_dnsrules_put_appinstanceid_dnsruleid

        Update the information about a mecDnsRule resource.
        """
        applications_dnsrules_get_appinstanceid_dnsruleid200_response = openapi_server.ApplicationsDnsrulesGETAppinstanceidDnsruleid200Response()
        headers = { 
            'Accept': 'application/json',
            'Content-Type': 'application/json',
        }
        response = self.client.open(
            '/mec_app_support/v1/applications/{app_instance_id}/dns_rules/{dns_rule_id}'.format(app_instance_id='app_instance_id_example', dns_rule_id='dns_rule_id_example'),
            method='PUT',
            headers=headers,
            data=json.dumps(applications_dnsrules_get_appinstanceid_dnsruleid200_response),
            content_type='application/json')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_applications_subscriptions_delete_appinstanceid_subscriptionid(self):
        """Test case for applications_subscriptions_delete_appinstanceid_subscriptionid

        Delete a mecAppSupportSubscription resource.
        """
        headers = { 
            'Accept': 'application/problem+json',
        }
        response = self.client.open(
            '/mec_app_support/v1/applications/{app_instance_id}/subscriptions/{subscription_id}'.format(app_instance_id='app_instance_id_example', subscription_id='subscription_id_example'),
            method='DELETE',
            headers=headers)
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_applications_subscriptions_get_appinstanceid(self):
        """Test case for applications_subscriptions_get_appinstanceid

        Retrieve information about a list of mecAppSupportSubscription resources for this subscriber.
        """
        headers = { 
            'Accept': 'application/json',
        }
        response = self.client.open(
            '/mec_app_support/v1/applications/{app_instance_id}/subscriptions'.format(app_instance_id='app_instance_id_example'),
            method='GET',
            headers=headers)
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_applications_subscriptions_get_appinstanceid_subscriptionid(self):
        """Test case for applications_subscriptions_get_appinstanceid_subscriptionid

        Retrieve information about a mecAppSupportSubscription resource for this subscriber.
        """
        headers = { 
            'Accept': 'application/json',
        }
        response = self.client.open(
            '/mec_app_support/v1/applications/{app_instance_id}/subscriptions/{subscription_id}'.format(app_instance_id='app_instance_id_example', subscription_id='subscription_id_example'),
            method='GET',
            headers=headers)
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_applications_subscriptions_post_appinstanceid(self):
        """Test case for applications_subscriptions_post_appinstanceid

        Create a mecAppSupportSubscription resource.
        """
        applications_subscriptions_post_appinstanceid_request = openapi_server.ApplicationsSubscriptionsPOSTAppinstanceidRequest()
        headers = { 
            'Accept': 'application/json',
            'Content-Type': 'application/json',
        }
        response = self.client.open(
            '/mec_app_support/v1/applications/{app_instance_id}/subscriptions'.format(app_instance_id='app_instance_id_example'),
            method='POST',
            headers=headers,
            data=json.dumps(applications_subscriptions_post_appinstanceid_request),
            content_type='application/json')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_applications_trafficrules_get_appinstanceid(self):
        """Test case for applications_trafficrules_get_appinstanceid

        Retrieve information about a list of mecTrafficRule resources for an application instance.
        """
        headers = { 
            'Accept': 'application/json',
        }
        response = self.client.open(
            '/mec_app_support/v1/applications/{app_instance_id}/traffic_rules'.format(app_instance_id='app_instance_id_example'),
            method='GET',
            headers=headers)
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_applications_trafficrules_get_appinstanceid_trafficruleid(self):
        """Test case for applications_trafficrules_get_appinstanceid_trafficruleid

        Retrieve information about a mecTrafficRule resource.
        """
        headers = { 
            'Accept': 'application/json',
        }
        response = self.client.open(
            '/mec_app_support/v1/applications/{app_instance_id}/traffic_rules/{traffic_rule_id}'.format(app_instance_id='app_instance_id_example', traffic_rule_id='traffic_rule_id_example'),
            method='GET',
            headers=headers)
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_applications_trafficrules_put_appinstanceid_trafficruleid(self):
        """Test case for applications_trafficrules_put_appinstanceid_trafficruleid

        Update the information about a mecTrafficRule resource.
        """
        applications_trafficrules_get_appinstanceid_trafficruleid200_response = openapi_server.ApplicationsTrafficrulesGETAppinstanceidTrafficruleid200Response()
        headers = { 
            'Accept': 'application/json',
            'Content-Type': 'application/json',
        }
        response = self.client.open(
            '/mec_app_support/v1/applications/{app_instance_id}/traffic_rules/{traffic_rule_id}'.format(app_instance_id='app_instance_id_example', traffic_rule_id='traffic_rule_id_example'),
            method='PUT',
            headers=headers,
            data=json.dumps(applications_trafficrules_get_appinstanceid_trafficruleid200_response),
            content_type='application/json')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_registrations_delete_appinstanceid(self):
        """Test case for registrations_delete_appinstanceid

        Request deregistration of the application instance from the MEC platform.
        """
        headers = { 
            'Accept': 'application/problem+json',
        }
        response = self.client.open(
            '/mec_app_support/v1/registrations/{app_instance_id}'.format(app_instance_id='app_instance_id_example'),
            method='DELETE',
            headers=headers)
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_registrations_get_appinstanceid(self):
        """Test case for registrations_get_appinstanceid

        Retrieve information about the MEC application instance registration to the MEC platform.
        """
        headers = { 
            'Accept': 'application/json',
        }
        response = self.client.open(
            '/mec_app_support/v1/registrations/{app_instance_id}'.format(app_instance_id='app_instance_id_example'),
            method='GET',
            headers=headers)
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_registrations_post(self):
        """Test case for registrations_post

        Register the MEC application instance to the MEC platform.
        """
        registrations_post_request = openapi_server.RegistrationsPOSTRequest()
        headers = { 
            'Accept': 'application/json',
            'Content-Type': 'application/json',
        }
        response = self.client.open(
            '/mec_app_support/v1/registrations',
            method='POST',
            headers=headers,
            data=json.dumps(registrations_post_request),
            content_type='application/json')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_registrations_put_appinstanceid(self):
        """Test case for registrations_put_appinstanceid

        Update the existing registration of that MEC application instance to the MEC platform.
        """
        registrations_post_request = openapi_server.RegistrationsPOSTRequest()
        headers = { 
            'Accept': 'application/json',
            'Content-Type': 'application/json',
        }
        response = self.client.open(
            '/mec_app_support/v1/registrations/{app_instance_id}'.format(app_instance_id='app_instance_id_example'),
            method='PUT',
            headers=headers,
            data=json.dumps(registrations_post_request),
            content_type='application/json')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_timing_currenttime_get(self):
        """Test case for timing_currenttime_get

        Retrieve information about the mecCurrentTime resource.
        """
        headers = { 
            'Accept': 'application/json',
        }
        response = self.client.open(
            '/mec_app_support/v1/timing/current_time',
            method='GET',
            headers=headers)
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_timing_timingcaps_get(self):
        """Test case for timing_timingcaps_get

        Retrieve information about the mecTimingCaps resource.
        """
        headers = { 
            'Accept': 'application/json',
        }
        response = self.client.open(
            '/mec_app_support/v1/timing/timing_caps',
            method='GET',
            headers=headers)
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))


if __name__ == '__main__':
    unittest.main()
