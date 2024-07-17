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

SELECT k.naziv AS kontinent,
       Nvl(d.naziv, 'Nema drzave') AS drzava,
       Nvl(g.naziv, 'Nema grada') AS grad,
       Count(l.lokacija_id) AS brojlokacija
FROM kontinent k, drzava d, grad g, lokacija l
WHERE d.kontinent_id(+) = k.kontinent_id AND
      g.drzava_id(+) = d.drzava_id AND
      l.grad_id(+) = g.grad_id
GROUP BY k.naziv,
         d.naziv,
         g.naziv;

--2.

SELECT p.naziv AS naziv
FROM pravno_lice p, ugovor_za_pravno_lice u
WHERE u.pravno_lice_id = p.pravno_lice_id AND
      u.datum_raskidanja IS NULL AND
      Mod(To_Number(To_Char(u.datum_potpisivanja, 'yyyy')), 2) = 0;

--3.

SELECT d.naziv AS drzava,
       p.naziv AS proizvod,
       k.kolicina_proizvoda AS kolicina_proizvoda
FROM drzava d, grad g, lokacija l, skladiste s, kolicina k, proizvod p
WHERE g.drzava_id(+) = d.drzava_id AND
      l.grad_id(+) = g.grad_id AND
      s.lokacija_id(+) = l.lokacija_id AND
      k.skladiste_id(+) = s.skladiste_id AND
      k.proizvod_id = p.proizvod_id(+) AND
      k.kolicina_proizvoda > 50 AND
      Lower(d.naziv) NOT LIKE '%ss%';

--4.

SELECT DISTINCT pr.naziv,
                pr.broj_mjeseci_garancije
FROM proizvod pr, popust po, narudzba_proizvoda n
WHERE n.proizvod_id(+) = pr.proizvod_id AND
      n.popust_id = po.popust_id(+) AND
      Nvl(po.postotak, 0) > 0 AND
      Mod(pr.broj_mjeseci_garancije, 3) = 0;

--5.

SELECT f.ime || ' ' || f.prezime "ime i prezime",
       o.naziv "Naziv odjela",
       '19327' "Indeks"
FROM fizicko_lice f, uposlenik u, kupac k, odjel o
WHERE k.kupac_id = f.fizicko_lice_id AND
      u.uposlenik_id = f.fizicko_lice_id AND
      u.odjel_id = o.odjel_id AND
      u.uposlenik_id = k.kupac_id AND
      u.uposlenik_id <> o.sef_id;

--6.

SELECT pr.naziv AS proizvod,
       n.narudzba_id AS narudzba_id,
       pr.cijena AS cijena,
       Nvl(po.postotak, 0) AS postotak,
       Nvl(po.postotak, 0) / 100 AS postotakrealni
FROM narudzba_proizvoda n, proizvod pr, popust po
WHERE n.proizvod_id = pr.proizvod_id(+) AND
      n.popust_id = po.popust_id(+) AND
      Nvl(po.postotak, 0) / 100 * pr.cijena < 200;

--7.

SELECT Decode(k.kategorija_id, 1, 'Komp Oprema', NULL, 'Nema Kategorije', k.naziv) "Kategorija",
       Decode(n.kategorija_id, 1, 'Komp Oprema', NULL, 'Nema Kategorije', n.naziv) "Nadkategorija"
FROM kategorija k, kategorija n
WHERE k.nadkategorija_id = n.kategorija_id(+);

--8.

SELECT Nvl(datum_raskidanja, Add_Months(datum_potpisivanja, 24)) AS raskid
FROM ugovor_za_pravno_lice
WHERE To_Number(SubStr(ugovor_id, 1, 2)) <= 50;

--9.

SELECT f.ime AS ime,
       f.prezime AS prezime,
       Decode(o.naziv, 'Management', 'MANAGER', 'Human Resources', 'HUMAN', 'OTHER') AS odjel,
       o.odjel_id AS odjel_id
