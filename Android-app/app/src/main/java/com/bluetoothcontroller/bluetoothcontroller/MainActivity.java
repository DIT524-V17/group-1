// Package
package com.bluetoothcontroller.bluetoothcontroller;

// Joystickview
// source: https://github.com/controlwear/virtual-joystick-android

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.graphics.Color;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Gravity;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.MediaController;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.VideoView;

import org.videolan.libvlc.IVLCVout;
import org.videolan.libvlc.LibVLC;
import org.videolan.libvlc.MediaPlayer;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.UUID;

import io.github.controlwear.virtual.joystick.android.JoystickView;


public class MainActivity extends AppCompatActivity implements IVLCVout.Callback {
    private String vidUrl;


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
    private TextView speedText = null;
    WebView webView;


    // stream from this ip using vlc media player
    // stream from vlc using this command!
    // raspivid -o - -n -t 0 | cvlc -vvv stream:///dev/stdin --sout '#standard{access=http,mux=ts,dst=:8090}' :demux=h264
    private String mVideoUrl = "http://192.168.43.118:8090";

    // media player
    private LibVLC libvlc;
    private VideoView videoView;

    private MediaPlayer mMediaPlayer = null;
    private int mVideoWidth;
    private int mVideoHeight;
    private final static int VideoSizeChanged = -1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            WebView.enableSlowWholeDocumentDraw();
        }
        this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        setContentView(R.layout.activity_main);

        videoView = (VideoView) findViewById(R.id.videoView);
        //mVideoUrl = getIntent().getExtras().getString("videoUrl");

        // joystick to control movement
        JoystickView joystick = (JoystickView) findViewById(R.id.joystickView);

        // webview
        //webView = new WebView(this);
        //setContentView(webView);
        //webView = (WebView) findViewById(R.id.streamView);
        //webView.setWebViewClient(new WebViewClient());
        //webView.getSettings().setJavaScriptEnabled(true);


        // All text views
        final TextView angleText = (TextView) findViewById(R.id.angleText);
        final TextView powerText = (TextView) findViewById(R.id.powerText);
        final TextView speedText = (TextView) findViewById(R.id.speedText);

        // bluetooth connection
        btAdapter = BluetoothAdapter.getDefaultAdapter();

        // check if Bluetooth adapter exists
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
        // Url to stream from
        String liveUrl = "http://192.168.43.118:8082";
        String testUrl = "http://google.com";
        webView.loadUrl(liveUrl);
        webView.getSettings().setBuiltInZoomControls(true);
         */




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
                        angleText.setText("â†‘");
                        
                        sendData("w" + String.valueOf(strength));
                        //sendSpeed(strength);
                        speedText.setText(readSpeed());
                        //sendSpeed(sendPower);
                    }

                    // diag forward right
                    if (angle <= 60 && angle > 30) {
                        angleText.setText("â†—");
                        sendData("e" +  String.valueOf(strength));
                        speedText.setText(readSpeed());
                        //sendSpeed(sendPower);

                    }

                    // right
                    if (angle <= 30 || angle > 330) {
                        angleText.setText("â†’");
                        sendData("d" + String.valueOf(strength));
                        speedText.setText(readSpeed());
                        //sendSpeed(sendPower);
                    }

                    // diag backward right
                    if (angle <= 330 && angle > 300) {
                        angleText.setText("â†˜");
                        sendData("c" + String.valueOf(strength));
                        speedText.setText(readSpeed());
                        //sendSpeed(sendPower);
                    }

                    // backward
                    if (angle <= 300 && angle > 240) {
                        angleText.setText("â†“");
                        sendData("s" + String.valueOf(strength));
                        speedText.setText(readSpeed());
                        //sendSpeed(sendPower);
                    }

                    // diag backward left
                    if (angle <= 240 && angle > 210) {
                        angleText.setText("â†™");
                        sendData("z" + String.valueOf(strength));
                        speedText.setText(readSpeed());
                        //sendSpeed(sendPower);
                    }

                    // left
                    if (angle <= 210 && angle > 150) {
                        angleText.setText("â†�");
                        sendData("a" + String.valueOf(strength));
                        speedText.setText(readSpeed());
                        //sendSpeed(sendPower);
                    }

                    // diag forward left
                    if (angle <= 150 && angle > 120) {
                        angleText.setText("â†–");
                        sendData("q" + String.valueOf(strength));
                        speedText.setText(readSpeed());
                        //sendSpeed(sendPower);
                    }
                }
                else {
                    // stop when nothing is held down
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
        releasePlayer();

    }

    @Override
    public void onResume() {
        // resume app
        super.onResume();
        BluetoothDevice device = btAdapter.getRemoteDevice(MACaddress);


        // create bluetooth socket
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
        createMediaPlayer();

    }


    // Exit the app with an error message
    private void errorExit(String title, String message) {
        Toast msg = Toast.makeText(getBaseContext(), title + " : " + message, Toast.LENGTH_LONG);
        msg.show();
        finish();
    }


    // To be used to send how fast the speed should be
    private void sendSpeed(int send) {
        if (btSocket != null) {
            try {
                btSocket.getOutputStream().write(send);
                btSocket.getOutputStream().flush();
            }
            catch (IOException e) {

            }
        }
    }
    // send the character for what direction to drive
    // will have to be combined with sendspeed, since its serial communications
    private void sendData(String send) {
        if (btSocket != null) {
            try {
                btSocket.getOutputStream().write(send.getBytes());
                btSocket.getOutputStream().flush();
            }
            catch (IOException e) {

            }
        }

    }

    // read speed data, from cars odometers
    private String readSpeed()  {
        String s = "";
        try {
            InputStream in = btSocket.getInputStream();
            float f = in.read();

            s = Float.toString(f);
        }
        catch (IOException e) {

        }
        return s;
    }


    // The following methods are required by vlc library
    // https://github.com/mrmaffen/vlc-android-sdk/issues/18
    // the link above was used as reference for these methods

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        setSize(mVideoWidth, mVideoHeight);
    }



    @Override
    protected void onDestroy() {
        super.onDestroy();
        releasePlayer();
    }

    private void setSize(int width, int height) {
        mVideoWidth = width;
        mVideoHeight = height;
        if (mVideoWidth * mVideoHeight <= 1) {
            return;
        }


        if (videoView == null || videoView == null) {
            return;
        }


        // get screen size
        int w = getWindow().getDecorView().getWidth();
        int h = getWindow().getDecorView().getHeight();

        // getWindow().getDecorView() doesn't always take orientation into
        // account, we have to correct the values
        boolean isPortrait = getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT;
        if (w > h && isPortrait || w < h && !isPortrait) {
            int i = w;
            w = h;
            h = i;
        }

        float videoAR = (float) mVideoWidth / (float) mVideoHeight;
        float screenAR = (float) w / (float) h;

        if (screenAR < videoAR) {
            h = (int) (w / videoAR);
        }

        else {
            w = (int) (h * videoAR);
        }


        // force surface buffer size
        videoView.getHolder().setFixedSize(mVideoWidth, mVideoHeight);

        // set display size
        ViewGroup.LayoutParams lp = videoView.getLayoutParams();
        lp.width = w;
        lp.height = h;
        videoView.setLayoutParams(lp);
        videoView.invalidate();

    }

    private void createMediaPlayer() {
        releasePlayer();

        try {
            if (mVideoUrl.length() > 0) {
                Toast toast = Toast.makeText(this, mVideoUrl, Toast.LENGTH_LONG);
                toast.setGravity(Gravity.BOTTOM | Gravity.CENTER_HORIZONTAL, 0,
                        0);
                toast.show();
            }

            // Create LibVLC
            ArrayList<String> options = new ArrayList<String>();
            //options.add("--subsdec-encoding <encoding>");
            options.add("--aout=opensles");
            options.add("--audio-time-stretch"); // time stretching
            options.add("-vvv"); // verbosity
            libvlc = new LibVLC(this);
            //libvlc = new LibVLC(options);


            mMediaPlayer = new MediaPlayer(libvlc);
            mMediaPlayer.setEventListener(mPlayerListener);

            videoView.setVideoPath(mVideoUrl);
            videoView.setVideoURI(Uri.parse(mVideoUrl));
            videoView.setMediaController(new MediaController(this));
            videoView.setOnPreparedListener(new android.media.MediaPlayer.OnPreparedListener() {
                @Override
                public void onPrepared(android.media.MediaPlayer mp) {
                    Log.d("TAG", "OnPrepared called");
                }
            });

            videoView.start();

        } catch (Exception e) {
            e.printStackTrace();
            Toast.makeText(this, "Error creating player!", Toast.LENGTH_LONG).show();
        }

    }

    protected void releasePlayer() {
        if (libvlc == null)
            return;
        mMediaPlayer.stop();
        final IVLCVout vout = mMediaPlayer.getVLCVout();
        vout.removeCallback(this);
        vout.detachViews();
        videoView = null;
        libvlc.release();
        libvlc = null;

        mVideoWidth = 0;
        mVideoHeight = 0;

    }

    private MediaPlayer.EventListener mPlayerListener = new MainActivity.MyPlayerListener(this);

    @Override
    public void onNewLayout(IVLCVout vout, int width, int height, int visibleWidth, int visibleHeight, int sarNum, int sarDen) {
        if (width * height == 0)
            return;

        // store video size
        mVideoWidth = width;
        mVideoHeight = height;
        setSize(mVideoWidth, mVideoHeight);

    }

    @Override
    public void onSurfacesCreated(IVLCVout vout) {

    }

    @Override
    public void onSurfacesDestroyed(IVLCVout vout) {

    }

    private static class MyPlayerListener implements MediaPlayer.EventListener {
        private WeakReference mOwner;

        public MyPlayerListener(MainActivity owner) {
            mOwner = new WeakReference<MainActivity>(owner);
        }

        @Override
        public void onEvent(MediaPlayer.Event event) {
            MainActivity player = (MainActivity) mOwner.get();

            switch (event.type) {
                case MediaPlayer.Event.EndReached:
                    //Log.d(TAG, "MediaPlayerEndReached");
                    // player.releasePlayer();
                    break;
                case MediaPlayer.Event.Playing:
                case MediaPlayer.Event.Paused:
                case MediaPlayer.Event.Stopped:
                default:
                    break;
            }
        }

    }

    @Override
    public void onHardwareAccelerationError(IVLCVout vout) {
        // Handle errors with hardware acceleration
        //Log.e(TAG, "Error with hardware acceleration");
        this.releasePlayer();
        Toast.makeText(this, "Error with hardware acceleration", Toast.LENGTH_LONG).show();
    }


}
