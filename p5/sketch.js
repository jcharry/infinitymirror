var serial;
var portName = '/dev/cu.usbmodem1421';
var inData = 0;
var sound; 

function preload() {
  sound = loadSound('assets/beat2.mp3');
}

function setup() {
  createCanvas(600,400);
  
  serial = new p5.SerialPort();       // make a new instance of the serialport library
  serial.on('list', printList);  // set a callback function for the serialport list event
  serial.on('connected', serverConnected); // callback for connecting to the server
  serial.on('open', portOpen);        // callback for the port opening
  serial.on('data', serialEvent);     // callback for when new data arrives
  serial.on('error', serialError);    // callback for errors
  serial.on('close', portClose);      // callback for the port closing
 
  serial.list();                      // list the serial ports
  serial.open(portName, {baudrate: 115200} );              // open a serial port
}

function serverConnected() {
  println('connected to server.');
}
 
function portOpen() {
  println('the serial port opened.');
}
 
function serialEvent() {
  var bpm = serial.readLine();
  print(bpm);
  sound.play();
  serial.write('x');
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