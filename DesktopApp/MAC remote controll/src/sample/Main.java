package sample;

import javafx.application.Application;
import javafx.event.EventHandler;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.input.KeyEvent;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;

import javax.bluetooth.*;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import javax.security.auth.kerberos.KerberosTicket;
import javax.swing.*;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;




public class Main extends Application {

    //Declare BT variables
    String url = "btspp://201510201409:1";

    //declare gui variables
    Boolean moveCircle = false;
    Circle outerCircle = new Circle();
    Circle innerCircle = new Circle();
    Button backTrackButton = new Button("Backtrack");

    @Override
    public void start(Stage primaryStage) throws Exception{


        StreamConnection con = (StreamConnection) Connector.open(url);

        OutputStream os = con.openOutputStream();
        InputStream is = con.openInputStream();


        Group root = new Group();
        Scene scene = new Scene(root, 500, 600);
        primaryStage.setScene(scene);
        primaryStage.setTitle("MAC remote controller");
        Group g = new Group();
        innerCircle.setOnMousePressed(new EventHandler<MouseEvent>() {
            @Override
            public void handle(MouseEvent mouseEvent) {
                moveCircle = true;
            }
        });
        innerCircle.setOnMouseDragged(new EventHandler<MouseEvent>() {
            @Override
            public void handle(MouseEvent mouseEvent) {
                if(mouseEvent.getX() < (outerCircle.getCenterX() + innerCircle.getRadius() * 2) && mouseEvent.getX() > (outerCircle.getCenterX() - innerCircle.getRadius() * 2) &&
                        mouseEvent.getY() < (outerCircle.getCenterY() + innerCircle.getRadius() * 2) && mouseEvent.getY() > (outerCircle.getCenterY() - innerCircle.getRadius() * 2)){
                    innerCircle.setCenterX(mouseEvent.getX());
                    innerCircle.setCenterY(mouseEvent.getY());
                }
            }
        });
        innerCircle.setOnMouseReleased(new EventHandler<MouseEvent>() {
            @Override
            public void handle(MouseEvent mouseEvent) {
                moveCircle = false;
                innerCircle.setCenterX(250);
                innerCircle.setCenterY(300);
            }
        });

        outerCircle.setCenterX(250);
        outerCircle.setCenterY(300);
        outerCircle.setRadius(120);
        innerCircle.setCenterX(250);
        innerCircle.setCenterY(300);
        innerCircle.setRadius(40);
        outerCircle.setFill(Color.GREEN);
        innerCircle.setFill(Color.BLACK);

        backTrackButton.setLayoutX(200);
        backTrackButton.setLayoutY(100);
        backTrackButton.setOnMouseClicked(new EventHandler<MouseEvent>() {
            @Override
            public void handle(MouseEvent event) {
                try {
                    os.write("h".getBytes());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });

        g.getChildren().add(outerCircle);
        g.getChildren().add(innerCircle);
        g.getChildren().add(backTrackButton);

        scene.setOnKeyPressed(new EventHandler<KeyEvent>() {
            @Override
            public void handle(KeyEvent keyEvent) {
                switch (keyEvent.getCode()) {
                    case W:
                        innerCircle.setCenterY(outerCircle.getCenterY() - innerCircle.getRadius() * 2);
                        break;
                    case S:
                        innerCircle.setCenterY(outerCircle.getCenterY() + innerCircle.getRadius() * 2);
                        break;
                    case D:
                        innerCircle.setCenterX(outerCircle.getCenterX() + innerCircle.getRadius() * 2);
                        break;
                    case A:
                        innerCircle.setCenterX(outerCircle.getCenterX() - innerCircle.getRadius() * 2);
                        break;
                }
                double x = outerCircle.getCenterX() - innerCircle.getCenterX();
                double y = outerCircle.getCenterY() - innerCircle.getCenterY();
                double angle = Math.atan2(y,x);

                try {
                    switch (Double.toString(Math.toDegrees(angle))){
                        case "0.0":
                            os.write("a80".getBytes());
                            break;
                        case "45.0":
                            os.write("q80".getBytes());
                            break;
                        case "90.0":
                            os.write("w80".getBytes());
                            break;
                        case "135.0":
                            os.write("e80".getBytes());
                            break;
                        case "180.0":
                            os.write("d80".getBytes());
                            break;
                        case "-45.0":
                            os.write("z80".getBytes());
                            break;
                        case "-90.0":
                            os.write("s80".getBytes());
                            break;
                        case "-180.0":
                            os.write("c80".getBytes());
                            break;
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        scene.setOnKeyReleased(new EventHandler<KeyEvent>() {
            @Override
            public void handle(KeyEvent keyEvent) {

                try {
                    switch (keyEvent.getCode()) {
                        case W:
                            os.write("x".getBytes());
                            innerCircle.setCenterY(300);
                            break;
                        case S:
                            os.write("x".getBytes());
                            innerCircle.setCenterY(300);
                            break;
                        case D:
                            os.write("x".getBytes());
                            innerCircle.setCenterX(250);
                            break;
                        case A:
                            os.write("x".getBytes());
                            innerCircle.setCenterX(250);
                            break;
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        scene.setRoot(g);
        primaryStage.show();

        primaryStage.setOnCloseRequest(new EventHandler<WindowEvent>() {
            @Override
            public void handle(WindowEvent event) {
                try {
                    os.close();
                    is.close();
                    con.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
    }


    public static void main(String[] args) {
        launch(args);
    }
}