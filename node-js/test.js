my_http = require("http");
// Retrieve
var MongoClient = require('mongodb').MongoClient;

// Connect to the db
MongoClient.connect("mongodb://localhost:27017/test", function(err, db) {
  if(!err) {
	console.log("connected to mongodb!");
  }else{
	return console.log("error connecting " + err );
  }

  var collection = db.collection('restaurants');

  collection.find( {"restaurant_id": "40400429"} ).toArray(function( err, item ) {
	if( !err ){
		console.log( "found restaurant! " + err );
		console.log( item );
	}else{
		console.log( "error finding restaurant!" );
	}
  });
});

var mainHandler = function (request, response){
  console.log("I got kicked");
	if( request.method == "GET" ){
		if( request.url == "/index" ){
			response.writeHeader(200, {"Content-Type": "text/plain"});
			response.write("Index-html");
			response.end();
		}else{
			response.writeHead(404, {'Content-Type': 'text/html'});
			response.write('<!doctype html><html><head><title>404</title></head><body>404: Resource Not Found</body></html>');
			response.end();
		}
	}
}
my_http.createServer( mainHandler ).listen(8080);
console.log("Server Running on 8080"); 
