ALTER TABLE Brokers
ADD FOREIGN KEY (brokerID) REFERENCES Persons(personID);

ALTER TABLE Offers
ADD FOREIGN KEY (offererID) REFERENCES Persons(personID) 
				       ON DELETE CASCADE 
				       ON UPDATE CASCADE;

ALTER TABLE SoldHouses
ADD FOREIGN KEY (buyerID) REFERENCES Persons(personID)
				     ON DELETE SET NULL
				     ON UPDATE CASCADE;


