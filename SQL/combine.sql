BEGIN TRANSACTION ISOLATION LEVEL SERIALIZABLE;

UPDATE SoldHouses
SET buyerID = c.buyerID, soldPrice = c.soldPrice, soldDate = CURRENT_DATE
FROM SoldHouseChanges c
WHERE c.houseID = SoldHouses.houseID 
      AND c.forSaleDate = SoldHouses.forSaleDate;

INSERT INTO SoldHouses
	SELECT c.houseID, c.forSaleDate, NULL, c.buyerID, c.soldPrice
	FROM SoldHouseChanges c
	WHERE (c.houseID, c.forSaleDate) NOT IN
		(SELECT houseID,forSaleDate
		 FROM SoldHouses);


COMMIT;