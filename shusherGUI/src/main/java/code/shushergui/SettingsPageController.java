package code.shushergui;

import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.RadioButton;
import javafx.stage.Stage;
import org.eclipse.paho.client.mqttv3.MqttException;
import java.io.IOException;


public class SettingsPageController {

    private Stage stage;
    private MyMqttClient mqttClient;
    @FXML
    private Button setupButton, homepageButton;
    @FXML
    private RadioButton highThresholdButton, mediumThresholdButton, lowThresholdButton;

    private String currentThreshold; // Hold the value of the current threshold set by thresholdButtons()

    // Setter for MqttClient
    public void setMqttClient(MyMqttClient mqttClient) {
        if (mqttClient != null) {
            this.mqttClient = mqttClient;
            System.out.println("Successfully passed mqtt instance to SettingsPageController");
        } else {
            System.out.println("ERROR: mqttClient object is null");
        }
    }

    @FXML
    private void switchToHomepage() throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(MainApplication.class.getResource("homepage-view.fxml"));
        stage = (Stage)homepageButton.getScene().getWindow();
        Scene scene = new Scene(fxmlLoader.load(), 500, 300);

        // Add css file to the scene
        scene.getStylesheets().add(getClass().getResource("style.css").toExternalForm());


        // Pass the MqttClient instance to the homepage scene controller
        HomepageController homepageController = fxmlLoader.getController();
        homepageController.setMqttClient(mqttClient);

        // Pass current threshold to homepage
        homepageController.setThresholdLabel(currentThreshold);

        // Set the window and display scene
        stage.setScene(scene);
        stage.show();
    }

    // Initialize method is the entry point for initializing the controller and its associated UI elements.
    // 'e' is a variable used to refer to the event object that is passed to the event handler, call thresholdButtons(button)
    @FXML
    private void initialize() {
        lowThresholdButton.setOnAction( e -> thresholdButtons(lowThresholdButton, "Low"));
        mediumThresholdButton.setOnAction( e -> thresholdButtons(mediumThresholdButton, "Medium"));
        highThresholdButton.setOnAction( e -> thresholdButtons(highThresholdButton, "High"));
    }

    // Publish when thresholdButtons are clicked
    @FXML
    private void thresholdButtons(RadioButton button, String threshold) {
        currentThreshold = threshold;
        try {
            switch (button.getId()) {
                case "lowThresholdButton" -> {
                    mqttClient.publish("shusher/test", "Low");
                }
                case "mediumThresholdButton" -> {
                    mqttClient.publish("shusher/test", "Medium");
                }
                case "highThresholdButton" -> {
                    mqttClient.publish("shusher/test", "High");
                }
            }
        } catch (MqttException e) {
            throw new RuntimeException(e);
        }
    }

}
