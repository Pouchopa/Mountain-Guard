<?php

$servername = "mysql.serversfree.com";
$username = "u304318472_thoma";
$password = "azerty1234";
$dbname = "u304318472_gps";

try {
    $pdo = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	$sth = $pdo->prepare("SELECT latitude, longitude FROM gps");
	$sth->execute();
	$locations = $sth->fetchAll(PDO::FETCH_ASSOC);
}
catch(PDOException $e) {
    echo "Error: " . $e->getMessage();
}

$pdo = null;

//Encode the $locations array in JSON format and print it out.
header('Content-Type: application/json');
echo json_encode($locations);

?>