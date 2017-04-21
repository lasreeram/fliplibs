HttpServer = require("http");
HttpServerContext = require("./HttpServerContext");
HandleGetIndex = require("./HandleGetIndex" );
HandlePageNotFound = require("./HandlePageNotFound");
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
		console.log( JSON.stringify(queryObj) );
		console.log( parsedUrl.pathname );
		var ctxt = new HttpServerContext(db, request, response, queryObj);
		if( ctxt.request.method == "GET" ){
			if( parsedUrl.pathname == "/index/" ){
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
