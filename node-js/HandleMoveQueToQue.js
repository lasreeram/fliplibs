// Handler class for GET /index URL

function HandleMoveQueToQue(){
}

//Call this service using the following URL
//For example: http://localhost:8080/filetoque/?filename=data.json&queuename=xyz-queue
HandleMoveQueToQue.prototype.process = function process(ctxt){
	console.log( "process called within MoveQueToQue handler!" );
	var fs = require('fs');
    	var jsonstream = require('JSONStream');

	//get parameters
	var que1 = ctxt.query.queue1;
	var que2 = ctxt.query.queue2;
	var error = false;
	if( !que1){
		console.log( "ERROR: queue 1 not passed!");
		error = true;
	}
	if( !que2){
		console.log( "ERROR: queue 2 not passed!");
		error = true;
	}
	if( error == true ){
		ctxt.response.writeHeader(200, {"Content-Type": "application/json"});
		ctxt.response.write("processing failed!, check server log");
		ctxt.response.end();
		return;
	}

	var qobj1 = ctxt.que(ctxt.db, que1 );
	var qobj2 = ctxt.que(ctxt.db, que2 );
	var readone = function( err, msg ){
		if( err || !msg ){
			console.log( "finished reading all" );
			return;
		}
		console.log('msg.id=' + msg.id);
    		console.log('msg.ack=' + msg.ack);
    		console.log('msg.payload=' + msg.payload);
    		console.log('msg.tries=' + msg.tries);
		qobj2.add( msg.payload, function(err, id2){
			console.log( "message = " + id2 + " moved from " + que1 + " to " + que2 );
		});
		qobj1.ack( msg.ack, function(err, id) {
			console.log( 'message ' + id + ' removed' );
		});
		qobj1.get(readone);
	}
	qobj1.get(readone);

	ctxt.response.writeHeader(200, {"Content-Type": "application/json"});
	ctxt.response.write("processing initiated!, check file status");
	ctxt.response.end();
	return;
}

module.exports = HandleMoveQueToQue;
