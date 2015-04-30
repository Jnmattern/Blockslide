<!DOCTYPE html>
<html>
        <head>
                <title>BlockSlide-Date</title>
                <meta charset='utf-8'>
                <meta name='viewport' content='width=device-width, initial-scale=1'>
                <link rel='stylesheet' href='../jquery.mobile-1.3.2.min.css' />
                <script src='../jquery-1.9.1.min.js'></script>
                <script src='../jquery.mobile-1.3.2.min.js'></script>

                <link rel="stylesheet" type="text/css" href="spectrum.css">
                <script type='text/javascript' src="spectrum.js"></script>

                <style>
                        .ui-header .ui-title { margin-left: 1em; margin-right: 1em; text-overflow: clip; }
                </style>
        </head>
		<body>
<div data-role="page" id="page1">
    <div data-theme="a" data-role="header" data-position="fixed">
        <h3>
            BlockSlide Configuration
        </h3>
        <div class="ui-grid-a">
            <div class="ui-block-a">
                <input id="cancel" type="submit" data-theme="c" data-icon="delete" data-iconpos="left"
                value="Cancel" data-mini="true">
            </div>
            <div class="ui-block-b">
                <input id="save" type="submit" data-theme="b" data-icon="check" data-iconpos="right"
                value="Save" data-mini="true">
            </div>
        </div>
    </div>
    <div data-role="content">

    <div id="dateorder" data-role="fieldcontain" class="ui-bar">
			<fieldset data-role="controlgroup" data-type="horizontal" data-mini="true">
        <legend>Date format</legend>
<?php
	if (!isset($_GET['dateorder'])) {
		$dateorder = 1;
	} else {
		$dateorder = $_GET['dateorder'];
	}
	
	if ($dateorder == 0) {
		$s1 = " checked";
		$s2 = "";
	} else {
		$s1 = "";
		$s2 = " checked";
	}
	
	echo '<input id="format1" name="dateorder" value="0" data-theme="" type="radio"' . $s1 . '><label for="format1">dd mm</label>';
	echo '<input id="format2" name="dateorder" value="1" data-theme="" type="radio"' . $s2 . '><label for="format2">mm dd</label>';
?>
			</fieldset>
		</div>

    <div data-role="fieldcontain" class="ui-bar">
          <fieldset data-role="controlgroup" data-type="horizontal" data-mini="true">
            <legend>Show day of week</legend>
<?php
	if (!isset($_GET['weekday'])) {
		$weekday = 0;
	} else {
		$weekday = $_GET['weekday'];
	}
	
	if ($weekday == 0) {
		$s1 = " checked";
		$s2 = "";
	} else {
		$s1 = "";
		$s2 = " checked";
	}

	echo '<input id="weekday1" name="weekday" value="0" data-theme="" type="radio"' . $s1 . '><label for="weekday1">Off</label>';
	echo '<input id="weekday2" name="weekday" value="1" data-theme="" type="radio"' . $s2 . '><label for="weekday2">On</label>';
?>
        </fieldset>
        </div>

    <div id="battery" data-role="fieldcontain" class="ui-bar">
			<fieldset data-role="controlgroup" data-type="horizontal" data-mini="true">
        <legend>Tap to show battery</legend>
<?php
	if (!isset($_GET['battery'])) {
		$battery = 1;
	} else {
		$battery = $_GET['battery'];
	}
	
	if ($battery == 0) {
		$s1 = " checked";
		$s2 = "";
	} else {
		$s1 = "";
		$s2 = " checked";
	}
	
	echo '<input id="battery1" name="battery" value="0" data-theme="" type="radio"' . $s1 . '><label for="battery1">Off</label>';
	echo '<input id="battery2" name="battery" value="1" data-theme="" type="radio"' . $s2 . '><label for="battery2">On</label>';
?>
			</fieldset>
		</div>

    <div id="bluetooth" data-role="fieldcontain" class="ui-bar">
      <fieldset data-role="controlgroup" data-type="horizontal" data-mini="true">
        <legend>Vibrate on BT disconnect</legend>
<?php
	if (!isset($_GET['bluetooth'])) {
		$bluetooth = 1;
	} else {
		$bluetooth = $_GET['bluetooth'];
	}
	
	if ($bluetooth == 0) {
		$s1 = " checked";
		$s2 = "";
	} else {
		$s1 = "";
		$s2 = " checked";
	}

	echo '<input id="bluetooth1" name="bluetooth" value="0" data-theme="" type="radio"' . $s1 . '><label for="bluetooth1">Off</label>';
	echo '<input id="bluetooth2" name="bluetooth" value="1" data-theme="" type="radio"' . $s2 . '><label for="bluetooth2">On</label>';
