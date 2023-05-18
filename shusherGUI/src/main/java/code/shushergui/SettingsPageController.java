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
import java.util.Objects;


public class SettingsPageController {

    private Stage stage;
    private MyMqttClient mqttClient;
    private Counter counter;
    private String currentThreshold; // Store the value of the current threshold set by thresholdButtons()
    private String[] lightTheme;    // Store the current light theme
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
            System.out.println("Successfully passed mqtt instance to SettingsPageController");
        } else {
            System.out.println("ERROR: mqttClient object is null in settings page");
        }
    }

    // Set threshold text and threshold button
    public void setThreshold(String currentThreshold) {
        this.currentThreshold = currentThreshold;
        if (currentThreshold.equals("Low")) {
            lowThresholdButton.setSelected(true);
        } else if (currentThreshold.equals("Medium")) {
            mediumThresholdButton.setSelected(true);
        } else if (currentThreshold.equals("High")) {
            highThresholdButton.setSelected(true);
        }
    }

    // Set light theme colors and buttons
    public void setLightTheme(String[] lightTheme) {
        this.lightTheme = lightTheme;
        if (Arrays.equals(lightTheme, new String[]{"#AAFF00", "#FFEA00", "#EE4B2B"})) {
            defaultTheme.setSelected(true);
        } else if (Arrays.equals(lightTheme, new String[]{"#0047AB", "#880808", "#800080"})) {
            darkTheme.setSelected(true);
        } else if (Arrays.equals(lightTheme, new String[]{"#00FFFF", "#DA70D6", "#FF69B4"})) {
            partyTheme.setSelected(true);
        }
    }

    // Set threshold counter
    public void setCounter(Counter counter) {
        this.counter = counter;
    }

    // Add function to switch to homepage scene. Pass variables to homepage controller
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

        // Create an instance of the counter and pass HomepageController
        counter.getInstance();
        homepageController.setCounter(counter);

        // Pass current threshold to homepage
        homepageController.setThresholdLabel(currentThreshold);

        // Pass current light theme to homepage
        homepageController.setLightTheme(lightTheme);

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
        try {
            switch (button.getId()) {
                case "lowThresholdButton" -> mqttClient.publish("shusher/threshold", "Low");
                case "mediumThresholdButton" -> mqttClient.publish("shusher/threshold", "Medium");
                case "highThresholdButton" -> mqttClient.publish("shusher/threshold", "High");
            }
        } catch (MqttException e) {
            throw new RuntimeException(e);
        }
    }

    // Publish payload to LED strip sections when light theme buttons are clicked.
    // Payload color codes differ from gui color codes because Grove RGB LED strip can not display complex colors
    private void changeLightTheme(RadioButton button, String[] updateLightTheme) {
        lightTheme = updateLightTheme;
        try {
            switch (button.getId()) {
                case "defaultTheme" -> {
                    mqttClient.publish("shusher/lights/section1", "FF0000");        // green
                    mqttClient.publish("shusher/lights/section2", "FFFF00");        // yellow
                    mqttClient.publish("shusher/lights/section3", "00FF00");        // red
                    mqttClient.publish("shusher/lights/theme", "defaultTheme");
                }
                case "darkTheme" -> {
                    mqttClient.publish("shusher/lights/section1", "00008B");        // blue
                    mqttClient.publish("shusher/lights/section2", "008B00");        // dark red
                    mqttClient.publish("shusher/lights/section3", "00FFFF");        // purple
                    mqttClient.publish("shusher/lights/theme", "darkTheme");
                }
                case "partyTheme" -> {
                    mqttClient.publish("shusher/lights/section1", "FF10F0");        // bright blue
                    mqttClient.publish("shusher/lights/section2", "008080");        // bright purple
                    mqttClient.publish("shusher/lights/section3", "FFFF00");        // yellow
                    mqttClient.publish("shusher/lights/theme", "partyTheme");
                }
            }
        } catch (MqttException e) {
            throw new RuntimeException(e);
        }
    }

}
