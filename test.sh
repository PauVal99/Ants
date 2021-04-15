#!/bin/bash

function game {
  `./Game $1 $2 $3 $4 -s $5 -i default.cnf -o default.res |& grep "player(s)" >> resultats`
}

make all
rm resultats

echo -n "Enter de first seed: "
read seedfirst
echo -n "Enter de last seed: "
read seedsecond
echo -n "Add Player1: "
read player1
echo -n "Add Player2: "
read player2
echo -n "Add Player3: "
read player3
echo -n "Add Player4: "
read player4

count=$seedfirst

while [ $count -ne $((seedsecond+1)) ]; do
  echo $count
  game $player1 $player2 $player3 $player4 $count 1 &
  game $player4 $player1 $player2 $player3 $count 2 &
  game $player3 $player4 $player1 $player2 $count 3 &
  game $player2 $player3 $player4 $player1 $count 4 &
  count=$((count+1))
  wait
done

sort resultats | uniq -c
rm resultats
