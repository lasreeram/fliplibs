// Context class

function HttpServerContext(db, request, response, query ){
	this.db = db;
	this.request = request;
	this.response = response;
	this.query = query;
}

module.exports = HttpServerContext;
