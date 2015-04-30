var dateorder;
var weekday;
var battery;
var bluetooth;
var lang;
var stripes;
var roundcorners;
var fulldigits;
var colortheme;
var bgcolor;
var fgcolor;

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
    colortheme = 0;
  }

  bgcolor = localStorage.getItem("bgcolor");
  if (!bgcolor) {
    bgcolor = "#000000";
  }

  fgcolor = localStorage.getItem("fgcolor");
  if (!fgcolor) {
    fgcolor = "#ffffff";
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
  console.log("	bgcolor: " + bgcolor);
  console.log("	fgcolor: " + fgcolor);
};

function isWatchColorCapable() {
  var colorCapable = 0;

  if(Pebble.getActiveWatchInfo) {
    var watch_name = Pebble.getActiveWatchInfo().model;

    if (watch_name.indexOf("pebble_time") >= 0) {
      colorCapable = 1;
    } else if (watch_name.indexOf("qemu_platform_basalt") >= 0) {
      colorCapable = 1;
    }
  }
  
  return colorCapable;
}

Pebble.addEventListener("ready", function() {
  logVariables("Ready Event");
  readConfig();
	Pebble.sendAppMessage(JSON.parse('{"dateorder":'+dateorder+',"weekday":'+weekday+',"battery":'+battery+',"bluetooth":'+bluetooth+',"lang":'+lang+',"stripes":'+stripes+',"roundcorners":'+roundcorners+',"fulldigits":'+fulldigits+',"colortheme":'+colortheme+',"bgcolor":"'+bgcolor+'","fgcolor":"'+fgcolor+'"}'));
});

Pebble.addEventListener("showConfiguration", function(e) {
	logVariables("showConfiguration Event");
  var url = "http://www.famillemattern.com/jnm/pebble/Blockslide/Blockslide.php?dateorder=" + dateorder + "&weekday=" + weekday + "&battery=" + battery + "&bluetooth=" + bluetooth + "&lang=" + lang + "&stripes=" + stripes + "&roundcorners=" + roundcorners + "&fulldigits=" + fulldigits + "&colortheme=" + colortheme + "&bgcolor=" + encodeURIComponent(bgcolor) + "&fgcolor=" + encodeURIComponent(fgcolor) + "&colorCapable=" + isWatchColorCapable();
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

  bgcolor = configuration["bgcolor"];
	localStorage.setItem("bgcolor", bgcolor);

  fgcolor = configuration["fgcolor"];
	localStorage.setItem("fgcolor", fgcolor);

});

