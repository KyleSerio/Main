SELECT b.brokerid, p.personname
FROM brokers b, persons p
WHERE b.brokerid = p.personid
	AND b.companyname = 'Weathervane Group Realty'
	AND b.brokerid IN (SELECT h.brokerid
		     FROM forsalehouses h, soldhouses s
		     WHERE h.forsaledate < '2020-10-01'
			   AND h.isstillforsale = false
			   AND h.forsaledate = s.forsaledate 
			   AND h.houseid = s.houseid
			   AND s.soldprice >= 1000000)
	     