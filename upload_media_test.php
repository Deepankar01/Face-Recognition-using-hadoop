<?php

$s='fail';
$target_path1 = "uploads/";
$target_path1 = $target_path1 . basename( $_FILES['uploaded_file']['name']);
if(move_uploaded_file($_FILES['uploaded_file']['tmp_name'], $target_path1)) {
    $s='success';
} else{
    $s='fail';
}
$headers = apache_request_headers();
$name=$headers['username'];
$q=$headers['quality'];


class Person {
    public $status = "";
    public $name = "";
    public $quality  = "";
}
$e = new Person();
$e->status =$s;

$command = "python map2.py";
error_reporting(E_ALL);
exec($command,$output);
$data = explode(",", $output[0]);

$e->name = $data[0];
$e->quality  = $data[1];

echo json_encode($e);

?>
