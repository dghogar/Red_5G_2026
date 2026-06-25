import connexion
from typing import Dict
from typing import Tuple
from typing import Union

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
from openapi_server import util

###
# Base de datos en memoria para registrar las aplicaciones Edge (TFG)
MEC_APPS_REGISTRY = {}
# Base de datos en memoria para las Reglas DNS (Idea 2)
MEC_DNS_RULES = {}
###

def applications_confirmready_post_appinstanceid(app_instance_id, body):  # noqa: E501
    """Confirm the application instance is up and running.

    The POST method may be used by the MEC application instance to notify the MEC platform that it is up and running. POST HTTP method shall support the URI query parameters, request and response data structures, and response codes, as specified in tables 7.2.12.3.4-1 and 7.2.12.3.4-2. # noqa: E501

    :param app_instance_id: The unique identifier of the appInstanceId.
    :type app_instance_id: str
    :param applications_confirmready_post_appinstanceid_request: Message content in the request contains the indication that the application instance is up and running.
    :type applications_confirmready_post_appinstanceid_request: dict | bytes

    :rtype: Union[None, Tuple[None, int], Tuple[None, int, Dict[str, str]]
    """
    #applications_confirmready_post_appinstanceid_request = body
    #if connexion.request.is_json:
    #    applications_confirmready_post_appinstanceid_request = ApplicationsConfirmreadyPOSTAppinstanceidRequest.from_dict(connexion.request.get_json())  # noqa: E501
    #return 'do some magic!'

    applications_confirmready_post_appinstanceid_request = body
    if connexion.request.is_json:
        applications_confirmready_post_appinstanceid_request = ApplicationsConfirmreadyPOSTAppinstanceidRequest.from_dict(connexion.request.get_json())  # noqa: E501
    
    # === INICIO DE LA LÓGICA DEL TFG ===
    if app_instance_id in MEC_APPS_REGISTRY:
        # Comprobamos qué nos dice la app (debería decir indication: "READY")
        estado = applications_confirmready_post_appinstanceid_request.indication
        print(f"[MEP] 🚀 App {app_instance_id} reporta estado: {estado}")
        
        # En el estándar MEC011, esta ruta devuelve un HTTP 204 (No Content) si todo va bien
        return None, 204
    else:
        # Si intenta confirmarse pero no está registrada, da error 404
        return ProblemDetails(detail="App no registrada. Registrese primero.", status=404), 404
    # === FIN DE LA LÓGICA DEL TFG ===


def applications_confirmtermination_post_appinstanceid(app_instance_id, body):  # noqa: E501
    """Confirm the application level termination of an App instance.

    Table 7.2.11.3.4-1: URI query parameters supported by the POST method on this resource # noqa: E501

    :param app_instance_id: The unique identifier of the appInstanceId.
    :type app_instance_id: str
    :param applications_confirmtermination_post_appinstanceid_request: Message content in the request contains the operational action the application instance is responding to.
    :type applications_confirmtermination_post_appinstanceid_request: dict | bytes

    :rtype: Union[None, Tuple[None, int], Tuple[None, int, Dict[str, str]]
    """
    applications_confirmtermination_post_appinstanceid_request = body
    if connexion.request.is_json:
        applications_confirmtermination_post_appinstanceid_request = ApplicationsConfirmterminationPOSTAppinstanceidRequest.from_dict(connexion.request.get_json())  # noqa: E501
    return 'do some magic!'


def applications_dnsrules_get_appinstanceid(app_instance_id):  # noqa: E501
    """Retrieve information about a list of mecDnsRule resources for an application instance.

    This method retrieves information about all the DNS rules associated with a MEC application instance. # noqa: E501

    :param app_instance_id: The unique identifier of the appInstanceId.
    :type app_instance_id: str

    :rtype: Union[List[DnsRule], Tuple[List[DnsRule], int], Tuple[List[DnsRule], int, Dict[str, str]]
    """
    #return 'do some magic!'

    """[LÓGICA TFG - IDEA 2] Devuelve las reglas DNS de una App específica"""
    global MEC_DNS_RULES
    
    if app_instance_id in MEC_DNS_RULES:
        # Devolvemos la lista de reglas DNS que tenga esta app
        return list(MEC_DNS_RULES[app_instance_id].values()), 200
    
    # IMPORTANTE: Devolvemos una LISTA VACÍA [], no un string
    return [], 200


