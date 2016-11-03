window.onload = init; //the init here is a first class object
function save(item) {
	var playlistArray = getStoreArray("playlist");
	playlistArray.push(item);
	//alert( "saving " + playlistArray.length );
	localStorage.setItem("playlist", JSON.stringify(playlistArray));
}

function getStoreArray(key){
	var playlistArray = localStorage.getItem(key);
	//alert ("load play list " + playlistArray );
	
	if( playlistArray == null || playlistArray == "" ){
		playlistArray = new Array();
	}else{
		playlistArray = JSON.parse(playlistArray);
	}
	return playlistArray;
}

function getSavedSongs(){
	return getStoreArray("playlist");
}

function loadPlayList() {
	//alert("loading play list");
	var playlistArray = getSavedSongs();
	var ul = document.getElementById("playlist");
	if ( playlistArray != null ){
		for( var i = 0; i < playlistArray.length; i++ ){
			var li = document.createElement("li");
			li.innerHTML = playlistArray[i];
			ul.appendChild(li);
		}
	}
}

function handleClearClick() {
	localStorage.setItem("playlist", "" );
}

function handleButtonClick() {
	var textInput = document.getElementById("songTextInput");
	var songText = textInput.value;
	if( songText == "" ){
		alert ( "enter a song name!" );
		return;
	}
	//alert( "adding '" + songText + "'" );
	var ul = document.getElementById("playlist");
	var li = document.createElement("li");
	li.innerHTML = songText;
	ul.appendChild(li);
	save(songText);
	textInput.value = "";
}

function init() {
	//alert("init");
	var addbutton = document.getElementById("addButton");
	addbutton.onclick = handleButtonClick; //the function here is a first class object
	var clearButton = document.getElementById("clearButton");
	clearButton.onclick = handleClearClick;
	loadPlayList();
}
