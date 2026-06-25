FROM python:3.9-slim
WORKDIR /app
RUN pip install flask requests
COPY mec_discovery_proxy.py /app/
EXPOSE 8085
CMD ["python", "-u", "mec_discovery_proxy.py"] 
# El -u fuerza a Python a imprimir los logs en consola en tiempo real
