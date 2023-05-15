<?php
function bsave($file,$dt) {
    $size=strlen($dt);
    printf("%s %d\n",$file,$size);
    file_put_contents($file,$dt);    
}


$img = imagecreatefrompng("e12.png");
$sr3 = "";
for($i=0;$i<16;$i++) {
    $c = imagecolorsforindex($img, $i % 4);
    $sr3 .= sprintf("%c%c",(int)($c["red"]/36)*16+(int)($c["blue"]/36),(int)($c["green"]/36));
}
for($i=0;$i<16;$i++) {
    $c = imagecolorsforindex($img, (int)($i / 4));
    $sr3 .= sprintf("%c%c",(int)($c["red"]/36)*16+(int)($c["blue"]/36),(int)($c["green"]/36));
}
for($k=0;$k<64;$k++) $sr3.=sprintf("%c",0);
//
//for($j=0;$j<8;$j++) for($k=0;$k<128;$k++) $sr3.=sprintf("%c",($k % 32)+$j*32);
    

//
for($n=0;$n<63;$n++) {
    if($n % 2 == 0) {
        $ny=$n*48*2;$ny2=$ny+48;
    } else {
        $ny2=$n*48*2;$ny=$ny2+48;
    }
    for($y=0;$y<48;$y+=8) {
        for($x=0;$x<64;$x+=2) {
            for($yy=0;$yy<8;$yy++) {
                $c1 = (imagecolorat($img,$x,$y+$yy+$ny)<<4)+imagecolorat($img,$x+1,$y+$yy+$ny);
                $c2 = (imagecolorat($img,$x,$y+$yy+$ny2)<<4)+imagecolorat($img,$x+1,$y+$yy+$ny2);
                $sr3.=sprintf("%c",$c1+$c2*4);
            }
        }
    }
}
bsave(sprintf("dsk/dat.sr3",$n),$sr3);
