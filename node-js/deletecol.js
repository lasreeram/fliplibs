// Retrieve
var MongoClient = require('mongodb').MongoClient;
var mongodbQueue = require('mongodb-queue');
var assert = require('assert');

MongoClient.connect("mongodb://localhost:27017/test", function(err, db){
	if(!err) {
		console.log("connected to mongodb!");
		//return db;
	}else{
		console.log("error connecting " + err );
		return null;
	}

	var colname = "";
	if( process.argv.length != 3 ){
		console.log( "node deletecol.js colname");
		process.exit(0);
	}
	process.argv.forEach(function (val, index, array) {
  		//console.log(index + ': ' + val);
		if( index == 2 ){
			colname = val;
		}
	});
	var removeCollection = function(db, callback) {
   		db.collection(colname).deleteMany(
      			{},
      			function(err, results) {
         			console.log(results);
         			callback();
      			});
	};

	removeCollection(db, function() {
		db.close();
	});

});

