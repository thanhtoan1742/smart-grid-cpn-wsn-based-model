## Result

| Magnitude | Have loss | State Searched |
| --------- | --------- | -------------- |
| 10        | no        | 39             |
| 100       | no        | 423            |
| 10        | yes       | 243            |
| 100       | yes       | 231265         |



### Magnitude 10 - No loss
```
C20 -- 0 -- G30
```
```
DEBUG:  SATISFIED STATE [0:20 0:20]
       [0:20 0:20]
      [0:20 4:16]
     [4:20 0:16]
    [4:20 6:10]
   [10:20 0:10]
  [10:20 10:0]
 [20:20 0:0]
[0:0 0:0]
39
```


## Magnitude 100 - No loss
```
C200 -- 0 -- G300
```
```
DEBUG:  SATISFIED STATE [0:200 0:200]
           [0:200 0:200]
          [0:200 11:189]
         [11:200 0:189]
        [11:200 15:174]
       [26:200 0:174]
      [26:200 29:145]
     [55:200 0:145]
    [55:200 69:76]
   [124:200 0:76]
  [124:200 76:0]
 [200:200 0:0]
[0:0 0:0]
423
```

### Magnitude 10 - Loss
```
C20 -- 5 -- G30
```
```
DEBUG:  SATISFIED STATE [0:20 0:22]
DEBUG:  SATISFIED STATE [0:20 0:21]
   [0:20 0:21]
  [0:20 21:0]
 [20:20 0:0]
[0:0 0:0]
243
```


### Magnitude 100 - Loss
```
C200 -- 5 -- G300
```
```
EBUG:  SATISFIED STATE [0:200 0:216]
DEBUG:  SATISFIED STATE [0:200 0:215]
DEBUG:  SATISFIED STATE [0:200 0:214]
DEBUG:  SATISFIED STATE [0:200 0:213]
DEBUG:  SATISFIED STATE [0:200 0:212]
DEBUG:  SATISFIED STATE [0:200 0:211]
DEBUG:  SATISFIED STATE [0:200 0:210]
                     [0:200 0:210]
                    [0:200 21:189]
                   [20:200 0:189]
                  [20:200 21:168]
                 [40:200 0:168]
                [40:200 21:147]
               [60:200 0:147]
              [60:200 21:126]
             [80:200 0:126]
            [80:200 21:105]
           [100:200 0:105]
          [100:200 21:84]
         [120:200 0:84]
        [120:200 21:63]
       [140:200 0:63]
      [140:200 21:42]
     [160:200 0:42]
    [160:200 21:21]
   [180:200 0:21]
  [180:200 21:0]
 [200:200 0:0]
[0:0 0:0]
231265
```



### More grid
```
G30 -- 5 \
          \
           B -- 2 -- C20
          /
        /
G30 -- 3
```
```
DEBUG:  SATISFIED STATE [0:20 0:12 0:25 0:0]
DEBUG:  SATISFIED STATE [0:20 0:10 0:25 0:0]
DEBUG:  SATISFIED STATE [0:20 0:11 0:23 0:0]
DEBUG:  SATISFIED STATE [0:20 0:6 0:27 0:0]
DEBUG:  SATISFIED STATE [0:20 0:6 0:26 0:0]
DEBUG:  SATISFIED STATE [0:20 0:6 0:25 0:0]
DEBUG:  SATISFIED STATE [0:20 0:6 0:24 0:0]
DEBUG:  SATISFIED STATE [0:20 0:6 0:23 0:0]
DEBUG:  SATISFIED STATE [0:20 0:6 0:22 0:0]
DEBUG:  SATISFIED STATE [0:20 0:2 0:25 0:0]
DEBUG:  SATISFIED STATE [0:20 0:2 0:24 0:0]
DEBUG:  SATISFIED STATE [0:20 0:0 0:25 0:0]
DEBUG:  SATISFIED STATE [0:20 0:14 0:10 0:0]
DEBUG:  SATISFIED STATE [0:20 0:0 0:23 0:0]
DEBUG:  SATISFIED STATE [0:20 0:0 0:22 0:0]
    [0:20 0:0 0:22 0:0]
   [0:20 0:0 22:0 0:0]
  [0:20 0:0 0:0 21:0]
 [20:20 0:0 0:0 0:0]
[0:0 0:0 0:0 0:0]
34802
```












