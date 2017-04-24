HttpsServer = require("https");
HttpServerContext = require("./HttpServerContext");
HandleGetIndex = require("./HandleGetIndex" );
HandlePageNotFound = require("./HandlePageNotFound");
FileToQueHandler = require("./FileToQueHandler");
Url = require("url");

var MongoClient = require('mongodb').MongoClient;
var mainHandler = function (request, response) {
		MongoClient.connect("mongodb://localhost:27017/test", function(err, db){
	  	if(!err) {
			console.log("connected to mongodb!");
			//return db;
	  	}else{
			console.log("error connecting " + err );
			return null;
	  	}

		//console.log("I got kicked");
		var parsedUrl = Url.parse(request.url, true);
		var queryObj = parsedUrl.query;
		var mongodbQueue = require('mongodb-queue');
		console.log( JSON.stringify(queryObj) );
		console.log( parsedUrl.pathname );
		var ctxt = new HttpServerContext(db, request, response, queryObj, mongodbQueue);
		if( ctxt.request.method == "GET" ){
			if( (parsedUrl.pathname == "/filetoque") || (parsedUrl.pathname == "/filetoque/") ){
				var handler = new FileToQueHandler();
				handler.process(ctxt);
			}else if( (parsedUrl.pathname == "/index/") || (parsedUrl.pathname == "/index") ){
				var handler = new HandleGetIndex();
				handler.process(ctxt);
			}else{
				var handler = new HandlePageNotFound();
				handler.process(ctxt);
			}
		}
	});
}
const fs = require('fs');
const options = {
  key: fs.readFileSync('privatekey.pem'),
  cert: fs.readFileSync('certificate.pem'),
  passphrase: 'sree.123'
};

HttpsServer.createServer(options, mainHandler ).listen(8081);
console.log("Server Running on 8081"); 
