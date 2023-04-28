package code.shushergui;


import javafx.event.ActionEvent;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.stage.Stage;


public class HomepageController {
    private Stage stage;
    private MyMqttClient mqttClient;

    // setter for MqttClient
    public void setMqttClient(MyMqttClient mqttClient) {
        this.mqttClient = mqttClient;
    }

    public void switchToSettings(ActionEvent event) throws Exception {
        FXMLLoader fxmlLoader = new FXMLLoader(MainApplication.class.getResource("settingsPage-view.fxml"));
        stage = (Stage)((Node)event.getSource()).getScene().getWindow();
        Scene scene = new Scene(fxmlLoader.load(), 600, 400);

        // pass the MqttClient instance to the settings scene controller
        SettingsPageController settingsPageController = fxmlLoader.getController();
        settingsPageController.setMqttClient(mqttClient);

        // set the window and display scene
        stage.setScene(scene);
        stage.show();
    }

}