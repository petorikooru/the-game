# The Game 
You just lost the game!

Welcome to Game Center, where you can play varieties of games. Do you love gambling? I do!

To compile, just type this in the terminal
```
gcc project.c -o project.bin
```
If you want to optimize it even further, you can type this (idk why you would want to optimize this small (and unimportant) program)
```
gcc project.c -o project.bin -O3
```
Then run with 
```
./project.bin
```

There are both regular user and admin, in which an admin has these features :
1. View every user registered
2. Delete user
3. Ban and Unban user
4. Cheat

Basically there are 3 games, which are : 
1. Slot Machine
2. Coin Toss
3. Random Number Guesser

Additional options :
1. Market (to buy credits)
2. Achievement
3. Leaderboard

Planned Features :
1. Login and Sign-up [Done]
2. Leaderboard [Done]
3. Replace scanf with sscanf+fgets (memory safety reason, atleast it is what they say) [Done]
4. ~~ncurses integration so it will look much nicer~~ ~~(nvm im too lazy)~~ (Ended up creating an inferior version of it) [Done?]
5. idk, something that will comes up in my dream

---

For the oldest codes, you can check the "Old" branch.

For the code before the tui rewrite, you can check the "Rewrite" Branch.

For the tui rewrite (and i guess this main before i fix something), you can check the "Another-Rewrite" Branch.
