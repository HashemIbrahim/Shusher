package code.shushergui;

import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.HBox;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;
import org.eclipse.paho.client.mqttv3.*;

public class HomepageController {
    private Stage stage;
    private MyMqttClient mqttClient;
    private Counter counter;
    private String [] lightTheme = {"#AAFF00", "#FFEA00", "#EE4B2B"};   // Default light theme colors

    //  Annotate variables used in SceneBuilder with @FXML
    @FXML
    private StackPane light1, light2, light3, light4, light5, light6, light7, light8, light9, light10;
    @FXML
    private Button settingsButton, exitButton;
    @FXML
    private Label thresholdLabel, counterLabel, distanceLabel;

    // **Group setter functions at top of class**

    // Set light theme
    public void setLightTheme(String[] lightTheme) {
        this.lightTheme = lightTheme;
    }

    // Set MqttClient
    public void setMqttClient(MyMqttClient mqttClient) {
        if (mqttClient != null) {
            this.mqttClient = mqttClient;
            System.out.println("Successfully passed mqtt instance to homepageController");
            // Run 'runMqtt' function
            runMqtt();
        } else {
            System.out.println("ERROR: mqttClient object is null in homepage");
        }
    }

    // Set threshold counter
    public void setCounter(Counter counter) {
        this.counter = counter;
    }

    // Set threshold label
    public void setThresholdLabel(String newThreshold) {
        thresholdLabel.setText(newThreshold);
    }

    // Add function to switch to settings scene. Pass variables to settings controller
    @FXML
    private void switchToSettings() throws Exception {
        FXMLLoader fxmlLoader = new FXMLLoader(MainApplication.class.getResource("settingsPage-view.fxml"));
        stage = (Stage)settingsButton.getScene().getWindow();
        Scene scene = new Scene(fxmlLoader.load(), 500, 300);

        // Add css file to the scene
        scene.getStylesheets().add(getClass().getResource("style.css").toExternalForm());

        // Pass the MqttClient instance to the settingsPage controller
        SettingsPageController settingsPageController = fxmlLoader.getController();
        settingsPageController.setMqttClient(mqttClient);

        // Pass counter to SettingsPageController
        counter.getInstance();
        settingsPageController.setCounter(counter);

        // Pass threshold label text to SettingsPageController and select threshold button
        settingsPageController.setThreshold(thresholdLabel.getText());

        // Pass the current light theme to SettingsPageController
        settingsPageController.setLightTheme(lightTheme);

        // Set the window and display scene
        stage.setScene(scene);
        stage.show();
    }


    // Add MQTT function, subscribe to topics and perform actions on incoming payload
    private void runMqtt() {
        if (mqttClient != null) {
            try {
                mqttClient.subscribe("shusher/loudness");
                mqttClient.subscribe("shusher/distance");
                    // Set the callback function for arriving messages.
                    mqttClient.setCallback(new MqttCallback() {
                        @Override
                        public void connectionLost(Throwable throwable) {
                            System.out.println("Connection lost: " + throwable.getMessage());
                        }
                        @Override
                        public void messageArrived(String topic, MqttMessage mqttMessage) throws Exception {
                            Platform.runLater(() -> {       // updates the UI from a background thread
                                if (topic.equals("shusher/loudness")) {
                                    int payload = Integer.parseInt(new String(mqttMessage.getPayload()));       // Parse incoming payload into integer
                                    // Call function to reset the background color of all lights
                                    resetLights();
                                    // Call function to set color of lights depending on the payload
                                    setLightsOnPayload(payload);
                                    // Call function to update the counter every time the payload is 10
                                    updateCounter(payload);
                                }
                                if (topic.equals("shusher/distance")) {
                                    String payload = new String(mqttMessage.getPayload());                      // Parse incoming payload into String
                                    // Call function to update the distance label on incoming payload
                                    displayDistance(payload);
                                }
                            });
                        }
                        @Override
                        public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {
                        }
                    });
            } catch (MqttException e) {
                throw new RuntimeException(e);
            }
        } else {
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
        HBox ledStrip = (HBox) light1.getParent();                          // Store HBox object in ledStrip variable
        for (int i = 1; i <= payload; i++){                                 // Loop through lights, payload represents the number of lights that should be turned on
            String lightId = "light" + i;
            Node light = ledStrip.lookup("#" + lightId);
            if (i <= 3) {
                light.setStyle("-fx-background-color:" + lightTheme[0]);    // Light 1-3
            } else if (i <= 7) {
                light.setStyle("-fx-background-color:" + lightTheme[1]);    // Light 4-7
            } else {
                light.setStyle("-fx-background-color:" + lightTheme[2]);    // Light 8-10
            }
        }
    }

    // Set text on distance label. Display the distance from the ultrasonic ranger in cm
    private void displayDistance(String payload) {
        distanceLabel.setText(payload + "cm");
    }

    // Increment the threshold counter whenever the payload from the "shusher/loudness" topic is 10
    private void updateCounter(int payload) {
        if (payload == 10) {
            counter.increment();
        }
        // Update the counter label with the new value.
        counterLabel.setText(String.valueOf(counter.getCounter()));
    }

    // Exit button function
    @FXML
    private void exitApp() {
        stage = (Stage)exitButton.getScene().getWindow();   // Get stage
        stage.close();                                      // Close stage
        System.out.println("Application closed");
    }

}