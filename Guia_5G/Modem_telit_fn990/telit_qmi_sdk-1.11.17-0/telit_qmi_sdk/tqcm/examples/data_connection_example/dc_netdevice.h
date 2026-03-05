 #ifndef DC_NETDEVICE_H_
#define DC_NETDEVICE_H_

uint8_t netdevice_get_ipv4_prefix_from_mask(uint32_t mask);
int netdevice_configure_address(char *netdevice, char *address);
int netdevice_configure_route(char *netdevice, char *gateway);
int netdevice_configure_dns(char *netdevice, char *dns);
int netdevice_set_mtu(char *netdevice, uint32_t mtu);
int netdevice_set_link(char *netdevice, uint8_t up);
int netdevice_dispose(char *netdevice);
int netdevice_set_ra(char *netdevice, int enable);

#endif  /* DC_QMI_WWAN_H_ */
