HttpServer = require("http");
HttpServerContext = require("./HttpServerContext");
HandleGetIndex = require("./HandleGetIndex" );
HandlePageNotFound = require("./HandlePageNotFound");

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
		var ctxt = new HttpServerContext(db, request, response);

		console.log("I got kicked");
		if( ctxt.request.method == "GET" ){
			if( request.url == "/index" ){
				var handler = new HandleGetIndex();
				handler.process(ctxt);
			}else{
				var handler = new HandlePageNotFound();
				handler.process(ctxt);
			}
		}
	});
}

HttpServer.createServer( mainHandler ).listen(8080);
console.log("Server Running on 8080"); 
