// Handler class for GET /index URL
var fs = require('fs');
HandlePageNotFound = require("./HandlePageNotFound");

function HandleGetHtmlPage(){
}

HandleGetHtmlPage.prototype.process = function process(ctxt){
	console.log( "process called within GetHtmlPage handler!" );
	var filename = ctxt.parsedUrl.pathname;
	console.log( "file name = " + filename );
	filename = filename.replace( "/", "" );
	filename = filename.replace( "/", "" );
	if( !filename.includes("html") ){
		filename += ".html";
	}
	console.log( "file name = " + filename );
	//filename = ctxt.query.file;
	var response = ctxt.response;
	fs.readFile(filename, function (err, data) {
		if(err){
			var handler = new HandlePageNotFound();
			handler.process(ctxt);
			return;
		}
		response.writeHead(200, {'Content-Type': 'text/html'});
		response.write(data);
		response.end();
	});
}

module.exports = HandleGetHtmlPage;
