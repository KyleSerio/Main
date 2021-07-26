SELECT s.houseid AS thehouseID, h.ownerid AS theownerID, s.buyerid AS thebuyerID, 
       s.soldprice, f.forsaleprice, b.brokerid AS thebrokerid, b.companyname AS thecompany
FROM brokers b, soldhouses s, forsalehouses f, houses h, persons p
WHERE     (s.buyerid = p.personid)
      AND (b.brokerid = f.brokerid)
      AND (f.houseid = s.houseid)
      AND (h.houseid = f.houseid)
      AND (p.personname LIKE 'S%')
      AND (f.forsaledate = s.forsaledate)
      AND (s.solddate >= '2020-02-10' AND s.solddate <= '2020-04-29')
      AND (s.soldprice > f.forsaleprice)
      AND b.brokerid = (SELECT f.brokerid
			FROM forsalehouses f
			WHERE f.brokerid = b.brokerid
			  AND f.isstillforsale = true
			  AND f.houseid <> s.houseid)
    


/**/