FROM uposlenik u, fizicko_lice f, odjel o
WHERE u.uposlenik_id = f.fizicko_lice_id(+) AND
      u.odjel_id = o.odjel_id(+)
ORDER BY 1, 2 DESC;

--10.

SELECT k.naziv,
      (SELECT p1.naziv FROM proizvod p1
       WHERE p1.kategorija_id = k.kategorija_id AND
             p1.cijena = (SELECT Max(p2.cijena) FROM proizvod p2 WHERE p2.kategorija_id = k.kategorija_id)) AS najskuplji,

      (SELECT p1.naziv FROM proizvod p1
       WHERE p1.kategorija_id = k.kategorija_id AND
             p1.cijena = (SELECT Min(p2.cijena) FROM proizvod p2 WHERE p2.kategorija_id = k.kategorija_id)) AS najjeftiniji,

      (SELECT Min(p.cijena) + Max(p.cijena) FROM proizvod p WHERE p.kategorija_id = k.kategorija_id) AS zcijena
FROM kategorija k
ORDER BY 4;















SELECT k.naziv AS kategorija,
       maks.naziv AS najskuplji,
       mini.naziv AS najjeftiniji,
       Max(p.cijena) + Min(p.cijena) AS zcijena
FROM proizvod p,
     kategorija k,
     (SELECT kategorija_id, naziv FROM proizvod WHERE cijena IN (SELECT Max(cijena) FROM proizvod GROUP BY kategorija_id)) maks,
     (SELECT kategorija_id, naziv FROM proizvod WHERE cijena IN (SELECT Min(cijena) FROM proizvod GROUP BY kategorija_id)) mini
WHERE p.kategorija_id = k.kategorija_id AND
      maks.kategorija_id = k.kategorija_id AND
      mini.kategorija_id = k.kategorija_id
GROUP BY k.naziv, maks.naziv, mini.naziv
ORDER BY 4;


SELECT k.naziv,
       maks.naziv AS najskuplji,
       mini.naziv AS najjeftiniji,
       Max(p.cijena) + Min(p.cijena) AS zcijena
FROM proizvod p,
     kategorija k,

     (SELECT kategorija_id, naziv
      FROM proizvod
      WHERE kategorija_id IN (SELECT kategorija_id FROM (SELECT kategorija_id, Max(cijena) AS maks FROM proizvod GROUP BY kategorija_id)) AND
            cijena IN (SELECT maks FROM (SELECT kategorija_id, Max(cijena) AS maks FROM proizvod GROUP BY kategorija_id))) maks,

     (SELECT kategorija_id, naziv
      FROM proizvod
      WHERE kategorija_id IN (SELECT kategorija_id FROM (SELECT kategorija_id, Min(cijena) AS mini FROM proizvod GROUP BY kategorija_id)) AND
            cijena IN (SELECT mini FROM (SELECT kategorija_id, Min(cijena) AS mini FROM proizvod GROUP BY kategorija_id))) mini

WHERE p.kategorija_id = k.kategorija_id AND
      maks.kategorija_id = k.kategorija_id AND
      mini.kategorija_id = k.kategorija_id
GROUP BY k.naziv, maks.naziv, mini.naziv
ORDER BY 4;


SELECT k.naziv,
       p.naziv AS najskuplji,
       p.naziv AS najjeftiniji,
       Max(p.cijena) + Min(p.cijena) AS zcijena
FROM proizvod p,
     kategorija k,

     (SELECT kategorija_id, Max(cijena) AS max_cijena FROM proizvod GROUP BY kategorija_id) kid_max,
     (SELECT kategorija_id, Min(cijena) AS min_cijena FROM proizvod GROUP BY kategorija_id) kid_min,

     (SELECT p.proizvod_id, p.naziv FROM proizvod p, kid_max k WHERE p.kategorija_id = k.kategorija_id) naziv_max
WHERE p.kategorija_id = k.kategorija_id AND
      kid_max.kategorija_id = k.kategorija_id AND
      kid_min.kategorija_id = k.kategorija_id
GROUP BY k.naziv, p.naziv
ORDER BY 4;
