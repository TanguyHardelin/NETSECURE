<!DOCTYPE HTML>
<html>
	<head>
		<meta charset="utf-8" />
		<link rel="stylesheet" href="style.css" />
		<link rel="shortcut icon" href="Images/lanestel.png">

 		<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">

		<!--BootStrap-->
		<!-- Latest compiled and minified CSS -->
		<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" integrity="sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u" crossorigin="anonymous">

		<!-- Optional theme -->
		<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap-theme.min.css" integrity="sha384-rHyoN1iRsVXV4nD0JutlnGaslCJuC7uwjduW9SVrLvRYooPp2bWYgmgJQIXwl/Sp" crossorigin="anonymous">

		<!-- Latest compiled and minified JavaScript -->
		<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js" integrity="sha384-Tc5IQib027qvyjSMfHjOMaLkfuWVxZxUPnCJA7l2mCWNIpG9mGCD8wGNIcPD7Txa" crossorigin="anonymous"></script>

		<!--Dygraph -->
		<script src="//cdnjs.cloudflare.com/ajax/libs/dygraph/2.0.0/dygraph.min.js"></script>
		<link rel="stylesheet" src="//cdnjs.cloudflare.com/ajax/libs/dygraph/2.0.0/dygraph.min.css" />

		<!--JQuerry-->
		<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
		<script type="text/javascript" src="script/js/jquery.canvasjs.min.js"></script>

		<!--TimerPicker-->
		<link rel="stylesheet" href="script/js/Timepicker/dist/css/timepicker.css">
		<script src="script/js/Timepicker/dist/js/timepicker.js"></script>

		<!-- Include pickadate -->
		<link rel="stylesheet" href="//cdnjs.cloudflare.com/ajax/libs/pickadate.js/3.5.3/themes/default.css">
		<link rel="stylesheet" href="//cdnjs.cloudflare.com/ajax/libs/pickadate.js/3.5.3/themes/default.date.css">
		<link rel="stylesheet" href="//cdnjs.cloudflare.com/ajax/libs/pickadate.js/3.5.3/themes/default.time.css">

		<script src="//cdnjs.cloudflare.com/ajax/libs/pickadate.js/3.5.3/picker.js"></script>
		<script src="//cdnjs.cloudflare.com/ajax/libs/pickadate.js/3.5.3/picker.date.js"></script>
		<script src="//cdnjs.cloudflare.com/ajax/libs/pickadate.js/3.5.3/picker.time.js"></script>
		<script src="//cdnjs.cloudflare.com/ajax/libs/pickadate.js/3.5.3/legacy.js"></script>


		<title>Lanestel supervison</title>

	</head>
	<body>
		<header>
			<div class="haut_header">
				<img id="logo" src="Images/lanestel.png" class="haut_header"/>
				<h2 class="haut_header">Supervision Lanestel</h2>
			</div>
		</header>
		
		<!--		      -->
		<!--Légende du tableau-->
		<!--		      -->

		<div class="container-fluid hidden-xs ip_ok">
			<p class="col-xs-4 col-lg-4 content-site center-block text-center"><b>Nom</b></p>
			<p class="col-xs-2 col-lg-2 content-site center-block text-center"><b>Ip</b></p>
			<p class="col-xs-1 col-lg-1 content-site center-block text-center"><b>Valeur du ping</b></p>
			<p class="col-xs-1 col-lg-1 content-site center-block text-center"><b>Tension (V)</b></p>
			<p class="col-xs-1 col-lg-1 content-site center-block text-center"><b>Courant (mA)</b></p>
			<p class="col-xs-1 col-lg-1 content-site center-block text-center"><b>Température (°C)</b></p>
			<p class="col-xs-2 col-lg-2 content-site center-block text-center"><b>Graphique</b></p>
			

		</div>

		<!--		      	   -->
		<!-- Le tableau en lui meme-->
		<!--		      	   -->

		<div class="container-fluid" id ="all_ip">
			<!--Div qui est actualisée toute les x secondes pour montrer l'evolution du ping-->
		</div>



		<!-- La partie d'observation pour une seule ip -->
		<div id="display_graph">
			
			<div id="param_select_graph">
				<Button id="revenir_all_ip" class="param_graph">Revenir à toute les IP</Button>
				<p id="heure_debut_p" class="param_graph"><input type="text" id="heure_debut" class="param_graph" value=" " name="heure_debut"/> H</p>
				<p id="heure_fin_p" class="param_graph"><input type="text" id="heure_fin" class="param_graph" value=" " name="heure_debut"/>  H</p>
				<p id="champ_date_p" class="param_graph">Jour : <input type="text" value=" " name="date" id="champ_date" size="12" maxlength="10" class="param_graph"></p>
				<Button id="valider_date"class="param_graph">Faire la recherche</Button>
			</div>
			<script type="text/javascript">
				$("#champ_date").pickadate({
					format: 'yyyy.mm.dd',
					formatSubmit: 'yyyy.mm.dd',
					hiddenName: true
				 });
    
			</script>



				
			<!-- div contenant le graph -->
			<div id="graphdiv_ping" class="div_graph"></div>
			<div id="graphdiv_tension" class="div_graph"></div>
			<div id="graphdiv_courant" class="div_graph"></div>
			<div id="graphdiv_temperature" class="div_graph"></div>

			<div id="error" class="div_graph"></div>

		</div>


		<div id=all_script>
			<script type="text/javascript" src="script/js/script_actualisation.js"></script>
			<script type="text/javascript" src="script/js/script_page.js"></script>

			<script type="text/javascript" id="script_graph_ping"></script>
			<script type="text/javascript" id="script_graph_tension"></script>
			<script type="text/javascript" id="script_graph_courant"></script>
			<script type="text/javascript" id="script_graph_temperature"></script>

			<script type="text/javascript" id="script_reload_graph_ping"></script>
			<script type="text/javascript" id="script_reload_graph_tension"></script>
			<script type="text/javascript" id="script_reload_graph_courant"></script>
			<script type="text/javascript" id="script_reload_graph_temperature"></script>

			<script id="bouton_error"></script>
		</div>

		
	    	<!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->
	   	<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js"></script>
	  	<!-- Include all compiled plugins (below), or include individual files as needed -->
	    	<script src="js/bootstrap.min.js"></script>

	</body>


</html>
