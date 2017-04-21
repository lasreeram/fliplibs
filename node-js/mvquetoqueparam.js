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

	var queuename1 = "";
	var queuename2 = "";
	if( process.argv.length != 4 ){
		console.log( "node mvquetoque.js <queuename1> <queuename2>");
		process.exit(0);
	}
	process.argv.forEach(function (val, index, array) {
  		//console.log(index + ': ' + val);
		if( index == 2 ){
			queuename1 = val;
		}
		if( index == 3 ){
			queuename2 = val;
		}
	});


	var qobj1 = mongodbQueue(db, queuename1);
	var qobj2 = mongodbQueue(db, queuename2);
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

