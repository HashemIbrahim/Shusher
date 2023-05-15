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
        // Enables the application to run even if the application is not connected to mqtt.
        try {
            mqttClient = new MyMqttClient("tcp://localhost:1883", "shusherApp");
            mqttClient.connect();
            System.out.println("Connected to MQTT broker");
        } catch (MqttException e) {
            mqttClient = null;
            System.out.println("Failed to connect to MQTT broker");
        }

        // Load homepage from fxml file
        FXMLLoader fxmlLoader = new FXMLLoader(MainApplication.class.getResource("homepage-view.fxml"));
        Scene scene = new Scene(fxmlLoader.load(), 500, 300);
        stage.setTitle("Shusher");
        stage.setResizable(false);

        // Add css file to the scene
        scene.getStylesheets().add(getClass().getResource("style.css").toExternalForm());

        // Pass MqttClient instance to HomepageController
        HomepageController homepageController = fxmlLoader.getController();
        homepageController.setMqttClient(mqttClient);

        // Create an instance of the counter and pass HomepageController
        Counter counter = Counter.getInstance();
        homepageController.setCounter(counter);

        // Set the window and display scene
        stage.setScene(scene);
        stage.show();

    }

    public static void main(String[] args) {
        launch();
    }
}