CREATE OR UPDATE FUNCTION increaseSomeOfferPricesFunction() RETURNS quantity
LANGUAGE plpgsql
AS $$

BEGIN

    RETURN quantity;
END;
$$ 


CREATE FUNCTION increaseSomeOfferPricesFunction() RETURNS integer AS $$

DECLARE
    quantity integer := 30;
BEGIN
    RAISE NOTICE 'Quantity here is %', quantity;  -- Prints 30
    quantity := 50;

    DECLARE
        quantity integer := 80;
    BEGIN
        RAISE NOTICE 'Quantity here is %', quantity;  -- Prints 80
        RAISE NOTICE 'Outer quantity here is %', outerblock.quantity;  -- Prints 50
    END;

    RAISE NOTICE 'Quantity here is %', quantity;  -- Prints 50

    RETURN quantity;
END;
$$ LANGUAGE plpgsql;
