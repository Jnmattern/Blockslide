var dateorder = localStorage.getItem("dateorder");
if (!dateorder) {
	dateorder = 1;
}

var weekday = localStorage.getItem("weekday");
if (!weekday) {
	weekday = 0;
}

var battery = localStorage.getItem("battery");
if (!battery) {
	battery = 1;
}

var bluetooth = localStorage.getItem("bluetooth");
if (!bluetooth) {
	bluetooth = 1;
}

var lang = localStorage.getItem("lang");
if (!lang) {
	lang = 1;
}

var stripes = localStorage.getItem("stripes");
if (!stripes) {
	stripes = 1;
}

var roundcorners = localStorage.getItem("roundcorners");
if (!roundcorners) {
	roundcorners = 1;
}

var fulldigits = localStorage.getItem("fulldigits");
if ((fulldigits != 0) && (fulldigits != 1)) {
  fulldigits = 0;
}

var colortheme = localStorage.getItem("colortheme");
if (!colortheme) {
  colortheme = 0;
}

var bgcolor = localStorage.getItem("bgcolor");
if (!bgcolor) {
  bgcolor = "#000";
}

function logVariables(msg) {
  console.log(msg);
  console.log("	dateorder: " + dateorder);
  console.log("	weekday: " + weekday);
  console.log("	battery: " + battery);
  console.log("	bluetooth: " + bluetooth);
  console.log("	lang: " + lang);
  console.log("	stripes: " + stripes);
  console.log("	roundcorners: " + roundcorners);
  console.log("	fulldigits: " + fulldigits);
  console.log("	colortheme: " + colortheme);
  console.log("	bgcolor: " + bgcolor);

};

Pebble.addEventListener("ready", function() {
  logVariables("Ready Event");
	Pebble.sendAppMessage(JSON.parse('{"dateorder":'+dateorder+',"weekday":'+weekday+',"battery":'+battery+',"bluetooth":'+bluetooth+',"lang":'+lang+',"stripes":'+stripes+',"roundcorners":'+roundcorners+',"fulldigits":'+fulldigits+',"colortheme":'+colortheme+',"bgcolor":'+bgcolor+'}'));
});

Pebble.addEventListener("showConfiguration", function(e) {
	logVariables("showConfiguration Event");
	Pebble.openURL("http://www.famillemattern.com/jnm/pebble/Blockslide/Blockslide.php?dateorder=" + dateorder + "&weekday=" + weekday + "&battery=" + battery + "&bluetooth=" + bluetooth + "&lang=" + lang + "&stripes=" + stripes + "&roundcorners=" + roundcorners + "&fulldigits=" + fulldigits + "&colortheme=" + colortheme + "&bgcolor=" + bgcolor);
});

Pebble.addEventListener("webviewclosed", function(e) {
	console.log("Configuration window closed");
  console.log(e.type);
  console.log("Response: " + decodeURIComponent(e.response));

	var configuration = JSON.parse(decodeURIComponent(e.response));
	Pebble.sendAppMessage(configuration);
	
	dateorder = configuration["dateorder"];
	localStorage.setItem("dateorder", dateorder);
	
	weekday = configuration["weekday"];
	localStorage.setItem("weekday", weekday);

	battery = configuration["battery"];
	localStorage.setItem("battery", battery);
	
	bluetooth = configuration["bluetooth"];
	localStorage.setItem("bluetooth", bluetooth);

	lang = configuration["lang"];
	localStorage.setItem("lang", lang);
						
	stripes = configuration["stripes"];
	localStorage.setItem("stripes", stripes);

	roundcorners = configuration["roundcorners"];
	localStorage.setItem("roundcorners", roundcorners);

  fulldigits = configuration["fulldigits"];
	localStorage.setItem("fulldigits", fulldigits);

  colortheme = configuration["colortheme"];
	localStorage.setItem("colortheme", colortheme);

  bgcolor = configuration["bgcolor"];
	localStorage.setItem("bgcolor", bgcolor);

});
