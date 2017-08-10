<?php
function actualiser_element($id_element,$id_parent,$type){
	echo("$('#$id_element').remove();
		var reload_script = document.createElement(\"$type\");
		reload_script.id=\"$id_element\";
		$('$id_parent').append(reload_script);");
}
function print_debut($graph,$legende){
	echo("//On montre le graph:
	$('#display_graph').show();

	//On cache tout le reste:
	$('#all_ip').hide();
	$('nav').hide();
	$('#menu_header').hide();
	$('#haut_tableau_div').hide();
	g_$graph = new Dygraph(

	// containing div
	document.getElementById('$graph'),

	// CSV or path to a CSV file.
	'$legende\\n'+");
}
function construire_donnees($nom_fichier,$display){
	$myfile = fopen("$nom_fichier", "r") or die("Unable to open file!");
	$code="";
	$old_min="";
	$compteur=4;
	$mettre_dans_graph=false;

	if($display=="recherche"){
		//On passe les parametres en GET:
		$heure_debut=$_GET["heure_debut"];
		$heure_fin=$_GET["heure_fin"];
		$date_recherche=$_GET["date_recherche"];	

		//On split pour bien tout recuperer:
		$heure_debut=explode(":",$heure_debut)[0];
		$heure_fin=explode(":",$heure_fin)[0];
		$min_debut=explode(":",$heure_debut)[1];
		$min_fin=explode(":",$heure_fin)[1];
	}

	while(!feof($myfile)) {
		$command=fgets($myfile);
		$date_non_formate=explode(":",$command);
		//On verifie que on est pas à la fin du fichier:
		if(!feof($myfile)){
			$heure=explode(" ",$date_non_formate[0])[1];
			$min=$date_non_formate[1];
			$command=explode("\n",$command)[0];

			if($display=="global"){
				if($old_min!=$min){
					$old_min=$min;
					$compteur+=1;
				}
				if($compteur>=5){
					$code.="\"$command\\n\"FIN";
					$compteur=0;
				}
			}
			if($display=="recherche"){
				if(intval($heure)>intval($heure_debut) and intval($heure)<intval($heure_fin)){
					$code.="\"$command\\n\"FIN";
				}
				if(intval($heure)==intval($heure_debut) and intval($min)>=intval($min_debut)){
					$code.="\"$command\\n\"FIN";
				}
				if(intval($heure)==intval($heure_fin) and intval($min)<=intval($min_fin)){
					$code.="\"$command\\n\"FIN";
				}
			}
		}
	}
	fclose($myfile);
	$tmp=explode("FIN",$code);
	$code="";
	for($i=0;$i<(sizeof($tmp)-1);$i++){
		if($i<(sizeof($tmp)-2))
			$code.=$tmp[$i]."+";
		else
			$code.=$tmp[$i];
		
	}

	echo($code."\n");
}
function print_fin($title,$origine){
	echo(",
		{
			title:'$title',
			strokeWidth: 2,
			strokeWidth: 0.0,
			drawPoints: true,
			pointSize: 4,
			highlightCircleSize: 6,
			labels: [$origine]
      		});
	$('#revenir_all_ip').click(function(){
		location.reload();
	});");
}

function print_js($type){
	global $param,$legend,$graph,$ip,$nom,$display,$type,$date_recherche;
	if($display=="global"){
	echo("$(document).on(\"click\",\"#valider_date\", function(){
		$('#all_ip').hide();
		var heure_debut=$('#heure_debut').val()+':00';
		var heure_fin=$('#heure_fin').val()+':00';
		var date=$('#champ_date').val();
		$('#graphdiv').html('');

		$('#script_reload_graph').remove();
		var reload_script = document.createElement(\"script\");
		reload_script.id=\"script_reload_graph\";
		$('#all_script').append(reload_script);


		$('#script_graph').remove();
		var s = document.createElement(\"script\");
		s.id=\"script_graph\";
		$('#all_script').append(s);

		$('#script_reload_graph_$type').load('script/php/get_graph.php/?display=recherche&graph=$param&ip=$ip&nom=$nom&heure_debut='+encodeURIComponent(heure_debut)+'&heure_fin='+encodeURIComponent(heure_fin)+'&date_recherche='+date+'&div_graph=graphdiv_$type&legend=Date,$type&type=$type');
	});");
	}


	//On regarde si le client a deja ete en erreur:
	if(file_exists("/var/www/html/log/log_error/error_client_$param.txt")==True and $type=="ping"){
		actualiser_element("error","body","div");

		$myfile = fopen("/var/www/html/log/log_error/error_client_$param.txt", "r") or die("Unable to open file!");

		$compteur=0;
		while(!feof($myfile)) {
			$text=fgets($myfile);

			$date_non_formate=explode(":",$text);
			//On verifie que on est pas à la fin du fichier:
			if(!feof($myfile)){
				$heure=explode(" ",$date_non_formate[0])[1];
				$min=$date_non_formate[1];

				$temp=explode(" ",$date_non_formate[0])[0];
				$date=explode("/",$temp)[0].".".explode("/",$temp)[1].".".explode("/",$temp)[2];

				echo("

				$('#error').append($('<button class=\"bouton_error\" id=\"$date"."_$heure"."_$min\" onclick=\"fonction_$heure"."_$min()\"  >erreur le $date à $heure : $min </button>'));	
				function fonction_$heure"."_$min(){
					");
				
				echo("
					$('#script_reload_graph').remove();
					var reload_script = document.createElement(\"script\");
					reload_script.id=\"script_reload_graph\";
					$('#all_script').append(reload_script);


					$('#script_graph').remove();
					var s = document.createElement(\"script\");
					s.id=\"script_graph\";
					$('#all_script').append(s);

	$('#script_reload_graph_ping').load('script/php/get_graph.php/?display=recherche&graph=$param&ip=$ip&nom=$nom&heure_debut=".strval(intval($heure)-1)."&heure_fin=".strval(intval($heure)+1)."&date_recherche=$date&div_graph=graphdiv_ping&legend=Date,ping&type=ping');
	$('#script_reload_graph_courant').load('script/php/get_graph.php/?display=recherche&graph=$param&ip=$ip&nom=$nom&heure_debut=".strval(intval($heure)-1)."&heure_fin=".strval(intval($heure)+1)."&date_recherche=$date&div_graph=graphdiv_courant&legend=Date,courant&type=courant');
	$('#script_reload_graph_tension').load('script/php/get_graph.php/?display=recherche&graph=$param&ip=$ip&nom=$nom&heure_debut=".strval(intval($heure)-1)."&heure_fin=".strval(intval($heure)+1)."&date_recherche=$date&div_graph=graphdiv_tension&legend=Date,tension&type=tension');

				}
				");
			}
		}


	}
}

	//Recuperation des parametres principaux:
	$param=$_GET['graph'];
	$legend=$_GET["legend"];
	$graph=$_GET["div_graph"];
	$ip=$_GET["ip"];
	$nom=$_GET["nom"];
	$display=$_GET["display"];
	$type=$_GET["type"];
	$date_recherche=date("Y.m.d");

	global $param,$legend,$graph,$ip,$nom,$display,$type,$date_recherche;
	print_debut($graph,$legend);

	if($display=="recherche"){
		$date_recherche=$_GET["date_recherche"];	
	}

	if($type=="ping"){
		construire_donnees("/var/www/html/log/log_ping/".$date_recherche."_ping_client_$param.txt",$display);
	}
	else if($type=="tension" or $type=="courant" or $type=="temperature"){
		construire_donnees("/var/www/html/log/log_$type/$date_recherche"."_"."$type"."_".$ip.".txt",$display);
	}
	print_fin("Graph de $type IP: $ip nom: $nom","'date','$type'");
	print_js($type);


?>
