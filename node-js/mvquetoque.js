// Retrieve
var MongoClient = require('mongodb').MongoClient;
var mongodbQueue = require('mongodb-queue');

MongoClient.connect("mongodb://localhost:27017/test", function(err, db){
	if(!err) {
		console.log("connected to mongodb!");
		//return db;
	}else{
		console.log("error connecting " + err );
		return null;
	}
	var qobj1 = mongodbQueue(db, 'xyz-queue');
	var qobj2 = mongodbQueue(db, 'abc-queue');
	var readone = function( err, msg ){
		if( err || !msg ){
			console.log( "finished reading all" );
			process.exit(0);
		}
		console.log('msg.id=' + msg.id);
    		console.log('msg.ack=' + msg.ack);
    		console.log('msg.payload=' + msg.payload);
    		console.log('msg.tries=' + msg.tries);
		qobj2.add( msg.payload, function(err, id2){
			console.log( "message = " + id2 + " moved from queue 1 to queue 2" );
		});
		qobj1.ack( msg.ack, function(err, id) {
			console.log( 'message ' + id + ' removed' );
		});
		qobj1.get(readone);
	}
	qobj1.get(readone);



});

