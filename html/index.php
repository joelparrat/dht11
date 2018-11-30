
<!DOCTYPE html>

<html>

    <head>
        <title>Projet DHT11</title>        
        <meta charset="UTF-8">
        <link rel="stylesheet" type="text/css" href="../css/style.css">
    </head>

    <body>
		<div>
			<h1>Thermometre</h1> 
		</div>
		<p>
		<?php
			$txt = file_get_contents("../txt/objet.json");
			//$fch = fopen("../txt/objet.json", "r");
			//$txt = fgets($fch);
			$objJSON = json_decode($txt);
			echo  "Il fait ".$objJSON->temperature."°C avec ".$objJSON->humidite."% d'humidite.";
			//fclose($fch);
			//echo date();
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