?>
        </fieldset>
        </div>


		<div data-role="fieldcontain">
			<label for="lang">
				Language
			</label>
			<select id="lang" data-native-menu="true" name="lang">
<?php
	$langs = array(
		0 => 'Dutch',
		1 => 'English',
		2 => 'French',
		3 => 'German',
		5 => 'Italian',
		4 => 'Spanish'
	);
	
	if (!isset($_GET['lang'])) {
		$lang = 1;
	} else {
		$lang = $_GET['lang'];
	}
	
	foreach ($langs as $v => $n) {
		if ($lang == $v) {
			$s = " selected";
		} else {
			$s = "";
		}
		echo '<option value="' . $v . '" ' . $s . '>' . $n . '</option>';
	}
	?>
			</select>
		</div>

      <div id="stripes" data-role="fieldcontain">
        <fieldset data-role="controlgroup" data-type="horizontal">
          <legend>Stripes</legend>

<?php
	if (!isset($_GET['stripes'])) {
		$stripes = 1;
	} else {
		$stripes = $_GET['stripes'];
	}

	$s = array( "", "");
	$s[$stripes] = " checked";

	echo '<input id="stripes1" name="stripes" value="0" data-theme="" type="radio"' . $s[0] . '><label for="stripes1"><img src="digitsPlain.png" width="72" height="72"><br/>Plain</label>';
	echo '<input id="stripes2" name="stripes" value="1" data-theme="" type="radio"' . $s[1] . '><label for="stripes2"><img src="digits1.png" width="72" height="72"><br/>Striped</label>';
?>
        </fieldset>
      </div>

      <div id="roundcorners" data-role="fieldcontain">
        <fieldset data-role="controlgroup" data-type="horizontal">
          <legend>Corners</legend>

<?php
	if (!isset($_GET['roundcorners'])) {
		$roundcorners = 1;
	} else {
		$roundcorners = $_GET['roundcorners'];
	}

	$s = array( "", "");
	$s[$roundcorners] = " checked";

	echo '<input id="corners1" name="roundcorners" value="0" data-theme="" type="radio"' . $s[0] . '><label for="corners1"><img src="digitsFlatCorners.png" width="72" height="72"><br/>Flat</label>';
	echo '<input id="corners2" name="roundcorners" value="1" data-theme="" type="radio"' . $s[1] . '><label for="corners2"><img src="digitsRoundCorners.png" width="72" height="72"><br/>Round</label>';
?>
        </fieldset>
      </div>


      <div id="fulldigits" data-role="fieldcontain">
        <fieldset data-role="controlgroup" data-type="horizontal">
          <legend>Digits Shape</legend>

<?php
	if (!isset($_GET['fulldigits'])) {
		$fulldigits = 0;
	} else {
		$fulldigits = $_GET['fulldigits'];
	}

	$s = array( "", "");
	$s[$fulldigits] = " checked";

	echo '<input id="format1" name="fulldigits" value="0" data-theme="" type="radio"' . $s[0] . '><label for="format1"><img src="digits1.png" width="72" height="72"><br/>Original</label>';
	echo '<input id="format2" name="fulldigits" value="1" data-theme="" type="radio"' . $s[1] . '><label for="format2"><img src="digits2.png" width="72" height="72"><br/>Regular</label>';
?>
        </fieldset>
      </div>

      <div id="colortheme" data-role="fieldcontain">
        <fieldset data-role="controlgroup" data-type="horizontal">
          <legend>Color Theme (Color Pebble only)</legend>

<?php
	if (!isset($_GET['colortheme'])) {
		$colortheme = 0;
	} else {
		$colortheme = $_GET['colortheme'];
	}

  for ($i=0; $i<5; $i++) {
    $s = "";
    if ($i == $colortheme) {
      $s = " checked";
    }
    echo '<input id="colortheme' . $i . '" name="colortheme" value="' . $i . '" data-theme="" type="radio"' . $s . '><label for="colortheme' . $i . '"><img src="color_theme_' . $i . '.png" width="68" height="66"><br/></label>';
  }
?>
        </fieldset>
      </div>

      <div id="colorpickerdiv" data-role="fieldcontain">
        <fieldset data-role="controlgroup">
          <legend>Background Color (Color Pebble only)</legend>
            <input id="bgcolorpicker" name="bgcolorpicker">
        </fieldset>
        <fieldset data-role="controlgroup">
          <legend>Foreground Color (Color Pebble only)</legend>
            <input id="fgcolorpicker" name="fgcolorpicker">
        </fieldset>
      </div>


</div>


  <script>
