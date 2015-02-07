QIXGame
=======

### Introduction

    There are two areas in the game. One is map area, and the other is player area.
    In the beginning, the bird(player) is in the player area, while the other map elements are in the map area.
    The player should launch the bird to draw polygons, which will kill the elements in the map area,
    and decrease the map area.

### HOW TO PLAY
    
    The player should launch the bird to draw polygons constantly until he(she) win. If the draw succeeded, 
    (it is a 'CLEAR' in the game)
    1) all the elements contained in the polygon will be killed.
    2) the map area will decrease accordingly.

    Else if something happened,
    1) when the bird encountered with an enemy, 'MISS' will happen.
    2) when draw the polygon in less than three steps, 'SKIP' will happen. (In this case, do nothing)
    3) when draw the polygon in more than three steps, 'MISS' will happen.

### Conditions for WIN

    the player can win only if
    1) all the enemies in the map area are killed.
    2) the remaining map area is less than 25%.
    3) before 'MISS' is greater than 5.
    4) before 'CLEAR' is greater than 10.
