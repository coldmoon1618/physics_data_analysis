#!/usr/local/bin/perl

use strict;

#system("source /etc/csh.login");

#@list = system("ls /etc/profile.d/*.csh");


system("echo $ENV{LD_LIBRARY_PATH}");

my $itemNum = $ARGV[0];
print "$ARGV[0]\n";

my $RUNNUMBER = 13;

my $outputDir = "";
if($RUNNUMBER == 13){
    $outputDir = "/sphenix/user/yzhai/JetSubStructure/GenSimTree/outTree";
}
if($RUNNUMBER == 12){
    $outputDir = "/sphenix/user/yzhai/JetSubStructure/GenSimTree/run12_outTree";
}
#we don't actually need these as those are built into my genNewTree

if($itemNum == 0){ 
  system("root -l -b -q 'genNewTree.C+(5)'");
}
if($itemNum == 1){ 
  system("root -l -b -q 'genNewTree.C+(10)'");
}
if($itemNum == 2){
  system("root -l -b -q 'genNewTree.C+(20)'");
}
if($itemNum == 3){
  system("root -l -b -q 'genNewTree.C+(30)'");
}
if($itemNum == 4){
  system("root -l -b -q 'genNewTree.C+(50)'");
}
if($itemNum == 5){
  system("root -l -b -q 'genNewTree.C+(80)'");
}
if($itemNum == 6){
  system("root -l -b -q 'genNewTree.C+(100)'");
}
if($itemNum == 7){
  system("root -l -b -q 'genNewTree.C+(120)'");
}
if($itemNum == 8){
  system("root -l -b -q 'genNewTree.C+(135)'");
}
if($itemNum == 9){
  system("root -l -b -q 'genNewTree.C+(150)'");
}
if($itemNum == 10){
  system("root -l -b -q 'genNewTree.C+(170)'");
}
if($itemNum == 11){
  system("root -l -b -q 'genNewTree.C+(190)'");
}
if($itemNum == 12){
  system("root -l -b -q 'genNewTree.C+(210)'");
}
if($itemNum == 13){
  system("root -l -b -q 'genNewTree.C+(225)'");
}
if($itemNum == 14){
  system("root -l -b -q 'genNewTree.C+(240)'");
}
if($itemNum == 15){
  system("root -l -b -q 'genNewTree.C+(170,13,1)'");
}
if($itemNum == 16){
  system("root -l -b -q 'genNewTree.C+(190,13,1)'");
}
if($itemNum == 17){
  system("root -l -b -q 'genNewTree.C+(210,13,1)'");
}
if($itemNum == 18){
  system("root -l -b -q 'genNewTree.C+(225,13,1)'");
}
if($itemNum == 19){
  system("root -l -b -q 'genNewTree.C+(240,13,1)'");
}

