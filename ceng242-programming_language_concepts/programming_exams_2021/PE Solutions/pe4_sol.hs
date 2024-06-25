module PE4 where

import Data.Maybe -- optional

-- Generic DictTree definition
data DictTree k v = Node [(k, DictTree k v)] | Leaf v deriving Show

newtype Digit = Digit Char deriving (Show, Eq, Ord) -- derive equality and comparison too!

type DigitTree = DictTree Digit String
type PhoneNumber = [Digit]

--------------------------

----------
-- Part I:
-- Some Maybe fun! Basic practice with an existing custom datatype.

-- Q1: toDigit
-- Safely convert a character to a digit (10 pts)

-- I saw way too many guards/ors for digits! Remember that they're comparable...
toDigit :: Char -> Maybe Digit
toDigit c = if '0' <= c && c <= '9'
               then Just $ Digit c
               else Nothing

-- Q2: toDigits
-- Safely convert a bunch of characters to digits. (20 pts)
-- Particularly, an empty string should fail.

-- A special case for the empty string, and then two nested pattern matches.
toDigits :: String -> Maybe PhoneNumber
toDigits "" = Nothing
toDigits digs = combineDigits $ map toDigit digs
  where combineDigits :: [Maybe Digit] -> Maybe [Digit]
        combineDigits [] = Just []
        combineDigits (Nothing:_) = Nothing
        combineDigits ((Just d):xs) = let mds = combineDigits xs
                                       in case mds of Nothing -> Nothing
                                                      (Just ds) -> Just (d:ds)

-- This is what the more advanced Haskell typeclasses exist for though! Of course
-- you were not supposed to know this, but I wanted to tease them a little :3

-- Using the Applicative instance of Maybe (more general)
toDigits2 :: String -> Maybe PhoneNumber
toDigits2 = traverse toDigit

-- Using the Monad instance of Maybe
toDigits3 :: String -> Maybe PhoneNumber
toDigits3 = mapM toDigit

-----------
-- Part II:
-- Some phonebook trie business.

-- Q1: numContacts
-- Count the number of contacts in the phonebook... (10 pts)

-- Easy peasy with a nice map! How does it work? Well, entries is of
-- type [(Int, DigitTree)]. For each element (using map), we get the
-- tree and disregard the digit using snd which will turn (Int, DigitTree) into
-- DigitTree. Then, we apply numContacts to the subtree which gives us the
-- number of contacts in the subtree, transforming our DigitTree to Int.
-- This leaves us with the number of contacts of each subtree [Int],
-- and we just sum them up! Note the function composition: numContacts . snd
numContacts :: DigitTree -> Int
numContacts (Node entries) = sum $ map (numContacts . snd) entries
numContacts (Leaf _) = 1
    

-- Q2: getContacts
-- Generate the contacts and their phone numbers given a tree (30 pts)

-- This is a bit more complicated to unpack! But let's do it anyway...
getContacts :: DigitTree -> [(PhoneNumber, String)]
getContacts (Leaf name) = [([], name)] -- Base case. A name with no phone number (yet!)
getContacts (Node entries) = -- Aaaand it's pretty much a one-liner :)
    concatMap (\(p, child) -> map (first (p:)) $ getContacts child) entries

-- First, see below for 'first'. Now, it's nice to think in terms of data transformations
-- here. What we want to do is essentially get the contacts from each subtree and gathe
-- them, but also add the prefixing digit in the entry to each contact in the subtree.
--
-- If we simply wanted to gather the contacts from the children, we could do this:
-- concatMap (\(_, child) -> getContacts child) entries
--
-- With this, we would be applying getContacts to each child (deconstructing the tuple
-- in the lambda) with the Map part, and gathering the results via concat. (Remember
-- that concatMap = concat . map). This would leave us with empty phone numbers
-- due to the base case, but it's a start!
--
-- The only extra thing we need to do is add the prefix digit 'p' in the entry to each
-- contact in the subtree, to properly compose their phone number. This calls
-- for a map. Prefixing can be done using (:) in the form of a section (p:). But,
-- we want to apply this function to the first element of the tuple only, which is
-- why we transform it via 'first'. We could still handle it with another pattern
-- matching lambda though, like this:
-- concatMap (\(p, child) -> map (\(num, person) -> (p:num, person)) $ getContacts child) entries
--
-- Slightly less abstract and a bit uglier, which is why I went with first. 
-- Hope this convinces you that higher order functions can be pretty awesome!
-- Maybe you already did though... If so, good going!

-- Helper for applying function to first tuple element. More general version exists
-- in Data.Bifunctor as a built-in. Cool stuff...
first :: (a -> c) -> (a, b) -> (c, b)
first f (x, y) = (f x, y)


-- Q3: autocomplete (30 pts)
-- Create an autocomplete list of contacts given a prefix (relies on getContacts and toDigits)
-- e.g. autocomplete [Digit '3', Digit '2'] areaCodes -> 
--      [([Digit '2'], "Adana"), ([Digit '6'], "Hatay"), ([Digit '8'], "Osmaniye")]

-- This is actually easy, but relies on the previous toDigits and getContacts.
-- If toDigits fails on the string, then we can return an empty list immediately.
-- If it succeeds, we will walk through the digits one by one while going down the
-- tree. See below for what happens at each pattern match!
autocomplete :: String -> DigitTree -> [(PhoneNumber, String)]
autocomplete string tree = case toDigits string of Nothing -> []
                                                   (Just digits) -> autocomplete' digits tree
  where
    autocomplete' [] tree = getContacts tree -- consumed all the digits, generate the prefixes!
    autocomplete' _ (Leaf _) = [] -- still have digits (otherwise would match above), but path ended! Fails to match anything.
    autocomplete' (d:ds) (Node entries) = -- At an internal node, have to choose the correct subtree
        case lookup d entries of Nothing -> [] -- d not in the entries, can't move on, stop
                                 (Just child) -> autocomplete' ds child -- continue!

