SELECT * FROM employees;
SELECT * FROM departments;
SELECT * FROM locations;
SELECT * FROM jobs;
SELECT * FROM countries;
SELECT * FROM regions;

-- ZADATAK 1

SELECT e.first_name || ' ' || e.last_name AS employee,
       e.department_id AS D_ID_E,
       d.department_id AS D_ID_D,
       d.department_name
FROM employees e, departments d
WHERE e.department_id = d.department_id(+);

-- ZADATAK 2

SELECT j.job_title,
       e.department_id
FROM jobs j, employees e
WHERE e.job_id = j.job_id AND
      e.department_id = 30;

-- ZADATAK 3

SELECT e.first_name || ' ' || e.last_name AS employee,
       d.department_name,
       l.*
FROM employees e, departments d, locations l
WHERE e.department_id = d.department_id(+) AND
      d.location_id = l.location_id(+);

--

SELECT e.first_name || ' ' || e.last_name AS employee,
       d.department_name,
       d.location_id
FROM employees e, departments d
WHERE e.department_id = d.department_id(+);

-- ZADATAK 4

SELECT e.first_name || ' ' || e.last_name AS employee,
       d.department_name
FROM employees e, departments d
WHERE e.department_id = d.department_id(+) AND
      e.first_name || e.last_name LIKE '%a%';

-- ZADATAK 5

SELECT e.first_name || ' ' || e.last_name AS employee,
       j.job_title,
       d.department_name,
       l.city
FROM employees e, departments d, locations l, jobs j
WHERE e.department_id = d.department_id(+) AND
      e.job_id = j.job_id(+) AND
      d.location_id = l.location_id(+) AND
      Lower(l.city) = 'oxford';

--

SELECT e.first_name || ' ' || e.last_name AS employee,
       e.job_id,
       d.department_name,
       l.city
FROM employees e, departments d, locations l
WHERE e.department_id = d.department_id(+) AND
      d.location_id = l.location_id(+) AND
      Lower(l.city) = 'oxford';


-- ZADATAK 6

SELECT e.first_name || ' ' || e.last_name "Naziv zaposlenog",
       m.first_name || ' ' || m.last_name "Naziv sefa",
       l.city "Grad sefa"
FROM employees e, employees m, locations l, departments d
WHERE e.manager_id = m.employee_id AND
      m.department_id = d.department_id AND
      d.location_id = l.location_id
ORDER BY e.first_name, e.last_name;

-- ZADATAK 7

SELECT e.first_name || ' ' || e.last_name "Naziv zaposlenog",
       m.first_name || ' ' || m.last_name "Naziv sefa",
       l.city "Grad sefa"
FROM employees e, employees m, locations l, departments d
WHERE e.manager_id = m.employee_id(+) AND
      m.department_id = d.department_id(+) AND
      d.location_id = l.location_id(+)
ORDER BY e.first_name, e.last_name;

-- ZADATAK 8

SELECT e.first_name || ' ' || e.last_name "Naziv zaposlenog",
       e.department_id "Sifra odjela"
FROM employees e, employees t
WHERE t.employee_id = 120 AND
      e.employee_id <> 120 AND -- ako ne zelimo prikazati uzetog zaposlenog
      e.department_id = t.department_id;

-- ZADATAK 9

SELECT e.first_name || ' ' || e.last_name "Zaposlenik",
       j.job_title "Posao",
       d.department_name "Odjel",
       e.salary "Osnovna plata",
       e.salary * (1 + Nvl(e.commission_pct, 0)) "Plata sa dodatkom",
       j.min_salary || ' ... ' || j.max_salary "Raspon plate za dati posao"
FROM employees e, departments d, jobs j
WHERE e.department_id = d.department_id(+) AND
      e.job_id = j.job_id(+) AND
      e.salary * (1 + Nvl(e.commission_pct, 0)) NOT BETWEEN j.min_salary AND j.max_salary
ORDER BY e.salary DESC;

-- ZADATAK 10

SELECT e.first_name || ' ' || e.last_name "Zaposlenik",
       e.hire_date "Datum zaposlenja"
FROM employees e, employees t
WHERE t.employee_id = 144 AND -- Lower(t.first_name) = 'blake'
      e.hire_date > t.hire_date
ORDER BY e.hire_date DESC;

-- ZADATAK 11

SELECT e.first_name || ' ' || e.last_name "Zaposlenik",
       e.hire_date "D.Z. zaposlenika",
       m.first_name || ' ' || m.last_name "Sef",
       m.hire_date "D.Z. sefa"
FROM employees e, employees m
WHERE e.manager_id = m.employee_id AND
      e.hire_date < m.hire_date;

-- OSTALO

SELECT e.employee_id, e.first_name, e.last_name, e.department_id, d.department_id, d.department_name
FROM employees e, departments d
WHERE e.department_id = d.department_id AND d.department_id = 10;

SELECT e.employee_id, e.first_name, e.last_name, e.department_id, d.department_id, d.department_name
FROM employees e JOIN departments d ON d.department_id = 10;

SELECT e.employee_id, e.first_name, e.last_name, e.department_id, d.department_id, d.department_name
FROM employees e RIGHT OUTER JOIN departments d ON e.department_id = d.department_id;

SELECT e.employee_id, e.first_name, e.last_name, e.department_id, d.department_id, d.department_name
FROM employees e, departments d
WHERE e.department_id(+) = d.department_id;
