window.onload = init; //the init here is a first class object
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
	textInput.value = "";
}
function init() {
	var button = document.getElementById("addButton");
	button.onclick = handleButtonClick; //the function here is a first class object
}

