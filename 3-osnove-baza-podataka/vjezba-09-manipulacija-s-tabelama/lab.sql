SELECT * FROM employees;
SELECT * FROM departments;
SELECT * FROM locations;
SELECT * FROM countries;
SELECT * FROM regions;
SELECT * FROM jobs;

SELECT * FROM z19327 ORDER BY employee_id DESC;
SELECT * FROM o19327 ORDER BY department_id DESC;

SELECT * FROM odjeli_19327;
SELECT * FROM zaposleni_19327;
SELECT * FROM zaposleni_19327_backup;

-- ZADATAK 1

CREATE TABLE odjeli_19327 (
  id VARCHAR2(25) NOT NULL,
  naziv VARCHAR2(10) NOT NULL,
  opis CHAR(15),
  datum DATE NOT NULL,
  korisnik VARCHAR2(30) NOT NULL,
  napomena VARCHAR2(10)
);

ALTER TABLE odjeli_19327 MODIFY (naziv VARCHAR2(20));

-- ZADATAK 2

INSERT INTO odjeli_19327 (id, naziv, datum, korisnik)
SELECT department_id, department_name, SYSDATE, 'X' FROM departments;

-- ZADATAK 3

ALTER TABLE odjeli_19327 ADD (id_sef NUMBER);
ALTER TABLE odjeli_19327 ADD (id_lokacija NUMBER);

UPDATE odjeli_19327 o SET
id_sef = (SELECT d.manager_id FROM departments d WHERE d.department_id = o.id),
id_lokacija = (SELECT d.location_id FROM departments d WHERE d.department_id = o.id);

-- ZADATAK 4

CREATE TABLE zaposleni_19327 (
  id NUMBER(4) NOT NULL,
  sifra_zaposlenog VARCHAR2(5) NOT NULL,
  naziv_zaposlenog CHAR(50),
  godina_zaposlenja NUMBER(4) NOT NULL,
  mjesec_zaposlenja CHAR(2) NOT NULL,
  sifra_odjela VARCHAR2(5),
  naziv_odjela VARCHAR2(15) NOT NULL,
  grad CHAR(10) NOT NULL,
  sifra_posla VARCHAR2(25),
  naziv_posla CHAR(50) NOT NULL,
  iznos_dodatak_na_platu NUMBER(5),
  plata NUMBER(6) NOT NULL,
  kontinent VARCHAR2(20),
  datum_unosa DATE NOT NULL,
  korisnik_unio CHAR(20) NOT NULL
);

ALTER TABLE zaposleni_19327 MODIFY (id NUMBER(8));
ALTER TABLE zaposleni_19327 MODIFY (naziv_zaposlenog VARCHAR2(50));
ALTER TABLE zaposleni_19327 MODIFY (naziv_odjela VARCHAR2(20));
ALTER TABLE zaposleni_19327 MODIFY (naziv_posla VARCHAR2(50));
ALTER TABLE zaposleni_19327 MODIFY (grad CHAR(20));

-- ZADATAK 5

INSERT INTO zaposleni_19327
SELECT To_Number(To_Char(e.employee_id) || To_Char(d.department_id)),
       To_Char(e.employee_id),
       e.first_name || ' ' || e.last_name,
       To_Number(To_Char(e.hire_date, 'yyyy')),
       To_Char(e.hire_date, 'mm'),
       To_Char(d.department_id),
       d.department_name,
       l.city,
       To_Char(j.job_id),
       j.job_title,
       e.commission_pct,
       e.salary,
       r.region_name,
       SYSDATE,
       'X'
FROM employees e, departments d, locations l, countries c, regions r, jobs j
WHERE e.department_id = d.department_id AND
      d.location_id = l.location_id AND
      l.country_id = c.country_id AND
      c.region_id = r.region_id AND
      e.job_id = j.job_id;

-- ZADATAK 6

CREATE TABLE zaposleni_19327_nova AS
SELECT * FROM zaposleni_19327;

-- ZADATAK 7

ALTER TABLE zaposleni_19327_nova ADD (zaposleni VARCHAR2(60));
ALTER TABLE zaposleni_19327_nova ADD (odjel VARCHAR2(30));
ALTER TABLE zaposleni_19327_nova ADD (posao VARCHAR2(70));

UPDATE zaposleni_19327_nova n SET
n.zaposleni = (SELECT z.sifra_zaposlenog || ' =>> ' || z.naziv_zaposlenog FROM zaposleni_19327 z WHERE z.sifra_zaposlenog = n.sifra_zaposlenog),
n.odjel = (SELECT z.sifra_odjela || ' =>> ' || z.naziv_odjela FROM zaposleni_19327 z WHERE z.sifra_zaposlenog = n.sifra_zaposlenog),
n.posao = (SELECT z.sifra_posla || ' =>> ' || z.naziv_posla FROM zaposleni_19327 z WHERE z.sifra_zaposlenog = n.sifra_zaposlenog);

ALTER TABLE zaposleni_19327_nova DROP COLUMN sifra_zaposlenog;
ALTER TABLE zaposleni_19327_nova DROP COLUMN naziv_zaposlenog;
ALTER TABLE zaposleni_19327_nova DROP COLUMN sifra_odjela;
ALTER TABLE zaposleni_19327_nova DROP COLUMN naziv_odjela;
ALTER TABLE zaposleni_19327_nova DROP COLUMN sifra_posla;
ALTER TABLE zaposleni_19327_nova DROP COLUMN naziv_posla;

-- ZADATAK 8

RENAME zaposleni_19327_nova TO zaposleni_19327_backup;

-- ZADATAK 9

-- ZADATAK 10

-- ZADATAK 11

ALTER TABLE zaposleni_19327_backup SET UNUSED (datum_unosa);
ALTER TABLE zaposleni_19327_backup SET UNUSED (korisnik_unio);

-- ZADATAK 12

-- ZADATAK 13

ALTER TABLE zaposleni_19327_backup DROP UNUSED COLUMNS;
