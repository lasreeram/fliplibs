// Handler class for GET /index URL

function HandlePageNotFound(){
}

HandlePageNotFound.prototype.process = function process(ctxt){
	console.log( "process called within page not found handler!");
	var response = ctxt.response;
	response.writeHead(404, {'Content-Type': 'text/html'});
	response.write('<!doctype html><html><head><title>404</title></head><body>404: Resource Not Found</body></html>');
	response.end();
}

module.exports = HandlePageNotFound;
