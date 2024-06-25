module PE4 where

import Data.Maybe -- up to you if you want to use it or not

-- Generic DictTree definition with two type arguments
data DictTree k v = Node [(k, DictTree k v)] | Leaf v deriving Show

-- Lightweight Char wrapper as a 'safe' Digit type
newtype Digit = Digit Char deriving (Show, Eq, Ord) -- derive equality and comparison too!

-- Type aliases
type DigitTree = DictTree Digit String
type PhoneNumber = [Digit]


---------------------------------------------------------------------------------------------
------------------------- DO NOT CHANGE ABOVE OR FUNCTION SIGNATURES-------------------------
--------------- DUMMY IMPLEMENTATIONS ARE GIVEN TO PROVIDE A COMPILABLE TEMPLATE ------------
--------------------- REPLACE THEM WITH YOUR COMPILABLE IMPLEMENTATIONS ---------------------
---------------------------------------------------------------------------------------------


----------
-- Part I:
-- Some Maybe fun! Basic practice with an existing custom datatype.

-- toDigit: Safely convert a character to a digit
toDigit :: Char -> Maybe Digit
toDigit x | not (is_in x ['0'..'9'])      =  Nothing
          | otherwise =  Just (Digit x)
          
is_in n [] = False
is_in n (x:xs)
    | n == x = True
    | otherwise = is_in n xs


-- toDigits: Safely convert a bunch of characters to a list of digits.
--           Particularly, an empty string should fail.
toDigit2 x = Digit x

helper2 (x:xs) | helper (x:xs) = (toDigit2 x):helper2 (xs)
helper2 _ = []

helper (x:xs) | not (is_in x ['0'..'9']) = False
              | otherwise = helper (xs)
helper _ = True

toDigits :: String -> Maybe PhoneNumber
toDigits x | (helper2 x == []) = Nothing
           | otherwise = Just (helper2 x)


-----------
-- Part II:
-- Some phonebook business.

-- numContacts: Count the number of contacts in the phonebook...
numContacts :: DigitTree -> Int
numContacts a = fun a

fun (Leaf v) = 1
fun (Node ((a,b):xs)) = fun (b) + fun (Node (xs))
fun _ = 0


    
-- getContacts: Generate the contacts and their phone numbers in order given a tree. 
getContacts :: DigitTree -> [(PhoneNumber, String)]
getContacts a = fun4 (fun3 a ([])) (fun2 a)

fun2 (Leaf v) = [v]
fun2 (Node ((a,b):xs)) = fun2 (b) ++ fun2 (Node (xs))
fun2 _ = []

fun3 (Leaf v) ls = [ls] --  ++ [Digit 'k']
fun3 (Node ((a,b):xs)) ls = fun3 (b) (ls++[a]) ++ fun3 (Node (xs)) (ls)
fun3 _ _ = []

fun4 (x:xs) (y:ys) = [(x,y)] ++ fun4 (xs) (ys)
fun4 _ _ = []

-- autocomplete: Create an autocomplete list of contacts given a prefix
-- e.g. autocomplete "32" areaCodes -> 
--      [([Digit '2'], "Adana"), ([Digit '6'], "Hatay"), ([Digit '8'], "Osmaniye")]
autocomplete :: String -> DigitTree -> [(PhoneNumber, String)]
autocomplete "" _ = []
autocomplete st tr = getContacts (fun5 st tr)

fun5 (y:ys) (Node ((a,b):xs)) = if  (Digit(y)) == a then (fun5 (ys) b) else (fun5 (y:ys) (Node (xs)))
fun5 [] m = m
fun5 _ _ = Node []

-----------
-- Example Trees
-- Two example trees to play around with, including THE exampleTree from the text. 
-- Feel free to delete these or change their names or whatever!

exampleTree :: DigitTree
exampleTree = Node [
    (Digit '1', Node [
        (Digit '3', Node [
            (Digit '7', Node [
                (Digit '8', Leaf "Jones")])]),
        (Digit '5', Leaf "Steele"),
        (Digit '9', Node [
            (Digit '1', Leaf "Marlow"),
            (Digit '2', Node [
                (Digit '3', Leaf "Stewart")])])]),
    (Digit '3', Leaf "Church"),
    (Digit '7', Node [
        (Digit '2', Leaf "Curry"),
        (Digit '7', Leaf "Hughes")])]

areaCodes :: DigitTree
areaCodes = Node [
    (Digit '3', Node [
        (Digit '1', Node [
            (Digit '2', Leaf "Ankara")]),
        (Digit '2', Node [
            (Digit '2', Leaf "Adana"),
            (Digit '6', Leaf "Hatay"),
            (Digit '8', Leaf "Osmaniye")])]),
    (Digit '4', Node [
        (Digit '6', Node [
            (Digit '6', Leaf "Artvin")])])]

