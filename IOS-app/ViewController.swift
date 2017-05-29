//
//  ViewController.swift
//  SmartCarVR
//
//  Created by Murat Kan on 2017-04-20.
//  Copyright © 2017 Murat Kan. All rights reserved.
//

import UIKit
import CoreMotion

class ViewController: UIViewController, UIWebViewDelegate, StreamDelegate {

    //declaring the two view boxes
    @IBOutlet weak var webView1: UIWebView!
    @IBOutlet weak var webView2: UIWebView!

    //declaring and initializing the motion sensor object
    var motionManager = CMMotionManager()

    override func viewDidLoad() {
        super.viewDidLoad()

        //video feed link
        UIApplication.shared.isIdleTimerDisabled = true
        let stream_url  = "http://192.168.43.118:8090/stream"

        //connection to the servo motor
        let socket = Connection(address: "192.168.43.118" as CFString, port: 5000)

        //streams the video feed in the first view box
        webView1.delegate = self
        webView1.loadRequest(URLRequest(url: URL(string: stream_url )!))

        //streams the video feed in the second view box
        webView2.delegate = self
        webView2.loadRequest(URLRequest(url: URL(string: stream_url )!))

        //receiving data from the gyroscope on the smart device
        if motionManager.isDeviceMotionAvailable == true {

            motionManager.deviceMotionUpdateInterval = 0.1;

            let queue = OperationQueue()
            motionManager.startDeviceMotionUpdates(to: queue, withHandler: {(motion, error) -> Void in

                if let attitude = motion?.attitude {

                    socket.connect()
                    //converts the radians to degrees, converts it to an integer and finally converts it to a String after which the value is sent to the rpi
                    socket.sendMessage(message: String(Int(90 + attitude.yaw * 180.0/Double.pi)) + " " + String(Int(attitude.roll * 180.0/Double.pi)))

                }
            })
        }
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
}


class Connection: NSObject, StreamDelegate {
    
    private var outputStream: OutputStream!

    private var my_address: CFString
    private var my_port: UInt32

    public var changeWhenHasBytesAvailable: ((String)->())?

    init (address: CFString, port:UInt32) {
        self.my_address = address
        self.my_port = port
        super.init()
    }

    public func connect() {

        var readStream:  Unmanaged<CFReadStream>?
        var writeStream: Unmanaged<CFWriteStream>?

        CFStreamCreatePairWithSocketToHost(nil, self.my_address, self.my_port, &readStream, &writeStream)

        self.outputStream = writeStream!.takeRetainedValue()

        self.outputStream.delegate = self

        self.outputStream.schedule(in: RunLoop.current, forMode: RunLoopMode.defaultRunLoopMode)

        self.outputStream.open()

    }


    public func sendMessage(message: String) -> Bool {
        var sent: Bool = false
        let buff = [UInt8](message.utf8)
        let result = self.outputStream.write(buff, maxLength: buff.count)

        if result != -1 {
            sent = true
        }
        return sent
    }


}
