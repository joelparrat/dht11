
<!DOCTYPE html>

<html>

    <head>
        <title>Projet DHT11</title>        
        <meta charset="UTF-8">
        <meta http-equiv="refresh" content="10">
        <link rel="stylesheet" type="text/css" href="../css/style.css">
    </head>

    <body>
		<div>
			<h1>Température</h1> 
		</div>
		<p>
		<?php
			$fch = "../txt/objet.json";
			$txt = file_get_contents($fch);
			//$fch = fopen("../txt/objet.json", "r");
			//$txt = fgets($fch);
			$objJSON = json_decode($txt);
			echo  "Il fait ".$objJSON->temperature."°C avec ".$objJSON->humidite."% d'humidite.<br>";
			//fclose($fch);
			if (file_exists($fch))
    			echo "Le ".date("d/m/Y \à H:i:s.", filemtime($fch));
			$baragraph_height = 161 + $objJSON->temperature * 4;
			$baragraph_top = 315 - $objJSON->temperature * 4;
		?>
		</p>
		<div id="thermometer">
			<div id="bargraph" style="height:<?php echo $baragraph_height; ?>px; top:<?php echo $baragraph_top; ?>px;">
			</div>
		</div>
		
		<?php include("../php/fonction.php"); ?>
		<script src="../js/script.js"></script>
    </body>

</html>