def applications_dnsrules_get_appinstanceid_dnsruleid(app_instance_id, dns_rule_id):  # noqa: E501
    """Retrieve information about a mecDnsRule resource.

    This method retrieves information about a DNS rule associated with a MEC application instance.  # noqa: E501

    :param app_instance_id: The unique identifier of the appInstanceId.
    :type app_instance_id: str
    :param dns_rule_id: The unique identifier of the dnsRuleId.
    :type dns_rule_id: str

    :rtype: Union[ApplicationsDnsrulesGETAppinstanceidDnsruleid200Response, Tuple[ApplicationsDnsrulesGETAppinstanceidDnsruleid200Response, int], Tuple[ApplicationsDnsrulesGETAppinstanceidDnsruleid200Response, int, Dict[str, str]]
    """
    #return 'do some magic!'

    """[LÓGICA TFG - IDEA 2] Devuelve las reglas DNS de una App específica"""
    global MEC_DNS_RULES
    
    # Si la aplicación existe y tiene reglas DNS inyectadas, las devolvemos
    if app_instance_id in MEC_DNS_RULES:
        # Convertimos los valores del diccionario en una lista para el JSON
        lista_reglas = list(MEC_DNS_RULES[app_instance_id].values())
        return lista_reglas, 200
    
    # Si la app no tiene reglas (o no existe), devolvemos una lista vacía y un 200 OK
    return [], 200


def applications_dnsrules_put_appinstanceid_dnsruleid(app_instance_id, dns_rule_id, body):  # noqa: E501
    """Update the information about a mecDnsRule resource.

    PUT HTTP method shall comply with the URI query parameters, request and response data structures, and response codes, as specified in tables 7.2.10.3.2-1 and 7.2.10.3.2-2. # noqa: E501

    :param app_instance_id: The unique identifier of the appInstanceId.
    :type app_instance_id: str
    :param dns_rule_id: The unique identifier of the dnsRuleId.
    :type dns_rule_id: str
    :param applications_dnsrules_get_appinstanceid_dnsruleid200_response: The updated \&quot;state\&quot; is included in the message content of the request.
    :type applications_dnsrules_get_appinstanceid_dnsruleid200_response: dict | bytes

    :rtype: Union[ApplicationsDnsrulesGETAppinstanceidDnsruleid200Response, Tuple[ApplicationsDnsrulesGETAppinstanceidDnsruleid200Response, int], Tuple[ApplicationsDnsrulesGETAppinstanceidDnsruleid200Response, int, Dict[str, str]]
    """
    #applications_dnsrules_get_appinstanceid_dnsruleid200_response = body
    #if connexion.request.is_json:
    #    applications_dnsrules_get_appinstanceid_dnsruleid200_response = ApplicationsDnsrulesGETAppinstanceidDnsruleid200Response.from_dict(connexion.request.get_json())  # noqa: E501
    #return 'do some magic!'

    """[LÓGICA TFG - IDEA 2] Guarda una regla DNS inyectada por una App Edge"""
    global MEC_DNS_RULES
    
    try:
        import connexion
        raw_data = connexion.request.get_json()
        
        # Si la app no tiene aún un hueco para sus reglas, se lo creamos
        if app_instance_id not in MEC_DNS_RULES:
            MEC_DNS_RULES[app_instance_id] = {}
            
        # Guardamos la regla DNS usando su ID
        MEC_DNS_RULES[app_instance_id][dns_rule_id] = raw_data
        
        dominio = raw_data.get('domainName', 'Desconocido')
        ip_destino = raw_data.get('ipAddress', 'Desconocida')
        
        print(f"[MEP DNS] 🌐 ¡Nueva Regla Inyectada para {app_instance_id}!")
        print(f"[MEP DNS] 🔀 Tráfico de '{dominio}' redirigido a -> {ip_destino}")
        
        # El estándar exige devolver la regla creada y un HTTP 200 OK
        return raw_data, 200
    except Exception as e:
        print(f"[MEP DNS] ❌ Error procesando regla DNS: {e}")
        return {"detail": str(e), "status": 400}, 400



def applications_subscriptions_delete_appinstanceid_subscriptionid(app_instance_id, subscription_id):  # noqa: E501
    """Delete a mecAppSupportSubscription resource.

    This method deletes a mecAppSupportSubscription. This method is typically used in \&quot;Unsubscribing from event notifications\&quot; procedure as described in clause 5.2.6.3. Figure 7.2.4.3.5-1 shows the example message flows using DELETE method. # noqa: E501

    :param app_instance_id: The unique identifier of the appInstanceId.
    :type app_instance_id: str
    :param subscription_id: The unique identifier of the subscriptionId.
    :type subscription_id: str

    :rtype: Union[None, Tuple[None, int], Tuple[None, int, Dict[str, str]]
    """
    return 'do some magic!'


