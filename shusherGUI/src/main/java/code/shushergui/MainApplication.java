package code.shushergui;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;
import org.eclipse.paho.client.mqttv3.MqttException;
import java.io.IOException;

public class MainApplication extends Application {

    MyMqttClient mqttClient;

    @Override
    public void start(Stage stage) throws IOException, MqttException {
        // Create a try catch block that creates an instance of MyMqttClient if the instance connects to MQTT, otherwise the MQTT instance is null
        // Enable the application to run even if the application is not connected to mqtt.
        try {
            mqttClient = new MyMqttClient("tcp://localhost:1883", "shusherApp");
            mqttClient.connect();
        } catch (MqttException e) {
            mqttClient = null;
        }

        // Load homepage from fxml file
        FXMLLoader fxmlLoader = new FXMLLoader(MainApplication.class.getResource("homepage-view.fxml"));
        Scene scene = new Scene(fxmlLoader.load(), 500, 300);
        stage.setTitle("Shusher");
        stage.setResizable(false);

        // Add css file to the scene
        scene.getStylesheets().add(getClass().getResource("style.css").toExternalForm());

        HomepageController homepageController = fxmlLoader.getController();         // Load HomePageController
        homepageController.setMqttClient(mqttClient);                               // Pass MqttClient instance to HomepageController

        // Set the window and display scene
        stage.setScene(scene);
        stage.show();
    }

    public static void main(String[] args) {
        launch();
    }
}