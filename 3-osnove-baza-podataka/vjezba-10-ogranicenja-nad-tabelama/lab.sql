SELECT * FROM employees;
SELECT * FROM departments;
SELECT * FROM locations;
SELECT * FROM countries;
SELECT * FROM regions;
SELECT * FROM jobs;

SELECT * FROM emp_19327_1;
SELECT * FROM dep_19327_1;

SELECT * FROM view_19327_1;
SELECT * FROM view_19327_2;
SELECT * FROM view_19327_3;

-- ZADATAK 1

CREATE TABLE emp_19327_1 AS SELECT * FROM employees;
ALTER TABLE emp_19327_1 ADD (id NUMBER);
UPDATE emp_19327_1 SET id = employee_id;
ALTER TABLE emp_19327_1 ADD CONSTRAINT emp_19327_1_id_PK PRIMARY KEY (id);
ALTER TABLE emp_19327_1 SET UNUSED (employee_id);

-- ZADATAK 2

CREATE TABLE dep_19327_1 AS SELECT * FROM departments;
ALTER TABLE dep_19327_1 ADD (id NUMBER);
UPDATE dep_19327_1 SET id = department_id;
ALTER TABLE dep_19327_1 ADD CONSTRAINT dep_19327_1_id_PK PRIMARY KEY (id);
ALTER TABLE dep_19327_1 SET UNUSED (department_id);
ALTER TABLE dep_19327_1 ADD (datum DATE);

-- ZADATAK 3

ALTER TABLE emp_19327_1 ADD (dep_id NUMBER);
UPDATE emp_19327_1 SET dep_id = department_id;
ALTER TABLE emp_19327_1 SET UNUSED (department_id);
ALTER TABLE emp_19327_1 ADD CONSTRAINT emp_19327_1_dep_id_FK FOREIGN KEY (dep_id) REFERENCES dep_19327_1 (id);

-- ZADATAK 4

SELECT * FROM all_constraints WHERE owner = 'HR';
SELECT * FROM all_constraints WHERE owner = 'TEST';

-- ZADATAK 5

SELECT * FROM all_objects WHERE object_name = 'EMPLOYEES' AND owner = 'HR';
SELECT * FROM all_objects WHERE object_name = 'DEPARTMENTS' AND owner = 'HR';

-- ZADATAK 6

ALTER TABLE emp_19327_1 ADD (plata_dodatak NUMBER);

UPDATE emp_19327_1
SET plata_dodatak = salary * (1 + Nvl(commission_pct, 0))
WHERE id IN (SELECT e.employee_id
            FROM employees e, departments d, locations l, countries c
            WHERE e.department_id = d.department_id AND
                  d.location_id = l.location_id AND
                  l.country_id = c.country_id AND
                  Lower(c.country_name) LIKE '%america%');

-- ZADATAK 7

ALTER TABLE emp_19327_1 ADD CONSTRAINT emp_19327_1_plata_dodatak_CK CHECK (plata_dodatak BETWEEN 0 AND 1000000);

-- ZADATAK 8

CREATE OR REPLACE VIEW emp_view_19327_1
(sifra_zaposlenog, naziv_zaposlenog, naziv_odjela) AS
SELECT e.employee_id,
       e.first_name || ' ' || e.last_name,
       d.department_name
FROM employees e, departments d
WHERE e.department_id = d.department_id AND
      e.salary > (SELECT Avg(q.salary)
                  FROM employees q
                  WHERE q.department_id = e.department_id);

-- ZADATAK 9

SELECT e.salary
FROM employees e, view_19327_1 v
WHERE e.employee_id = v.sifra_zaposlenog;

-- ZADATAK 10

CREATE OR REPLACE VIEW view_19327_2
(naziv_posla, naziv_odjela, prosjecna_plata, iznos_dodatka) AS
SELECT j.job_title,
       d.department_name,
       Round(Avg(e.salary), 2),
       Round(Avg(e.commission_pct * e.salary), 2)
FROM employees e, departments d, jobs j
WHERE e.department_id = d.department_id AND
      e.job_id = j.job_id AND
      Lower(d.department_name) LIKE '%a%' AND
      Lower(d.department_name) LIKE '%b%' AND
      Lower(d.department_name) LIKE '%c%'
GROUP BY j.job_title, d.department_name
WITH READ ONLY;

-- ZADATAK 11

CREATE OR REPLACE VIEW view_19327_2
(naziv_posla, naziv_odjela, prosjecna_plata, iznos_dodatka, avg_plata_odjel) AS
SELECT j.job_title,
       d.department_name,
       Round(Avg(e.salary), 2),
       Round(Avg(e.commission_pct * e.salary), 2),
      (SELECT Round(Avg(q.salary), 2)
       FROM employees q
       WHERE q.department_id = e.department_id)
FROM employees e, departments d, jobs j
WHERE e.department_id = d.department_id AND
      e.job_id = j.job_id AND
      Lower(d.department_name) LIKE '%a%' AND
      Lower(d.department_name) LIKE '%b%' AND
      Lower(d.department_name) LIKE '%c%'
GROUP BY j.job_title, d.department_name, e.department_id
WITH READ ONLY;

-- ZADATAK 12

CREATE OR REPLACE VIEW view_19327_2
(naziv_posla, naziv_odjela, prosjecna_plata, iznos_dodatka, prosjecna_plata_odjela) AS
SELECT j.job_title,
       d.department_name,
       Round(Avg(e.salary), 2),
       Round(Avg(e.commission_pct * e.salary), 2),
      (SELECT Round(Avg(q.salary), 2)
       FROM employees q
       WHERE q.department_id = e.department_id)
FROM employees e, departments d, jobs j
WHERE e.department_id = d.department_id AND
      e.job_id = j.job_id AND
      Lower(d.department_name) LIKE '%a%' AND
      Lower(d.department_name) LIKE '%b%' AND
      Lower(d.department_name) LIKE '%c%'
GROUP BY j.job_title, d.department_name, e.department_id
WITH READ ONLY;

-- ZADATAK 13

CREATE OR REPLACE VIEW view_19327_3
(naziv_sefa, broj_podredjenih, min_plata, max_plata) AS
SELECT e.first_name || ' ' || e.last_name,
      (SELECT Count(*) FROM employees q WHERE q.manager_id = e.employee_id),
      (SELECT Min(q.salary) FROM employees q WHERE q.department_id = e.department_id),
      (SELECT Max(q.salary) FROM employees q WHERE q.department_id = e.department_id)
FROM employees e
WHERE e.employee_id IN (SELECT q.manager_id FROM employees q);

-- ZADATAK 14

CREATE OR REPLACE VIEW view_19327_3
(naziv_sefa, broj_podredjenih, min_plata, max_plata, sum_plata) AS
SELECT e.first_name || ' ' || e.last_name,
      (SELECT Count(*) FROM employees q WHERE q.manager_id = e.employee_id),
      (SELECT Min(q.salary) FROM employees q WHERE q.department_id = e.department_id),
      (SELECT Max(q.salary) FROM employees q WHERE q.department_id = e.department_id),
      (SELECT Sum(q.salary * (1 + Nvl(q.commission_pct, 0))) FROM employees q WHERE q.manager_id = e.employee_id)
FROM employees e
WHERE e.employee_id IN (SELECT q.manager_id FROM employees q)
WITH READ ONLY;
