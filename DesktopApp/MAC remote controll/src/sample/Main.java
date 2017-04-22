package sample;

import javafx.application.Application;
import javafx.event.EventHandler;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.input.KeyEvent;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.stage.Stage;

import javax.bluetooth.*;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import javax.swing.*;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;




public class Main extends Application {

    //Declare BT variables
    String url = "btspp://201510201409:1";
    final RemoteDevice[] hc05device = new RemoteDevice[1];
    final boolean[] scanFinished = {false};
    boolean deviceFound = false;

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
                        deviceFound = true;
                        //System.out.println("got it!"); //Used for debugging
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                    JOptionPane.showMessageDialog(null, "An error has occurred, please restart the application and make sure bluetooth is turned on.");
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

        if (!deviceFound){
            System.exit(0);
            JOptionPane.showMessageDialog(null, "The application will quit as the car has not been found, restart once car is ready to connect.");
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

                //System.out.println(Math.toDegrees(angle));

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
                            innerCircle.setCenterY(300);
                            os.write("x".getBytes());
                            break;
                        case S:
                            innerCircle.setCenterY(300);
                            os.write("x".getBytes());
                            break;
                        case D:
                            innerCircle.setCenterX(250);
                            os.write("x".getBytes());
                            break;
                        case A:
                            innerCircle.setCenterX(250);
                            os.write("x".getBytes());
                            break;
                    }
                } catch (IOException e) {
                    e.printStackTrace();
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