<?php
 
/*
 * Following code will get single row details
 * A row is identified by id
 */
 

// array for JSON response
$response = array();
 
// include db connect class
require_once __DIR__ . '/db_config.php';
 
// connecting to db
$db = new PDO('mysql:host=localhost;dbname=postman;charset=utf8mb4', DB_USER ,DB_PASSWORD);

 
// check for post data
if (isset($_GET["id"])) {
    $id = $_GET['id'];

    $result_rcv = $db->query("SELECT *FROM transport WHERE receiver = $id and state = 3");
    
    $result_send = $db->query("SELECT *FROM transport WHERE sender = $id and state = 1");
    
    if (!empty($result_rcv)) {
        if ($result_rcv->rowCount() > 0) {
            $response["data"] = array();
            while ($row = $result_rcv->fetch(PDO::FETCH_ASSOC)) {
 
                $data = array();
                $data["id"] = $row["_id"];
                $data["requireTime"] = $row["requireTime"];
                $data["sender"] = $row["sender"];
                $data["receiver"]  = $row["receiver"];
                $data["des_id"]  = $row["des_id"];
                $data["car_id"] = $row["car_id"];
                $data["state"] = $row["state"];
                $data["key"] = $row["packetKey"]; 
                array_push($response["data"], $data);
            }
            $response["success"] = 1;
            echo json_encode($response);
        }else{
            $response["success"] = 0;
            $response["message"] = "result_rcv row count = 0";
            // echo json_encode($response);
        }
  
    }
    if(!empty($result_send)){
            if ($result_send->rowCount() > 0) {
                $response["data"] = array();
                while ($row = $result_send->fetch(PDO::FETCH_ASSOC)) {
 
                    $data = array();
                    $data["id"] = $row["_id"];
                    $data["requireTime"] = $row["requireTime"];
                    $data["sender"] = $row["sender"];
                    $data["receiver"]  = $row["receiver"];
                    $data["des_id"]  = $row["des_id"];
                    $data["car_id"] = $row["car_id"];
                    $data["state"] = $row["state"];
                    $data["key"] = $row["packetKey"]; 
                    array_push($response["data"], $data);
                }
                $response["success"] = 1;
                echo json_encode($response);
            }else{
                $response["success"] = 0;
                $response["message"] = "result_send row count = 0 ";
                echo json_encode($response);
            }
        }
        else{
            // no product found
            $response["success"] = 0;
            $response["message"] = "result_send empty";
 
            // echo no users JSON
            echo json_encode($response);
        }
        
} else {
    // required field is missing
    $response["success"] = 0;
    $response["message"] = "Required field(s) is missing";
 
    // echoing JSON response
    echo json_encode($response);
}

?>