module PE2 where

import Text.Printf

type Point = (Int, Int)
type Dimensions = (Int, Int)
type Vector = (Int, Int)

getRounded :: Double -> Double
getRounded x = read s :: Double
               where s = printf "%.2f" x

castIntToDouble x = (fromIntegral x) :: Double

-------------------------------------------------------------------------------------------
----------------------- DO NOT CHANGE ABOVE OR FUNCTION SIGNATURES-------------------------
------------- DUMMY IMPLEMENTATIONS ARE GIVEN TO PROVIDE A COMPILABLE TEMPLATE ------------
------------------- REPLACE THEM WITH YOUR COMPILABLE IMPLEMENTATIONS ---------------------
-------------------------------------------------------------------------------------------

-------------------------------------------------------------------------------------------------------------------------------
getVector :: String -> Vector
getVector "Stay" = (0,0)
getVector "North" = (0,1)
getVector "South" = (0,-1)
getVector "East" = (1,0)
getVector "West" = (-1,0)
getVector "NorthWest" = (-1,1)
getVector "NorthEast" = (1,1)
getVector "SouthWest" = (-1,-1)
getVector "SouthEast" = (1,-1)

-------------------------------------------------------------------------------------------------------------------------------
getAllVectors :: [String] -> [Vector]
getAllVectors [] = []
getAllVectors (a:b) = [getVector a] ++ getAllVectors b

-------------------------------------------------------------------------------------------------------------------------------


producePath :: Point -> [String] -> [Point]
producePath x y = [x] ++  (producePath2 x y)
producePath2 x [] = []
producePath2 x (a:l) = [addtup x (getVector a)] ++ producePath2 (addtup x (getVector a)) (l)
addtup (a,b) (x,y)= (a+x, b+y)
-------------------------------------------------------------------------------------------------------------------------------
takePathInArea :: [Point] -> Dimensions -> [Point]
takePathInArea [] _ = []
takePathInArea ((ax,ay):l) (bx, by) = if (ax<bx && ax>=0 && ay>=0 && ay<by)
                                         then [(ax,ay)] ++ takePathInArea (l) (bx, by)
                                         else []

-------------------------------------------------------------------------------------------------------------------------------

remainingObjects :: [Point] -> Dimensions -> [Point] -> [Point]
remainingObjects path border objects = dmatch objects (match (takePathInArea path border) objects)

match1 _ [] = False
match1 a (x:y) = if (a==x)
                then True 
                else (match1 a (y))
match _ [] =[]
match [] _ =[]
match (a:b) l = if (match1 a l)
                    then [a] ++ (match (b) l)
                    else (match (b) l)
                    
dmatch _ [] =[]
dmatch [] _ =[]
dmatch (a:b) l = if (match1 a l)
                    then (dmatch (b) l)
                    else [a] ++(dmatch (b) l)

-------------------------------------------------------------------------------------------------------------------------------
averageStepsInSuccess :: [[Point]] -> Dimensions -> [Point] -> Double
averageStepsInSuccess2 [] _ _ a b = getRounded ((castIntToDouble b) / (castIntToDouble a))
averageStepsInSuccess2 (p:paths) border objects a b= if (success p border objects)
                                                        then (averageStepsInSuccess2 (paths) border objects (a+1) (b+length(p)))
                                                        else (averageStepsInSuccess2 (paths) border objects a b)


success path border objects = if ((length(path) == length (takePathInArea path border)) && (remainingObjects path border objects) ==[])
                                then True
                                else False

averageStepsInSuccess paths border objects = (averageStepsInSuccess2 paths border objects 0 0)