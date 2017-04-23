// Context class

function HttpServerContext(db, request, response, query, que ){
	this.db = db;
	this.request = request;
	this.response = response;
	this.query = query;
	this.que = que;
}

module.exports = HttpServerContext;
