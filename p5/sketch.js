/* eslint-disable */
var serial;
var portName = '/dev/cu.usbmodem1421';
var inData = 0;
var sound;
var isAudioPlaying;

//var startupsound1;
//var startupsound2;
var audioTrack;
var beat;

function preload() {
  //beat = loadSound('assets/beat.mp3');
  //beat2 = loadSound('assets/beat2.mp3');
  //doublebeat = loadSound('assets/doublebeat.wav');
  //drumbeat = loadSound('assets/drumbeat.wav');
  //singlebeat = loadSound('assets/singlebeat.wav');
  beat = loadSound('assets/new-beat.mp3');
  //hb = loadSound('assets/HB.mp3');
  audioTrack = loadSound('assets/plasmic-reflection-no-beat.mp3');
}

function setup() {
  createCanvas(600, 400);
  audioTrack.loop();
  audioTrack.pause();

  serial = new p5.SerialPort(); // make a new instance of the serialport library
  serial.on('list', printList); // set a callback function for the serialport list event
  serial.on('connected', serverConnected); // callback for connecting to the server
  serial.on('open', portOpen); // callback for the port opening
  serial.on('data', serialEvent); // callback for when new data arrives
  serial.on('error', serialError); // callback for errors
  serial.on('close', portClose); // callback for the port closing

  serial.list(); // list the serial ports
  serial.open(portName, {
    baudrate: 115200
  }); // open a serial port
}

function serverConnected() {
  println('connected to server.');
}

function portOpen() {
  println('the serial port opened.');
}

function serialEvent() {
  var data = serial.readLine();
  //print('data: ' + data);

  if (!data) {
    return false;
  }

  if (data.length > 0) {
    //console.log(Number(data));
    if (data == "startup") {
      // play startup audio
        isAudioPlaying = true;
      audioTrack.play();
    }
    if (data == "beat") {
      beat.play();
    }

    if (data == "no hand" && isAudioPlaying) {
        console.log('hand was lifted, fade music');
        isAudioPlaying = false;
        //var timer = 1;
        //audioTrack.setVolume(0.1, 500);
        audioTrack.setVolume(0.01, 1);
        setTimeout(function() {
            audioTrack.pause();
        }, 1000);
    }
  }

  // if (data.length !== 0 && (inData != data)) {
  //   print("new data to update");
  //   inData = data;
  // }

}

function serialError(err) {
  println('Something went wrong with the serial port. ' + err);
}

function portClose() {
  println('The serial port closed.');
}

// get the list of ports:
function printList(portList) {
  // portList is an array of serial port names
  for (var i = 0; i < portList.length; i++) {
    // Display the list the console:
    println(i + " " + portList[i]);
  }


}

function draw() {

}
