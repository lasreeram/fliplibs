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
		var qobj = mongodbQueue(db, 'cleared-queue');
		qobj.clean(function(err){
			console.log( "queue is cleaned");	
		});
});

