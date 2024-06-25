module PE3 where

-- can use these if you want to...
import Data.List
import Data.Maybe

data Private = Private { idNumber :: Int, height :: Int, timeToSwap :: Int } deriving Show

data Cell = Empty | Full Private deriving Show

type Area = [[Cell]] 

---------------------------------------------------------------------------------------------
------------------------- DO NOT CHANGE ABOVE OR FUNCTION SIGNATURES-------------------------
--------------- DUMMY IMPLEMENTATIONS ARE GIVEN TO PROVIDE A COMPILABLE TEMPLATE ------------
--------------------- REPLACE THEM WITH YOUR COMPILABLE IMPLEMENTATIONS ---------------------
---------------------------------------------------------------------------------------------


-- Note: undefined is a value that causes an error when evaluated. Replace it with
-- a viable definition! Name your arguments as you like by changing the holes: _


--------------------------
-- Part I: Warming up with Abstractions

-- bubbleHumans: Applying a parallel bubble step on integers.
bubbleHumans :: [Int] -> [Int]
bubbleHumans [] = []
bubbleHumans [a] = [a]
bubbleHumans [a,b] = if (a>b) then [a,b] else [b,a]
bubbleHumans (a:b:c:rest) = if(a<b) 
                                then ([b,a] ++ bubbleHumans (c:rest))
                                else ( if (c>b) 
                                        then ([a,c,b] ++ bubbleHumans (rest)) 
                                            else ([a,b] ++ bubbleHumans (c:rest))
                                            )

-- bubblePrivates: The same thing, but on privates with a key function and an asc/desc option.
bubblePrivates :: (Private -> Int) -> Bool -> [Private] -> [Private]

descending _ [] = []
descending _ [a] = [a]
descending prop [a,b] = if ((prop a)>(prop b)) then [a,b] else [b,a]
descending prop (a:b:c:rest) = if((prop a)<(prop b)) 
                                then ([b,a] ++ descending prop (c:rest))
                                else ( if ((prop c)>(prop b)) 
                                        then ([a,c,b] ++ descending prop (rest)) 
                                            else ([a,b] ++ descending prop (c:rest))
                                            )

ascending _ [] = []
ascending _ [a] = [a]
ascending prop [a,b] = if ((prop a)<=(prop b)) then [a,b] else [b,a]
ascending prop (a:b:c:rest) = if((prop a)>(prop b)) 
                                then ([b,a] ++ ascending prop (c:rest))
                                else ( if ((prop c)<(prop b)) 
                                        then ([a,c,b] ++ ascending prop (rest)) 
                                            else ([a,b] ++ ascending prop (c:rest))
                                            )

pri a b c =  Private {idNumber = a, height = b, timeToSwap = c}
fun [] = []
fun ( (Private {idNumber = x, height = y, timeToSwap = z}):rest) = [Private {idNumber = x, height = y, timeToSwap = z}]  ++ fun (rest)
bubblePrivates prop True list = descending prop (fun list)
bubblePrivates prop False list = ascending prop (fun list)

-- sortPrivatesByHeight: Full sort via multiple bubble steps, calculate the sorting time too!
sortPrivatesByHeight :: [Private] -> ([Private], Int)
mm (w,t) = if (w>t) then w else t

qdescending [] = []
qdescending [a] = [a]
qdescending [a,b] = if ((height a)>=(height b)) then [a,b] else [b,a]
qdescending (a:b:c:rest) = if((height a)<(height b)) 
                                then ([b,a] ++ qdescending  (c:rest))
                                else ( if ((height c)>(height b)) 
                                        then ([a,c,b] ++ qdescending  (rest)) 
                                            else ([a,b] ++ qdescending  (c:rest))
                                            )
time [] = []
time [a] = []
time [a,b] = if ((height a)>=(height b)) then [] else [mm ((timeToSwap a), ( timeToSwap b))]
time (a:b:c:rest) = if((height a)<(height b)) 
                                then ([mm (timeToSwap a, timeToSwap b)] ++ time (c:rest))
                                else ( if ((height c)>(height b)) 
                                        then ([mm (timeToSwap c, timeToSwap b)] ++ time (rest)) 
                                            else (time  (c:rest))
                                            )    
