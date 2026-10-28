SELECT * FROM employees;
SELECT * FROM departments;
SELECT * FROM locations;
SELECT * FROM countries;
SELECT * FROM regions;
SELECT * FROM jobs;

-- ZADATAK 1

SELECT e.first_name || ' ' || e.last_name AS zaposlenik,
       d.department_name AS odjel,
       j.job_title AS posao
FROM employees e, departments d, jobs j
WHERE e.department_id = d.department_id AND
      e.job_id = j.job_id(+) AND
      e.department_id = (SELECT department_id FROM employees WHERE Lower(first_name) = 'diana') AND
      Lower(e.first_name) <> 'diana';

-- ZADATAK 2

SELECT employee_id AS sifra,
       first_name AS ime,
       last_name AS prezime,
       salary AS plata
FROM employees
WHERE salary > (SELECT Avg(salary) FROM employees WHERE department_id IN (30, 90));

-- ZADATAK 3

SELECT *
FROM employees
WHERE department_id IN (SELECT department_id FROM employees WHERE first_name LIKE '%C%');

-- ZADATAK 4

SELECT e.employee_id AS sifra,
       e.first_name || ' ' || e.last_name AS zaposlenik,
       j.job_title AS posao
FROM employees e, jobs j
WHERE e.job_id = j.job_id(+) AND
      e.department_id IN (SELECT d.department_id
                          FROM departments d, locations l
                          WHERE d.location_id = l.location_id AND
                                Lower(l.city) = 'toronto');

-- ZADATAK 5

SELECT *
FROM employees
WHERE manager_id = (SELECT employee_id FROM employees WHERE Lower(first_name) = 'steven' AND Lower(last_name) = 'king');

-- ZADATAK 6

SELECT e.employee_id AS sifra,
       e.first_name AS ime,
       e.last_name AS prezime,
       e.salary AS plata
FROM employees e
WHERE e.salary > (SELECT Avg(salary) FROM employees WHERE department_id = e.department_id);

-- ZADATAK 7

SELECT e.first_name || ' ' || e.last_name AS zaposlenik,
       d.department_name AS odjel,
       e.salary AS plata
FROM employees e, departments d
WHERE e.department_id = d.department_id(+) AND
      e.commission_pct IS NULL AND
      e.salary IN (SELECT salary * (1 + commission_pct)
                   FROM employees
                   WHERE department_id = e.department_id AND
                         commission_pct IS NOT NULL);

-- ZADATAK 8

SELECT e.first_name || ' ' || e.last_name AS zaposlenik,
       d.department_name AS odjel,
       e.salary AS plata,
       l.city AS grad
FROM employees e, departments d, locations l
WHERE e.department_id = d.department_id AND
      d.location_id = l.location_id AND
      (e.salary, Nvl(e.commission_pct, 0)) IN (SELECT e.salary, Nvl(e.commission_pct, 0)
                                               FROM employees e, departments d, locations l
                                               WHERE e.department_id = d.department_id AND
                                                     d.location_id = l.location_id AND
                                                     Lower(l.city) = 'seattle') AND
      Lower(l.city) <> 'seattle';

-- ZADATAK 9

SELECT first_name || ' ' || last_name AS zaposlenik,
       hire_date AS datum_zaposlenja,
       salary AS plata
FROM employees
WHERE (salary, Nvl(commission_pct, 0)) IN (SELECT salary, Nvl(commission_pct, 0) FROM employees WHERE Lower(first_name) = 'oliver') AND
      Lower(first_name) <> 'oliver';

-- ZADATAK 10

SELECT *
FROM employees
WHERE salary > ALL (SELECT e.salary
                    FROM employees e, departments d
                    WHERE e.department_id = d.department_id AND
                          Lower(d.department_name) = 'sales')
ORDER BY salary DESC;

--

SELECT *
FROM employees
WHERE salary > (SELECT Max(e.salary)
                FROM employees e, departments d
                WHERE e.department_id = d.department_id AND
                      Lower(d.department_name) = 'sales')
ORDER BY salary DESC;

-- ZADATAK 11

SELECT e.first_name || ' ' || e.last_name AS zaposlenik,
       d.department_name AS odjel,
       j.job_title AS posao,
       l.city AS grad
FROM employees e, departments d, jobs j, locations l
WHERE e.department_id = d.department_id AND
      e.job_id = j.job_id(+) AND
      d.location_id = l.location_id(+) AND
      e.salary > (SELECT Avg(salary)
                  FROM employees
                  WHERE employee_id IN (SELECT manager_id
                                        FROM employees
                                        WHERE commission_pct IS NOT NULL AND
                                              department_id = d.department_id)) AND
      e.employee_id NOT IN (SELECT manager_id
                            FROM employees
                            WHERE commission_pct IS NOT NULL AND
                                  department_id = d.department_id);

-- ZADATAK 12

SELECT e.employee_id AS id_zaposlenik,
       e.first_name || ' ' || e.last_name AS zaposlenik,
       d.department_id AS id_odjel,
       d.department_name AS odjel,
       e.salary AS plata,
       (SELECT Round(Avg(salary), 2) FROM employees WHERE department_id = d.department_id) AS prosjecna_plata_odjela,
       (SELECT Min(salary) FROM employees WHERE department_id = d.department_id) AS minimalna_plata_odjela,
       (SELECT Max(salary) FROM employees WHERE department_id = d.department_id) AS maksimalna_plata_odjela,
       a.prosjek AS prosjecna_plata_firme,
       a.minimum AS minimalna_plata_firme,
       a.maksimum AS maksimalna_plata_firme
FROM employees e,
     departments d,
     (SELECT Round(Avg(salary), 2) AS prosjek, Min(salary) AS minimum, Max(salary) AS maksimum FROM employees) a
WHERE e.department_id = d.department_id AND
      e.salary > (SELECT Min(salary)
                  FROM employees
                  WHERE employee_id IN (SELECT manager_id
                                        FROM employees
                                        WHERE department_id = d.department_id)) AND
      e.employee_id NOT IN (SELECT manager_id
                            FROM employees
                            WHERE department_id = d.department_id)
ORDER BY 1;

-- POKUSAJ

SELECT e.employee_id AS id_zaposlenik,
       e.first_name || ' ' || e.last_name AS zaposlenik,
       d.department_id AS id_odjel,
       d.department_name AS odjel,
       e.salary AS plata,
       a.prosjek AS prosjecna_plata_firme,
       a.minimum AS minimalna_plata_firme,
       a.maksimum AS maksimalna_plata_firme
FROM employees e,
     departments d,
     (SELECT Round(Avg(salary), 2) AS prosjek, Min(salary) AS minimum, Max(salary) AS maksimum FROM employees) a,
     -- treba li uslov spajanja s obzirom da je rezultat samo jedan red
     (SELECT Round(Avg(salary), 2) AS prosjek, Min(salary) AS minimum, Max(salary) AS maksimum FROM employees WHERE department_id = d.department_id) b
     -- trebalo bi da inace moze, mozda sqltools pravi problem
WHERE e.department_id = d.department_id;
