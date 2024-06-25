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

-------------------------------------- PART I ---------------------------------------------

isInGrid :: Grid -> Coordinate -> Bool
isInGrid grid (x,y)
                | x < 0 || x >= length (grid !! 0) = False      -- check for negative numbers and numbers exceeding the border
                | y < 0 || y >= length grid = False
                | otherwise = True

-------------------------------------------------------------------------------------------

countCell (Rock r) = r
countCell _ = 0

countRow [] result = result
countRow (c:cs) result = countRow cs (result + countCell c)  -- counting the rocks for given row cell by cell


totalCount :: Grid -> Int
totalCount grid = sum [countRow row 0 | row <- grid ]       -- counting the rocks in 2D row by row

-------------------------------------------------------------------------------------------

isPit grid (x,y) = (grid !! y) !! x == Pit

coordinatesOfPits :: Grid -> [Coordinate]
coordinatesOfPits grid = [(x,y) | x <- [0..maxX], y <- [0..maxY], isPit grid (x,y)]         -- list comprehension by filtering the coordinates depending on being a pit or not
                         where maxX = (length (grid !! 0)) - 1
                               maxY = (length grid) - 1

-------------------------------------------------------------------------------------------

newEnergy energy PickUp = energy - 5
newEnergy energy PutDown = energy - 3
newEnergy energy _ = energy - 1

isEnergyEnough energy PickUp = energy >= 5
isEnergyEnough energy PutDown = energy >= 3
isEnergyEnough energy _ = energy >= 1


moveRobot (x,y) m = case m of North -> (x, y-1)
                              East -> (x+1, y)
                              South -> (x, y+1)
                              West -> (x-1, y)
                              _ -> (x, y)           -- PickUp and PutDown does not affect the location

nextCoordinate grid coor m = if isPit grid coor then coor else if isInGrid grid nextCoor then nextCoor else coor
                             where nextCoor = moveRobot coor m


-- helper function for tracePath that accumulates the result in the last argument
tracePathHelper _ 0 coor moves result = result ++ (take (length moves) $ repeat coor)       -- if energy is 0, the rest of the coordinates will be the same, hence repeat is used.
tracePathHelper grid energy coor [] result = result
tracePathHelper grid energy coor moves@(m:ms) result = if isEnergyEnough energy m then tracePathHelper grid (newEnergy energy m) newCoor ms (result ++ [newCoor])
                                                       else tracePathHelper grid 0 coor moves result
                                                       where newCoor = nextCoordinate grid coor m

tracePath :: Grid -> Robot -> [Move] -> [Coordinate]
tracePath grid robot moves = tracePathHelper grid (energy robot) (location robot) moves []      -- tracePath is interested in only energy and location of the robot.

------------------------------------- PART II ----------------------------------------------

isSpaceCraft (SpaceCraft _) = True
isSpaceCraft _ = False

getSpaceCraftLoc grid = [(x,y) | x <- [0..maxX], y <- [0..maxY], isSpaceCraft (( grid !!y ) !! x)] !! 0
                         where maxX = (length (grid !! 0)) - 1
                               maxY = (length grid) - 1

additionalEnergy (cx,cy) (lx,ly) = max 0 (100 - (abs (cx - lx) + abs (cy - ly)) * 20)       -- additional energy cannot be negative

calculateEnergy coor location energy = min (energy + (additionalEnergy coor location)) 100      -- total energy cannot exceed 100

energiseRobot coor (Robot name location capacity energy storage) = (Robot name location capacity (calculateEnergy coor location energy) storage)        -- calculation of energy for single robot


-- helper function for energiseRobots that accumulates the result in the last argument
energiseRobotsHelper _ [] result = result
energiseRobotsHelper coor (r:rs) result = energiseRobotsHelper coor rs (result ++ [energiseRobot coor r])

energiseRobots :: Grid -> [Robot] -> [Robot]
energiseRobots grid robots = energiseRobotsHelper sCoor robots []
                             where sCoor = getSpaceCraftLoc grid        -- get the location of SpaceCraft

-------------------------------------------------------------------------------------------

-- Helper functions below reconstruct the grid row by row, and updating the related cells for the PickUp and PutDown functions -- 

reduceRockCell (Rock r) = (Rock (r-1))

updateCellForRock cell (j,i) (x,y) = if j == x && i == y then reduceRockCell cell else cell

updateRowForRock row i (x,y) = [ updateCellForRock (row !! j) (j,i) (x,y) | j <- [0..maxX]]
                               where maxX = (length row) - 1

reduceRock grid coor = [updateRowForRock (grid !! i) i coor | i <- [0..maxY]]
                       where maxY = (length grid) - 1

increaseRockInSC (SpaceCraft r) = (SpaceCraft (r+1))

updateCellForSpaceCraft cell (j,i) (x,y) = if j == x && i == y then increaseRockInSC cell else cell

updateRowForSpaceCraft row i (x,y) = [ updateCellForSpaceCraft (row !! j) (j, i) (x,y) | j <- [0..maxX]]
                                     where maxX = (length row) - 1

getRockToSpaceCraft grid coor = [updateRowForSpaceCraft (grid !! i) i coor | i <- [0..maxY]]
                                where maxY = (length grid) - 1

-- this function updates the grid for given move. PickUp and PutDown requires the reconstruction of grid
updateGrid grid (Robot _ _ _ 0 _) _ = grid      -- robot has no energy, grid will be the same in any case.
updateGrid grid robot PickUp = if hasRock ((grid !! y) !! x) && (capacity robot) > (storage robot) then reduceRock grid (location robot) else grid
                               where x = fst $ location robot
                                     y = snd $ location robot
updateGrid grid robot PutDown = if isSpaceCraft ((grid !! y) !! x) then getRockToSpaceCraft grid (x,y) else grid
                                where x = fst $ location robot
                                      y = snd $ location robot
updateGrid grid robot _ = grid          -- navigational actions do not affect the grid

hasRock (Rock 0) = False
hasRock (Rock _) = True
hasRock _ = False


-- this function updates the robot for given move. PickUp and PutDown needs special conditions
updateRobot grid robot@(Robot name location capacity 0 storage) _ = robot       -- no energy, robot will stay same
updateRobot grid (Robot name location@(x,y) capacity energy storage) PickUp = if hasRock ((grid !! y) !! x) && capacity > storage then (Robot name location capacity (energy - 5) (storage + 1)) else (Robot name location capacity (energy - 5) storage)
updateRobot grid (Robot name location@(x,y) capacity energy storage) PutDown = if isSpaceCraft ((grid !! y) !! x) then (Robot name location capacity (energy - 3) (storage - 1)) else (Robot name location capacity (energy - 3) storage)
updateRobot grid (Robot name location capacity energy storage) m = (Robot name newLocation capacity (energy-1) storage)
                                                                       where newLocation = nextCoordinate grid location m

setEnergyZero (Robot name location capacity energy storage) = (Robot name location capacity 0 storage)

applyMoves :: Grid -> Robot -> [Move] -> (Grid, Robot)
applyMoves grid robot [] = (grid, robot) -- no more moves, reached the result
applyMoves grid robot (m:ms) = if isEnergyEnough (energy robot) m then applyMoves newGrid newRobot ms       -- if robot does not have enough energy (already implemented for tracePath), then set its energy to 0.
                               else (grid, setEnergyZero robot)
                               where newGrid = updateGrid grid robot m      -- updating the grid for given move
                                     newRobot = updateRobot grid robot m    -- updating the robot for given move
