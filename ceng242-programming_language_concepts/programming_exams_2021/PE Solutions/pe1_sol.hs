module PE1 where

import Text.Printf


-- This function takes a Double and rounds it to 2 decimal places as requested in the PE --
getRounded :: Double -> Double
getRounded x = read s :: Double
               where s = printf "%.2f" x

-------------------------------------------------------------------------------------------

-- converting the given amount of turkish liras to other currencies
convertTL :: Double -> String -> Double
convertTL money currency = getRounded $ convertTLHelper money currency

convertTLHelper money currency = case currency of "EUR" -> money / 9.62
                                                 "USD" -> money / 8.18
                                                 "BTC" -> money / 473497.31

-------------------------------------------------------------------------------------------

countOnWatch :: [String] -> String -> Int -> Int
countOnWatch arr employee days = length [x | x <- take days arr, x == employee]     -- list comprehension by taking its elements as many as given days and filtering with the given employee

-------------------------------------------------------------------------------------------

regenerate [] x = x
regenerate all@(d:ds) x = regenerate ds ((10^(length all - 1) * d) + x) -- this function calculates the actual number from given digits

digits 0 = []
digits x = digits (x `div` 10) ++ [x `mod` 10]      -- this function produces the digits of given number

shift x
    | x `mod` 3 == 0 = (x - 1) `mod` 10
    | x `mod` 4 == 0 = (x * 2) `mod` 10
    | x `mod` 5 == 0 = (x + 3) `mod` 10
    | otherwise = (x + 4) `mod` 10

process d = [ shift x | x <- d ] -- list comprehension to process all digits one by one.

encrypt :: Int -> Int
encrypt x = regenerate (process $ digits x) 0  -- regenerate the password from partitioned and processed digits

-------------------------------------------------------------------------------------------

getAnnualRate (money, year) = if money >= 10000 then (if year >= 2 then 11.5 else 10.5) else (if year >= 2 then 9.5 else 9.0)   -- calculation of annual rate according to the given table

calculateOne inv@(money, year) = money * ((1 + (getAnnualRate inv) / (12 * 100)) ^ (12 * year))     -- calculation of an interest according to given money and year


compoundInterests :: [(Double, Int)] -> [Double]
compoundInterests investments = [getRounded $ calculateOne x | x <- investments]        -- calculation of interests, by using list comprehension

-------------------------------------------------------------------------------------------
