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
var invert;
var mirror;
var splash;

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

  invert = localStorage.getItem("invert");
  if (!invert) {
    invert = 0;
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

  mirror = localStorage.getItem("mirror");
  if (!mirror) {
    mirror = 0;
  }

  splash = localStorage.getItem("splash");
  if (!splash) {
    splash = 1;
  }
}

function logVariables(msg) {
  console.log(msg);
  console.log("	dateorder: " + dateorder);
  console.log("	weekday: " + weekday);
  console.log("	battery: " + battery);
  console.log("	bluetooth: " + bluetooth);
  console.log("	invert: " + invert);
  console.log("	lang: " + lang);
  console.log("	stripes: " + stripes);
  console.log("	roundcorners: " + roundcorners);
  console.log("	fulldigits: " + fulldigits);
  console.log("	colortheme: " + colortheme);
  console.log("	themecode: " + themecode);
  console.log("	mirror: " + mirror);
  console.log("	splash: " + splash);
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
  } else {
    return false;
  }
  //return ((typeof Pebble.getActiveWatchInfo === "function") && Pebble.getActiveWatchInfo().platform!='aplite');
}

Pebble.addEventListener("ready", function() {
  readConfig();
  logVariables("Ready Event");
  var msg = '{';
  msg += '"dateorder":'+dateorder;
  msg += ',"weekday":'+weekday;
  msg += ',"battery":'+battery;
  msg += ',"bluetooth":'+bluetooth;
  msg += ',"invert":'+invert;
  msg += ',"lang":'+lang;
  msg += ',"stripes":'+stripes;
  msg += ',"roundcorners":'+roundcorners;
  msg += ',"fulldigits":'+fulldigits;
  msg += ',"colortheme":'+colortheme;
  msg += ',"themecode":"'+themecode+'"';
  msg += ',"mirror":'+mirror;
  msg += ',"splash":'+splash;
  msg += '}';
  console.log("Sending message to watch :");
  console.log(" " + msg);
	Pebble.sendAppMessage(JSON.parse(msg));
});

Pebble.addEventListener("showConfiguration", function(e) {
	logVariables("showConfiguration Event");
  var url = "http://www.famillemattern.com/jnm/pebble/Blockslide/Blockslide_3.17.html?dateorder=" + dateorder + "&weekday=" + weekday + "&battery=" + battery + "&bluetooth=" + bluetooth + "&invert=" + invert + "&lang=" + lang + "&stripes=" + stripes + "&roundcorners=" + roundcorners + "&fulldigits=" + fulldigits + "&colortheme=" + colortheme + "&themecode=" + themecode + "&colorCapable=" + isWatchColorCapable() + "&mirror=" + mirror + "&splash=" + splash;
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

	invert = configuration["invert"];
	localStorage.setItem("invert", invert);

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

  mirror = configuration["mirror"];
	localStorage.setItem("mirror", mirror);

  splash = configuration["splash"];
	localStorage.setItem("splash", splash);
});


