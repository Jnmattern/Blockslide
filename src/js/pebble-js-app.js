var dateorder;
var weekday;
var battery;
var bluetooth;
var lang;
var stripes;
var roundcorners;
var fulldigits;
var colortheme;
var themecode;

function readConfig() {
  dateorder = localStorage.getItem("dateorder");
  if (!dateorder) {
    dateorder = 1;
  }

  weekday = localStorage.getItem("weekday");
  if (!weekday) {
    weekday = 0;
  }

  battery = localStorage.getItem("battery");
  if (!battery) {
    battery = 1;
  }

  bluetooth = localStorage.getItem("bluetooth");
  if (!bluetooth) {
    bluetooth = 1;
  }

  lang = localStorage.getItem("lang");
  if (!lang) {
    lang = 1;
  }

  stripes = localStorage.getItem("stripes");
  if (!stripes) {
    stripes = 1;
  }

  roundcorners = localStorage.getItem("roundcorners");
  if (!roundcorners) {
    roundcorners = 1;
  }

  fulldigits = localStorage.getItem("fulldigits");
  if ((fulldigits != 0) && (fulldigits != 1)) {
    fulldigits = 0;
  }

  colortheme = localStorage.getItem("colortheme");
  if (!colortheme) {
    colortheme = 1;
  }

  themecode = localStorage.getItem("themecode");
  if (!themecode) {
    themecode = "ffffffffffc0";
  }
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
  console.log("	themecode: " + themecode);
}

function isWatchColorCapable() {
  if (typeof Pebble.getActiveWatchInfo === "function") {
    try {
      if (Pebble.getActiveWatchInfo().platform != 'aplite') {
        return true;
      } else {
        return false;
      }
    } catch(err) {
      console.log('ERROR calling Pebble.getActiveWatchInfo() : ' + JSON.stringify(err));
      // Assuming Pebble App 3.0
      return true;
    }
  }
  //return ((typeof Pebble.getActiveWatchInfo === "function") && Pebble.getActiveWatchInfo().platform!='aplite');
}

Pebble.addEventListener("ready", function() {
  readConfig();
  logVariables("Ready Event");
	Pebble.sendAppMessage(JSON.parse('{"dateorder":'+dateorder+',"weekday":'+weekday+',"battery":'+battery+',"bluetooth":'+bluetooth+',"lang":'+lang+',"stripes":'+stripes+',"roundcorners":'+roundcorners+',"fulldigits":'+fulldigits+',"colortheme":'+colortheme+',"themecode":"'+themecode+'"}'));
});

Pebble.addEventListener("showConfiguration", function(e) {
	logVariables("showConfiguration Event");
  var url = "http://www.famillemattern.com/jnm/pebble/Blockslide/Blockslide_3.9.html?dateorder=" + dateorder + "&weekday=" + weekday + "&battery=" + battery + "&bluetooth=" + bluetooth + "&lang=" + lang + "&stripes=" + stripes + "&roundcorners=" + roundcorners + "&fulldigits=" + fulldigits + "&colortheme=" + colortheme + "&themecode=" + themecode + "&colorCapable=" + isWatchColorCapable();
  console.log(url);
	Pebble.openURL(url);
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

  themecode = configuration["themecode"];
	localStorage.setItem("themecode", themecode);
});

