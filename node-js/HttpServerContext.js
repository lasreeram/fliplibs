// Context class

function HttpServerContext(db, request, response){
	this.db = db;
	this.request = request;
	this.response = response;
}

module.exports = HttpServerContext;
