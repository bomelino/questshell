var net = require('net');

var server = net.createServer(function(socket) {
	// socket connected
    console.log("socket connected",socket)
    //socket.write('Echo server\r\n');
	//socket.pipe(socket);
});

server.listen(9006, '127.0.0.1');