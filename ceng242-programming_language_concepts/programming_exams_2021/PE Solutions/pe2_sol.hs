module PE2 where

-- Part I: Simple exercise with infinite lists

-- naturals: The infinite list of natural numbers. That's it!

-- The straightforward list comprehension
naturals :: [Integer]
naturals = [0..]

-- Alternative with iterate
naturals2 :: [Integer]
naturals2 = iterate (+1) 0

-- Alternative with recursive definition
naturals3 :: [Integer]
naturals3 = 0 : map (+1) naturals3
-- ^ succ instead of (+1) also works

-- interleave: Interleave two lists, cutting off on the shorter list.
-- e.g. interleave [1, 2, 3, 4, 5] [4, 5, 6] -> [1, 4, 2, 5, 3, 6]

-- The straightforward recursive definition
interleave :: [a] -> [a] -> [a]
interleave _ [] = []
interleave [] _ = []
interleave (x:xs) (y:ys) = x : y : interleave xs ys

-- Can be done with zip and concatMap!
interleave2 :: [a] -> [a] -> [a]
interleave2 xs ys = concatMap (\(x, y) -> [x, y]) $ zip xs ys

-- Very similarly, concat and zipWith
interleave3 :: [a] -> [a] -> [a]
interleave3 xs ys = concat $ zipWith (\x y -> [x, y]) xs ys

-- integers: The infinite list of integers. Ordered as [0, -1, 1, -2, 2, -3, 3, -4, 4...].

-- A definition using naturals only
integers :: [Integer]
integers = interleave naturals $ map negate $ drop 1 naturals

-- Another one, just mixing basic lists
integers2 :: [Integer]
integers2 = interleave [0..] [-1,-2..]

-- Finally, the not-so-straightforward map using the definition
integers3 :: [Integer]
integers3 = map (\i -> if even i then i `quot` 2 else -(i + 1) `quot` 2) [0..]

-- Part II: SJSON Prettification

-- splitOn: Split string on first occurence of character.

-- Recursive definition
splitOn :: Char -> String -> (String, String)
splitOn _ [] = ("", "")
splitOn c (x:xs)
  | x == c = ("", xs)
  | otherwise = let (left, right) = splitOn c xs
                 in (x : left, right)

-- Using break
splitOn2 :: Char -> String -> (String, String)
splitOn2 c s = let (prefix, suffix) = break (== c) s
                in if suffix == "" -- tail errors on empty string, so check
                      then (prefix, "")
                      else (prefix, tail suffix)

-- Can have a tail with no error using (drop 1)
splitOn3 :: Char -> String -> (String, String)
splitOn3 c s = let (prefix, suffix) = break (== c) s in (prefix, drop 1 suffix)

-- What if we had a helper for applying a function to the second element of a tuple?
second :: (b -> c) -> (a, b) -> (a, c)
second f (x, y) = (x, f y)

-- ... we would make an epic, let-less one-liner!
splitOn4 :: Char -> String -> (String, String)
splitOn4 c s = second (drop 1) $ break (== c) s

-- Note: A more general version of second actually exists in Data.Bifunctor!

-- tokenizeS: Transform SJSON into tokens
-- Strings should become one whole token, whitespace removed, and special chars
-- '{', '}', ':' and ',' should become tokens themselves. Tricky part is 
-- dealing with tokenizing whole strings, where splitOn will be helpful to get
-- until the next '"'. No escape sequences and stuff of course!

tokenizeS :: String -> [String]
tokenizeS "" = []
tokenizeS (c:cs) -- elem has probably slightly worse perf, but makes for concise code
  | c `elem` " \n\t" = tokenizeS cs -- whitespace, ignore
  | c `elem` "{}:," = [c] : tokenizeS cs -- special characters, tokenize as-is
  | c == '\'' = let (string, restOfS) = splitOn '\'' cs -- string, get the rest
                 in string : tokenizeS restOfS
  | otherwise = error $ "Unexpected character " ++ [c] ++ " in SJSON string"
  -- the error case is not important since input will be valid

-- prettifyS: Prettify SJSON, best done on tokens
-- Essentially, we only need to manage indentation levels using { and }
-- Other than that, printing a space after ':', a newline after ','
-- and enclosing strings in quotes '"' is all that is necessary.

prettifyS :: String -> String
prettifyS s = prettify 0 $ tokenizeS s -- < a helper to keep track of the depth
  where prettify _ [] = "" -- no tokens left, quit
        prettify n (token:rest) = decideOn token
          where decideOn "{" = "{\n" ++ indent (n + 1) ++ prettify (n + 1) rest -- incr indent
                decideOn "}" = "\n" ++ indent (n - 1) ++ "}" ++ prettify (n - 1) rest -- dec ind
                decideOn ":" = ": " ++ prettify n rest -- one space after ':'
                decideOn "," = ",\n" ++ indent n ++ prettify n rest -- newline after ','
                decideOn string = "'" ++ string ++ "'" ++ prettify n rest -- quote string
        indent x = replicate (4 * x) ' ' -- indentation will be 4 spaces per level

-- Here's the cool thing about where: it captures bindings of the containing scope, namely
-- the function parameters. For example, prettify can access 's' even though it's not necessary.
-- Better: decideOn can access 'n', 'token' and 'rest'. This makes decideOn much cleaner to
-- write as we only match on the token and we don't need any extra parameters which make
-- the function longer.

