<?php

$servername = "mysql.serversfree.com";
$username = "u304318472_thoma";
$password = "azerty1234";
$dbname = "u304318472_gps";

try {
    $pdo = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    $sth = $pdo->prepare("SELECT id, latitude, longitude, altitude, time, device FROM gps ORDER BY id ASC");
    $sth->execute();
    $locations = $sth->fetchAll(PDO::FETCH_ASSOC);
}
catch(PDOException $e) {
    echo "Error: " . $e->getMessage();
}

$pdo = null;

foreach($locations as $location){ 
  $ts = (!empty($location['time'])) ? $location['time'] : 0;
  $date = new DateTime("@$ts");
  $date->setTimezone(new DateTimeZone('Europe/Paris'));	

  echo "<tr>
    <td>" . $location['latitude'] . "</td>
    <td>" . $location['longitude'] . "</td>
    <td>" . $location['altitude'] . "</td>
    <td>" . $date->format('d/m/Y H:i:s') . "</td>
    <td>" . $location['device'] . "</td>
  </tr>";
}

?>