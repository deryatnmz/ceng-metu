module PE2 where

---------------------------------------------------------------------------------------------
------------------------- DO NOT CHANGE ABOVE OR FUNCTION SIGNATURES-------------------------
--------------- DUMMY IMPLEMENTATIONS ARE GIVEN TO PROVIDE A COMPILABLE TEMPLATE ------------
--------------------- REPLACE THEM WITH YOUR COMPILABLE IMPLEMENTATIONS ---------------------
---------------------------------------------------------------------------------------------

-- Note: undefined is a value that causes an error when evaluated. Replace it with
-- a viable definition! Name your arguments as you like by changing the holes: _

--------------------------
-- Part I: Time to inf up!

-- naturals: The infinite list of natural numbers. That's it!
naturals :: [Integer]
naturals = [0..]

-- interleave: Interleave two lists, cutting off on the shorter list.
interleave :: [a] -> [a] -> [a]
interleave [] _ = []
interleave _ [] = []
interleave (x:xs) (y:ys) = [x,y] ++ interleave xs ys

-- integers: The infinite list of integers. Ordered as [0, -1, 1, -2, 2, -3, 3, -4, 4...].
integers :: [Integer]
neg = [ (-1)*x | x <- [1..]]
integers = interleave [0..] neg

--------------------------------
-- Part II: SJSON Prettification

-- splitOn: Split string on first occurence of character.
splitOn :: Char -> String -> (String, String)
splitOn ch d = (helper ch d, helper2 ch d)

helper _ []=[]
helper a (x:xs) 
   |a==x        =  []
   |otherwise   = [x] ++ helper a (xs)
   
helper2 _ []=[]
helper2 a (x:xs) 
   |a==x        =  xs
   |otherwise   = helper2 a (xs)

-- tokenizeS: Transform an SJSON string into a list of tokens.
tokenizeS :: String -> [String]
tokenizeS []=[]
tokenizeS (x:xs) 
   |x=='{'        = ["{"] ++ tokenizeS (xs) 
   |x=='\''       = [helper '\'' (xs)] ++ tokenizeS (helper2 '\'' (xs))
   |x==':'        = [":"] ++ tokenizeS (xs) 
   |x=='}'        = ["}"] ++ tokenizeS (xs)
   |x==','        = [","] ++ tokenizeS (xs) 
   |otherwise     = tokenizeS (xs)  

-- prettifyS: Prettify SJSON, better tokenize first!
prettifyS :: String -> String
prettifyS inp = helper4 1 inp

mult _ 0 = []
mult stri num = stri ++ mult stri (num-1)

helper4 k [] = []
helper4 k (x:xs)
    |x=='{'        = "{\n"++ mult "    " k ++ helper4 (k+1) (xs)
    |x=='\''       = "\'"++helper '\'' (xs) ++ "\'" ++ helper4 k (helper2 '\'' (xs))
    |x==':'        = ": " ++ helper4 k (xs)
    |x==','        = ",\n" ++ mult "    " (k-1) ++ helper4 k (xs)
    |x=='}'        = "\n" ++ mult "    " (k-2) ++ "}" ++  helper4 (k-1) (xs)
    |otherwise     = helper4 k (xs)

-- Good luck to you, friend and colleague!

