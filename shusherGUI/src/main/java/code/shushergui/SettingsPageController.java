package code.shushergui;

import javafx.event.ActionEvent;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;

public class SettingsPageController {

    private Stage stage;
    private MyMqttClient mqttClient;

    // Setter for MqttClient
    public void setMqttClient(MyMqttClient mqttClient) {
        this.mqttClient = mqttClient;
    }

    public void switchToHomepage(ActionEvent event) throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(MainApplication.class.getResource("homepage-view.fxml"));
        stage = (Stage)((Node)event.getSource()).getScene().getWindow();
        Scene scene = new Scene(fxmlLoader.load(), 600, 400);

        // Add css file to the scene
        scene.getStylesheets().add(getClass().getResource("style.css").toExternalForm());


        // Pass the MqttClient instance to the homepage scene controller
        HomepageController homepageController = fxmlLoader.getController();
        homepageController.setMqttClient(mqttClient);

        // Set the window and display scene
        stage.setScene(scene);
        stage.show();
    }
}
