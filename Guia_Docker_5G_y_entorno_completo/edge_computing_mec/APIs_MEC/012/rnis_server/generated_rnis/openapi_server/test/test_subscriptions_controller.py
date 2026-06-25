import unittest

from flask import json

from openapi_server.models.problem_details import ProblemDetails  # noqa: E501
from openapi_server.models.subscriptions_get200_response import SubscriptionsGET200Response  # noqa: E501
from openapi_server.models.subscriptions_post_request import SubscriptionsPOSTRequest  # noqa: E501
from openapi_server.test import BaseTestCase


class TestSubscriptionsController(BaseTestCase):
    """SubscriptionsController integration test stubs"""

    def test_subscriptions_delete_subscriptionid(self):
        """Test case for subscriptions_delete_subscriptionid

        Cancel the existing subscription.
        """
        headers = { 
            'Accept': 'application/problem+json',
        }
        response = self.client.open(
            '/rni/v2/subscriptions/{subscription_id}'.format(subscription_id='subscription_id_example'),
            method='DELETE',
            headers=headers)
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_subscriptions_get(self):
        """Test case for subscriptions_get

        Retrieve a list of active subscriptions for this subscriber.
        """
        query_string = [('subscription_type', 'subscription_type_example')]
        headers = { 
            'Accept': 'application/json',
        }
        response = self.client.open(
            '/rni/v2/subscriptions',
            method='GET',
            headers=headers,
            query_string=query_string)
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_subscriptions_get_subscriptionid(self):
        """Test case for subscriptions_get_subscriptionid

        Retrieve information on current specific subscription.
        """
        headers = { 
            'Accept': 'application/json',
        }
        response = self.client.open(
            '/rni/v2/subscriptions/{subscription_id}'.format(subscription_id='subscription_id_example'),
            method='GET',
            headers=headers)
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_subscriptions_post(self):
        """Test case for subscriptions_post

        Create a new subscription.
        """
        subscriptions_post_request = openapi_server.SubscriptionsPOSTRequest()
        headers = { 
            'Accept': 'application/json',
            'Content-Type': 'application/json',
        }
        response = self.client.open(
            '/rni/v2/subscriptions',
            method='POST',
            headers=headers,
            data=json.dumps(subscriptions_post_request),
            content_type='application/json')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_subscriptions_put_subscriptionid(self):
        """Test case for subscriptions_put_subscriptionid

        Modify existing subscription by sending a new data structure.
        """
        subscriptions_post_request = openapi_server.SubscriptionsPOSTRequest()
        headers = { 
            'Accept': 'application/json',
            'Content-Type': 'application/json',
        }
        response = self.client.open(
            '/rni/v2/subscriptions/{subscription_id}'.format(subscription_id='subscription_id_example'),
            method='PUT',
            headers=headers,
            data=json.dumps(subscriptions_post_request),
            content_type='application/json')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))


if __name__ == '__main__':
    unittest.main()