def applications_subscriptions_get_appinstanceid(app_instance_id):  # noqa: E501
    """Retrieve information about a list of mecAppSupportSubscription resources for this subscriber.

    The GET method may be used to request information about all subscriptions for this requestor. Upon success, the response contains message content with all the subscriptions for the requestor. # noqa: E501

    :param app_instance_id: The unique identifier of the appInstanceId.
    :type app_instance_id: str

    :rtype: Union[ApplicationsSubscriptionsGETAppinstanceid200Response, Tuple[ApplicationsSubscriptionsGETAppinstanceid200Response, int], Tuple[ApplicationsSubscriptionsGETAppinstanceid200Response, int, Dict[str, str]]
    """
    return 'do some magic!'


def applications_subscriptions_get_appinstanceid_subscriptionid(app_instance_id, subscription_id):  # noqa: E501
    """Retrieve information about a mecAppSupportSubscription resource for this subscriber.

    The GET method requests information about a subscription for this requestor. Upon success, the response contains message content with the subscription for the requestor. # noqa: E501

    :param app_instance_id: The unique identifier of the appInstanceId.
    :type app_instance_id: str
    :param subscription_id: The unique identifier of the subscriptionId.
    :type subscription_id: str

    :rtype: Union[ApplicationsSubscriptionsPOSTAppinstanceidRequest, Tuple[ApplicationsSubscriptionsPOSTAppinstanceidRequest, int], Tuple[ApplicationsSubscriptionsPOSTAppinstanceidRequest, int, Dict[str, str]]
    """
    return 'do some magic!'


def applications_subscriptions_post_appinstanceid(app_instance_id, body):  # noqa: E501
    """Create a mecAppSupportSubscription resource.


    The POST method may be used to create a new subscription. One example use case is to create a new subscription to the MEC application termination notifications. Upon success, the response contains message content describing the created subscription.  # noqa: E501

    :param app_instance_id: The unique identifier of the appInstanceId.
    :type app_instance_id: str
    :param applications_subscriptions_post_appinstanceid_request: Message content in the request contains a subscription to the MEC application termination notifications that is to be created.
    :type applications_subscriptions_post_appinstanceid_request: dict | bytes

    :rtype: Union[ApplicationsSubscriptionsPOSTAppinstanceidRequest, Tuple[ApplicationsSubscriptionsPOSTAppinstanceidRequest, int], Tuple[ApplicationsSubscriptionsPOSTAppinstanceidRequest, int, Dict[str, str]]
    """
    applications_subscriptions_post_appinstanceid_request = body
    if connexion.request.is_json:
        applications_subscriptions_post_appinstanceid_request = ApplicationsSubscriptionsPOSTAppinstanceidRequest.from_dict(connexion.request.get_json())  # noqa: E501
    return 'do some magic!'


def applications_trafficrules_get_appinstanceid(app_instance_id):  # noqa: E501
    """Retrieve information about a list of mecTrafficRule resources for an application instance.

    This method retrieves information about all the traffic rules associated with a MEC application instance. # noqa: E501

    :param app_instance_id: The unique identifier of the appInstanceId.
    :type app_instance_id: str

    :rtype: Union[List[TrafficRule], Tuple[List[TrafficRule], int], Tuple[List[TrafficRule], int, Dict[str, str]]
    """
    return 'do some magic!'


def applications_trafficrules_get_appinstanceid_trafficruleid(app_instance_id, traffic_rule_id):  # noqa: E501
    """Retrieve information about a mecTrafficRule resource.

    This method retrieves information about a traffic rule associated with a MEC application instance. # noqa: E501

    :param app_instance_id: The unique identifier of the appInstanceId.
    :type app_instance_id: str
    :param traffic_rule_id: The unique identifier of the trafficRuleId.
    :type traffic_rule_id: str

    :rtype: Union[ApplicationsTrafficrulesGETAppinstanceidTrafficruleid200Response, Tuple[ApplicationsTrafficrulesGETAppinstanceidTrafficruleid200Response, int], Tuple[ApplicationsTrafficrulesGETAppinstanceidTrafficruleid200Response, int, Dict[str, str]]
    """
    return 'do some magic!'


