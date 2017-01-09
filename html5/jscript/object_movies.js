window.onload = test;
class Movie {
	constructor (title, genre, showtimes){
		this.title = title;
		this.genre = genre;
		this.showtimes = showtimes;
	}

	getInfo() {
		alert( this.title + "," + this.genre + "," + this.showtimes );
	}

};

function test(){
	alert("test!");
	var movie = new Movie ( "Some movie", "action", ["9:00", "13:00", "17:00", "10:30"] );
	//movie.getInfo();
	var movieclone = JSON.parse( JSON.stringify( movie ) );
	alert( "clone of movie" + movieclone.title + "," + movieclone.genre + "," + movieclone.showtimes );
	
}
