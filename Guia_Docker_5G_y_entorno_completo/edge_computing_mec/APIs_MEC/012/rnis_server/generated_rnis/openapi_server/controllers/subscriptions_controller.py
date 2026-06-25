import connexion
from typing import Dict
from typing import Tuple
from typing import Union

from openapi_server.models.problem_details import ProblemDetails  # noqa: E501
from openapi_server.models.subscriptions_get200_response import SubscriptionsGET200Response  # noqa: E501
from openapi_server.models.subscriptions_post_request import SubscriptionsPOSTRequest  # noqa: E501
from openapi_server import util


def subscriptions_delete_subscriptionid(subscription_id):  # noqa: E501
    """Cancel the existing subscription.

    The DELETE method is used to cancel the existing subscription. Cancellation can be made by deleting the resource that represents existing subscription. # noqa: E501

    :param subscription_id: The unique identifier of the subscriptionId.
    :type subscription_id: str

    :rtype: Union[None, Tuple[None, int], Tuple[None, int, Dict[str, str]]
    """
    return 'do some magic!'


def subscriptions_get(subscription_type=None):  # noqa: E501
    """Retrieve a list of active subscriptions for this subscriber.

    The GET method is used to request information about the subscriptions for this requestor. Upon success, the response contains entity body with the list of links to the subscriptions that are present for the requestor. # noqa: E501

    :param subscription_type: Query parameter to filter on a specific subscription type. Permitted values: cell_change Cell Change  rab_est RAB Establishment rab_mod RAB Modification rab_rel RAB Release meas_rep_ue UE Measurement Report nr_meas_rep_ue 5G UE Measurement Report timing_advance_ue UE Timing Advance ca_reconf Carrier Aggregation Reconfig s1_bearer S1 Bearer Notification .
    :type subscription_type: str

    :rtype: Union[SubscriptionsGET200Response, Tuple[SubscriptionsGET200Response, int], Tuple[SubscriptionsGET200Response, int, Dict[str, str]]
    """
    return 'do some magic!'


def subscriptions_get_subscriptionid(subscription_id):  # noqa: E501
    """Retrieve information on current specific subscription.

    The GET method is used to retrieve information about this subscription. Upon success, the response contains entity body with the data type describing the subscription. # noqa: E501

    :param subscription_id: The unique identifier of the subscriptionId.
    :type subscription_id: str

    :rtype: Union[SubscriptionsPOSTRequest, Tuple[SubscriptionsPOSTRequest, int], Tuple[SubscriptionsPOSTRequest, int, Dict[str, str]]
    """
    return 'do some magic!'


def subscriptions_post(body=None):  # noqa: E501
    """Create a new subscription.

    The POST method is used to create a new subscription to Radio Network Information notifications. Upon success, the response contains entity body describing the created subscription. # noqa: E501

    :param subscriptions_post_request: The entity body in the request contains data type of the specific RNI event subscription that is to be created, where the data type options are listed below and defined in clauses 6.3.2 through 6.3.9 and in clause 6.3.11: CellChangeSubscription RabEstSubscription RabModSubscription RabRelSubscription MeasRepUeSubscription NrMeasRepUeSubscription MeasTaSubscription CaReconfSubscription S1BearerSubscription.
    :type subscriptions_post_request: dict | bytes

    :rtype: Union[SubscriptionsPOSTRequest, Tuple[SubscriptionsPOSTRequest, int], Tuple[SubscriptionsPOSTRequest, int, Dict[str, str]]
    """
    subscriptions_post_request = body
    if connexion.request.is_json:
        subscriptions_post_request = SubscriptionsPOSTRequest.from_dict(connexion.request.get_json())  # noqa: E501
    return 'do some magic!'


def subscriptions_put_subscriptionid(subscription_id, body=None):  # noqa: E501
    """Modify existing subscription by sending a new data structure.

    The PUT method is used to update the existing subscription. PUT method in this case has \&quot;replace\&quot; semantics. Upon successful operation, the target resource is updated with new Data Type received within the message body of the PUT request.  # noqa: E501

    :param subscription_id: The unique identifier of the subscriptionId.
    :type subscription_id: str
    :param subscriptions_post_request: New NotificationSubscription is included as entity body of the request. The allowed data types for subscriptions are defined in clauses 6.3.2 through 6.3.9 and in clause 6.3.11 and are as follows: CellChangeSubscription RabEstSubscription RabModSubscription RabRelSubscription MeasRepUeSubscription NrMeasRepUeSubscription MeasTaSubscription CaReconfSubscription S1BearerSubscription.
    :type subscriptions_post_request: dict | bytes

    :rtype: Union[SubscriptionsPOSTRequest, Tuple[SubscriptionsPOSTRequest, int], Tuple[SubscriptionsPOSTRequest, int, Dict[str, str]]
    """
    subscriptions_post_request = body
    if connexion.request.is_json:
        subscriptions_post_request = SubscriptionsPOSTRequest.from_dict(connexion.request.get_json())  # noqa: E501
    return 'do some magic!'
