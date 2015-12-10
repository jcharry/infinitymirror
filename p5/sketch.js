var serial;
var portName = '/dev/cu.usbmodem1421';
var inData = 0;
var sound;

var startupsound1;
var startupsound2;

function preload() {
  beat = loadSound('assets/beat.mp3');
  beat2 = loadSound('assets/beat2.mp3');
  doublebeat = loadSound('assets/doublebeat.wav');
  drumbeat = loadSound('assets/drumbeat.wav');
  singlebeat = loadSound('assets/singlebeat.wav');
  hb1 = loadSound('assets/HB1.mp3');
  hb = loadSound('assets/HB.mp3');
  startupsound1 = loadSound('assets/1.wav');
  startupsound2 = loadSound('assets/3.wav');
}

function setup() {
  createCanvas(600, 400);

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
  print('data: ' + data);

  if (!data) {
    return false;
  }



  if (data.length > 0) {
    
    console.log(Number(data));
    
    if (data == "startup") {
      // play startup audio
      console.log('startup');
      startupsound2.play();
    }
    if (Number(data) !== NaN) {
      hb1.play();
      serial.write('x');
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