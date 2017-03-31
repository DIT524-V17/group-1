package sample;

import javafx.application.Application;
import javafx.event.EventHandler;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.input.KeyEvent;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
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

        UpTriangle.getPoints().addAll(new Double[]{
                260.0, 170.0, //top
                200.0, 250.0, //left
                320.0, 250.0  //right
        });

        scene.setOnKeyPressed(new EventHandler<KeyEvent>() {
            @Override
            public void handle(KeyEvent keyEvent) {
                switch (keyEvent.getCode()){
                    case W:
                        UpTriangle.setFill(Color.RED);
                        try {
                            os.write("w".getBytes());
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        break;
                    case S:
                        DownTriangle.setFill(Color.RED);
                        try {
                            os.write("s".getBytes());
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        break;
                    case D:
                        RightTriangle.setFill(Color.RED);
                        try {
                            os.write("d".getBytes());
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        break;
                    case A:
                        LeftTriangle.setFill(Color.RED);
                        try {
                            os.write("a".getBytes());
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        break;

                }
            }
        });

        scene.setOnKeyReleased(new EventHandler<KeyEvent>() {
            @Override
            public void handle(KeyEvent keyEvent) {
                switch (keyEvent.getCode()){
                    case W:
                        UpTriangle.setFill(Color.BLACK);
                        try {
                            os.write("x".getBytes());
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        break;
                    case S:
                        DownTriangle.setFill(Color.BLACK);
                        try {
                            os.write("x".getBytes());
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        break;
                    case D:
                        RightTriangle.setFill(Color.BLACK);
                        try {
                            os.write("x".getBytes());
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        break;
                    case A:
                        LeftTriangle.setFill(Color.BLACK);
                        try {
                            os.write("x".getBytes());
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        break;
                }
            }
        });



        UpTriangle.setOnMousePressed(new EventHandler<MouseEvent>()
        {
            @Override
            public void handle(MouseEvent t) {
                UpTriangle.setFill(Color.RED);
            }
        });

        UpTriangle.setOnMouseReleased(new EventHandler<MouseEvent>() {
            @Override
            public void handle(MouseEvent mouseEvent) {
                UpTriangle.setFill(Color.BLACK);
            }
        });

        DownTriangle.getPoints().addAll(new Double[]{
                260.0, 420.0, //top
                200.0, 350.0, //left
                320.0, 350.0  //right
        });

        DownTriangle.setOnMousePressed(new EventHandler<MouseEvent>()
        {
            @Override
            public void handle(MouseEvent t) {
                DownTriangle.setFill(Color.RED);
            }
        });

        DownTriangle.setOnMouseReleased(new EventHandler<MouseEvent>() {
            @Override
            public void handle(MouseEvent mouseEvent) {
                DownTriangle.setFill(Color.BLACK);
            }
        });


        RightTriangle.getPoints().addAll(new Double[]{
                410.0, 300.0, //top
                330.0, 250.0, //left
                330.0, 350.0  //right
        });

        RightTriangle.setOnMousePressed(new EventHandler<MouseEvent>()
        {
            @Override
            public void handle(MouseEvent t) {
                RightTriangle.setFill(Color.RED);
            }
        });

        RightTriangle.setOnMouseReleased(new EventHandler<MouseEvent>() {
            @Override
            public void handle(MouseEvent mouseEvent) {
                RightTriangle.setFill(Color.BLACK);
            }
        });



        LeftTriangle.getPoints().addAll(new Double[]{
                120.0, 300.0, //top
                190.0, 250.0, //left
                190.0, 350.0  //right
        });

        LeftTriangle.setOnMousePressed(new EventHandler<MouseEvent>()
        {
            @Override
            public void handle(MouseEvent t) {
                LeftTriangle.setFill(Color.RED);
            }
        });

        LeftTriangle.setOnMouseReleased(new EventHandler<MouseEvent>() {
            @Override
            public void handle(MouseEvent mouseEvent) {
                LeftTriangle.setFill(Color.BLACK);
            }
        });


        g.getChildren().add(UpTriangle);
        g.getChildren().add(DownTriangle);
        g.getChildren().add(RightTriangle);
        g.getChildren().add(LeftTriangle);


        Button connectBtn = new Button();
        

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