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
        // create an instance of MqttClient and use component via forwarding
        MyMqttClient mqttClient = new MyMqttClient("tcp://localhost:1883", "shusherApp");
        mqttClient.connect();

        // load homepage from fxml file
        FXMLLoader fxmlLoader = new FXMLLoader(MainApplication.class.getResource("homepage-view.fxml"));
        Scene scene = new Scene(fxmlLoader.load(), 600, 400);
        stage.setTitle("Shusher");

        // create css string for to enable adding css file to scenes
        String css = this.getClass().getResource("style.css").toExternalForm();
        // add css to the scene
        scene.getStylesheets().add(css);

        // pass MqttClient instance to homepageController
        HomepageController homepageController = fxmlLoader.getController();
        homepageController.setMqttClient(mqttClient);

        // set the window and display scene
        stage.setScene(scene);
        stage.show();
    }

    public static void main(String[] args) {
        launch();
    }
}