ALTER TABLE SoldHouses ADD CONSTRAINT positive_soldPrice
	CHECK (soldPrice > 0);

ALTER TABLE Brokers ADD CONSTRAINT valid_level
	CHECK (brokerLevel = 'A' OR 
		brokerLevel = 'B' OR 
		brokerLevel = 'C' OR 
		brokerLevel = 'D' OR
		brokerLevel IS NULL);

ALTER TABLE Offers ADD CONSTRAINT price_match
	CHECK(offerPrice IS NOT NULL OR
	     (offerPrice IS NULL AND isACurrentOffer IS NULL));