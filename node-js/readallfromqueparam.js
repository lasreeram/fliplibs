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

	var queuename = "";
	if( process.argv.length != 3 ){
		console.log( "node readallfromqueparam.js <queuename>");
		process.exit(0);
	}
	process.argv.forEach(function (val, index, array) {
  		//console.log(index + ': ' + val);
		if( index == 2 ){
			queuename = val;
		}
	});
	var qobj = mongodbQueue(db, queuename );
	var readone = function( err, msg ){
		if( err || !msg ){
			console.log( "finished reading all" );
			process.exit(0);
		}
		console.log('msg.id=' + msg.id);
    		console.log('msg.ack=' + msg.ack);
    		console.log('msg.payload=' + msg.payload);
    		console.log('msg.tries=' + msg.tries);
		qobj.ack( msg.ack, function(err, id) {
			console.log( 'message ' + id + ' removed' );
		});
		qobj.get(readone);
	}

	qobj.get(readone);


});

