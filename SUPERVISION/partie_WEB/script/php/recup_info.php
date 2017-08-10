<?php
function init_BDD(){
	/*
		Fonction dont le role est de se connecter à la BDD
	*/
	global $serverName,$user,$password,$dbName,$conn;
	$serverName='localhost';
	$user='root';
	$password='bba9e4d';	
	$dbName='lanestel';

	$conn = mysqli_connect($serverName, $user, $password, $dbName);		

	if (!$conn) {
		die("Connection failed: " . mysqli_connect_error());
	}
}
function recup_param(){
	global $max_ping,$time_before_sms;
	$max_ping=30000;
	$time_before_sms=3*60000;
}
function enleve_espace($text){
	$final="";
	for($i=0;$i<strlen($text);$i++){
		if($text[$i]!=' ')
			$final.=$text[$i];
		else
			$final.='_';
	}
	return $final;
	
}
function swap(&$a,&$b){
	$tempo=$a;
	$a=$b;
	$b=$tempo;
}
function display_row($style,$nom,$ip,$ping,$tension,$courant,$temperature,$i,$class){
	//Vue classique sur navigateur:
	$style.="color: black;";
	echo('<div class="row row-bottom-margin '.$class.' vertical-center" style="'.$style.'">');
	echo('		<p class="col-xs-4 content-site center-block text-center"><b>'.$nom.'</b></p>');
 	echo('		<p class="col-xs-2 content-site center-block text-center">'.$ip.'</p>');
	echo('		<p class="col-xs-1 content-site center-block text-center">'.$ping.'ms</p>');
	if($tension!="NS"){
		echo('	<p class="col-xs-1 content-site center-block text-center">'.$tension.' V</p>');
	}
	else{
		echo('	<p class="col-xs-1 content-site center-block text-center">'.$tension.'</p>');
	}
	if($courant!="NS"){
		echo('	<p class="col-xs-1 content-site center-block text-center">'.$courant.' mA</p>');
	}
	else{
		echo('	<p class="col-xs-1 content-site center-block text-center">'.$courant.'</p>');
	}
	if($temperature!="NS"){
		echo('	<p class="col-xs-1 content-site center-block text-center">'.$temperature.' °C</p>');
	}
	else{
		echo('	<p class="col-xs-1 content-site center-block text-center">'.$temperature.'</p>');
	}
	if($type[$i]=='error'){
		echo('	<button class="col-xs-1" id="bouton_important_'.$i.'" >Aquittement</button>');
	}
	echo('		<img src="Images/image_graphique.png" id="button_'.$i.'" class="col-xs-2 align-self-center center-block" style="width:100px;height:50px;"/>');
	echo('</div>');


}

init_BDD();
recup_param();
$command='select id_client,nom,ip,ping,tension,courant,temperature,type from lanestel_ping natural join lanestel_informations';
$result = mysqli_query($conn, $command);

/*
	On regarde d'abord si ça ping ou pas
	Puis on les classent par ordre decroissant
*/

//Les differentes variables utilisee:

$ip=array();
$nom=array();
$ping=array();
$id_client=array();
$type=array();
$tension=array();
$courant=array();
$temperature=array();

if (mysqli_num_rows($result) > 0) {
	//On lit les donnees de la BDD et on rempli les variables avec celles-ci
	while($row = mysqli_fetch_assoc($result)) {
		//On verifie le ping
		$pingInt=intval(explode("ms",$row["ping"])[0]);

		array_push($ip,$row["ip"]);				
		array_push($nom,$row["nom"]);				
		array_push($ping,explode("ms",$row["ping"])[0]);				
		array_push($id_client,$row["id_client"]);
		array_push($type,$row['type']);
		array_push($tension,$row['tension']);
		array_push($courant,$row['courant']);
		array_push($temperature,$row['temperature']);
	}

	$index=array();
	$resultat=array();
	//On tri les donnees par ordre decroissant
	do{
		$permut=false;
		
		for($j=0;$j<(count($ping)-1);$j++){
			if(floatval($ping[$j])<floatval($ping[$j+1])){
				//On inverse les cases du tableau:
				swap($ping[$j],$ping[$j+1]);
				swap($nom[$j],$nom[$j+1]);
				swap($ip[$j],$ip[$j+1]);
				swap($id_client[$j],$id_client[$j+1]);
				swap($type[$j],$type[$j+1]);
				swap($tension[$j],$tension[$j+1]);
				swap($courant[$j],$courant[$j+1]);
				swap($temperature[$j],$temperature[$j+1]);

				$permut=true;

			}
		}
	}while($permut==true);
	
	//Construction de l'HTML
	$bascule_couleur=0;
	for($i=0;$i<count($ping);$i++){
		$pingInt=$ping[$i];
		$bascule_couleur=1-$bascule_couleur;
	
		if($pingInt<$max_ping){				
			$style="";
			if($bascule_couleur==1){
				$style="background-color:#95E860;";
				$style.="border: #95E860 10px solid;";
			 }
			else{
				$style="background-color:#9AE8BC;";
				$style.="border: #9AE8BC 10px solid;";
			}
			display_row($style,$nom[$i],$ip[$i],$ping[$i],$tension[$i],$courant[$i],$temperature[$i],$i,"ik_ok");
		}
		else{
			$style="";
			if($bascule_couleur==1){
				$style="background-color:#E84C41;";
				$style.="border: #E84C41 10px solid;";
			 }
			else{
				$style="background-color:#E8698C;";
				$style.="border: #E8698C 10px solid;";
			}
			display_row($style,$nom[$i],$ip[$i],$ping[$i],$tension[$i],$courant[$i],$temperature[$i],$i,"ik_ok");

		}
	}
	echo('<script type="text/javascript" >');
	for($i=0;$i<sizeof($id_client);$i++){
		echo('$(document).on("click", "#button_'.$i.'",function(){
			$("#all_ip").hide();
			$("#script_graph_ping").load("script/php/get_graph.php/?graph='.$id_client[$i].'&ip='.$ip[$i].'&nom='.enleve_espace($nom[$i]).'&display=global&div_graph=graphdiv_ping&legend=Date,ping&type=ping");	
			$("#script_graph_tension").load("script/php/get_graph.php/?graph='.$id_client[$i].'&ip='.$ip[$i].'&nom='.enleve_espace($nom[$i]).'&display=global&div_graph=graphdiv_tension&legend=Date,tension&type=tension");	
			$("#script_graph_courant").load("script/php/get_graph.php/?graph='.$id_client[$i].'&ip='.$ip[$i].'&nom='.enleve_espace($nom[$i]).'&display=global&div_graph=graphdiv_courant&legend=Date,courant&type=courant");	
			$("#script_graph_temperature").load("script/php/get_graph.php/?graph='.$id_client[$i].'&ip='.$ip[$i].'&nom='.enleve_espace($nom[$i]).'&display=global&div_graph=graphdiv_temperature&legend=Date,temperature&type=temperature");	
			});');
		echo('$(document).on("click", "#bouton_important_'.$i.'",function(){
			$.get("script/php/aquittement.php/?id='.$id_client[$i].'");
			});');
	}
	echo('</script>');
	}
 ?>
