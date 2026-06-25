from scapy.all import sniff, IP, ARP

def mostrar_paquete(paquete):
    # Si es un paquete IP normal
    if IP in paquete:
        origen = paquete[IP].src
        destino = paquete[IP].dst
        protocolo = paquete[IP].proto
        print(f"[IP]  Origen: {origen:15} -> Destino: {destino:15} | Proto: {protocolo}")
        
    # Si es un paquete ARP (¿Quién tiene esta IP?)
    elif ARP in paquete:
        origen = paquete[ARP].psrc
        print(f"[ARP] Alguien preguntando o anunciándose desde: {origen}")
        
    # Cualquier otra cosa (IPv6, protocolos raros)
    else:
        print(f"[OTRO] {paquete.summary()}")

if __name__ == '__main__':
    print("="*60)
    print("🔎 ESCÁNER DE RED L2/L3 EN MODO PROMISCUO")
    print("Escuchando absolutamente TODO el tráfico entrante...")
    print("="*60)
    # Escuchamos sin filtros en todas las interfaces
    sniff(prn=mostrar_paquete, store=0)
