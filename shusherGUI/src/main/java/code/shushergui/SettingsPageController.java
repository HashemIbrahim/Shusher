package code.shushergui;

import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.RadioButton;
import javafx.scene.control.ToggleGroup;
import javafx.stage.Stage;
import org.eclipse.paho.client.mqttv3.MqttException;
import java.io.IOException;
import java.util.Arrays;

public class SettingsPageController {

    private MyMqttClient mqttClient;
    private String currentThreshold; // Store the value of the current threshold set by thresholdButtons()
    private String[] lightTheme;     // Store the current light theme
    private String counterValue;     // Store counter value

    @FXML
    private Button setupButton, homepageButton;
    @FXML
    private RadioButton highThresholdButton, mediumThresholdButton, lowThresholdButton, defaultTheme, darkTheme, partyTheme;
    @FXML
    private ToggleGroup thresholdGroup, lightGroup;

    // **Group setter functions at top of class**

    // Set MqttClient
    public void setMqttClient(MyMqttClient mqttClient) {
        if (mqttClient != null) {
            this.mqttClient = mqttClient;
        }
    }

    // Set counter value
    public void setCounterValue(String counterValue) {
        this.counterValue = counterValue;
    }

    // Set threshold text and threshold button
    public void setThreshold(String currentThreshold) {
        this.currentThreshold = currentThreshold;
        switch (currentThreshold) {
            case "Low" -> lowThresholdButton.setSelected(true);
            case "Medium" -> mediumThresholdButton.setSelected(true);
            case "High" -> highThresholdButton.setSelected(true);
        }
    }

    // Set light theme colors and buttons
    public void setLightTheme(String[] lightTheme) {
        this.lightTheme = lightTheme;
        switch (Arrays.toString(lightTheme)) {
            case "[#AAFF00, #FFEA00, #EE4B2B]" -> defaultTheme.setSelected(true);
            case "[#0047AB, #880808, #800080]" -> darkTheme.setSelected(true);
            case "[#00FFFF, #DA70D6, #FFFF00]" -> partyTheme.setSelected(true);
        }
    }

    // Add function to switch to homepage scene. Pass variables to homepage controller
    @FXML
    private void switchToHomepage() throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(MainApplication.class.getResource("homepage-view.fxml"));
        Stage stage = (Stage)homepageButton.getScene().getWindow();
        Scene scene = new Scene(fxmlLoader.load(), 500, 300);

        // Add css file to the scene
        scene.getStylesheets().add(getClass().getResource("style.css").toExternalForm());

        // Pass variables to HomePageController by calling setters through forwarding
        HomepageController homepageController = fxmlLoader.getController();
        homepageController.setMqttClient(mqttClient);                           // Pass the MqttClient instance to the homepage scene controller
        homepageController.setThresholdLabel(currentThreshold);                 // Pass current threshold to homepage
        homepageController.setLightTheme(lightTheme);                           // Pass current light theme to homepage
        homepageController.setCounter(counterValue);                            // Update the threshold counter in homepage

        // Set the window and display scene
        stage.setScene(scene);
        stage.show();
    }

    // Initialize method is the entry point for initializing the controller and its associated UI elements.
    // 'e' is a variable used to refer to the event object that is passed to the event handler, call thresholdButtons(button)
    @FXML
    private void initialize() {
        // Threshold buttons
        lowThresholdButton.setOnAction( e -> thresholdButtons(lowThresholdButton, "Low"));
        mediumThresholdButton.setOnAction( e -> thresholdButtons(mediumThresholdButton, "Medium"));
        highThresholdButton.setOnAction( e -> thresholdButtons(highThresholdButton, "High"));

        // Theme buttons
        defaultTheme.setOnAction( e -> changeLightTheme(defaultTheme, lightTheme = new String[] {"#AAFF00", "#FFEA00", "#EE4B2B"}));
        darkTheme.setOnAction( e -> changeLightTheme(darkTheme, lightTheme = new String[] {"#0047AB", "#880808", "#800080"}));
        partyTheme.setOnAction( e -> changeLightTheme(partyTheme, lightTheme = new String[] {"#00FFFF", "#DA70D6", "#FFFF00"}));
    }

    // Publish payload to "shusher/threshold" topic when thresholdButtons are clicked
    @FXML
    private void thresholdButtons(RadioButton button, String threshold) {
        currentThreshold = threshold;

        final String THRESHOLD_TOPIC = "shusher/threshold";

        try {
            switch (button.getId()) {
                case "lowThresholdButton" -> mqttClient.publish(THRESHOLD_TOPIC, "Low");
                case "mediumThresholdButton" -> mqttClient.publish(THRESHOLD_TOPIC, "Medium");
                case "highThresholdButton" -> mqttClient.publish(THRESHOLD_TOPIC, "High");
            }
        } catch (MqttException e) {
            throw new RuntimeException(e);
        }
    }

    // Publish payload to LED strip sections when light theme buttons are clicked.
    // Payload color codes differ from gui color codes because Grove RGB LED strip can not display complex colors
    private void changeLightTheme(RadioButton button, String[] updateLightTheme) {
        lightTheme = updateLightTheme;

        final String LIGHT_SECTION_1_TOPIC = "shusher/lights/section1";
        final String LIGHT_SECTION_2_TOPIC = "shusher/lights/section2";
        final String LIGHT_SECTION_3_TOPIC = "shusher/lights/section3";

        try {
            switch (button.getId()) {
                case "defaultTheme" -> {
                    mqttClient.publish(LIGHT_SECTION_1_TOPIC, "FF0000");        // green
                    mqttClient.publish(LIGHT_SECTION_2_TOPIC, "FFFF00");        // yellow
                    mqttClient.publish(LIGHT_SECTION_3_TOPIC, "00FF00");        // red
                }
                case "darkTheme" -> {
                    mqttClient.publish(LIGHT_SECTION_1_TOPIC, "00008B");        // blue
                    mqttClient.publish(LIGHT_SECTION_2_TOPIC, "008B00");        // dark red
                    mqttClient.publish(LIGHT_SECTION_3_TOPIC, "00FFFF");        // purple
                }
                case "partyTheme" -> {
                    mqttClient.publish(LIGHT_SECTION_1_TOPIC, "FF10F0");        // bright blue
                    mqttClient.publish(LIGHT_SECTION_2_TOPIC, "008080");        // bright purple
                    mqttClient.publish(LIGHT_SECTION_3_TOPIC, "FFFF00");        // yellow
                }
            }
        } catch (MqttException e) {
            throw new RuntimeException(e);
        }
    }

}
