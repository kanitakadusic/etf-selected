SELECT * FROM employees;
SELECT * FROM departments;
SELECT * FROM locations;
SELECT * FROM countries;
SELECT * FROM regions;
SELECT * FROM jobs;

-- ZADATAK 1

SELECT Sum(salary * (1 + commission_pct)) "Suma iznosa dodataka na platu",
       Count(commission_pct) "Broj zaposlenih",
       Count(*) "Ukupan broj zaposlenih"
FROM employees;

-- ZADATAK 2

SELECT j.job_title "Naziv posla",
       d.department_name "Naziv odjela",
       Count(e.employee_id) "Broj uposlenih"
FROM employees e, departments d, jobs j
WHERE e.department_id = d.department_id AND
      e.job_id = j.job_id
GROUP BY j.job_title, d.department_name;

-- ZADATAK 3

SELECT Max(salary),
       Min(salary),
       Sum(salary),
       Round(Avg(salary), 6)
FROM employees;

-- ZADATAK 4

SELECT j.job_title "Posao",
       Max(e.salary) "Max plata",
       Min(e.salary) "Min plata",
       Sum(e.salary) "Suma plata",
       Round(Avg(e.salary), 6) "Prosjecna plata"
FROM employees e, jobs j
WHERE e.job_id = j.job_id
GROUP BY j.job_title;

-- ZADATAK 5

SELECT j.job_title "Posao",
       Count(e.employee_id) "Broj zaposlenih"
FROM employees e, jobs j
WHERE e.job_id = j.job_id
GROUP BY j.job_title;

-- ZADATAK 6

SELECT Count(Count(m.employee_id)) "Broj menadzera"
FROM employees e, employees m
WHERE e.manager_id = m.employee_id(+)
GROUP BY m.employee_id;

-- ZADATAK 7
-- gradivo sljedeceg laba

-- ZADATAK 8

SELECT Nvl(d.department_name, 'X') "Odjel",
       Nvl(l.city, 'X') "Grad",
       Count(e.employee_id) "Broj zaposlenih",
       Round(Avg(e.salary), 2) "Prosjecna plata"
FROM employees e, departments d, locations l
WHERE e.department_id = d.department_id(+) AND
      d.location_id = l.location_id(+)
GROUP BY d.department_name, l.city;

-- ZADATAK 9

SELECT Count(Decode(To_Char(hire_date, 'yyyy'), '2003', 1, NULL)) "2003",
       Count(Decode(To_Char(hire_date, 'yyyy'), '2004', 1, NULL)) "2004",
       Count(Decode(To_Char(hire_date, 'yyyy'), '2005', 1, NULL)) "2005",
       Count(Decode(To_Char(hire_date, 'yyyy'), '2006', 1, NULL)) "2006",
       Count(Decode(To_Char(hire_date, 'yyyy'), '2003', 1, '2004', 1, '2005', 1, '2006', 1, NULL)) "UKUPNO"
FROM employees;

-- ZADATAK 10
-- gradivo sljedeceg laba
