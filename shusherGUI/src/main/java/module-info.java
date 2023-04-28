module code.shushergui {
    requires javafx.controls;
    requires javafx.fxml;
    requires org.eclipse.paho.client.mqttv3;


    opens code.shushergui to javafx.fxml;
    exports code.shushergui;
}