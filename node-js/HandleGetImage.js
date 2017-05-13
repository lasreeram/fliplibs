// Handler class for GET /index URL
var fs = require('fs');
HandlePageNotFound = require("./HandlePageNotFound");

function HandleGetImage(){
}

HandleGetImage.prototype.process = function process(ctxt){
	console.log( "process called within Get Image handler!" );
	//var filename = ctxt.parsedUrl.pathname;
	//console.log( "file name = " + filename );
	//filename = filename.replace( "/", "" );
	//filename = filename.replace( "/", "" );
	//if( !filename.includes("gif") ){
	//	filename += ".gif";
	//}
	var filename = "image_default.gif";
	console.log( "serving file = " + filename );
	//filename = ctxt.query.file;
	var response = ctxt.response;
	fs.readFile(filename, function (err, data) {
		if(err){
			console.log(err);
			var handler = new HandlePageNotFound();
			handler.process(ctxt);
			return;
		}
		response.writeHead(200, {'Content-Type': 'image/gif'});
		response.end(data, 'binary');
	});
}

module.exports = HandleGetImage;
