HttpServer = require("http");
HttpServerContext = require("./HttpServerContext");
HandleGetHtmlPage = require("./HandleGetHtmlPage");
HandlePostAction = require("./HandlePostAction");
Url = require("url");
var querystring = require('querystring');

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

		var parsedUrl = Url.parse(request.url, true);
		var mongodbQueue = require('mongodb-queue');
		//console.log( JSON.stringify(queryObj) );
		//console.log( parsedUrl.pathname );
		if( request.method == "GET" ){
			var queryObj = parsedUrl.query;
			var ctxt = new HttpServerContext(db, request, response, queryObj, mongodbQueue, parsedUrl);
			var handler = new HandleGetHtmlPage();
			handler.process(ctxt);
		}else if ( request.method = "POST" ){
			var queryData = "";
			var queryObj = null;
		        request.on('data', function(data) {
		            queryData += data;
            		    queryObj = querystring.parse(queryData);
		            if(queryData.length > 1e6) {
		                queryData = "";
		                response.writeHead(413, {'Content-Type': 'text/plain'}).end();
		                request.connection.destroy();
		            }
		        });
        		request.on('end', function() {
				console.log( "queryData = " + queryData );
				console.log( "query obj = " + JSON.stringify(queryObj) ); 
				var ctxt = new HttpServerContext(db, request, response, queryObj, mongodbQueue, parsedUrl);
				var handler = new HandlePostAction();
				handler.process(ctxt);
        		});
		}
	});
}

HttpServer.createServer( mainHandler ).listen(8080);
console.log("Server Running on 8080"); 
