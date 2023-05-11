package code.shushergui;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;
import org.eclipse.paho.client.mqttv3.MqttException;

import java.io.IOException;

public class MainApplication extends Application {

    @Override
    public void start(Stage stage) throws IOException, MqttException {
        // Create an instance of MqttClient and use component via forwarding
        MyMqttClient mqttClient = new MyMqttClient("tcp://localhost:1883", "shusherApp");
        mqttClient.connect();

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