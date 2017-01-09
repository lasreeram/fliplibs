//window.onload = callWebService;

function displayResponse( entries ){
	alert( entries );
	if( entries == null )
		alert ( "entries is null!" );
	else
		alert ( "size of entries = " + entries.size() );
	var salesDiv = document.getElementById( "sales" );
	for( var i = 0; i < entries.length; i++ ){
		var entry = entries[i];
		var div = document.createElement("div");
		div.setAttribute("class", "item" );
		div.innerHTML = entry.name + " sold " + entry.sales + " items";
		salesDiv.appendChild(div);
	}
}


function callWebService(){
	var url = "http://127.0.0.1:8080/sales.json/?callback=displayResponse";

	var request = new XMLHttpRequest();
	request.onload = function(){
		//alert( "recvd response " + request.status );
		if( request.status == 200 ){
			displayResponse( request.responseText );
		}
	};
	request.open( "GET", url );
	//alert( "sending to webservice" );
	request.send(null);
}
