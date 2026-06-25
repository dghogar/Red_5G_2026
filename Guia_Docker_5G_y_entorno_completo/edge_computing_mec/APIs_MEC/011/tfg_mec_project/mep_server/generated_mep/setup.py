import sys
from setuptools import setup, find_packages

NAME = "openapi_server"
VERSION = "1.0.0"

# To install the library, run the following
#
# python setup.py install
#
# prerequisite: setuptools
# http://pypi.python.org/pypi/setuptools

REQUIRES = [
    "connexion>=2.0.2",
    "swagger-ui-bundle>=0.0.2",
    "python_dateutil>=2.6.0"
]

setup(
    name=NAME,
    version=VERSION,
    description="ETSI GS MEC 011 Edge Platform Application Enablement",
    author_email="",
    url="",
    keywords=["OpenAPI", "ETSI GS MEC 011 Edge Platform Application Enablement"],
    install_requires=REQUIRES,
    packages=find_packages(),
    package_data={'': ['openapi/openapi.yaml']},
    include_package_data=True,
    entry_points={
        'console_scripts': ['openapi_server=openapi_server.__main__:main']},
    long_description="""\
    The present document focuses on the functionalities enabled via the Mp1 reference point between MEC applications and MEC platform, which allows these applications to interact with the MEC system. Service management functionality includes registration/deregistration, discovery and event notifications. Application support functionality includes application start-up, registration and termination, traffic rules, DNS and time of day. It describes the information flows, required information, and specifies the necessary operations, data models and API definitions. 
    """
)

