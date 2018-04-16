<?php
$mysqli = new mysqli("p:83.212.127.86", "MIS", "#MIS2018", "podatki");
if (mysqli_connect_errno()) {
    printf("Database connect failed: %s\n", mysqli_connect_error());
    exit();
}

if(isset($_GET["id"], $_GET["stamp"], $_GET["vol"])) {
    $idEscaped = $mysqli->real_escape_string($_GET["id"]);
    $stampEscaped = $mysqli->real_escape_string($_GET["stamp"]);
    $volEscaped = $mysqli->real_escape_string($_GET["vol"]);
    if (!$mysqli->query("INSERT into vsipodatki (id_naprave, timestamp, volumen) VALUES ('$idEscaped', '$stampEscaped', '$volEscaped')")) {
        printf("Error: %s\n", $mysqli->sqlstate);
    }
}

?>