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
import javafx.scene.shape.Polygon;
import javafx.stage.Stage;

import javax.bluetooth.*;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class Main extends Application {

    Polygon UpTriangle = new Polygon();
    Polygon DownTriangle = new Polygon();
    Polygon RightTriangle = new Polygon();
    Polygon LeftTriangle = new Polygon();

    //Declare BT variables
    String url = "btspp://201510201409:1";
    final RemoteDevice[] hc05device = new RemoteDevice[1];
    final boolean[] scanFinished = {false};

    //declare gui variables
    Boolean moveCircle = false;
    Circle outerCircle = new Circle();
    Circle innerCircle = new Circle();

    @Override
    public void start(Stage primaryStage) throws Exception{

        LocalDevice.getLocalDevice().getDiscoveryAgent().startInquiry(DiscoveryAgent.GIAC, new DiscoveryListener() {
            @Override
            public void deviceDiscovered(RemoteDevice btDevice, DeviceClass cod) {
                try {
                    String name = btDevice.getFriendlyName(false);
                    System.out.format("%s (%s)\n", name, btDevice.getBluetoothAddress());
                    if (name.matches("g1bt")) {
                        hc05device[0] = btDevice;
                        System.out.println("got it!");
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            @Override
            public void servicesDiscovered(int transID, ServiceRecord[] servRecord) {
            }

            @Override
            public void serviceSearchCompleted(int transID, int respCode) {
            }

            @Override
            public void inquiryCompleted(int discType) {
                scanFinished[0] = true;
            }
        });

        while (!scanFinished[0]) {
            //this is easier to understand (for me) as the thread stuff examples from bluecove
            Thread.sleep(500);
        }

        StreamConnection con = (StreamConnection) Connector.open(url);

        OutputStream os = con.openOutputStream();
        InputStream is = con.openInputStream();

        RemoteDevice dev = RemoteDevice.getRemoteDevice(con);

        //os.write("w".getBytes()); //just send '1' to the device


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
                innerCircle.setCenterX(130);
                innerCircle.setCenterY(90);
            }
        });
        outerCircle.setCenterX(130);
        outerCircle.setCenterY(90);
        outerCircle.setRadius(60);
        innerCircle.setCenterX(130);
        innerCircle.setCenterY(90);
        innerCircle.setRadius(20);
        outerCircle.setFill(Color.GREEN);
        innerCircle.setFill(Color.BLACK);
        g.getChildren().add(outerCircle);
        g.getChildren().add(innerCircle);
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
                System.out.println(Math.toDegrees(angle));
            }
        });
        scene.setOnKeyReleased(new EventHandler<KeyEvent>() {
            @Override
            public void handle(KeyEvent keyEvent) {
                switch (keyEvent.getCode()) {
                    case W:
                        innerCircle.setCenterY(90);
                        break;
                    case S:
                        innerCircle.setCenterY(90);
                        break;
                    case D:
                        innerCircle.setCenterX(130);
                        break;
                    case A:
                        innerCircle.setCenterX(130);
                        break;
                }
            }
        });
        scene.setRoot(g);
        primaryStage.show();


        //Close these when app closes
        //os.close();
        //is.close();
        //con.close();
    }


    public static void main(String[] args) {
        launch(args);
    }

    }