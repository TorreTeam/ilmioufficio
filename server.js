// Node JS app, handling communication with arduino

var http = require('http');
var fs = require('fs');


var SerialPort = require('serialport');

var portTTY1 = '/dev/ttyACM0';

var dataFromArduino1;
var data1 = '';
var mod1 = 0;

var serialport = new SerialPort(portTTY1, {
    baudRate: 115200
});
serialport.on('open', function(){
  console.log('Serial Port Opened');
  serialport.on('data', function(data){
      dataFromArduino1 = data.toString('utf-8');
      console.log(dataFromArduino1);
      if(mod1 === 0){
          if(dataFromArduino1.search('{') !== -1){
              data1 = dataFromArduino1;
              mod1 = 1;
          }
      }else {
          data1 = data1.concat(dataFromArduino1);
          if (data1.search('}') !== -1){
              mod1 = 0;
          }
      }

      if(data1.search('}') !== -1){
          var stream = fs.createWriteStream("data.json");
          stream.once('open', function(fd) {
              stream.write(data1);
              stream.end();
              data1 = '';
          });
      }

  });
});

function onRequest(request,response){
    if(request.method == 'GET' && request.url == '/') {
        response.writeHead(200,{"Content-Type":"text/html"});
        fs.createReadStream("./index.html").pipe(response);
    } else if(request.method == 'GET' && request.url == '/jquery-3.2.0.min.js') {
        response.writeHead(200,{"Content-Type":"text/javascript"});
        fs.createReadStream("./jquery-3.2.0.min.js").pipe(response);
    } else if(request.method == 'GET' && request.url == '/app.js') {
        response.writeHead(200,{"Content-Type":"text/javascript"});
        fs.createReadStream("./app.js").pipe(response);
    }
    else if(request.method == 'GET' && request.url == '/data.json') {
        response.writeHead(200,{"Content-Type":"text/json"});
        fs.createReadStream("./data.json").pipe(response);
    }
}

http.createServer(onRequest).listen(1234, '0.0.0.0');
console.log("Server is running ....");