def applications_trafficrules_put_appinstanceid_trafficruleid(app_instance_id, traffic_rule_id, body):  # noqa: E501
    """Update the information about a mecTrafficRule resource.

    PUT HTTP method shall comply with the URI query parameters, request and response data structures, and response codes, as specified in tables 7.2.8.3.2-1 and 7.2.8.3.2-2. # noqa: E501

    :param app_instance_id: The unique identifier of the appInstanceId.
    :type app_instance_id: str
    :param traffic_rule_id: The unique identifier of the trafficRuleId.
    :type traffic_rule_id: str
    :param applications_trafficrules_get_appinstanceid_trafficruleid200_response: One or more updated attributes that are allowed to be changed (i.e. \&quot;state\&quot; or other attributes based on definition in clause 7.1.2.2) are included in the TrafficRule data structure in the message content of the request.
    :type applications_trafficrules_get_appinstanceid_trafficruleid200_response: dict | bytes

    :rtype: Union[ApplicationsTrafficrulesGETAppinstanceidTrafficruleid200Response, Tuple[ApplicationsTrafficrulesGETAppinstanceidTrafficruleid200Response, int], Tuple[ApplicationsTrafficrulesGETAppinstanceidTrafficruleid200Response, int, Dict[str, str]]
    """
    applications_trafficrules_get_appinstanceid_trafficruleid200_response = body
    if connexion.request.is_json:
        applications_trafficrules_get_appinstanceid_trafficruleid200_response = ApplicationsTrafficrulesGETAppinstanceidTrafficruleid200Response.from_dict(connexion.request.get_json())  # noqa: E501
    return 'do some magic!'


def registrations_delete_appinstanceid(app_instance_id):  # noqa: E501
    """Request deregistration of the application instance from the MEC platform.

    The DELETE method is used to cancel the existing MEC application instance registration. Cancellation can be made by deleting the resource that represents the existing MEC application instance registration. DELETE HTTP method shall comply with the URI query parameters, request and response data structures, and response codes, as specified in tables 7.2.14.3.5-1 and 7.2.14.3.5-2. # noqa: E501

    :param app_instance_id: The unique identifier of the appInstanceId.
    :type app_instance_id: str

    :rtype: Union[None, Tuple[None, int], Tuple[None, int, Dict[str, str]]
    """
    	#return 'do some magic!'

    """[LÓGICA TFG] Da de baja una aplicación y limpia sus reglas DNS"""

    global MEC_APPS_REGISTRY
    global MEC_DNS_RULES
    
    if app_instance_id in MEC_APPS_REGISTRY:
        # 1. Borramos la app del catálogo general
        del MEC_APPS_REGISTRY[app_instance_id]
        
        # 2. Borramos sus reglas DNS asociadas (si tiene)
        if app_instance_id in MEC_DNS_RULES:
            del MEC_DNS_RULES[app_instance_id]
            
        print(f"[MEP] 🗑️ SERVICIO ELIMINADO: La App {app_instance_id} y sus reglas DNS han sido dadas de baja.")
        return None, 204
    else:
        print(f"[MEP] ⚠️ Intento de borrar App inexistente: {app_instance_id}")
        return {"detail": "App no encontrada", "status": 404}, 404


def registrations_get_appinstanceid(app_instance_id):  # noqa: E501
    """Retrieve information about the MEC application instance registration to the MEC platform.

    The GET method may be used by the MEC application instance to retrieve information about the existing MEC application instance registration to the MEC platform. GET HTTP method shall comply with the URI query parameters, request and response data structures, and response codes, as specified in tables 7.2.14.3.1-1 and 7.2.14.3.1-2. # noqa: E501

    :param app_instance_id: The unique identifier of the appInstanceId.
    :type app_instance_id: str

    :rtype: Union[RegistrationsPOSTRequest, Tuple[RegistrationsPOSTRequest, int], Tuple[RegistrationsPOSTRequest, int, Dict[str, str]]
    """
    #return 'do some magic!'
    
    # === INICIO DE LA LÓGICA DEL TFG ===
    print(f"[MEP] 🔍 Solicitud GET para la App ID: {app_instance_id}")
    
    if app_instance_id in MEC_APPS_REGISTRY:
        # Si la app existe, devolvemos el objeto y un HTTP 200 (OK)
        return MEC_APPS_REGISTRY[app_instance_id], 200
    else:
        # Si no existe, ETSI exige devolver un error 404 (Not Found)
        error_msg = f"La aplicacion con ID {app_instance_id} no esta registrada."
        print(f"[MEP] ❌ {error_msg}")
        return ProblemDetails(detail=error_msg, status=404), 404
    # === FIN DE LA LÓGICA DEL TFG ===