<?php
  if (isset($_GET['return_to'])) {
    echo "var closeURL = \"" . $_GET['return_to'] . "\";\n";
  } else {
    echo "var closeURL = \"pebblejs://close#\";\n";
  }

  if (isset($_GET['bgcolor'])) {
    echo "var curBGColor = \"" . $_GET['bgcolor'] . "\";\n";
  } else {
    echo "var curBGColor = \"#000000\";\n";
  }

  if (isset($_GET['fgcolor'])) {
    echo "var curFGColor = \"" . $_GET['fgcolor'] . "\";\n";
  } else {
    echo "var curFGColor = \"#ffffff\";\n";
  }
?>

      function setBGColor(color) {
        curBGColor = color.toHexString();
      }

      function setFGColor(color) {
        curFGColor = color.toHexString();
      }

      function saveOptions() {
        var options = {
          'dateorder': parseInt($("input[name=dateorder]:checked").val(), 10),
          'weekday': parseInt($("input[name=weekday]:checked").val(), 10),
          'battery': parseInt($("input[name=battery]:checked").val(), 10),
          'bluetooth': parseInt($("input[name=bluetooth]:checked").val(), 10),
          'lang': parseInt($("#lang").val(), 10),
          'stripes': parseInt($("input[name=stripes]:checked").val(), 10),
          'roundcorners': parseInt($("input[name=roundcorners]:checked").val(), 10),
          'fulldigits': parseInt($("input[name=fulldigits]:checked").val(), 10),
          'colortheme': parseInt($("input[name=colortheme]:checked").val(), 10),
          'bgcolor': curBGColor,
          'fgcolor': curFGColor
        }
        return options;
      }

      $().ready(function() {

        $("#bgcolorpicker").spectrum({
<?php
  if (!isset($_GET['bgcolor'])) {
    echo "              color: \"#000\",\n";
  } else {
    echo "              color: \"" . $_GET['bgcolor'] . "\",\n" ;
  }
?>
            showPaletteOnly: true,
            hideAfterPaletteSelect:true,
            preferredFormat: "hex3",
            change: function(color) {
              setBGColor(color);
            },
            palette: [
              [ '000', '005', '00a', '00f', '050', '055', '05a', '05f' ],
              [ '500', '505', '50a', '50f', '550', '555', '55a', '55f' ],
              [ 'a00', 'a05', 'a0a', 'a0f', 'a50', 'a55', 'a5a', 'a5f' ],
              [ 'f00', 'f05', 'f0a', 'f0f', 'f50', 'f55', 'f5a', 'f5f' ],
              [ '0a0', '0a5', '0aa', '0af', '0f0', '0f5', '0fa', '0ff' ],
              [ '5a0', '5a5', '5aa', '5af', '5f0', '5f5', '5fa', '5ff' ],
              [ 'aa0', 'aa5', 'aaa', 'aaf', 'af0', 'af5', 'afa', 'aff' ],
              [ 'fa0', 'fa5', 'faa', 'faf', 'ff0', 'ff5', 'ffa', 'fff' ]
            ]
        });

        $("#fgcolorpicker").spectrum({
<?php
  if (!isset($_GET['fgcolor'])) {
    echo "              color: \"#000\",\n";
  } else {
    echo "              color: \"" . $_GET['fgcolor'] . "\",\n" ;
  }
?>
            showPaletteOnly: true,
            hideAfterPaletteSelect:true,
            preferredFormat: "hex3",
            change: function(color) {
              setFGColor(color);
            },
            palette: [
              [ '000', '005', '00a', '00f', '050', '055', '05a', '05f' ],
              [ '500', '505', '50a', '50f', '550', '555', '55a', '55f' ],
              [ 'a00', 'a05', 'a0a', 'a0f', 'a50', 'a55', 'a5a', 'a5f' ],
              [ 'f00', 'f05', 'f0a', 'f0f', 'f50', 'f55', 'f5a', 'f5f' ],
              [ '0a0', '0a5', '0aa', '0af', '0f0', '0f5', '0fa', '0ff' ],
              [ '5a0', '5a5', '5aa', '5af', '5f0', '5f5', '5fa', '5ff' ],
              [ 'aa0', 'aa5', 'aaa', 'aaf', 'af0', 'af5', 'afa', 'aff' ],
              [ 'fa0', 'fa5', 'faa', 'faf', 'ff0', 'ff5', 'ffa', 'fff' ]
            ]
        });

        $("#cancel").click(function() {
          console.log("Cancel");
          document.location = closeURL;
        });

        $("#save").click(function() {
          console.log("Submit");
          
          var location = closeURL + encodeURIComponent(JSON.stringify(saveOptions()));
          console.log("Close: " + location);
          console.log(location);
          document.location = location;
        });

        $('.sp-replacer').unwrap();
      });
    </script>
</body>
</html>
