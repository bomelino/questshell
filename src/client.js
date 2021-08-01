/* Or use this example tcp client written in node.js.  (Originated with 
example code from 
http://www.hacksparrow.com/tcp-socket-programming-in-node-js.html.) */

var net = require('net');

var client = new net.Socket();
client.connect(9007, 'localhost', function() {
	console.log('Connected');
	client.write('234*1.23');
	client.write("log(18+12)")
});

client.on('data', function(data) {
	console.log('Received: ' + data.toString());
	//client.destroy(); // kill client after server's response
});

client.on('close', function() {
	console.log('Connection closed');
});

