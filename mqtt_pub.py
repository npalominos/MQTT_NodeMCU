import paho.mqtt.client as mqtt 

broker_address="paradigma.no-ip.org" #aca va la ip o el host del broker

client = mqtt.Client("P1") #crear una nueva instancia o sesion

client.connect(broker_address) #conecta al broker

client.publish("nestor/sensor","OFF") #publicar
