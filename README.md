# random-forest-from-scratch

Freakin machine learning Random Forest Binary Classification model done from scratch with our bare hands @nates1803. 

- How to run it?
Install gcc and run
```gcc main.cpp -o main.exe -lm```
then
```./main.exe```

The accurancy is really good, only binary classification.

TO-DO:
-> make it easier to be plug and play by:
  - making dataset easier to change, adding more settings to read_csv lib so the user can specify his own dataset.
-> model changes:
  - test if instead of getting a random feature on "find_best_split", to iterate within all features and get the best one in that(maybe a flag to activate that)
-> code changes:
  - make it more readable, review variable names and check overall logic
-> usability:
  - make a proper read.me explaining the code
