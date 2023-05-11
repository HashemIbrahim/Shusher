package code.shushergui;

import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.HBox;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;
import org.eclipse.paho.client.mqttv3.*;

public class HomepageController {
    private Stage stage;
    private MyMqttClient mqttClient;

    // Variables used in SceneBuilder need to be annotated with @FXML
    // Assign fx:id to lights
    @FXML
    private StackPane light1, light2, light3, light4, light5, light6, light7, light8, light9, light10;
    @FXML
    private Button settingsButton, exitButton;

    // Setter for MqttClient
    public void setMqttClient(MyMqttClient mqttClient) {
        if (mqttClient != null) {
            this.mqttClient = mqttClient;
            System.out.println("Successfully passed mqtt instance to homepageController");
            // Run 'runMqtt' function
            runMqtt();
        } else {
            System.out.println("ERROR: mqttClient object is null");
        }

    }

    @FXML
    private void switchToSettings() throws Exception {
        FXMLLoader fxmlLoader = new FXMLLoader(MainApplication.class.getResource("settingsPage-view.fxml"));
        stage = (Stage)settingsButton.getScene().getWindow();
        Scene scene = new Scene(fxmlLoader.load(), 500, 300);

        // Add css file to the scene
        scene.getStylesheets().add(getClass().getResource("style.css").toExternalForm());

        // Pass the MqttClient instance to the settings scene controller
        SettingsPageController settingsPageController = fxmlLoader.getController();
        settingsPageController.setMqttClient(mqttClient);
        runMqtt();

        // Set the window and display scene
        stage.setScene(scene);
        stage.show();
    }

    private void runMqtt() {
        if (mqttClient != null) {
            try {
                mqttClient.subscribe("shusher/loudness");
                // Create a new thread using lambda expression. Handle incoming messages without blocking the main thread.
                // The thread runs constantly in the background while the program is running.
                new Thread(() -> {
                    // Set the callback function for arriving messages.
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
                                    // Call function to reset the background color of all lights
                                    resetLights();
                                    // Call function to set color of lights depending on the payload
                                    setLightsOnPayload(payload);

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

    // ChatGPT helped me create this loop. I believe it is a good solution because:
    // - It uses get.Parent() to get the HBox object which enables the use of the lookup() method.
    // - It efficiently uses the lookup() method to find nodes.
    // - It is shorter than having 10 lines of setting the background color of all lights.
    private void resetLights() {
        HBox ledStrip = (HBox) light1.getParent();                  // Store HBox object in ledStrip variable
        for (int i = 1; i <= 10; i++) {                             // Loop through lights
            String lightId = "light" + i;
            // Use lookup() method to find the node with the given lightId. The '#' is used to identify the elements id in CSS.
            // When passing a string that start with '#' to lookup(), it will search for a node with the given id in the scene.
            Node light = ledStrip.lookup("#" + lightId);
            light.setStyle("-fx-background-color: #202020");
        }


    }
    // Set the background color of the lights that should light up on a given payload.
    private void setLightsOnPayload(int payload) {
        HBox ledStrip = (HBox) light1.getParent();                  // Store HBox object in ledStrip variable
        for (int i = 1; i <= payload; i++){                         // Loop through lights, payload represents the number of lights that should be turned on
            String lightId = "light" + i;
            Node light = ledStrip.lookup("#" + lightId);
            if (i <= 3) {                                           // Lights 1-3 are set to green
                light.setStyle("-fx-background-color: #AAFF00");
            } else if (i <= 7) {                                    // Lights 4-7 are set to yellow
                light.setStyle("-fx-background-color: #FFEA00");
            } else {                                                // Lights 8-10 are set to red
                light.setStyle("-fx-background-color: #EE4B2B");
            }
        }
    }

    @FXML
    public void exitApp() {                                 // Exit button function
        stage = (Stage)exitButton.getScene().getWindow();   // Get stage
        stage.close();                                      // Close stage
    }



}