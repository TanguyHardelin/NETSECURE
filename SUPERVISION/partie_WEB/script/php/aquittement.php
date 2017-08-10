<?php 
	function init_BDD(){
		global $serverName,$user,$password,$dbName,$conn;
		//Connection to the BDD:
		$serverName='localhost';
		$user='root';
		$password='bba9e4d';	
		$dbName='lanestel';

		$conn = mysqli_connect($serverName, $user, $password, $dbName);		

		if (!$conn) {
			die("Connection failed: " . mysqli_connect_error());
		}
	}
	init_BDD();
	$id_client=$_GET["id"];
	$command="UPDATE lanestel_ping SET type='aquitte' where id_client=$id_client";
	$result=mysqli_query($conn,$command);
?>
