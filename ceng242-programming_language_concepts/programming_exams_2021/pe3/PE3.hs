module PE3 where

data Cell = SpaceCraft Int | Sand | Rock Int | Pit deriving (Eq, Read, Show)

type Grid = [[Cell]]
type Coordinate = (Int, Int)

data Move = North | East | South | West | PickUp | PutDown deriving (Eq, Read, Show)

data Robot = Robot { name :: String,
                     location :: Coordinate,
                     capacity :: Int,
                     energy :: Int,
                     storage :: Int } deriving (Read, Show)

-------------------------------------------------------------------------------------------
--------------------------------- DO NOT CHANGE ABOVE -------------------------------------
------------- DUMMY IMPLEMENTATIONS ARE GIVEN TO PROVIDE A COMPILABLE TEMPLATE ------------
------------------- REPLACE THEM WITH YOUR COMPILABLE IMPLEMENTATIONS ---------------------
-------------------------------------------------------------------------------------------
-------------------------------------- PART I ---------------------------------------------

isInGrid :: Grid -> Coordinate -> Bool
isInGrid grid (x,y)
       | x<0 || y<0 = False
       |(length grid - y) > 0 && length (grid!!y)-x > 0 = True
       | otherwise = False
-------------------------------------------------------------------------------------------

totalCount :: Grid -> Int
totalCount grid = helper2 0 grid

rock_value (Rock n) = n
is_rock (Rock n) = "yes"
is_rock _ = "no" 
is_spacecraft (SpaceCraft n)= "yes"
is_spacecraft _ = "no"

helper2 a [] = a
helper2 a (x:xs) = helper a (x) + helper2 a (xs)

helper a []= a
helper a (x:xs) 
   | is_rock (x) == "yes" =  helper (a+ rock_value (x)) (xs)
   | otherwise   = helper a (xs)

-------------------------------------------------------------------------------------------

coordinatesOfPits :: Grid -> [Coordinate]
coordinatesOfPits grid = srt(helper4 0 grid [])

helper4 a [] r = r
helper4 a (x:xs) r = helper3 a 0 (x) r ++ helper4 (a+1) (xs) r

helper3 a b [] r = r
helper3 a b (x:xs) r 
    | x == Pit  = helper3 a (b+1) (xs)  (r ++[(b,a)])
    |otherwise = helper3 a (b+1) (xs) r

srt []     = []
srt (x:xs) = (srt l) ++ [x] ++ (srt r)
    where
        l  = filter (< x) xs
        r  = filter (>= x) xs   

-------------------------------------------------------------------------------------------

tracePath :: Grid -> Robot -> [Move] -> [Coordinate]
tracePath grid robot moves = myfun grid (tolist robot) moves ([])

tolist Robot { name= a, location = (b,c) , capacity =d , energy =e , storage =f} =  [b,c,d,e,f]
myfun grid [x,y,cap,engy,stg] [] r = r
myfun grid [x,y,cap,engy,stg] (a:as) r
    | grid!!y!!x == Pit || cap<=0 || engy<=0 || stg>=cap = myfun grid [x,y,cap,engy,stg] (as) (r++[(x,y)])
    | a == West && isInGrid grid ((x-1),y) = if grid!!y!!(x-1) == Pit then myfun grid [x-1,y,cap,0,stg] (as) (r++[((x-1),y)])  else  myfun grid [x-1,y,cap,engy-1,stg] (as) (r++[((x-1),y)])
    | a == West && not (isInGrid grid ((x-1),y)) = myfun grid [x,y,cap,engy-1,stg] (as) (r++[(x,y)])
    | a == North && isInGrid grid (x,(y-1)) = if grid!!(y-1)!!x == Pit then myfun grid [x,y-1,cap,0,stg] (as) (r++[(x,y-1)]) else myfun grid [x,y-1,cap,engy-1,stg] (as) (r++[(x,y-1)])
    | a == North && not (isInGrid grid (x,(y-1))) = myfun grid [x,y,cap,engy-1,stg] (as) (r++[(x,y)])
    | a == South && isInGrid grid (x,(y+1)) = if grid!!(y+1)!!x == Pit then myfun grid [x,y+1,cap,0,stg] (as) (r++[(x,y+1)]) else myfun grid [x,y+1,cap,engy-1,stg] (as) (r++[(x,y+1)])
    | a == South && not (isInGrid grid (x,(y+1))) = myfun grid [x,y,cap,engy-1,stg] (as) (r++[(x,y)])
    | a == East && isInGrid grid ((x+1),y)  = if grid!!y!!(x+1) == Pit then myfun grid [x+1,y,cap,0,stg] (as) (r++[((x+1),y)]) else myfun grid [x+1,y,cap,engy-1,stg] (as) (r++[((x+1),y)])
    | a == West && not (isInGrid grid ((x+1),y)) = myfun grid [x,y,cap,engy-1,stg] (as) (r++[(x,y)])
    | a == PickUp = if is_rock(grid!!y!!x) == "yes" || rock_value(grid!!y!!x)>0 then myfun grid [x,y,cap-1,engy-5,stg+1] (as) (r++[(x,y)]) else myfun grid [x,y,cap,engy-5,stg] (as) (r++[(x,y)])
    | a == PutDown = if is_spacecraft(grid!!y!!x) == "yes" then myfun grid [x,y,cap+1,engy-3,stg-1] (as) (r++[(x,y)]) else myfun grid [x,y,cap,engy-3,stg](as) (r++[(x,y)])

  

------------------------------------- PART II ----------------------------------------------

helper6 a [] r = r
helper6 a (x:xs) r = helper5 a 0 (x) r ++ helper6 (a+1) (xs) r

helper5 a b [] r = r
helper5 a b (x:xs) r 
    | is_sc(x) == "yes"  = helper5 a (b+1) (xs)  (r ++[(b,a)])
    |otherwise = helper5 a (b+1) (xs) r
    
coord [(a,b)] = (a,b)
sc_loc gr = coord (helper6 0 gr [] )

is_sc (SpaceCraft n) = "yes"
is_sc _ = "no" 

r_loc Robot { name= a, location = (b,c) , capacity =d , energy =e , storage =f} = (b,c)
cal (rx,ry) (sx,sy) = max 0 (100-(abs(rx-sx) + abs(ry-sy))*20)

energiseRobots :: Grid -> [Robot] -> [Robot]
energiseRobots grid robots = myy grid robots ([])


myy grid (x:xs) r = [update (cal (sc_loc grid) (r_loc (x))) x] ++ myy grid (xs) r
myy grid _ r = r

update n Robot { name= a, location = (b,c) , capacity =d , energy =e , storage =f} = if (n+e)>=100 then Robot { name= a, location = (b,c) , capacity =d , energy =100 , storage =f} else Robot { name= a, location = (b,c) , capacity =d , energy =(e+n) , storage =f}



-------------------------------------------------------------------------------------------

applyMoves :: Grid -> Robot -> [Move] -> (Grid, Robot)
applyMoves grid robot moves = (grid, robot)
