// Context class

function HttpServerContext(db, request, response, query, que, parsedUrl ){
	this.db = db;
	this.request = request;
	this.response = response;
	this.query = query;
	this.que = que;
	this.parsedUrl = parsedUrl;
}

module.exports = HttpServerContext;
