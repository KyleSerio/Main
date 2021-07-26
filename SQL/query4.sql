SELECT h.houseid, h.address , o.offerprice as highOffer
FROM houses h, offers o
WHERE o.offerprice = (SELECT MAX(o.offerprice)
			FROM offers o
			WHERE o.houseid = h.houseid)
			AND o.houseid = h.houseid

		    



