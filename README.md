# random-forest-from-scratch
<br />
Freakin machine learning Random Forest Binary Classification model done from scratch with our bare hands @nates1803. 
<br /><br />
- How to run it?<br />
Install gcc and run<br />

```gcc main.cpp -o main.exe -lm```
<br />

then <br />

```./main.exe``` 
<br />

<br />
The accurancy is really good, only binary classification.<br />
<br />
TO-DO:<br />
-> make it easier to be plug and play by:<br />
  - making dataset easier to change, adding more settings to read_csv lib so the user can specify his own dataset.<br />
-> model changes:<br />
  - test if instead of getting a random feature on "find_best_split", to iterate within all features and get the best one in that(maybe a flag to activate that)<br />
-> code changes:<br />
  - make it more readable, review variable names and check overall logic<br />
-> usability:<br />
  - make a proper read.me explaining the code<br />
