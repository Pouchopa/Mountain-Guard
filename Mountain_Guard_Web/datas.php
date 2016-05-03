<?php
$servername = "mysql.serversfree.com";
$username = "u304318472_thoma";
$password = "azerty1234";
$dbname = "u304318472_gps";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "INSERT INTO gps (latitude, longitude, altitude, time, device) VALUES ('".$_REQUEST['slot_lat']."','".$_REQUEST['slot_lon']."','".$_REQUEST['slot_alt']."','".$_REQUEST['time']."','".$_REQUEST['device']."')";

if ($conn->query($sql) === TRUE) {
    echo "New record created successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();