#!/bin/bash 

# Script para consultar el APN definido en MongoDB para una SIM (IMSI) en Open5GS 

  

# Requiere: mongo shell instalado 

# Uso: ./ver_apn.sh 001010123456789 

IMSI="$1"

if [[ -z "$IMSI" ]]; then
  echo "Uso: $0 <IMSI>"
  exit 1
fi

mongosh --quiet --eval "
use open5gs;
var sub = db.subscribers.findOne({ imsi: \"$IMSI\" });
if (!sub) {
  print('IMSI no encontrado');
} else if (sub.subscribed_ue && sub.subscribed_ue.apn) {
  print('APNs:');
  sub.subscribed_ue.apn.forEach(function(apnEntry) {
    print(' - ' + apnEntry.apn);
  });
} else {
  print('No hay APNs definidos para este IMSI');
}
"
