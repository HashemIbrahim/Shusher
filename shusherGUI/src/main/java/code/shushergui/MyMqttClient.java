package code.shushergui;

import org.eclipse.paho.client.mqttv3.*;        //mqtt library

public class MyMqttClient {

    public final MqttClient mqttClient;

    public MyMqttClient(String broker, String clientID) throws MqttException{
        this.mqttClient = new MqttClient(broker, clientID);
    }

    public void connect() throws MqttException {
        MqttConnectOptions options = new MqttConnectOptions();
        // Broker should discard any previous session state associated with this client and start a new one.
        options.setCleanSession(true);
        mqttClient.connect(options);
    }

    // Publish function, takes topic and message as argument. Publishes a payload that is converted to a byte array
    public void publish(String topic, String payload) throws MqttException {
        MqttMessage message = new MqttMessage(payload.getBytes());
        mqttClient.publish(topic, message);
    }

    // Subscribe function, takes topic as argument
    public void subscribe(String topic) throws MqttException {
        mqttClient.subscribe(topic);
    }

    // Callback function, used to perform actions whenever a new message arrives.
    public void setCallback(MqttCallback callback) {
        mqttClient.setCallback(callback);
    }

}

