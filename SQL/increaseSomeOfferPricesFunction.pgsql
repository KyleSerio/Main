CREATE FUNCTION increaseSomeOfferPricesFunction(theOffererID integer, numOfferIncreases integer) 
RETURNS integer AS 
$BODY$
DECLARE
	index integer := 0;
	rec record;
BEGIN


	FOR rec IN SELECT *
		FROM Offers o
		WHERE theOffererID = o.offererID AND
			o.isACurrentOffer = TRUE
		ORDER BY o.offerPrice
	LOOP

		IF index < numOfferIncreases THEN

			UPDATE Offers
			SET offerPrice = offerPrice + 8000
			WHERE offererID = theOffererID AND
				houseID = rec.houseID AND
				forSaleDate = rec.forSaleDate AND
				offerDate = rec.offerDate;

			index := index + 1;
		END IF;

	END LOOP;


	RETURN index;
END;
$BODY$
LANGUAGE plpgsql;