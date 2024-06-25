module PE1 where

import Text.Printf


-- This function takes a Double and rounds it to 2 decimal places as requested in the PE --
getRounded :: Double -> Double
getRounded x = read s :: Double
               where s = printf "%.2f" x

-------------------------------------------------------------------------------------------
----------------------- DO NOT CHANGE ABOVE OR FUNCTION SIGNATURES-------------------------
------------- DUMMY IMPLEMENTATIONS ARE GIVEN TO PROVIDE A COMPILABLE TEMPLATE ------------
------------------- REPLACE THEM WITH YOUR COMPILABLE IMPLEMENTATIONS ---------------------
-------------------------------------------------------------------------------------------

convertTL :: Double -> String -> Double
convertTL money "USD" = getRounded (money / 8.18)
convertTL money "EUR" = getRounded (money / 9.62)
convertTL money "BTC" = getRounded (money / 473497.31)

-------------------------------------------------------------------------------------------

countOnWatch :: [String] -> String -> Int -> Int
countOnWatch schedule employee 0 = 0
countOnWatch (frst:rest) employee days = if frst==employee 
                                            then 1 +countOnWatch (rest) employee (days-1)
                                            else countOnWatch (rest) employee (days-1)
                                            

-------------------------------------------------------------------------------------------

encrypt :: Int -> Int
encrypt x =  helper (x`mod` 10) + helper ((x `div` 10)`mod` 10) * 10 + helper ((x `div` 100)`mod` 10) * 100 + helper ((x `div` 1000)`mod` 10) * 1000
helper x 
    | x`mod`3 == 0 = (x-1) `mod` 10
    | x`mod`4 == 0 = (x*2) `mod` 10
    | x`mod`5 == 0 = (x+3) `mod` 10
    | otherwise = (x+4) `mod` 10

-------------------------------------------------------------------------------------------

compoundInterests :: [(Double, Int)] -> [Double]
compoundInterests investments = [rate x | x <-investments]
rate (money,year)
    | (money >= 10000) && (year >= 2) = getRounded (money * (1 + 0.115/12)^(12*year))
    | (money >= 10000) && (year < 2) = getRounded (money * (1 + 0.105/12)^(12*year))
    | (money < 10000) && (year >= 2) = getRounded (money * (1 + 0.095/12)^(12*year))
    | otherwise = getRounded (money * (1 + 0.09/12)^(12*year))




