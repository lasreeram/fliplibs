my_http = require("http");
// Retrieve
var MongoClient = require('mongodb').MongoClient;
var mongodbQueue = require('mongodb-queue');

var mainHandler = function (request, response) {
	MongoClient.connect("mongodb://localhost:27017/test", function(err, db){
	  	if(!err) {
			console.log("connected to mongodb!");
			//return db;
	  	}else{
			console.log("error connecting " + err );
			return null;
	  	}
		var qobj = mongodbQueue(db, 'sw-clearing-queue');
		qobj.add( '{ id="1", data="hello world" }', function( err, id ) {
			console.log( 'json message ' + id + ' added' );
		});

		
		

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
					//response.writeHeader(200, {"Content-Type": "application/json"});
					qobj.get(function (err, msg){
						if( !msg ){
							response.writeHeader(200, {"Content-Type": "text/plain"});
							response.write('queue is empty');
							response.end();
						}else{
							response.writeHeader(200, {"Content-Type": "text/plain"});
							response.write('msg.id=' + msg.id + '\n' );
							response.write('msg.ack=' + msg.ack + '\n' );
							response.write('msg.payload=' + msg.payload + '\n');
							response.write('msg.tries=' + msg.tries + '\n');
							//response.write(item);
							response.end();

							qobj.ack( msg.ack, function(err, id) {
								console.log( 'message ' + id + ' removed' );
							});
						}
					});
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
