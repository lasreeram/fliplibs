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

		var fs = require('fs');
    		var jsonstream = require('JSONStream');
		var stream = fs.createReadStream('./data.json', {encoding: 'utf8'});
    		var parser = jsonstream.parse();
		stream.pipe(parser);
		console.log( 'reading file data.json' );

		var qobj = mongodbQueue(db, 'xyz-queue');
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
				process.exit(0);
			});
		});

});


