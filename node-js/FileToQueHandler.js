// Handler class for GET /index URL

function FileToQueHandler(){
}

//Call this service using the following URL
//For example: http://localhost:8080/filetoque/?filename=data.json&queuename=xyz-queue
FileToQueHandler.prototype.process = function process(ctxt){
	console.log( "process called within FileToQue handler!" );
	var fs = require('fs');
    	var jsonstream = require('JSONStream');

	//get parameters
	var file_name = ctxt.query.filename;
	var queue_name = ctxt.query.queuename;
	var error = false;
	if( !file_name ){
		console.log( "ERROR: filename not passed!");
		error = true;
	}
	if( !queue_name ){
		console.log( "ERROR: queue_name not passed!");
		error = true;
	}
	if( error == true ){
		ctxt.response.writeHeader(200, {"Content-Type": "application/json"});
		ctxt.response.write("processing failed!, check server log");
		ctxt.response.end();
		return;
	}

	var stream = fs.createReadStream(file_name, {encoding: 'utf8'});
    	var parser = jsonstream.parse();
	stream.pipe(parser);
	console.log( 'reading file data.json' );
	var qobj = ctxt.que(ctxt.db, queue_name );
	parser.on('data', function (obj) {
		var strdata = JSON.stringify(obj);
  		console.log(strdata); // whatever you will do with each JSON object
		qobj.add( strdata, function( err, id ) {
		console.log( 'json message ' + id + ' added' );
		});
	});

	parser.on('end', ()=> {
		console.log( 'finished parsing' );	
		qobj.done(function(err,count){
		console.log( 'finished adding messages to queue' );	
		});
	});

	ctxt.response.writeHeader(200, {"Content-Type": "application/json"});
	ctxt.response.write("processing initiated!, check file status");
	ctxt.response.end();
	return;
}

module.exports = FileToQueHandler;