def registrations_post(body):  # noqa: E501
    """Register the MEC application instance to the MEC platform.

    The POST method may be used by the MEC application instance to request its registration to the MEC platform. Upon success, the response contains message content describing the created registration.  # noqa: E501

    :param registrations_post_request: The message content in the request contains the profile of the MEC application instance, calling the MEC platform to register the MEC application instance. .
    :type registrations_post_request: dict | bytes

    :rtype: Union[RegistrationsPOSTRequest, Tuple[RegistrationsPOSTRequest, int], Tuple[RegistrationsPOSTRequest, int, Dict[str, str]]
    """
    #registrations_post_request = body
    #if connexion.request.is_json:
    #    registrations_post_request = RegistrationsPOSTRequest.from_dict(connexion.request.get_json())  # noqa: E501
    #return 'do some magic!'

    # === INICIO DE LA LÓGICA DEL TFG ===
    try:
        # 1. Obtenemos el JSON puro y duro que nos acaba de enviar la App
        raw_data = connexion.request.get_json()
        
        # 2. Extraemos el nombre de la app de forma segura
        app_profile = raw_data.get('appProfile', {})
        app_name = app_profile.get('appName', 'App-TFG')
        
        # 3. Generamos el ID asignado por el Edge (el MEP)
        app_instance_id = f"TFG-APP-{app_name}" 

        # ========================================================
        # 🔥 EL FIX ESTÁ AQUÍ 🔥
        # Inyectamos el ID dentro del JSON antes de guardarlo
        raw_data['appInstanceId'] = app_instance_id
        # ========================================================
        
        # 4. Lo guardamos en nuestra base de datos
        MEC_APPS_REGISTRY[app_instance_id] = raw_data
        print(f"[MEP] ✅ Nueva App registrada: {app_name} con ID: {app_instance_id}")
        
        # 5. Preparamos la respuesta: Devolvemos lo mismo que nos enviaron, 
        # pero le inyectamos el ID oficial que le hemos asignado.
        respuesta = raw_data.copy()
        respuesta['appInstanceId'] = app_instance_id
        
        # ETSI exige devolver un código 201 (Created)
        return respuesta, 201
        
    except Exception as e:
        print(f"[MEP] ❌ Error interno en el registro: {str(e)}")
        # Devolvemos un JSON genérico de error
        return {"detail": str(e), "status": 400}, 400
    # === FIN DE LA LÓGICA DEL TFG ===

def registrations_put_appinstanceid(app_instance_id, body):  # noqa: E501
    """Update the existing registration of that MEC application instance to the MEC platform.

    The PUT method may be used by the MEC application instance to update its registration to the MEC platform. PUT HTTP method shall comply with the URI query parameters, request and response data structures, and response codes, as specified in tables 7.2.14.3.2-1 and 7.2.14.3.2-2. # noqa: E501

    :param app_instance_id: The unique identifier of the appInstanceId.
    :type app_instance_id: str
    :param registrations_post_request: Message content in the request contains the profile of the application, calling the MEC platform to update the existing MEC application instance registration.
    :type registrations_post_request: dict | bytes

    :rtype: Union[object, Tuple[object, int], Tuple[object, int, Dict[str, str]]
    """
    registrations_post_request = body
    if connexion.request.is_json:
        registrations_post_request = RegistrationsPOSTRequest.from_dict(connexion.request.get_json())  # noqa: E501
    return 'do some magic!'


def timing_currenttime_get():  # noqa: E501
    """Retrieve information about the mecCurrentTime resource.

    This method retrieves the information of the platform&#39;s current time which corresponds to the get platform time procedure as described in clause 5.2.10.2. Figure 7.2.6.3.1-1 shows message flow for retrieving current time using GET method. # noqa: E501


    :rtype: Union[TimingCurrenttimeGET200Response, Tuple[TimingCurrenttimeGET200Response, int], Tuple[TimingCurrenttimeGET200Response, int, Dict[str, str]]
    """
    return 'do some magic!'


def timing_timingcaps_get():  # noqa: E501
    """Retrieve information about the mecTimingCaps resource.

    This method retrieves the information of the platform&#39;s timing capabilities which corresponds to the timing capabilities query as described in clause 5.2.10.3. Figure 7.2.5.3.1-1 shows the example message flow for retrieving timing capabilities using GET method. # noqa: E501


    :rtype: Union[TimingTimingcapsGET200Response, Tuple[TimingTimingcapsGET200Response, int], Tuple[TimingTimingcapsGET200Response, int, Dict[str, str]]
    """
    return 'do some magic!'


def registrations_get():  # noqa: E501
    """
    [LÓGICA TFG - IDEA 1] Devuelve la lista completa de aplicaciones registradas
    """
    global MEC_APPS_REGISTRY
    print(f"[MEP] 📡 El Proxy ha solicitado la lista de Apps. Devolviendo {len(MEC_APPS_REGISTRY)} apps.")
    
    return list(MEC_APPS_REGISTRY.values()), 200
