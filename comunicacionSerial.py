import serial
import requests
import psycopg2


try:
    # Conexión a base de datos Django remota 
    # ~ conn = psycopg2.connect(
        # ~ database="growtechprueba",
        # ~ user="admin",
        # ~ password="adminadmin",
        # ~ host="db-growtech.cj1oon0keivs.us-east-1.rds.amazonaws.com"
        # ~ )
    # ~ cursor = conn.cursor()
    arduino = serial.Serial('/dev/ttyUSB0', 9600)  # Asegúrate de ajustar el puerto correcto
    # ~ user_id=requests.get('http://backend-growtech.servehttp.com/login')
    token=""
    
    response= requests.post('http://backend-growtech.servehttp.com/login/', data={"username": "maria", "password":"1234"})
    token = response.json()['token']
    user_id= response.json()['user_id']
    response2 = requests.get('http://backend-growtech.servehttp.com/plants/', data={"usuario_id":user_id})
    print(response2)
    while True:
        line = arduino.readline().decode().strip()
        data = line.split(':')

        if data[0] == 'TEMP':
            temperature = float(data[1])
            # Guardar temperatura en la base de datos Django
            response= requests.post('http://backend-growtech.servehttp.com/temperatura/',
             data={'valor': temperature},
             headers={"Authorization": "Bearer "+ token})
            print(response.content)

        elif data[0] == 'HUM':
            humidity = float(data[1])
            # Guardar humedad en la base de datos Django
            requests.post('http://backend-growtech.servehttp.com/humedad/ambiente/', data={'valor': humidity,
             headers={"Authorization": "Bearer "+ token})
            print(response.content)

        elif data[0] == 'HUM_SUELO ':
            soil_moisture = float(data[1])
            # Guardar humedad del suelo en la base de datos Django
            requests.post('http://backend-growtech.servehttp.com/humedad/suelo/', data={'valor': soil_moisture,
             headers={"Authorization": "Bearer "+ token})
            print(response.content)

        elif data[0] == 'UV_INTENSITY':
            uv_intensity = float(data[1])
            # Guardar intensidad de luz UV en la base de datos Django
            requests.post('http://backend-growtech.servehttp.com/intensidad/', data={'valor': uv_intensity,
             headers={"Authorization": "Bearer "+ token})
            print(response.content)

except serial.SerialException as e:
    print("Error al abrir el puerto serial:", str(e))
    if 'arduino' in locals():
        arduino.close()
