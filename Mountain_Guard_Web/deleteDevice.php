<?php
$device = $_POST['device'];

$servername = "mysql.serversfree.com";
$username = "u304318472_thoma";
$password = "azerty1234";
$dbname = "u304318472_gps";

try {
    $pdo = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	$sth = $pdo->prepare("DELETE FROM gps WHERE device = '" . $device . " ' LIMIT 1");
	$sth->execute();
	$locations = $sth->fetchAll(PDO::FETCH_ASSOC);
}
catch(PDOException $e) {
    echo "Error: " . $e->getMessage();
}

$pdo = null;

header('Location: http://www.iot.bugs3.com/');
?>