sortt list 0 coun = (list, coun)                                          
sortt list times coun=  if (length (time list) == 0)
                         then sortt (qdescending list) (times-1) coun
                         else sortt (qdescending list) (times-1) (maximum (time list)+coun)

sortPrivatesByHeight list = sortt list (10*length(list)) 0


--------------------------
-- Part II: Squeezing your Brain Muscles

-- ceremonialFormation: Sorting again, but with multiple files. Don't forget the time!
ceremonialFormation :: Area -> (Area, Int)


isEmpty (Empty) = True
isEmpty _ = False
heg (Full (Private _ a _)) = a
tim (Full (Private _ _ a)) = a





maxx [] = 0
maxx (a:l)= (maximum a) + maxx l

tt2 [] rett = []
tt2 (a:list) rett = [tttt a (10*length(a)) rett]++(tt2 (list) rett)

tttt list 0 coun = coun                                         
tttt list times coun=  if (length (qtime list) == 0)
                         then tttt (qdes list) (times-1) (0:coun)
                         else tttt (qdes list) (times-1) (maximum (qtime list):coun)

qdes [] = []
qdes [a] = [a]
qdes [a,b] 
         | (isEmpty a) && not (isEmpty b) = [b,a]
         | not (isEmpty a) && not (isEmpty b) && (heg b > heg a)= [b,a]
         | otherwise = [a,b]
qdes (a:b:c:rest)
         |(isEmpty a) && not (isEmpty b) = ([b,a] ++ qdes  (c:rest))
         |not (isEmpty a) && not (isEmpty b) && (heg b > heg a) = ([b,a] ++ qdes  (c:rest))
         |((isEmpty b) && not (isEmpty c)) = ([a,c,b] ++ qdes (rest))
         |(not (isEmpty b) && not (isEmpty c) && (heg c > heg b)) = ([a,c,b] ++ qdes (rest))
         |otherwise = ([a,b] ++ qdes  (c:rest))


qtime [] = []
qtime [a] = []
qtime [a,b]
         | (isEmpty a) && not (isEmpty b) = [tim b]
         | not (isEmpty a) && not (isEmpty b) && (heg b > heg a)= [mm((tim a),(tim b))]
         | otherwise = []
qtime (a:b:c:rest)
         |(isEmpty a) && not (isEmpty b) = ([tim b] ++ qtime (c:rest))
         |not (isEmpty a) && not (isEmpty b) && (heg b > heg a) = ([mm((tim a),(tim b))] ++ qtime  (c:rest))
         |((isEmpty b) && not (isEmpty c)) = ([tim c] ++ qtime (rest))
         |(not (isEmpty b) && not (isEmpty c) && (heg c > heg b)) = ([mm((tim c),(tim b))] ++ qtime (rest))
         |otherwise = (qtime  (c:rest))                                                                                                  







sortt2 list 0  = (list)                                          
sortt2 list times =  if (length (qtime list) == 0)
                         then sortt2 (qdes list) (times-1)
                         else sortt2 (qdes list) (times-1) 

yey (a:l)= [sortt2 a (10*length(a))] ++ yey l
yey [] =[]
summ [] =[]
summ (a:l) = [sum a] ++  summ l

ceremonialFormation list = (yey list,maximum (summ (tt2 list []))  )


-- swapPrivates: Swap two arbitrary privates by ID if they are in the area. Big ouch!
swapPrivates :: Int -> Int -> Area -> Area
idd (Full (Private a _ _)) = a
idd _ = -1

ini=Full (Private (-1) 0 0)

hey f (a:l) = if (idd a == f) then a else hey f l
hey f [] = ini
hey2 f (a:l) = if ((idd (hey f a))== (-1)) then hey2 f (l) else (hey f a)
hey2 f [] = ini

--fisrt = hey2 f list
--secon = hey2 s list

rep first second [] =[]
rep first second (a:l) = if (idd a == idd first) 
                            then ([second] ++ rep first second l) 
                            else if (idd a == idd second) 
                            then ([first] ++ rep first second l)
                            else ([a] ++ rep first second l)
rep2 first second [] = []                          
rep2 first second (a:l) = [rep first second (a)] ++ rep2 first second (l)
swapPrivates a b exam = if( idd (hey2 a exam)==(-1) || idd (hey2 b exam)==(-1) ) then exam else (rep2 (hey2 a exam) (hey2 b exam) (exam))

-- Best of luck to you, friend and colleague!

