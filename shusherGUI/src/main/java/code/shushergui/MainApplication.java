package code.shushergui;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;
import org.eclipse.paho.client.mqttv3.MqttException;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class MainApplication extends Application {

    MyMqttClient mqttClient;

    @Override
    public void start(Stage stage) throws IOException, MqttException {
        // Create a try catch block that creates an instance of MyMqttClient if the instance connects to MQTT, otherwise the MQTT instance is null
        // Enable the application to run even if the application is not connected to mqtt.
        // Read broker and clientId from configuration file.
        // config.txt file below is in the same folder as the executable jar file in release 1.0. If you're not using version 1.0, specify the path to your config file.
        try {
            Path configFilePath = Paths.get("config.txt");
            // Read the contents of the file
            String configContent = Files.readString(configFilePath);
            // Parse the contents to extract the broker and clientId
            String[] configLines = configContent.split("\n");
            String broker = configLines[0].trim();
            String clientId = configLines[1].trim();

            mqttClient = new MyMqttClient(broker, clientId);
            mqttClient.connect();
        } catch (IOException e) {
            // Handle file reading error
            e.printStackTrace();
        } catch (MqttException e) {
            mqttClient = null;
            e.printStackTrace();
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