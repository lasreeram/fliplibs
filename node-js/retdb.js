my_http = require("http");
// Retrieve
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

		console.log("I got kicked");
		if( request.method == "GET" ){
			if( request.url == "/index" ){
  				var collection = db.collection('restaurants');
  				collection.find( {"restaurant_id": "40400429"} ).toArray(function( err, item ) {
					if( !err ){
						console.log( "found restaurant! " );
						console.log( item );
					}else{
						console.log( "error finding restaurant!" );
					}
					//response.writeHeader(200, {"Content-Type": "text/plain"});
					response.writeHeader(200, {"Content-Type": "application/json"});
					response.write('{"Index-html"}');
					//response.write(item);
					response.end();
  				});
			}else{
				//response.writeHead(404, {'Content-Type': 'text/html'});
				response.writeHead(404, {'Content-Type': 'text/html'});
				response.write('<!doctype html><html><head><title>404</title></head><body>404: Resource Not Found</body></html>');
				response.end();
			}
		}
	});
}

my_http.createServer( mainHandler ).listen(8080);
console.log("Server Running on 8080"); 
