// Handler class for GET /index URL

function HandleGetJson(){
}

//Call this service using the following URL
//For example: http://localhost:8080/getjson
HandleGetJson.prototype.process = function process(ctxt){
	console.log( "process called within Get Json handler!" );
	var fs = require('fs');

	var db = ctxt.db;
	var response = ctxt.response;
	//get parameters
	var restaurant_id = ctxt.query.restaurant_id;
	var error = false;
	if( !restaurant_id){
		console.log( "ERROR: queue 1 not passed!");
		error = true;
	}
	if( error == true ){
		ctxt.response.writeHeader(200, {"Content-Type": "application/json"});
		ctxt.response.write("processing failed!, check server log");
		ctxt.response.end();
		return;
	}
	var db_query_string = JSON.parse('{"restaurant_id":' + ' "' + restaurant_id + '"' + '}');

	var collection = db.collection('restaurants');
	collection.find( db_query_string ).toArray(function( err, item ) {
		if( !err ){
			console.log( "found restaurant! " );
			console.log( item );
			response.writeHeader(200, {"Content-Type": "application/json"});
			response.write( JSON.stringify(item) );
			response.end();
		}else{
			console.log( "error finding restaurant!" );
			response.writeHeader(200, {"Content-Type": "application/json"});
			response.write("cannot get restaurant " + restaurant_id + ", check server log");
			response.end();
		}
	});

	return;
}

module.exports = HandleGetJson;
