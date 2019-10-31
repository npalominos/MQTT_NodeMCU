import paho.mqtt.client as mqtt 
broker_address="paradigma.no-ip.org" 
print("creating new instance")
client = mqtt.Client("P1") 
print("connecting to broker")
client.connect(broker_address) 
print("Subscribing to topic","house/bulbs/bulb1")
client.subscribe("house/bulbs/bulb1")
