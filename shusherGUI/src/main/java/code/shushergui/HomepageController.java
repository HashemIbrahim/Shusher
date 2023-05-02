package code.shushergui;

import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;
import org.eclipse.paho.client.mqttv3.*;

public class HomepageController {
    private Stage stage;
    private MyMqttClient mqttClient;

    // variables used in SceneBuilder need to be annotated with @FXML
    // assign fx:id to lights
    @FXML
    private StackPane light1, light2, light3, light4, light5, light6, light7, light8, light9, light10;

    // setter for MqttClient
    public void setMqttClient(MyMqttClient mqttClient) {
        if (mqttClient != null) {
            this.mqttClient = mqttClient;
            System.out.println("Successfully passed mqtt instance to homepageController");
            // run 'runMqtt' function
            runMqtt();
        } else {
            System.out.println("ERROR: mqttClient object is null");
        }

    }


    public void switchToSettings(ActionEvent event) throws Exception {
        FXMLLoader fxmlLoader = new FXMLLoader(MainApplication.class.getResource("settingsPage-view.fxml"));
        stage = (Stage)((Node)event.getSource()).getScene().getWindow();
        Scene scene = new Scene(fxmlLoader.load(), 600, 400);

        // pass the MqttClient instance to the settings scene controller
        SettingsPageController settingsPageController = fxmlLoader.getController();
        settingsPageController.setMqttClient(mqttClient);

        // set the window and display scene
        stage.setScene(scene);
        stage.show();
    }

    private void runMqtt() {
        if (mqttClient != null) {
            try {
                mqttClient.subscribe("shusher/loudness");
                new Thread(() -> {
                    // set the callback function for arriving messages
                    mqttClient.setCallback(new MqttCallback() {
                        @Override
                        public void connectionLost(Throwable throwable) {
                            System.out.println("Connection lost: " + throwable.getMessage());
                        }
                        @Override
                        public void messageArrived(String topic, MqttMessage mqttMessage) throws Exception {
                            // parse incoming payload into integer
                            int payload = Integer.parseInt(new String(mqttMessage.getPayload()));
                            if (topic.equals("shusher/loudness")) {
                                Platform.runLater(() -> {
                                    if (payload == 1) {
                                        light1.setStyle("-fx-background-color: #AAFF00");
                                    }
                                    if (payload == 2) {
                                        light2.setStyle("-fx-background-color: #AAFF00");
                                    }
                                    if (payload == 3) {
                                        light3.setStyle("-fx-background-color: #AAFF00");
                                    }
                                    if (payload == 4) {
                                        light4.setStyle("-fx-background-color: #FFEA00");
                                    }
                                    if (payload == 5) {
                                        light5.setStyle("-fx-background-color: #FFEA00");
                                    }
                                    if (payload == 6) {
                                        light6.setStyle("-fx-background-color: #FFEA00");
                                    }
                                    if (payload == 7) {
                                        light7.setStyle("-fx-background-color: #FFEA00");
                                    }
                                    if (payload == 8) {
                                        light8.setStyle("-fx-background-color: #EE4B2B");
                                    }
                                    if (payload == 9) {
                                        light9.setStyle("-fx-background-color: #EE4B2B");
                                    }
                                    if (payload == 10) {
                                        light10.setStyle("-fx-background-color: #EE4B2B");
                                    }
                                });
                            }
                        }
                        @Override
                        public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {

                        }
                    });
                }).start();

            } catch (MqttException e) {
                throw new RuntimeException(e);
            }
        }
        else {
            System.out.println("ERROR: mqttClient object is null");
        }
    }

}