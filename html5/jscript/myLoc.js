window.onload = getMyLocation;
var map;

function computeDistance(startCoords, destCoords) {
    var startLatRads = degreesToRadians(startCoords.latitude);
    var startLongRads = degreesToRadians(startCoords.longitude);
    var destLatRads = degreesToRadians(destCoords.latitude);
    var destLongRads = degreesToRadians(destCoords.longitude);
    var Radius = 6371; // radius of the Earth in km
    var distance = Math.acos(Math.sin(startLatRads) * Math.sin(destLatRads) +
    Math.cos(startLatRads) * Math.cos(destLatRads) *
    Math.cos(startLongRads - destLongRads)) * Radius;
    return distance;
}

function degreesToRadians(degrees) {
    var radians = (degrees * Math.PI)/180;
    return radians;
}

function deg_to_dms (deg) {
	var d = Math.floor (deg);
	var minfloat = (deg-d)*60;
	var m = Math.floor(minfloat);
	var secfloat = (minfloat-m)*60;
	var s = Math.round(secfloat);
	// After rounding, the seconds might become 60. These two
	// if-tests are not necessary if no rounding is done.
	if (s==60) {
		m++;
		s=0;
	}
	if (m==60) {
		d++;
		m=0;
	}
	return ("" + d + ":" + m + ":" + s);
}


function showMap(coords){
	var ourCoords = {
		latitude: 47.62,
		longitude: -122.52
	}
	var googleLatAndLong = new google.maps.LatLng(ourCoords.latitude, ourCoords.longitude);
	var mapOptions = {
		zoom: 10,
		center: googleLatAndLong,
		mapTypeId: google.maps.MapTypeId.ROADMAP
	}

	var mapDiv = document.getElementById("map");
	map = new google.maps.Map(mapDiv, mapOptions);
	if( map != null )
		alert("inside showmap " + map);
}

function displayLocation(position){
	var ourCoords = {
		latitude: 47.62,
		longitude: -122.52
	}
	var latitude = position.coords.latitude;
	var longitude = position.coords.longitude;

	var div = document.getElementById( "location" );
	div.innerHTML = "You are at Latitude: " + deg_to_dms(latitude) + ", Longitude: " + deg_to_dms(longitude) + "<br>";
	div.innerHTML += "Decimal: " + latitude + ", Longitude: " + longitude + "\n";

	var divd = document.getElementById( "distance" );
	divd.innerHTML = "Your distance from our co-ordinates is " + computeDistance (position.coords, ourCoords ) + " km";

	//showMap(ourCoords);	
	showMap(position.coords);	
}

function handleError(error){
	var errorTypes = {
		0: "Unknown Error",
		1: "Permission Denied by user",
		2: "Position is not available",
		3: "Request timedout"
	}

	var errorMessage = errorTypes[error.code];
	if( error.code == 0 || error.code == 2 ){
		errorMessage = errorMessage + " " + error.message;
	}
	var div = document.getElementById( "location" );
	div.innerHTML = errorMessage;
}

function getMyLocation(){
	if( navigator.geolocation ){
		navigator.geolocation.getCurrentPosition( displayLocation, handleError );
	}else{
		alert( "browser does not support geolocation!" );
	}
}

