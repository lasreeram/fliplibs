// Handler class for GET /index URL

function HandleGetIndex(){
}

HandleGetIndex.prototype.process = function process(ctxt){
	console.log( "process called within GetIndex handler!" );
	var collection = ctxt.db.collection('restaurants');
	collection.find( {"restaurant_id": "40400429"} ).toArray(function( err, item ) {
	if( !err ){
		console.log( "found restaurant! " );
		console.log( item );
	}else{
		console.log( "error finding restaurant!" );
	}
	ctxt.response.writeHeader(200, {"Content-Type": "application/json"});
		ctxt.response.write(JSON.stringify(ctxt.query));
		ctxt.response.end();
	});
}

module.exports = HandleGetIndex;
