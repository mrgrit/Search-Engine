<title>검색 엔진 *** SHYAN *** - 검색 결과</title>
<br>
<script language="javascript">
function clk() {
		document.find.search.focus();
}
</script>
<body background="images/bg.jpg" onLoad="clk();">
<table width=75% align=center border=0 cellspacing=0>
<tr><td align=center width=25%><img src="images/linux4.jpg"></td>
    <td align=center width=25%><img src="images/search2.jpg"></td>
    <td align=left width=25%><img src="images/shyan.gif"></td>
	<td><embed src='images/kb.swf' width='200' height='50'></td>
</tr>
</table>
<br><br>

<form  name='find' method='post' action='search.php'>
<table border=0 width=700 align=center cellspacing=0>

  <tr align=center><td align=180 bgcolor=lightskyblue>키워드를 입력하세요</td>
                   <td><input type='text' name='search' size=45 value='<? echo $search ?>'></td>
				   <td align=right><input type='submit' value='검색 하러 가기'></td>
  </tr>
</table>
</form>
<?
echo "<style>
       a {text-decoration:none}
       </style>";

    $conn=mysql_connect("localhost","root","search");
	mysql_select_db("project",$conn);


    $exp=explode(" ",$search);      //여기부터
    $cnt=0;

    $blank="";
    
    while(list($key,$val) = each($exp)) {
       	$string[$cnt]=$val;
        $blank=$blank.$string[$cnt];
        $cnt=$cnt+1;
    }

   $query="select  *  from  search  where  comment  like  '%$search%' or title like '%$search%'";

    for($i=0;$i<$cnt;$i++) {
        $str[$i]=" or comment like '%$string[$i]%' or title like '%$string[$i]%'";
        if($string[$i]=="" || $string[$i]==" ") {
            echo "<script>location.href='index.html';
                  </script>";
                  
        }
    }
    $query1=" ";

    $andstr=" ";

    for($i=0;$i<$cnt;$i++) {
      $string[$i]=$query1.$str[$i];
      $andstr=$andstr.$string[$i];
    }
      
      $and_blank=" or comment like '%$blank%' or title like '%$blank%'";
      $que= "$query" . "$andstr" . "$and_blank";  //여기까지 검색창에 입력되는 내용을 토큰별로 검색 조건으로 연결한다
    	
        $result=mysql_query("$que");
        $num=mysql_num_rows($result);

        if($num==0) {

            echo "<br>
                  <table width=700 align=center bgcolor=lightyellow cellspacing=0 border=1 cellpadding=5>
                    <tr><td align=center><font color=darkblue>검색한 결과가 없습니다</font></td></tr>
                  </table>
                  ";
        }

    if(!$page) {
        $page=1;
    }

    $disnum=7;
    $totalpage=ceil($num/$disnum);
    $startnum=($page-1)*$disnum;

    $lastnum=$startnum+$disnum-1;
    $no=$disnum*($page-1);



   for($i=$startnum;$i<=$lastnum && $i<$num;$i++) {
         mysql_data_seek($result,$i);
         $rows=mysql_fetch_array($result);
         $no=$no+1;
         echo "<br><table border='0' cellspacing=0 cellpadding=5 width=700 align=center>
                   <!-- <tr><td width=50 align=center>$rows[no]</td>-->
                     <tr><td width=50 align=center bgcolor='eeffcc'><b>$no</b></td>
                         <td width=250 bgcolor='eeffcc'><a href='http://$rows[url]' target='_blank'>$rows[url]</a></td>
                         <td width=300 bgcolor='eeffcc'>$rows[title]</td>
                     </tr>
                   </table><br>";
    }

    $dispg=10;
    $startpg=floor(($page-1)/$dispg)*$dispg+1;
    $lastpg=($startpg+$dispg)-1;

    $backpg=$page-1;
    $nextpg=$page+1;

    $backdbpg=$startpg-$dispg;
    $nextdbpg=$startpg+$dispg;

    echo "<p align=center>";
    if($startpg==1) {
        if($page != 1) {
            echo "&nbsp; <a href='$PHP_SELF?page=$backpg&search=$search'> <img src='images/leftarrow.gif' border=0 size='10' alt='한 페이지 앞으로'> </a> &nbsp;";
        }
    }else {
        echo "&nbsp; <a href='$PHP_SELF?page=$backdbpg&search=$search'>[이전 10개]</a> &nbsp;";
        echo "&nbsp; <a href='$PHP_SELF?page=$backpg&search=$search'> <img src='images/leftarrow.gif' border=0  size='10' alt='한 페이지 앞으로'> </a> &nbsp;";
    }

    for($i=$startpg;$i<=$lastpg && $i<=$totalpage;$i++) {
        if($page==$i) {
            echo "<font size=3 color=red>[$i]</font>";
        }else {
            echo "&nbsp; <a href='$PHP_SELF?page=$i&search=$search'>".$i."</a> &nbsp;";
        }
    }

    if($i<$dispg || $i == $totalpage+1) {
        if($page !=1 && $page !=$totalpage) {
            echo "&nbsp; <a href='$PHP_SELF?page=$nextpg&search=$search'> <img src='images/rightarrow.gif' border=0 size='10' alt='한 페이지 뒤로'> </a> &nbsp;";
        }else if($page==1 && $netxpg <= $lastpg && $totalpage>1) {
           echo "&nbsp; <a href='$PHP_SELF?page=$nextpg&search=$search'><img src='images/rightarrow.gif' border=0 size='10' alt='한 페이지 뒤로'> </a> &nbsp;";
        }
    }else {
        echo "&nbsp; <a href='$PHP_SELF?page=$nextpg&search=$search'> <img src='images/rightarrow.gif' border=0 size='10' alt='한 페이지 뒤로'> </a> &nbsp;";
        echo "&nbsp; <a href='$PHP_SELF?page=$nextdbpg&search=$search'>[다음 10개]</a> &nbsp;";
    }

   echo "</p>";
?>
