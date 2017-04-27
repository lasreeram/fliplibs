// Handler class for GET /index URL
var fs = require('fs');
FileToQueHandler = require("./FileToQueHandler");
HandlePageNotFound = require("./HandlePageNotFound");


function HandlePostAction(){
}

HandlePostAction.prototype.process = function process(ctxt){
	console.log( "process called within PostAction handler!" );
	var action = ctxt.parsedUrl.pathname;
	action = action.replace( "/", "" );
	action = action.replace( "/", "" );
	console.log( "post action = " + action );
	var response = ctxt.response;
	if( action == 'filetoque' ){
		var handler = new FileToQueHandler();
		handler.process(ctxt);
	}else{
		var handler = new HandlePageNotFound();
		handler.process(ctxt);
	}
	return;
}

module.exports = HandlePostAction;
