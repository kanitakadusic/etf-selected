ALTER SESSION SET CURRENT_SCHEMA = erd;

--

SELECT * FROM kontinent;
SELECT * FROM drzava;
SELECT * FROM grad;
SELECT * FROM lokacija;

SELECT * FROM fizicko_lice;
SELECT * FROM pravno_lice;

SELECT * FROM proizvodjac;
SELECT * FROM uposlenik;
SELECT * FROM kupac;

SELECT * FROM ugovor_za_pravno_lice;
SELECT * FROM ugovor_za_uposlenika;

SELECT * FROM odjel;
SELECT * FROM skladiste;
SELECT * FROM kategorija;

SELECT * FROM kurirska_sluzba;
SELECT * FROM narudzba_proizvoda;
SELECT * FROM isporuka;

SELECT * FROM proizvod;
SELECT * FROM kolicina;
SELECT * FROM garancija;
SELECT * FROM popust;
SELECT * FROM faktura;

--1.

SELECT DISTINCT p.naziv AS resnaziv
FROM pravno_lice p, fizicko_lice f, lokacija l
WHERE p.lokacija_id = l.lokacija_id AND
      f.lokacija_id = l.lokacija_id AND
      p.lokacija_id = f.lokacija_id;

--2.

SELECT DISTINCT To_Char(u.datum_potpisivanja, 'DD.MM.YYYY') "Datum Potpisivanja",
                p.naziv AS resnaziv
FROM ugovor_za_pravno_lice u, pravno_lice p
WHERE u.pravno_lice_id = p.pravno_lice_id(+) AND
      u.datum_potpisivanja > (SELECT Min(f.datum_kupoprodaje)
                              FROM narudzba_proizvoda n, faktura f, proizvod p
                              WHERE n.faktura_id = f.faktura_id AND
                                    n.proizvod_id = p.proizvod_id AND
                                    p.broj_mjeseci_garancije IS NOT NULL);

--3.

SELECT naziv
FROM proizvod
WHERE kategorija_id IN (SELECT p.kategorija_id
                        FROM proizvod p, kolicina k
                        WHERE k.proizvod_id = p.proizvod_id
                        GROUP BY p.kategorija_id
                        HAVING Sum(k.kolicina_proizvoda) = (SELECT Max(Sum(kolicina_proizvoda))
                                                            FROM kolicina
                                                            GROUP BY proizvod_id));

--4.

SELECT pd.naziv "Proizvod",
       pl.naziv "Proizvodjac"
FROM pravno_lice pl, proizvodjac pc, proizvod pd
WHERE pc.proizvodjac_id = pl.pravno_lice_id AND
      pd.proizvodjac_id = pc.proizvodjac_id AND
      pd.proizvodjac_id IN (SELECT proizvodjac_id
                            FROM proizvod
                            WHERE cijena > (SELECT Avg(cijena) FROM proizvod));

--5.

SELECT l.ime || ' ' || l.prezime "Ime i prezime",
       Sum(f.iznos) "iznos"
FROM fizicko_lice l, uposlenik u, kupac k, faktura f
WHERE u.uposlenik_id = l.fizicko_lice_id AND
      k.kupac_id = l.fizicko_lice_id AND
      u.uposlenik_id = k.kupac_id AND
      f.kupac_id = k.kupac_id
GROUP BY l.ime, l.prezime
HAVING Sum(f.iznos) > (SELECT Round(Avg(Sum(f1.iznos)), 2)
                       FROM faktura f1, kupac k1, fizicko_lice l1
                       WHERE f1.kupac_id = k1.kupac_id AND
                             k1.kupac_id = l1.fizicko_lice_id
                       GROUP BY l1.ime, l1.prezime);

--6.

SELECT p.naziv "naziv"
FROM pravno_lice p, kurirska_sluzba k, isporuka i, faktura f, narudzba_proizvoda n, popust o
WHERE k.kurirska_sluzba_id = p.pravno_lice_id AND
      i.kurirska_sluzba_id = k.kurirska_sluzba_id AND
      f.isporuka_id = i.isporuka_id AND
      n.faktura_id = f.faktura_id AND
      n.popust_id = o.popust_id AND
      o.postotak > 0
GROUP BY p.naziv
HAVING Sum(n.kolicina_jednog_proizvoda) = (SELECT Max(Sum(n1.kolicina_jednog_proizvoda))
                                           FROM kurirska_sluzba k1, isporuka i1, faktura f1, narudzba_proizvoda n1, popust o1
                                           WHERE i1.kurirska_sluzba_id = k1.kurirska_sluzba_id AND
                                                 f1.isporuka_id = i1.isporuka_id AND
                                                 n1.faktura_id = f1.faktura_id AND
                                                 n1.popust_id = o1.popust_id AND
                                                 o1.postotak > 0
                                           GROUP BY k1.kurirska_sluzba_id);

--7.

SELECT l.ime || ' ' || l.prezime "Kupac",
       Sum(n.kolicina_jednog_proizvoda * p.cijena * Nvl(o.postotak, 0) / 100) "Usteda",
       '19327' "Indeks"
FROM fizicko_lice l, kupac k, faktura f, narudzba_proizvoda n, proizvod p, popust o
WHERE k.kupac_id = l.fizicko_lice_id AND
      f.kupac_id = k.kupac_id AND
      n.faktura_id = f.faktura_id AND
      n.proizvod_id = p.proizvod_id AND
      n.popust_id = o.popust_id
GROUP BY l.ime, l.prezime;

--8.

SELECT DISTINCT i.isporuka_id AS idisporuke,
                i.kurirska_sluzba_id AS idkurirske
FROM isporuka i, faktura f, narudzba_proizvoda n, proizvod p, popust o
WHERE f.isporuka_id = i.isporuka_id AND
      n.faktura_id = f.faktura_id AND
      n.proizvod_id = p.proizvod_id AND
      n.popust_id = o.popust_id AND
      o.postotak > 0 AND
      p.broj_mjeseci_garancije > 0;

--9.

SELECT naziv, cijena
FROM proizvod
WHERE cijena > (SELECT Round(Avg(Max(cijena)), 2)
                FROM proizvod
                GROUP BY kategorija_id);

--10.

SELECT p1.naziv, p1.cijena
FROM proizvod p1
WHERE p1.cijena < ALL (SELECT Avg(p2.cijena)
                       FROM proizvod p2, kategorija k2
                       WHERE p2.kategorija_id = k2.kategorija_id AND
                             k2.nadkategorija_id <> p1.kategorija_id
                       GROUP BY p2.kategorija_id);
