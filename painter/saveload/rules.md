> The .ourp files store declarative descriptions of primitives.

>The order in which they are included is **IMPORTANT**!
 
An object's points should go first, if it includes them.
For example, if we have a *section*, its ID should be **>=3**, since it includes 2 points. So the file should be written first
```
{
ID 1
point 0 0
}
{
ID 2
point 1000 200
}
{
ID 3
section
} 
```

The numbers in point mean its x and y coordinates respectively.
This means that the section consists of points with IDs 2 and 1 less than its own. If it is a *circle*, its ID must be **>=2**, because the center point must be stored.
For example:
```
{
ID 1
point 10 200
}
{
ID 2
circle 50
}
```
Where 50 is the number responsible for the radius of the circle.
