module PE1 where

import Text.Printf
import Data.List

-- Type synonyms --
type Point = (Double, Double)
type Signal = (Double, Double, Double, Double)

-- This function takes a Double and rounds it to 2 decimal places as requested in the PE --
getRounded :: Double -> Double
getRounded x = read s :: Double
               where s = printf "%.2f" x

-------------------------------------------------------------------------------------------
----------------------- DO NOT CHANGE ABOVE OR FUNCTION SIGNATURES-------------------------
------------- DUMMY IMPLEMENTATIONS ARE GIVEN TO PROVIDE A COMPILABLE TEMPLATE ------------
------------------- REPLACE THEM WITH YOUR COMPILABLE IMPLEMENTATIONS ---------------------
-------------------------------------------------------------------------------------------

getDistance :: Point -> Point -> Double
getDistance (x1, y1) (x2, y2) = getRounded (((y2-y1)^2 + (x2-x1)^2)**(0.5) )
-------------------------------------------------------------------------------------------

findAllDistances :: Point -> [Point] -> [Double]
findAllDistances _ [] = []
findAllDistances b (a:l) = [getDistance b a] ++(findAllDistances b l)

-------------------------------------------------------------------------------------------

findExtremes :: Point -> [Point] -> (Point, Point)
findExtremes b l = (minn b l, maxx b l)
index a (x:y) n = if a==x
                     then n
                     else index a y (n+1)
maxx b l = l !! (index (maximum (findAllDistances b l)) (findAllDistances b l) 0)
minn b l = l !! (index (minimum (findAllDistances b l)) (findAllDistances b l) 0) 

-------------------------------------------------------------------------------------------

getSingleAction :: Signal -> String
getSingleAction (n,e,s,w)
                    | (n-s)>0 && (e-w)>0 = "NorthEast"
                    | (n-s)>0 && (w-e)>0 = "NorthWest"
                    | (s-n)>0 && (e-w)>0 = "SouthEast"
                    | (s-n)>0 && (w-e)>0 = "SouthWest"
                    | (n-s)>0 && (e-w)==0 = "North"
                    | (s-n)>0 && (e-w)==0 = "South"
                    | (n-s)==0 && (w-e)>0 = "West"
                    | (n-s)==0 && (e-w)>0 = "East"  
                    | otherwise = "Stay"



-------------------------------------------------------------------------------------------

getAllActions :: [Signal] -> [String]
getAllActions [] = []
getAllActions (x:y) = [getSingleAction x ] ++ getAllActions (y)

-------------------------------------------------------------------------------------------

numberOfGivenAction :: Num a => [Signal] -> String -> a
numberOfGivenAction signals action = match (getAllActions signals) action

match [] elemen = 0
match (x:y) elemen = if x==elemen
                       then 1 + (match (y) elemen)
                       else  (match (y) elemen)