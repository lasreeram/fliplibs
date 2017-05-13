// Handler class for GET /index URL

function HandleGetPromotions(){
}

//Call this service using the following URL
//For example: http://localhost:8080/getjson
HandleGetPromotions.prototype.process = function process(ctxt){
	console.log( "process called within Get Json handler!" );
	var fs = require('fs');

	var db = ctxt.db;
	var response = ctxt.response;
	//get parameters
	//var beaconinfo = JSON.parse(ctxt.query.search_criteria);
	var uuid = ctxt.query.uuid;
	var major = ctxt.query.major;
	var minor = ctxt.query.minor;

	var error = false;

	if( !uuid ){
		console.log( "uuid not passed!");
		error = true;
	}
	if( !major ){
		console.log( "major not passed!");
		error = true;
	}
	if( !minor ){
		console.log( "minor not passed!");
		error = true;
	}

	if( error ){
		ctxt.response.writeHeader(200, {"Content-Type": "application/json"});
		ctxt.response.write("{ 'result': 'failure', 'reason': 'beacon info not found in request!' ");
		ctxt.response.end();
		return;
	}
	var major_query = '{ "major": { "$in": [' + major + '] } }'
	var minor_query = '{ "minor": { "$in": [' + minor + '] } }'


	var db_query_get_merchants = '{ "$and": [' + major_query + ',' + minor_query + '] }'
	var json_obj1 = JSON.parse(db_query_get_merchants);
	console.log( db_query_get_merchants );

	var col_beacon_merchant = db.collection('beacon_merchant');
	col_beacon_merchant.find( json_obj1 ).toArray(function( err, items ) {
		if( !err ){
			console.log( "found merchant! " );
			var merchantList = "";
			var length = items.length;
			console.log( items );
			console.log( "length = " + items.length );
			for( var j = 0; j < length; j++ ){
				console.log( items[j] );
				console.log( items[j].merchant_id );
				if( j == (length-1) ){
					merchantList += '"' + items[j].merchant_id + '"';	
				}else{
					merchantList += '"' + items[j].merchant_id + '",';
				}
			}
			var col_merchant_promotions = db.collection('merchant_promotions');
			//var db_query_get_promotions = '{ "merchant_id":"' + item[0].merchant_id+ '"}';
			var db_query_get_promotions = '{ "merchant_id": { "$in": [' + merchantList + '] } }';
			console.log( db_query_get_promotions );
			var json_obj2 = JSON.parse( db_query_get_promotions );
			col_merchant_promotions.find( json_obj2 ).toArray(function( err, items ){
				if( !err ){
					console.log( "found promotion! " );
					console.log( items );
					response.writeHeader(200, {"Content-Type": "application/json"});
					response.write( JSON.stringify(items) );
					response.end();
				}else{
					console.log( "error finding data!" );
					response.writeHeader(200, {"Content-Type": "application/json"});
					ctxt.response.write("{ 'result': 'failure', 'reason': 'cannot get promotions from db!' ");
					response.end();
				}
			});
		}
		else{
			console.log( "error finding data!" );
			response.writeHeader(200, {"Content-Type": "application/json"});
			ctxt.response.write("{ 'result': 'failure', 'reason': 'cannot get merchant from db!' ");
			response.end();
		}
	} );
	return;
}

module.exports = HandleGetPromotions;
