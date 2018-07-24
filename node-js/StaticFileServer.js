var app = require('http').createServer(handler), fs = require('fs');

var port = Number(process.env.PORT || 5000);
app.listen(port, function() {
  console.log("Listening on " + port);
});


function handler(req, res) {
	fs.readFile(__dirname + req['url'], function(err, data) {
		if (err) {
			res.writeHead(200);
			res.end("File not found!");
			return;
		}
		res.writeHead(200);
		res.end(data);
	});
}
