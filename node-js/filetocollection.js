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

		var filename = "";
		var colname = "";
		if( process.argv.length != 4 ){
			console.log( "node filetocollection.js filename colname");
			process.exit(0);
		}
		process.argv.forEach(function (val, index, array) {
  			//console.log(index + ': ' + val);
			if( index == 2 ){
				filename = val;
			}
			if( index == 3 ){
				colname = val;
			}
		});
		var fs = require('fs');
    		var jsonstream = require('JSONStream');
		var stream = fs.createReadStream(filename, {encoding: 'utf8'});
    		var parser = jsonstream.parse();
		stream.pipe(parser);
		console.log( 'reading file ' + filename );

		parser.on('data', function (obj) {
			db.collection(colname).insertOne( obj, function( err, result ){
				console.log( "inserted record in to collection " + colname );
			});
		});

		parser.on('end', ()=> {
			console.log( 'finished parsing' );
			//process.exit(0);
		});
});


