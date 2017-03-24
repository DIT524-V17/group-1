package com.bluetoothcontroller.bluetoothcontroller;

import io.github.controlwear.virtual.joystick.android.JoystickView;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;
import android.widget.Toast;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;


public class MainActivity extends AppCompatActivity {

    // bluetooth connection
    private BluetoothAdapter btAdapter = null;
    private BluetoothSocket btSocket = null;
    private OutputStream outStream = null;
    private InputStream inputStream = null;
    private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    // bluetooth module MAC address
    private static String MACaddress = "20:15:10:20:14:09";
    // send BT enable request
    private static final int REQUEST_ENABLE_BT = 1;

    // Display angle and power
    private TextView angleText = null;
    private TextView powerText = null;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // joystick to control movement
        JoystickView joystick = (JoystickView) findViewById(R.id.joystickView);
        final TextView angleText = (TextView) findViewById(R.id.angleText);
        final TextView powerText = (TextView) findViewById(R.id.powerText);

        // bluetooth connection
        btAdapter = BluetoothAdapter.getDefaultAdapter();

        if (btAdapter == null) {
            // device does not support bluetooth
            Context context = getApplicationContext();
            Toast toast = Toast.makeText(context, "Device does not support bluetooth", Toast.LENGTH_SHORT);
            toast.show();


        }
        else {
            // if disabled, enable
            if (!btAdapter.isEnabled()) {
                Intent enableBtIntent = new Intent(btAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            }

        }


        /**
         * Movement will be done using a Joystick
         *
         */

        // get the movement
        joystick.setOnMoveListener(new JoystickView.OnMoveListener() {
            @Override
            public void onMove(int angle, int strength) {
                String powerString = "Power: " + strength;
                // Make sure strength is not 0
                if (strength > 15) {
                    powerText.setText(powerString);
                    // forward
                    if (angle <= 120 && angle > 60) {
                        angleText.setText("↑");
                        sendData("w");
                    }

                    // diag forward right
                    if (angle <= 60 && angle > 30) {
                        angleText.setText("↗");
                        sendData("e");

                    }

                    // right
                    if (angle <= 30 || angle > 330) {
                        angleText.setText("→");
                        sendData("d");
                    }

                    // diag backward right
                    if (angle <= 330 && angle > 300) {
                        angleText.setText("↘");
                        sendData("c");
                    }

                    // backward
                    if (angle <= 300 && angle > 240) {
                        angleText.setText("↓");
                        sendData("s");
                    }

                    // diag backward left
                    if (angle <= 240 && angle > 210) {
                        angleText.setText("↙");
                        sendData("z");
                    }

                    // left
                    if (angle <= 210 && angle > 150) {
                        angleText.setText("←");
                        sendData("a");
                    }

                    // diag forward left
                    if (angle <= 150 && angle > 120) {
                        angleText.setText("↖");
                        sendData("q");
                    }
                } else {
                    sendData("x");
                }

            }
        });






    }

    @Override
    public void onPause() {
        super.onPause();

        if (outStream != null) {
            try {
                outStream.flush();
            } catch (IOException e) {
                errorExit("Fatal Error", "In onPause() and failed to flush output stream: " + e.getMessage() + ".");
            }
        }

        try     {
            btSocket.close();
        } catch (IOException e2) {
            errorExit("Fatal Error", "In onPause() and failed to close socket." + e2.getMessage() + ".");
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        BluetoothDevice device = btAdapter.getRemoteDevice(MACaddress);


        try {
            btSocket = device.createRfcommSocketToServiceRecord(MY_UUID);

        } catch (IOException e) {

            errorExit("Fatal Error", "In onResume() and socket create failed: " + e.getMessage() + ".");
        }

        // stop discovery, to avoid using too many resources
        btAdapter.cancelDiscovery();

        try {
            btSocket.connect();

        }
        catch (IOException e) {
            try {
                btSocket.close();
            } catch (IOException e2) {
                errorExit("Fatal error!", "onResume failed to close socket on connection error! " + e2.getMessage() + ".");
            }
        }

        // create stream to talk
        try {
            outStream = btSocket.getOutputStream();
        } catch (IOException e) {
            errorExit("Fatal Error", "In onResume() and output stream creation failed:" + e.getMessage() + ".");
        }

    }

    private void errorExit(String title, String message) {
        Toast msg = Toast.makeText(getBaseContext(), title + " : " + message, Toast.LENGTH_LONG);
        msg.show();
        finish();
    }

    private void sendData(String send) {
        if (btSocket != null) {
            try {
                btSocket.getOutputStream().write(send.getBytes());
            }
            catch (IOException e) {

            }
        }

    }



}
