module code.shushergui {
    requires javafx.controls;
    requires javafx.fxml;


    opens code.shushergui to javafx.fxml;
    exports code.shushergui;
}