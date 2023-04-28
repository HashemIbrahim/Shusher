package code.shushergui;

import org.eclipse.paho.client.mqttv3.*;        //mqtt library

public class MyMqttClient {

    public final MqttClient mqttClient;
    private static final String TOPIC = "shusher";

    public MyMqttClient(String broker, String clientID) throws MqttException{
        this.mqttClient = new MqttClient(broker, clientID);
    }

    public void connect() throws MqttException {
        MqttConnectOptions options = new MqttConnectOptions();
        options.setCleanSession(true);
        mqttClient.connect(options);
    }

    public void disconnect()throws MqttException {
        mqttClient.disconnect();
    }

    public void sendMessage(String payload) throws MqttException {
        MqttMessage message = new MqttMessage(payload.getBytes());
        mqttClient.publish(TOPIC, message);
    }

    public void subscribe() throws MqttException {
        mqttClient.subscribe(TOPIC, (topic, message) -> {
            System.out.println(new String(message.getPayload()));
        });
    }

}

