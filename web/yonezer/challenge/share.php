<html>
<head>
	<meta charset="UTF-8">
	<link rel="stylesheet" href="style.css">
</head>
<body>

<?php

function html($string) {
    return htmlspecialchars($string);
}

$flag = file_get_contents("../flag.txt");

class secret{

	public function data(){
		global $flag;
		echo($flag);
	}

}



class share_video{
	public $text="Hello Everone";

	public function data(){
		echo("<h1>" . html($this->text) . "</h1><br>");
		echo("<MARQUEE><h1>Do you like this video &#x1f440;?</h1></MARQUEE>\n");
		$urls = ["https://www.youtube.com/embed/s582L3gujnw","https://www.youtube.com/embed/gJX2iy6nhHc", "https://www.youtube.com/embed/SX_ViT4Ra7k","https://www.youtube.com/embed/Zw_FKq10S8M"];
		$num = rand(0,3);
		$url = $urls[$num];
		echo ("<div id=\"all\"><iframe width=\"1000\" height=\"600\" src=\"". $url . "\"></iframe></div>");

		}
	}


$serialized = @$_GET["data"];
$hoge = @unserialize($serialized);
if($hoge){
	$hoge->data();

}
?>
</body>
</html>



